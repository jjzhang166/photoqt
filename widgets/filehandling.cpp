/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "filehandling.h"
#include <unistd.h>
#include <iostream>

FileHandling::FileHandling(QWidget *parent, bool v, QString cf) : MyWidget(parent) {

	this->setVisibleArea(QSize(600,400));

	// Some variables
	currentfile = cf;
	verbose = v;

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
	renameTitle = new CustomLabel("<center>" + tr("Rename File") + "</center>");
	renameTitle->setFontSize(25);
	renameTitle->setBold(true);
	renameOldName = new CustomLabel;
	renameOldName->setFontSize(15);
	renameOldName->setItalic(true);
	renameOldName->setFontColor("grey");
	renameOldName->setPadding(10);
	QHBoxLayout *renameNewLay = new QHBoxLayout;
	renameNewName = new CustomLineEdit;
	renameNewName->setFixedWidth(300);
	renameNewName->setFontSize(15);
	renameOldEnding = new CustomLabel;
	renameOldEnding->setFontSize(15);
	renameNewLay->addStretch();
	renameNewLay->addWidget(renameNewName);
	renameNewLay->addWidget(renameOldEnding);
	renameNewLay->addStretch();

	renameNewNameExists = new CustomLabel(" ");
	renameNewNameExists->setFontColor("red");
	renameNewNameExists->setBold(true);

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
	deleteTitle = new CustomLabel("<center>" + tr("Delete File") + "</center>");
	deleteTitle->setFontSize(15);
	deleteTitle->setBold(true);
	deleteFilename = new CustomLabel(" ");
	deleteFilename->setFontSize(15);
	deleteFilename->setItalic(true);
	deleteFilename->setFontColor("grey");
	deleteFilename->setPadding(10);
	deleteQuestion = new CustomLabel("<center>" + tr("Do you really want to delete this file?") +"</center>");
	deleteQuestion->setFontSize(15);
	deleteQuestion->setPadding(10);



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
	deleteYes = new CustomPushButton;
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
	moveTitle = new CustomLabel("<center>" + tr("Moving File") + "</center>");
	moveTitle->setFontSize(25);
	moveTitle->setBold(true);

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

	moveNewNameExists = new CustomLabel(" ");
	moveNewNameExists->setFontColor("red");
	moveNewNameExists->setBold(true);

	moveNewNameLabel = new CustomLabel(tr("Filename") + ":");
	moveNewNameLabel->setFontSize(12);
	moveNewNameLabel->setBold(true);
	moveNewName = new CustomLineEdit;
	moveNewName->setFixedWidth(300);
	moveNewName->setFontSize(12);
	moveNewNameEnding = new CustomLabel(".xyz");
	moveNewNameEnding->setFontSize(12);
	moveNewNameEnding->setBold(true);

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
	copyTitle = new CustomLabel("<center>" + tr("Copying File") + "</center>");
	copyTitle->setFontSize(25);
	copyTitle->setBold(true);

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

	copyNewNameExists = new CustomLabel(" ");
	copyNewNameExists->setFontColor("red");
	copyNewNameExists->setBold(true);

	copyNewNameLabel = new CustomLabel(tr("Filename") + ":");
	copyNewNameLabel->setFontSize(12);
	copyNewNameLabel->setBold(true);
	copyNewName = new CustomLineEdit;
	copyNewName->setFixedWidth(300);
	copyNewName->setFontSize(12);
	copyNewNameEnding = new CustomLabel(".xyz");
	copyNewNameEnding->setFontSize(12);
	copyNewNameEnding->setBold(true);

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
		renameWidget->setGeometry(getRectHidden());
		deleteWidget->setGeometry(getRectHidden());
		moveWidget->setGeometry(getRectHidden());
		copyWidget->setGeometry(getRectHidden());

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
		makeShow();

	}

}


// Handle widget when animation is finished
void FileHandling::aniFinished() {

	// Move widget out of screen
	if(!isVisible()) {
		this->setGeometry(getRectHidden());
		emit widgetClosed();
		if(dialogType == "rename")
			renameNewName->setEnabled(false);
		else if(dialogType == "move")
			moveNewName->setEnabled(false);
	}

	if(isVisible()) {

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

	// Adjust target of animation
	resetAnimationTarget(renameWidget);

	// Validate new filename (at this point the old name, i.e. save button is disabled)
	validateRenameFilename();

}

// Set the delete dialog
void FileHandling::setDelete() {

	if(verbose) std::clog << "fhd: Set delete layout" << std::endl;

	// Update old filename
	deleteFilename->setText("<center>" + QFileInfo(currentfile).fileName() + "</center>");

	// Adjust target of animation
	resetAnimationTarget(deleteWidget);

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

	resetAnimationTarget(copyWidget);

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

	resetAnimationTarget(moveWidget);

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
			makeHide();
			currentfile = newfile;
			emit reloadDir("rename");
		} else {
			std::cerr << "ERROR! Couldn't rename file" << std::endl;
			makeHide();
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
		makeHide();
		emit reloadDir("delete");

	} else {
		makeHide();
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
			makeHide();
			std::cerr << "ERROR: Couldn't remove old file" << std::endl;
			if(QFileInfo(newfilename).absolutePath() == QFileInfo(currentfile).absolutePath())
				emit reloadDir("rename");
		} else {
			makeHide();
			if(QFileInfo(newfilename).absolutePath() == QFileInfo(currentfile).absolutePath())
				emit reloadDir("rename");
			else
				emit reloadDir("delete");
		}

	} else {
		makeHide();
		std::cerr << "ERROR: Couldn't move file" << std::endl;
	}

}

void FileHandling::doCopy() {

	QString newfilename = copyTreeModel->filePath(copyTree->selectionModel()->selectedIndexes().at(0)) + "/" + copyNewName->text() + copyNewNameEnding->text();

	if(verbose) std::clog << "fhd: Copy file: " << currentfile.toStdString() << " - " << newfilename.toStdString() << std::endl;

	QFile file(currentfile);

	if(file.copy(newfilename)) {

		makeHide();
		if(QFileInfo(newfilename).absolutePath() == QFileInfo(currentfile).absolutePath())
			emit reloadDir("rename");

	} else {
		makeHide();
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

FileHandling::~FileHandling() { }
