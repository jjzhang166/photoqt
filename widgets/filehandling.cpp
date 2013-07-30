#include "filehandling.h"
#include <unistd.h>
#include <iostream>

FileHandling::FileHandling(QWidget *parent, bool v, QString cf) : QWidget(parent) {

	verbose = v;

	// The different QRects
	rectShown = QRect(0,0,parent->width(),parent->height());
	rectHidden = QRect(0,-parent->height(),parent->width(),parent->height());
	rectAni = QRect(parent->width()/2,parent->height()/2,1,1);

	// The current geometry and position
	isShown = false;
	this->setGeometry(rectHidden);

	// The current widget look
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaShow));

	// Some variables
	currentfile = cf;
	fadeBack = new QTimeLine;
	fadeBack->setLoopCount(5);
	backAlphaShow = 130;
	backAlphaCur = 0;
	fadeBackIN = true;
	connect(fadeBack, SIGNAL(valueChanged(qreal)), this, SLOT(fadeStep()));

	// The current animation framework
	ani = new QPropertyAnimation(this,"geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));


	QString fileTreeCSS = "QTreeView {";
	fileTreeCSS += "color:white;";
	fileTreeCSS += "background: rgba(0,0,0,0);";
	fileTreeCSS += "show-decoration-selected: 1;";
	fileTreeCSS += "}";

	fileTreeCSS += "QTreeView::item:hover{";
	fileTreeCSS += "background: rgba(255,255,255,128);";
	fileTreeCSS += "color: black";
	fileTreeCSS += "}";

	fileTreeCSS += "QTreeView::item:selected{";
	fileTreeCSS += "background: white;";
	fileTreeCSS += "color: black;";
	fileTreeCSS += "}";

	fileTreeCSS += "QTreeView::item:selected:active{";
	fileTreeCSS += "background: white;";
	fileTreeCSS += "color: black";
	fileTreeCSS += "}";

	fileTreeCSS += "QTreeView::item:selected:!active{";
	fileTreeCSS += "background: white;";
	fileTreeCSS += "color: black";
	fileTreeCSS += "}";


	/**********************************
	/////////// RENAME ////////////////
	**********************************/

	renameWidget = new QWidget(this);
	renameWidget->setStyleSheet("background: rgba(0,0,0,200); border-radius: 25px");

	renameLay = new QVBoxLayout;
	renameTitle = new QLabel("<center>" + tr("Rename File") + "</center>");
	renameTitle->setStyleSheet("font-size: 25pt; font-weight: bold; color: white; background: transparent;");
	renameOldName = new QLabel;
	renameOldName->setStyleSheet("font-size: 15pt; font-style: italic; color: grey; background: transparent; padding: 10px;");
	QHBoxLayout *renameNewLay = new QHBoxLayout;
	renameNewName = new QLineEdit;
	renameNewName->setFixedWidth(300);
	renameNewName->setStyleSheet("color: white; background: rgba(0,0,0,150); font-size: 15pt; selection-background-color: white; selection-color: black;");
	renameOldEnding = new QLabel;
	renameOldEnding->setStyleSheet("color: white; background: transparent; font-size: 15pt;");
	renameNewLay->addStretch();
	renameNewLay->addWidget(renameNewName);
	renameNewLay->addWidget(renameOldEnding);
	renameNewLay->addStretch();

	renameNewNameExists = new QLabel(" ");
	renameNewNameExists->setStyleSheet("color: red; font-weight: bold; background: none");

	renameSave = new CustomPushButton(tr("Save"));
	renameSave->setPadding(10);
	renameSave->setFontSize("13pt");
	renameSave->setBold(true);

	renameCancel = new CustomPushButton(tr("Cancel"));
	renameCancel->setPadding(10);
	renameCancel->setFontSize("13pt");
	renameCancel->setBold(true);

	renameLay->addStretch();
	renameLay->addWidget(renameTitle);
	renameLay->addWidget(renameOldName);
	renameLay->addSpacing(5);
	renameLay->addLayout(renameNewLay);
	renameLay->addWidget(renameNewNameExists);
	QHBoxLayout *renameButLay = new QHBoxLayout;
	renameButLay->addStretch();
	renameButLay->addWidget(renameSave);
	renameButLay->addWidget(renameCancel);
	renameButLay->addStretch();
	renameLay->addLayout(renameButLay);
	renameLay->addStretch();

	connect(renameNewName, SIGNAL(textEdited(QString)), this, SLOT(validateRenameFilename()));
	connect(renameSave, SIGNAL(clicked()), this, SLOT(doRename()));
	connect(renameCancel, SIGNAL(clicked()), this, SLOT(animate()));

	renameWidget->setLayout(renameLay);



	/**********************************
	/////////// DELETE ////////////////
	**********************************/

	// On Linux (X11) a "move to trash" is the default (according to freedesktop.org standard). On all other systems, this option is not (yet) available.

	deleteWidget = new QWidget(this);
	deleteWidget->setStyleSheet("background: rgba(0,0,0,200); border-radius: 25px");

	deleteLay = new QVBoxLayout;
	deleteTitle = new QLabel("<center>" + tr("Delete File") + "</center>");
	deleteTitle->setStyleSheet("font-size: 25pt; font-weight: bold; color: white; background: transparent;");
	deleteFilename = new QLabel(" ");
	deleteFilename->setStyleSheet("font-size: 15pt; font-style: italic; color: grey; background: transparent; padding: 10px;");
	deleteQuestion = new QLabel("<center>" + tr("Do you really want to delete this file?") +"</center>");
	deleteQuestion->setStyleSheet("font-size: 15pt; color: white; background: transparent; padding: 10px;");



	deleteYesHard = new CustomPushButton(tr("Delete permanently"));
	deleteYesHard->setPadding(6);
#if defined(Q_WS_X11)
	deleteYesHard->setFontSize("10pt");
#else
	deleteYesHard->setFontSize("13pt");
	deleteYesHard->setBold(true);
#endif

#if defined(Q_WS_X11)
	deleteYes = new CustomPushButton(tr("Move to Trash"));
	deleteYes->setPadding(6);
	deleteYes->setFontSize("13pt");
	deleteYes->setBold(true);
#else
	deleteYes = new QPushButton;
	deleteYes->hide();
#endif

	deleteNo = new CustomPushButton(tr("Cancel"));
	deleteNo->setPadding(6);
	deleteNo->setFontSize("13pt");
	deleteNo->setBold(true);

	QHBoxLayout *deleteButLay = new QHBoxLayout;
	deleteButLay->addStretch();

#if defined(Q_WS_X11)
	deleteButLay->addWidget(deleteYes);
#else
	deleteButLay->addWidget(deleteYesHard);
#endif

	deleteButLay->addWidget(deleteNo);
	deleteButLay->addStretch();

#if defined(Q_WS_X11)
	QHBoxLayout *deleteHardButLay = new QHBoxLayout;
	deleteHardButLay->addStretch();
	deleteHardButLay->addWidget(deleteYesHard);
	deleteHardButLay->addStretch();
#endif

	deleteLay->addStretch();
	deleteLay->addWidget(deleteTitle);
	deleteLay->addWidget(deleteFilename);
	deleteLay->addWidget(deleteQuestion);
	deleteLay->addLayout(deleteButLay);
#if defined(Q_WS_X11)
	deleteLay->addLayout(deleteHardButLay);
#endif
	deleteLay->addStretch();

	connect(deleteYes, SIGNAL(clicked()), this, SLOT(doDelete()));
	connect(deleteNo, SIGNAL(clicked()), this, SLOT(animate()));

	QSignalMapper *mapper = new QSignalMapper;
	mapper->setMapping(deleteYesHard,1);
	connect(deleteYesHard, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(mapper, SIGNAL(mapped(int)), this, SLOT(doDelete(int)));



	deleteWidget->setLayout(deleteLay);



	/********************************
	/////////// MOVE ////////////////
	********************************/

	moveWidget = new QWidget(this);
	moveWidget->setStyleSheet("background: rgba(0,0,0,200); border-radius: 25px");

	moveLay = new QVBoxLayout;
	moveTitle = new QLabel("<center>" + tr("Moving File") + "</center>");
	moveTitle->setStyleSheet("font-size: 25pt; font-weight: bold; color: white; background: transparent;");

	moveTree = new QTreeView;
	moveTree->setStyleSheet(fileTreeCSS);
	moveTree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	moveTree->setHeaderHidden(true);
	moveScrollbar = new CustomScrollbar;
	moveScrollbar->keepAlwaysVisible(true);
	moveTree->setVerticalScrollBar(moveScrollbar);
	moveTreeModel = new QFileSystemModel;
	moveTreeModel->setRootPath(QDir::rootPath());
	moveTreeModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	moveTree->setCurrentIndex(moveTreeModel->index(QDir::homePath()));
	moveTree->setModel(moveTreeModel);
	for(int i = 1; i < moveTreeModel->columnCount(); ++i)
		moveTree->setColumnHidden(i,true);

	moveNewNameExists = new QLabel(" ");
	moveNewNameExists->setStyleSheet("color: red; font-weight: bold; background: none");

	moveNewNameLabel = new QLabel(tr("Filename") + ":");
	moveNewNameLabel->setStyleSheet("font-size: 12pt; color: white; background: transparent; font-weight: bold;");
	moveNewName = new QLineEdit;
	moveNewName->setFixedWidth(300);
	moveNewName->setStyleSheet("color: white; background: rgba(0,0,0,150); font-size: 12pt; selection-background-color: white; selection-color: black;");
	moveNewNameEnding = new QLabel(".xyz");
	moveNewNameEnding->setStyleSheet("font-size: 12pt; color: white; background: transparent; font-weight: bold;");

	QHBoxLayout *moveNewNameLay = new QHBoxLayout;
	moveNewNameLay->addStretch();
	moveNewNameLay->addWidget(moveNewNameLabel);
	moveNewNameLay->addWidget(moveNewName);
	moveNewNameLay->addWidget(moveNewNameEnding);
	moveNewNameLay->addStretch();

	moveSave = new CustomPushButton(tr("Move"));
	moveSave->setPadding(10);
	moveSave->setFontSize("11pt");
	moveSave->setBold(true);
	moveCancel = new CustomPushButton(tr("Cancel"));
	moveCancel->setPadding(10);
	moveCancel->setFontSize("11pt");
	moveCancel->setBold(true);

	QHBoxLayout *moveButLay = new QHBoxLayout;
	moveButLay->addStretch();
	moveButLay->addWidget(moveSave);
	moveButLay->addWidget(moveCancel);
	moveButLay->addStretch();

	moveLay->addSpacing(20);
	moveLay->addWidget(moveTitle);
	moveLay->addWidget(moveTree);
	moveLay->addWidget(moveNewNameExists);
	moveLay->addLayout(moveNewNameLay);
	moveLay->addLayout(moveButLay);
	moveLay->addSpacing(20);

	connect(moveCancel, SIGNAL(clicked()), this, SLOT(animate()));
	connect(moveSave, SIGNAL(clicked()), this, SLOT(doMove()));
	connect(moveNewName, SIGNAL(textEdited(QString)), this, SLOT(validateMoveAndCopyFilename()));
	connect(moveTree, SIGNAL(clicked(QModelIndex)), this, SLOT(validateMoveAndCopyFilename()));

	moveWidget->setLayout(moveLay);



	/********************************
	/////////// COPY ////////////////
	********************************/

	copyWidget = new QWidget(this);
	copyWidget->setStyleSheet("background: rgba(0,0,0,200); border-radius: 25px");

	copyLay = new QVBoxLayout;
	copyTitle = new QLabel("<center>" + tr("Copying File") + "</center>");
	copyTitle->setStyleSheet("font-size: 25pt; font-weight: bold; color: white; background: transparent;");

	copyTree = new QTreeView;
	copyTree->setStyleSheet(fileTreeCSS);
	copyTree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	copyTree->setHeaderHidden(true);
	copyScrollbar = new CustomScrollbar;
	copyScrollbar->keepAlwaysVisible(true);
	copyTree->setVerticalScrollBar(copyScrollbar);
	copyTreeModel = new QFileSystemModel;
	copyTreeModel->setRootPath(QDir::rootPath());
	copyTreeModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	copyTree->setCurrentIndex(copyTreeModel->index(QDir::homePath()));
	copyTree->setModel(copyTreeModel);
	for(int i = 1; i < copyTreeModel->columnCount(); ++i)
		copyTree->setColumnHidden(i,true);

	copyNewNameExists = new QLabel(" ");
	copyNewNameExists->setStyleSheet("color: red; font-weight: bold; background: none");

	copyNewNameLabel = new QLabel(tr("Filename") + ":");
	copyNewNameLabel->setStyleSheet("font-size: 12pt; color: white; background: transparent; font-weight: bold;");
	copyNewName = new QLineEdit;
	copyNewName->setFixedWidth(300);
	copyNewName->setStyleSheet("color: white; background: rgba(0,0,0,150); font-size: 12pt; selection-background-color: white; selection-color: black;");
	copyNewNameEnding = new QLabel(".xyz");
	copyNewNameEnding->setStyleSheet("font-size: 12pt; color: white; background: transparent; font-weight: bold;");

	QHBoxLayout *copyNewNameLay = new QHBoxLayout;
	copyNewNameLay->addStretch();
	copyNewNameLay->addWidget(copyNewNameLabel);
	copyNewNameLay->addWidget(copyNewName);
	copyNewNameLay->addWidget(copyNewNameEnding);
	copyNewNameLay->addStretch();

	copySave = new CustomPushButton(tr("Copy"));
	copySave->setFontSize("11pt");
	copySave->setPadding(10);
	copySave->setBold(true);
	copyCancel = new CustomPushButton(tr("Cancel"));
	copyCancel->setFontSize("11pt");
	copyCancel->setPadding(10);
	copyCancel->setBold(true);

	QHBoxLayout *copyButLay = new QHBoxLayout;
	copyButLay->addStretch();
	copyButLay->addWidget(copySave);
	copyButLay->addWidget(copyCancel);
	copyButLay->addStretch();

	copyLay->addSpacing(20);
	copyLay->addWidget(copyTitle);
	copyLay->addWidget(copyTree);
	copyLay->addWidget(copyNewNameExists);
	copyLay->addLayout(copyNewNameLay);
	copyLay->addLayout(copyButLay);
	copyLay->addSpacing(20);

	connect(copyCancel, SIGNAL(clicked()), this, SLOT(animate()));
	connect(copySave, SIGNAL(clicked()), this, SLOT(doCopy()));
	connect(copyNewName, SIGNAL(textEdited(QString)), this, SLOT(validateMoveAndCopyFilename()));
	connect(copyTree, SIGNAL(clicked(QModelIndex)), this, SLOT(validateMoveAndCopyFilename()));

	copyWidget->setLayout(copyLay);


}

// Open the dialog type 't'
void FileHandling::openDialog(QString t) {

	if(verbose) std::clog << "fhd: Open widget:" << t.toStdString() << std::endl;

	if(currentfile != "") {

		// Make sure all widgets are hidden by default
		renameWidget->setGeometry(rectHidden);
		deleteWidget->setGeometry(rectHidden);
		moveWidget->setGeometry(rectHidden);
		copyWidget->setGeometry(rectHidden);

		// Save current state
		dialogType = t;

		// Set right widget
		if(t == "rename")
			setRename();
		if(t == "delete")
			setDelete();
		if(t == "copy")
			setCopy();
		if(t == "move")
			setMove();

		// And animate
		animate();

	}

}

void FileHandling::makeHide() {

	if(isShown) animate();

}

void FileHandling::makeShow() {

	if(!isShown) animate();

}

void FileHandling::setRect(QRect rect) {

	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect(rect.width()/2.0,rect.height()/2.0,1,1);
	rectShown = rect;

	if(isShown) this->setGeometry(rectShown);
	else this->setGeometry(rectHidden);

}


// The animation function
void FileHandling::animate() {

	QRect shown = QRect();

	if(dialogType == "move" || dialogType == "copy")
		shown = QRect((rectShown.width()-600)/2,(rectShown.height()-500)/2,600,500);
	else
		shown = QRect((rectShown.width()-600)/2,(rectShown.height()-300)/2,600,300);

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		// The background is initially transparent but the geometry is full
		this->setStyleSheet(QString("background: rgba(0,0,0,0);"));
		this->setGeometry(rectShown);

		// Stop Thumbnail creation
		emit stopThbCreation();

		// Widget is shown
		isShown = true;

		// Animate widget
		ani->setDuration(400);
		ani->setStartValue(rectAni);
		ani->setEndValue(shown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		// Fade background in
		fadeBack->setDuration(200);
		fadeBack->setLoopCount(5);
		fadeBackIN = true;
		fadeBack->start();

		// Block all base functions
		emit blockFunc(true);

		// Make sure this widget is on top
		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		// Fade background out
		fadeBack->setDuration(100);
		fadeBack->setLoopCount(5);
		fadeBackIN = false;
		fadeBack->start();

		// Widget is hidden again
		isShown = false;

		// Start animation out
		ani->setDuration(300);
		ani->setStartValue(shown);
		ani->setEndValue(rectAni);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

		// Unblock all base functions
		emit blockFunc(false);

	}

}

// Every fade step for the background
void FileHandling::fadeStep() {

	// Fade in
	if(fadeBackIN) {
		backAlphaCur += backAlphaShow/5;
		if(backAlphaCur > backAlphaShow)
			backAlphaCur = backAlphaShow;
	// Fade out
	} else {
		backAlphaCur -= backAlphaShow/5;
		if(backAlphaCur < 0)
			backAlphaCur = 0;
	}

	// Update stylesheet
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaCur));

}

// Handle widget when animation is finished
void FileHandling::aniFinished() {

	// Move widget out of screen
	if(!isShown) {
		this->setGeometry(rectHidden);
		if(dialogType == "rename")
			renameNewName->setEnabled(false);
		else if(dialogType == "move")
			moveNewName->setEnabled(false);
	}

	if(isShown) {

		if(dialogType == "move")
			moveTree->scrollTo(moveTree->selectionModel()->selectedIndexes().at(0));
		if(dialogType == "copy")
			copyTree->scrollTo(copyTree->selectionModel()->selectedIndexes().at(0));

	}

}

// Set the rename dialog
void FileHandling::setRename() {

	if(verbose) std::clog << "fhd: Set rename layout" << std::endl;

	// Update the labels and focus the LineEdit
	renameOldName->setText("<center>"+ tr("Old name") + ": " + QFileInfo(currentfile).fileName() + "</center>");
	renameNewName->setText(QFileInfo(currentfile).completeBaseName());
	renameNewName->setEnabled(true);
	renameNewName->setFocus();
	renameNewName->selectAll();
	renameOldEnding->setText("." + QFileInfo(currentfile).completeSuffix());

	ani->stop();

	// Adjust target of animation
	ani->setTargetObject(renameWidget);

	// Validate new filename (at this point the old name, i.e. save button is disabled)
	validateRenameFilename();

}

// Set the delete dialog
void FileHandling::setDelete() {

	if(verbose) std::clog << "fhd: Set delete layout" << std::endl;

	// Update old filename
	deleteFilename->setText("<center>" + QFileInfo(currentfile).fileName() + "</center>");

	ani->stop();

	// Adjust target of animation
	ani->setTargetObject(deleteWidget);

}

// Set the copy dialog
void FileHandling::setCopy() {

	if(verbose) std::clog << "fhd: Set copy layout" << std::endl;

	QModelIndex first = copyTreeModel->index(QFileInfo(currentfile).absolutePath());
	copyTree->setCurrentIndex(first);
	copyTree->setExpanded(first,true);
	copyTree->setFocus();

	copyNewName->setText(QFileInfo(currentfile).completeBaseName());
	copyNewNameEnding->setText("." + QFileInfo(currentfile).completeSuffix());

	copyNewName->setEnabled(true);
	copyNewName->selectAll();

	ani->stop();

	ani->setTargetObject(copyWidget);

	validateMoveAndCopyFilename();

}

// Set the move dialog
void FileHandling::setMove() {

	if(verbose) std::clog << "fhd: Set move layout" << std::endl;

	QModelIndex first = moveTreeModel->index(QFileInfo(currentfile).absolutePath());
	moveTree->setCurrentIndex(first);
	moveTree->setExpanded(first,true);
	moveTree->setFocus();

	moveNewName->setText(QFileInfo(currentfile).completeBaseName());
	moveNewNameEnding->setText("." + QFileInfo(currentfile).completeSuffix());

	moveNewName->setEnabled(true);
	moveNewName->selectAll();

	ani->stop();

	ani->setTargetObject(moveWidget);

	validateMoveAndCopyFilename();

}

// Execute renaming
void FileHandling::doRename() {

	// The old file
	QFile file(currentfile);

	// The new filename including full path
	QString newfile = QFileInfo(currentfile).absolutePath() + "/" + renameNewName->text() + renameOldEnding->text().toLower();

	if(verbose) std::clog << "fhd: Rename: " << currentfile.toStdString() << " - " << newfile.toStdString() << std::endl;

	// Do renaming (this first check of existence shouldn't be needed but just to be on the safe side)
	if(!QFile(newfile).exists()) {
		if(file.copy(newfile)) {
			if(!file.remove()) {
				std::cerr << "ERROR! Couldn't remove the old filename" << std::endl;
			}
			animate();
			currentfile = newfile;
			emit reloadDir("rename");
		} else {
			std::cerr << "ERROR! Couldn't rename file" << std::endl;
			animate();
		}

	}

}

// Execute deletion
void FileHandling::doDelete(int harddelete) {

#if defined(Q_WS_X11)

	if(harddelete == 0) {

		if(verbose) std::clog << "fhd: Move to trash" << std::endl;

		QString filepath = currentfile;

		// The file to delete
		QFile f(filepath);

		// Of course we only proceed if the file actually exists
		if(f.exists()) {

			// Create the meta .trashinfo file
			QString info = "[Trash Info]\n";
			info += "Path=" + filepath.replace(" ","%20") + "\n";
			info += "DeletionDate=" + QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss");

			// The base patzh for the Trah (files on external devices  use the external device for Trash)
			QString baseTrash = "";

			// If file lies in the home directory
			if(f.fileName().startsWith(QDir::homePath())) {

				// Set the base path and make sure all the dirs exist
				baseTrash = QString(qgetenv("XDG_DATA_HOME")) + "/Trash/";
				if(!QDir(QDir::homePath() + "/.local").exists())
					QDir().mkdir(QDir::homePath() + "/.local");
				if(!QDir(QDir::homePath() + "/.local/share").exists())
					QDir().mkdir(QDir::homePath() + "/.local/share");
				if(!QDir(baseTrash).exists())
					QDir().mkdir(baseTrash);
				if(!QDir(baseTrash + "files").exists())
					QDir().mkdir(baseTrash + "files");
				if(!QDir(baseTrash + "info").exists())
					QDir().mkdir(baseTrash + "info");
				} else {
					// Set the base path and make sure all the dirs exist
				baseTrash = "/" + filepath.split("/").at(1) + "/" + filepath.split("/").at(2) + QString("/.Trash-%1/").arg(getuid());
					if(!QDir(baseTrash).exists())
						QDir().mkdir(baseTrash);
					if(!QDir(baseTrash + "files").exists())
						QDir().mkdir(baseTrash + "files");
					if(!QDir(baseTrash + "info").exists())
						QDir().mkdir(baseTrash + "info");

			}

			// that's the new trash file
				QString trashFile = baseTrash + "files/" + QUrl::toPercentEncoding(QFileInfo(f).fileName(),""," ");

			// If there exists already a file with that name, we simply append the next higher number (sarting at 1)
			QFile ensure(trashFile);
			int j = 1;
			while(ensure.exists()) {
				trashFile = QFileInfo(trashFile).absolutePath() + "/" + QFileInfo(trashFile).baseName() + QString(" %1.").arg(j) + QFileInfo(trashFile).completeSuffix();
				ensure.setFileName(trashFile);
			}

			// Copy the file to the Trash
			if(f.copy(trashFile)) {

				// And remove the old file
				if(!f.remove())
					std::cerr << "ERROR: Old file couldn't be removed!" << std::endl;

				// Write the .trashinfo file
				QFile i(baseTrash + "info/" + QFileInfo(trashFile).fileName() + ".trashinfo");
				if(i.open(QIODevice::WriteOnly)) {
					QTextStream out(&i);
					out << info;
					i.close();
				} else
					std::cerr << "ERROR: *.trashinfo file couldn't be created!" << std::endl;

				emit reloadDir("delete");

			} else
				std::cerr << "ERROR: File couldn't be deleted (moving file failed)" << std::endl;

		} else
			std::cerr << "ERROR: File '" << filepath.toStdString() << "' doesn't exist...?" << std::endl;

		animate();

	} else {

		if(verbose) std::clog << "fhd: Hard delete file" << std::endl;

		// current file
		QFile file(currentfile);

		// Delete it if it exists (if it got here, the file should exist)
		if(file.exists()) {

			file.remove();
			animate();
			emit reloadDir("delete");

		} else {
			animate();
			std::cerr << "ERROR! File '" << currentfile.toStdString() << "' doesn't exist...?" << std::endl;
		}

	}

#else

	if(verbose) std::clog << "fhd: Delete file" << std::endl;

	// current file
	QFile file(currentfile);

	// Delete it if it exists (if it got here, the file should exist)
	if(file.exists()) {

		file.remove();
		animate();
		emit reloadDir("delete");

	} else {
		animate();
		std::cerr << "ERROR! File doesn't exist...?" << std::endl;
	}


#endif


}

void FileHandling::doMove() {

	QString newfilename = moveTreeModel->filePath(moveTree->selectionModel()->selectedIndexes().at(0)) + "/" + moveNewName->text() + moveNewNameEnding->text();

	if(verbose) std::clog << "fhd: Move file: " << currentfile.toStdString() << " - " << newfilename.toStdString() << std::endl;

	QFile file(currentfile);

	if(file.copy(newfilename)) {
		if(!file.remove()) {
			animate();
			std::cerr << "ERROR: Couldn't remove old file" << std::endl;
			if(QFileInfo(newfilename).absolutePath() == QFileInfo(currentfile).absolutePath())
				emit reloadDir("rename");
		} else {
			animate();
			if(QFileInfo(newfilename).absolutePath() == QFileInfo(currentfile).absolutePath())
				emit reloadDir("rename");
			else
				emit reloadDir("delete");
		}

	} else {
		animate();
		std::cerr << "ERROR: Couldn't move file" << std::endl;
	}

}

void FileHandling::doCopy() {

	QString newfilename = copyTreeModel->filePath(copyTree->selectionModel()->selectedIndexes().at(0)) + "/" + copyNewName->text() + copyNewNameEnding->text();

	if(verbose) std::clog << "fhd: Copy file: " << currentfile.toStdString() << " - " << newfilename.toStdString() << std::endl;

	QFile file(currentfile);

	if(file.copy(newfilename)) {

		animate();
		if(QFileInfo(newfilename).absolutePath() == QFileInfo(currentfile).absolutePath())
			emit reloadDir("rename");

	} else {
		animate();
		std::clog << "ERROR: Couldn't copy file" << std::endl;
	}

}

// Validate the new filename for renaming a file
void FileHandling::validateRenameFilename() {

	// If the new filename is not the same as the old name (i.e. no change)
	if(renameNewName->text() != QFileInfo(currentfile).baseName()) {

		// The full new filename
		QString newfile = QFileInfo(currentfile).absolutePath() + "/" + renameNewName->text() + renameOldEnding->text();

		// If file with new filename exists, show label showing this info and disable save button
		if(QFile(newfile).exists()) {
			renameNewNameExists->setText("<center>" + tr("File with this name already exists!") + "</center>");
			renameSave->setEnabled(false);
			renameSave->setToolTip(tr("File with this name already exists!"));
		} else {
			renameNewNameExists->setText(" ");
			renameSave->setEnabled(true);
			renameSave->setToolTip(tr("Rename file"));
		}

	// No change, i.e. save button is disabled
	} else {
		renameNewNameExists->setText(" ");
		renameSave->setEnabled(false);
		renameSave->setToolTip(tr("You need to specify a different name"));
	}

}

void FileHandling::validateMoveAndCopyFilename() {

	if(dialogType == "move") {

		QString newfilename = moveTreeModel->filePath(moveTree->selectionModel()->selectedIndexes().at(0)) + "/" + moveNewName->text() + moveNewNameEnding->text();

		if(currentfile != newfilename) {

			if(QFile(newfilename).exists()) {
				moveNewNameExists->setText("<center>" + tr("File with this name already exists!") + "</center>");
				moveSave->setEnabled(false);
				moveSave->setToolTip(tr("File with this name already exists!"));
			} else {
				moveNewNameExists->setText(" ");
				moveSave->setEnabled(true);
				moveSave->setToolTip(tr("Move File"));
			}

		} else {
			moveNewNameExists->setText(" ");
			moveSave->setEnabled(false);
			moveSave->setToolTip(tr("You need to specify a different file/name"));
		}

	} else if(dialogType == "copy") {

		QString newfilename = copyTreeModel->filePath(copyTree->selectionModel()->selectedIndexes().at(0)) + "/" + copyNewName->text() + copyNewNameEnding->text();

		if(currentfile != newfilename) {

			if(QFile(newfilename).exists()) {
				copyNewNameExists->setText("<center>" + tr("File with this name already exists!") + "</center>");
				copySave->setEnabled(false);
				copySave->setToolTip(tr("File with this name already exists!"));
			} else {
				copyNewNameExists->setText(" ");
				copySave->setEnabled(true);
				copySave->setToolTip(tr("Copy File"));
			}

		} else {
			copySave->setEnabled(false);
			copyNewNameExists->setText(" ");
			copySave->setToolTip(tr("You need to specify a different file/name"));
		}

	}

}


void FileHandling::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

// Click on background closes dialog
void FileHandling::mouseReleaseEvent(QMouseEvent *) {

	if(dialogType == "rename")
		renameCancel->animateClick();
	else if(dialogType == "delete")
		deleteNo->animateClick();

}


FileHandling::~FileHandling() { }
