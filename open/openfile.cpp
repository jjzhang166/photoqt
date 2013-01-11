#include "openfile.h"

Open::Open(QWidget *parent) : QWidget(parent) {

	// Style the widget
	this->setStyleSheet("background-color: rgba(0,0,0,200); ");

	// The different QRects for its shapes
	QRect desk = QApplication::desktop()->screenGeometry(1);
	rectHidden = QRect(0,-10,10,10);
	aniStart = QRect(desk.width()/2.0,desk.height()/2.0,1,1);
	rectShown = QRect(0,0,desk.width(),desk.height());


	// The item hovered
	pathHovered = "";

	// The widget is shown?
	isShown = false;

	// Setup the threads
	threadNumber = 0;
	openThreadNum = 10;
	for(int i = 0; i < openThreadNum; ++i) {
		ThumbThread *ot = new ThumbThread;
		connect(ot, SIGNAL(updateThb(QImage,QString,int)), this, SLOT(updateThumb(QImage,QString,int)));
		openThreadList << ot;
	}

	// The animation framework
	ani = new QPropertyAnimation(this,"geometry");
	QTimer *afterAniFin = new QTimer;
	afterAniFin->setSingleShot(true);
	afterAniFin->setInterval(200);
	connect(afterAniFin, SIGNAL(timeout()), this, SLOT(aniFinished()));
	connect(ani, SIGNAL(finished()),afterAniFin, SLOT(start()));

	// We start out by the home directory
	dir = QDir::homePath();

	QVBoxLayout *leftLay = new QVBoxLayout;

	// The filesystemtree
	fileSysTree = new QTreeView;
	fileSysTree->setFixedWidth(300);
	fileSysTree->setHeaderHidden(true);
	QString css = "QTreeView {";
	css += "color:white;";
	css += "background: rgba(0,0,0,0);";
	css += "show-decoration-selected: 1;";
	css += "}";

	css += "QTreeView::item:hover{";
	css += "background: rgba(255,255,255,128);";
	css += "color: black";
	css += "}";

	css += "QTreeView::item:selected{";
	css += "background: white;";
	css += "color: black;";
	css += "}";

	css += "QTreeView::item:selected:active{";
	css += "background: white;";
	css += "color: black";
	css += "}";

	css += "QTreeView::item:selected:!active{";
	css += "background: white;";
	css += "color: black";
	css += "}";
	fileSysTree->setStyleSheet(css);

	fileSysTreeScrollbar = new CustomScrollbar;
	fileSysTree->setVerticalScrollBar(fileSysTreeScrollbar);

	fileSysModel = new QFileSystemModel;
	fileSysModel->setRootPath(QDir::rootPath());
	fileSysModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	fileSysTree->setModel(fileSysModel);

	// We load the home directory at startup
	setDirFileSysTree(QDir::homePath());

	for(int i = 1; i < fileSysModel->columnCount(); ++i)
		fileSysTree->setColumnHidden(i,true);

	leftLay->addWidget(fileSysTree);
	leftLay->addSpacing(20);

	// A checkbox for hidden files/folders
	showHiddenFolders = new CustomCheckBox(tr("Show Hidden Files/Folders"));
	connect(showHiddenFolders, SIGNAL(clicked()), this, SLOT(setChkBoxToggled()));
	QHBoxLayout *showHiddenLay = new QHBoxLayout;
	showHiddenLay->setAlignment(Qt::AlignCenter);
	showHiddenLay->addWidget(showHiddenFolders);
	leftLay->addLayout(showHiddenLay);
	leftLay->addSpacing(10);

	// Automatically load thumbnails on directory load
	autoLoadThumbs = new CustomCheckBox(tr("Load Thumbnails"));
	connect(autoLoadThumbs, SIGNAL(clicked()), this, SLOT(setChkBoxToggled()));
	QHBoxLayout *autoLoadLay = new QHBoxLayout;
	autoLoadLay->setAlignment(Qt::AlignCenter);
	autoLoadLay->addWidget(autoLoadThumbs);
	leftLay->addLayout(autoLoadLay);
	leftLay->addSpacing(10);

	// Some buttons
	CustomPushButton *stopThbBut = new CustomPushButton(tr("Interrupt Thumbnail Creation"));
	connect(stopThbBut, SIGNAL(clicked()), this, SLOT(stopThb()));
	CustomPushButton *refreshDir = new CustomPushButton(tr("Refresh directory"));
	connect(refreshDir, SIGNAL(clicked()), this, SLOT(loadDir()));
	leftLay->addWidget(stopThbBut);
	leftLay->addSpacing(10);
	leftLay->addWidget(refreshDir);
	leftLay->addSpacing(10);

	CustomPushButton *cancel = new CustomPushButton(tr("Cancel and Close Widget"));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));
	leftLay->addWidget(cancel);

	// The main graphicsview
	view = new QGraphicsView;
	view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	view->setStyleSheet("background: transparent");
	CustomScrollbar *viewScroll = new CustomScrollbar;
	view->setVerticalScrollBar(viewScroll);
	view->setScene(&scene);

	// The main layout
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addLayout(leftLay);
	layout->addWidget(view);

	// The crumbs on top
	QVBoxLayout *crumbsLay = new QVBoxLayout;
	crumbs = new Crumbs;

	crumbsLay->addWidget(crumbs);
	crumbsLay->addLayout(layout);
	this->setLayout(crumbsLay);

	connect(crumbs, SIGNAL(clicked(QString)), this, SLOT(crumbsClicked(QString)));
	connect(fileSysTree, SIGNAL(clicked(QModelIndex)), this, SLOT(fileSysTreeClicked(QModelIndex)));

}

// Animate open widget
void Open::animate() {

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		emit stopThbCreation();

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(aniStart);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		emit blockFunc(true);

		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		stopThb();

		ani->setDuration(300);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(aniStart);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

		emit blockFunc(false);

	}

}

// Animation of the widget finished
void Open::aniFinished() {

	if(isShown) {
		loadDir();
		this->raise();
	}

}

// A crumb has been clicked
void Open::crumbsClicked(QString path) {

	pathHovered = "";

	stopThb();

	if(path.endsWith("/") && path != "/") {
		QStringList pathTmp = path.split("/");
		pathTmp.removeLast();
		path = pathTmp.join("/");
	}
	dir = path;
	loadDir();

}

// A click on a filesystem row
void Open::fileSysTreeClicked(QModelIndex clkd) {

	dir = fileSysModel->filePath(clkd);

	loadDir();

}

// Got a click on an item
void Open::gotClick(QString itemType, QString path) {

	pathHovered = "";

	stopThb();

	// If a folder was clicked - load folder
	if(itemType == "folder") {

		dir = path;
		loadDir();

	// If image was clicked - load image
	} else if(itemType == "image") {

		animate();
		emit loadNewImg(QFileInfo(path).absoluteFilePath());
		dir = QFileInfo(path).absolutePath();

	}

}

// Enter/Return loads the currently hovered item
void Open::gotEnter() {

	if(pathHovered != "") {

		if(allPixFold.keys().contains(pathHovered))
			gotClick(allPixFold[pathHovered]->itemType, allPixFold[pathHovered]->path);
		else if(allPixImg.keys().contains(pathHovered))
			gotClick(allPixImg[pathHovered]->itemType, allPixImg[pathHovered]->path);

	}

}

// Load the directory
void Open::loadDir() {

	setDirFileSysTree(dir);

	// Get the amount of crumbs to be shown
	crumbs->numCrumbsShown = globSet.value("NumCrumbsVisible").toInt();
	crumbs->setCrumbs(dir);

	scene.clear();

	// Stop all possibly still running threads
	for(int i = 0; i < openThreadNum; ++i)  {
		if(i != threadNumber)
			openThreadList.at(i)->breakme = true;
	}

	// How many items in a row?
	int c = view->width()/120;

	allPixFold.clear();
	allPixImg.clear();

	// Are hidden folders to be loaded
	QDir::Filters filters;
	if(globSet.value("OpenFileShowHiddenFolders").toBool())
		filters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden;
	else
		filters = QDir::Dirs | QDir::NoDotAndDotDot;

	// Iterate through all the folders. Using QDirIterator appeared to be a  little faster than using QDir::entry(Info)List (certainly not slower)
	QDirIterator itFold(dir,filters);
	while(itFold.hasNext()) {

		itFold.next();

		// The current item name
		QString str = itFold.fileName();

		// Current size
		int width = 120;
		int height = 90;

		// The normal folder
		QPixmap folderNorm(width,height);
		folderNorm.fill(Qt::transparent);
		QPainter paint(&folderNorm);
		paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
		paint.fillRect(0,0,width,height,QColor::fromRgba(qRgba(255,255,255,1)));
		QIcon f(QIcon::fromTheme("folder"));
		paint.drawPixmap(29,14,62,62,f.pixmap(80,80));
		QTextDocument txt;
		txt.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white\">" + str + "</div></center>");
		txt.setTextWidth(120);
		paint.translate(3,height*2/3.0);
		txt.drawContents(&paint);
		paint.end();

		// The hovered folder
		QPixmap folderHov(width,height);
		folderHov.fill(Qt::transparent);
		QPainter paintSel(&folderHov);
		paintSel.setCompositionMode(QPainter::CompositionMode_SourceOver);
		paintSel.fillRect(0,0,width,height,QColor::fromRgba(qRgba(255,255,255,100)));
		QIcon fSel(QIcon::fromTheme("folder"));
		paintSel.drawPixmap(30,15,60,60,fSel.pixmap(80,80));
		QTextDocument txtSel;
		txtSel.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white\">" + str + "</div></center>");
		txtSel.setTextWidth(120);
		paintSel.translate(3,height*2/3.0);
		txtSel.drawContents(&paintSel);
		paintSel.end();

		// Create Pixmap
		OpenPixmap *pixFold = new OpenPixmap;
		pixFold->setPixmap(folderNorm,folderHov);
		pixFold->itemType = "folder";
		pixFold->path = itFold.filePath();

		allPixFold.insert(itFold.fileName(),pixFold);

		connect(pixFold, SIGNAL(clicked(QString,QString)), this, SLOT(gotClick(QString,QString)));
		connect(pixFold, SIGNAL(newHover(QString)), this, SLOT(newHover(QString)));

	}

	QDir::Filters filtersFiles;
	if(globSet.value("OpenFileShowHiddenFolders").toBool())
		filtersFiles = QDir::Files | QDir::Hidden;
	else
		filtersFiles = QDir::Files;
	// We get the list of known image types from the global settings
	QStringList flt = globSet.value("KnownFileTypes").toString().split(",");
	QDirIterator itImag(dir,flt, filtersFiles);
	while(itImag.hasNext()) {

		itImag.next();

		// The current item name
		QString str = itImag.fileName();

		// The current item size
		int width = 120;
		int height = 90;

		// The normal image pixmap
		QPixmap imgNorm(width,height);
		imgNorm.fill(Qt::transparent);
		QPainter paint(&imgNorm);
		paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
		paint.fillRect(0,0,width,height,QColor::fromRgba(qRgba(255,255,255,1)));
		QIcon f(QIcon::fromTheme("image-x-generic"));
		paint.drawPixmap(29,14,62,62,f.pixmap(80,80));
		QTextDocument txt;
		txt.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white\">" + str + "</div></center>");
		txt.setTextWidth(120);
		paint.translate(3,height*2/3.0);
		txt.drawContents(&paint);
		paint.end();

		// The hovered image pixmap
		QPixmap imgHov(width,height);
		imgHov.fill(Qt::transparent);
		QPainter paintSel(&imgHov);
		paintSel.setCompositionMode(QPainter::CompositionMode_SourceOver);
		paintSel.fillRect(0,0,width,height,QColor::fromRgba(qRgba(255,255,255,100)));
		QIcon fSel(QIcon::fromTheme("image-x-generic"));
		paintSel.drawPixmap(30,15,60,60,fSel.pixmap(80,80));
		QTextDocument txtSel;
		txtSel.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white\">" + str + "</div></center>");
		txtSel.setTextWidth(120);
		paintSel.translate(3,height*2/3.0);
		txtSel.drawContents(&paintSel);
		paintSel.end();

		// This detects if width or height has to be used for adjusting preview in tooltips
		QString imgPrevDim = "width";
		if(QImageReader(itImag.filePath()).size().height() > QImageReader(itImag.filePath()).size().width())
			imgPrevDim = "height";

		// The tooltip of each item
		QString tooltip = "<div align=\"center\" style=\"background-color: rgba(0,0,0,230); padding-left: 15px; border-radius: 3px;\"><br>&nbsp;<b><span style=\"font-size: 13pt\">" + str + "</span></b>&nbsp;<br><br>&nbsp;&nbsp;<img " + imgPrevDim + "=300 src=\"" + itImag.filePath() + "\">&nbsp;&nbsp;<br><br>";
		tooltip += QString("&nbsp;<b>" + tr("Size") + "</b>:  %1 MB<br>").arg(qRound(10*(float(itImag.fileInfo().size())/1024.0/1024.0))/10.0);
		tooltip += QString("&nbsp;<b>" + tr("Width") + ":</b> %1 px&nbsp;<br>").arg(QImageReader(itImag.filePath()).size().width());
		tooltip += QString("&nbsp;<b>" + tr("Height") + ":</b> %1 px&nbsp;<br>").arg(QImageReader(itImag.filePath()).size().height());
		tooltip += QString("&nbsp;<b>" + tr("Created") + ":</b> %1&nbsp;<br>").arg(itImag.fileInfo().created().toString());
		tooltip += QString("&nbsp;<b>" + tr("Last Changed") + ":</b> %1&nbsp;<br></div>").arg(itImag.fileInfo().lastModified().toString());

		// Create the Pixmap
		OpenPixmap *pixImg = new OpenPixmap;
		pixImg->setPixmap(imgNorm,imgHov);
		pixImg->itemType = "image";
		pixImg->path = itImag.filePath();
		pixImg->setToolTip(tooltip);

		allPixImg.insert(itImag.fileName(),pixImg);

		connect(pixImg, SIGNAL(clicked(QString,QString)), this, SLOT(gotClick(QString,QString)));
		connect(pixImg, SIGNAL(newHover(QString)), this, SLOT(newHover(QString)));

	}

	int row = 1;
	int column = 0;

	// Load all folder items and add them to the graphicsview
	QStringList foldKeys = allPixFold.keys();
	foldKeys.sort();
	for(int i = 0; i < foldKeys.length(); ++i) {

		allPixFold.value(foldKeys.at(i))->setPos(column*120,row*90);

		// Get pos of next item
		++column;
		if(column == c) {
			column = 0;
			row += 1;
		}

		// Add item to GraphicsView
		scene.addItem(allPixFold.value(foldKeys.at(i)));

	}

	// This list is passed on to the thread
	QFileInfoList fileinfoList;

	// Load all image items and add them to the graphicsview
	QStringList imgKeys = allPixImg.keys();
	imgKeys.sort();
	for(int i = 0; i < imgKeys.length(); ++i) {

		fileinfoList << QFileInfo(dir + "/" + imgKeys.at(i));

		allPixImg.value(imgKeys.at(i))->setPos(column*120,row*90);

		// Get pos of next item
		++column;
		if(column == c) {
			column = 0;
			row += 1;
		}

		// Add item to GraphicsView
		scene.addItem(allPixImg.value(imgKeys.at(i)));

	}

	scene.setSceneRect(scene.itemsBoundingRect());


	// If the thumbnails are to be loaded straight away
	if(autoLoadThumbs->isChecked()) {

		if(threadNumber >= openThreadNum)
			threadNumber = 0;

		openThreadList.at(threadNumber)->counttot = allPixImg.keys().length();
		openThreadList.at(threadNumber)->allimgs.clear();
		openThreadList.at(threadNumber)->allimgs.append(fileinfoList);
		openThreadList.at(threadNumber)->height = 80;
		if(globSet.value("ThumbnailCache").toBool())
			openThreadList.at(threadNumber)->cacheEnabled = true;
		else
			openThreadList.at(threadNumber)->cacheEnabled = false;
		openThreadList.at(threadNumber)->breakme = false;
		openThreadList.at(threadNumber)->dbName = "open1";

		if(globSet.value("ThbCacheFile").toBool())
			openThreadList.at(threadNumber)->typeCache = "files";
		else
			openThreadList.at(threadNumber)->typeCache = "database";

		if(openThreadList.at(threadNumber)->counttot)
			openThreadList.at(threadNumber)->start();

		threadNumber += 1;
		if(threadNumber == openThreadNum)
			threadNumber = 0;

	}

	QCursor::setPos(QCursor::pos().x()+1,QCursor::pos().y());

}

// Move the selection (by keyboard)
void Open::moveSel(QString direction) {

	if(direction == "left") {

		if(pathHovered == "") {
			if(allPixImg.keys().length()) {
				pathHovered = allPixImg.keys().last();
				allPixImg[pathHovered]->setHover();
			} else if(allPixFold.keys().length()) {
				pathHovered = allPixFold.keys().last();
				allPixFold[pathHovered]->setHover();
			}
		} else if(allPixImg.keys().contains(pathHovered)) {

			if(allPixImg.keys().indexOf(pathHovered) == 0 && allPixFold.keys().length()) {
				allPixImg[pathHovered]->setNormal();
				pathHovered = allPixFold.keys().last();
				allPixFold[pathHovered]->setHover();
			} else {
				allPixImg[pathHovered]->setNormal();
				pathHovered = allPixImg.keys().at(allPixImg.keys().indexOf(pathHovered)-1);
				allPixImg[pathHovered]->setHover();
			}
		} else if(allPixFold.keys().contains(pathHovered)) {

			if(allPixFold.keys().indexOf(pathHovered) > 0) {
				allPixFold[pathHovered]->setNormal();
				pathHovered = allPixFold.keys().at(allPixFold.keys().indexOf(pathHovered)-1);
				allPixFold[pathHovered]->setHover();
			}

		}

	} else if(direction == "right") {

		if(pathHovered == "") {
			if(allPixFold.keys().length()) {
				pathHovered = allPixFold.keys().first();
				allPixFold[pathHovered]->setHover();
			} else if(allPixImg.keys().length()) {
				pathHovered = allPixImg.keys().first();
				allPixImg[pathHovered]->setHover();
			}
		} else if(allPixFold.keys().contains(pathHovered)) {

			if(allPixFold.keys().indexOf(pathHovered) == allPixFold.keys().length()-1 && allPixImg.keys().length()) {
				allPixFold[pathHovered]->setNormal();
				pathHovered = allPixImg.keys().first();
				allPixImg[pathHovered]->setHover();
			} else {
				allPixFold[pathHovered]->setNormal();
				pathHovered = allPixFold.keys().at(allPixFold.keys().indexOf(pathHovered)+1);
				allPixFold[pathHovered]->setHover();
			}

		} else if(allPixImg.keys().contains(pathHovered)) {

			if(allPixImg.keys().indexOf(pathHovered) < allPixImg.keys().length()-1) {
				allPixImg[pathHovered]->setNormal();
				pathHovered = allPixImg.keys().at(allPixImg.keys().indexOf(pathHovered)+1);
				allPixImg[pathHovered]->setHover();
			}
		}

	} else if(direction == "up") {

		int c = view->width()/120;

		if(pathHovered == "") {
			if(allPixImg.keys().length()) {
				pathHovered = allPixImg.keys().last();
				allPixImg[pathHovered]->setHover();
			} else if(allPixFold.keys().length()) {
				pathHovered = allPixFold.keys().last();
				allPixFold[pathHovered]->setHover();
			}
		} else if(allPixFold.keys().contains(pathHovered)) {

			if(allPixFold.keys().indexOf(pathHovered) >= c) {
				allPixFold[pathHovered]->setNormal();
				pathHovered = allPixFold.keys().at(allPixFold.keys().indexOf(pathHovered)-c);
				allPixFold[pathHovered]->setHover();
			}

		} else if(allPixImg.keys().contains(pathHovered)) {

			if(allPixImg.keys().indexOf(pathHovered) >= c) {
				allPixImg[pathHovered]->setNormal();
				pathHovered = allPixImg.keys().at(allPixImg.keys().indexOf(pathHovered)-c);
				allPixImg[pathHovered]->setHover();
			} else if(allPixImg.keys().indexOf(pathHovered)+allPixFold.keys().length() >= c) {
				allPixImg[pathHovered]->setNormal();
				pathHovered = allPixFold.keys().at(allPixFold.keys().length()-(c-allPixImg.keys().indexOf(pathHovered)));
				allPixFold[pathHovered]->setHover();
			}

		}

	} else if(direction == "down") {

		int c = view->width()/120;

		if(pathHovered == "") {
			if(allPixFold.keys().length()) {
				pathHovered = allPixFold.keys().first();
				allPixFold[pathHovered]->setHover();
			} else if(allPixImg.keys().length()) {
				pathHovered = allPixImg.keys().first();
				allPixImg[pathHovered]->setHover();
			}
		} else if(allPixImg.keys().contains(pathHovered)) {

			if(allPixImg.keys().indexOf(pathHovered) < allPixImg.keys().length()-c) {
				allPixImg[pathHovered]->setNormal();
				pathHovered = allPixImg.keys().at(allPixImg.keys().indexOf(pathHovered)+c);
				allPixImg[pathHovered]->setHover();
			}

		} else if(allPixFold.keys().contains(pathHovered)) {

			if(allPixFold.keys().indexOf(pathHovered) < allPixFold.keys().length()-c) {
				allPixFold[pathHovered]->setNormal();
				pathHovered = allPixFold.keys().at(allPixFold.keys().indexOf(pathHovered)+c);
				allPixFold[pathHovered]->setHover();
			} else if(allPixFold.keys().length()-allPixFold.keys().indexOf(pathHovered)+allPixImg.keys().length() > c) {
				allPixFold[pathHovered]->setNormal();
				pathHovered = allPixImg.keys().at(c-(allPixFold.keys().length()-allPixFold.keys().indexOf(pathHovered)));
				allPixImg[pathHovered]->setHover();
			}

		}

	}

}

// A new item is hovered
void Open::newHover(QString path) {

	if(path != "") {

		if(allPixFold.keys().contains(pathHovered))
			allPixFold[pathHovered]->setNormal();
		else if(allPixImg.keys().contains(pathHovered))
			allPixImg[pathHovered]->setNormal();

		if(allPixFold.keys().contains(path))
			allPixFold[path]->setHover();
		else if(allPixImg.keys().contains(path))
			allPixImg[path]->setHover();

		pathHovered = QFileInfo(path).fileName();

	} else
		pathHovered = "";

}

// Adjust the checkbox after the settings have been loaded/changed
void Open::setChkBoxToggled() {

	globSet["OpenFileAutoLoadThumb"] = autoLoadThumbs->isChecked();
	globSet["OpenFileShowHiddenFolders"] = showHiddenFolders->isChecked();

	QMap<QString,QVariant> changedSet;
	changedSet.insert("OpenFileAutoLoadThumb",autoLoadThumbs->isChecked());
	changedSet.insert("OpenFileShowHiddenFolders",showHiddenFolders->isChecked());

	if(!autoLoadThumbs->isChecked())
		stopThb();


	loadDir();

	emit settingChanged(changedSet);

}

// Select and open the right directory in the filesystemtree
void Open::setDirFileSysTree(QString d) {

	QModelIndex first = fileSysModel->index(d);
	fileSysTree->setCurrentIndex(first);
	fileSysTree->setExpanded(first,true);

}

// Stop the thumbnail creation
void Open::stopThb() {

	for(int i = 0; i < openThreadNum; ++i)
		openThreadList.at(i)->breakme = true;

}

// Update a thumbnail (coming from thread)
void Open::updateThumb(QImage p, QString f, int) {

	// Default size
	int width = 120;
	int height = 90;

	// The thumbnail image
	QPixmap pix(100,100);
	pix.fill(Qt::transparent);
	QPixmap thb = QPixmap::fromImage(p);
	QPainter pixp(&pix);

	// The position of the first item
	int x = 0;
	int y = 0;
	int w = 100;
	int h = 100;

	// We position the thb according to it's size
	if(thb.width() > thb.height()) {
		h = int((100.0/float(width))*pix.height());
		y = (100-h)/2;
	} else {
		w = int((100.0/float(height))*pix.width());
		x = (100-w)/2;
	}

	// Draw the pixmap
	pixp.drawPixmap(x,y,w,h,thb);
	pixp.end();

	// The normal image
	QPixmap imgNorm(width,height);
	imgNorm.fill(Qt::transparent);
	QPainter paint(&imgNorm);
	paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
	paint.fillRect(0,0,width,height,QColor::fromRgba(qRgba(255,255,255,1)));
	paint.drawPixmap(29,14,62,62,pix);
	QTextDocument txt;
	txt.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white\">" + QFileInfo(f).fileName() + "</div></center>");
	txt.setTextWidth(120);
	paint.translate(3,height*2/3.0);
	txt.drawContents(&paint);
	paint.end();

	// The hover image
	QPixmap imgHov(width,height);
	imgHov.fill(Qt::transparent);
	QPainter paintSel(&imgHov);
	paintSel.setCompositionMode(QPainter::CompositionMode_SourceOver);
	paintSel.fillRect(0,0,width,height,QColor::fromRgba(qRgba(255,255,255,100)));
	paintSel.drawPixmap(30,15,60,60,pix);
	QTextDocument txtSel;
	txtSel.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white\">" + QFileInfo(f).fileName() + "</div></center>");
	txtSel.setTextWidth(120);
	paintSel.translate(3,height*2/3.0);
	txtSel.drawContents(&paintSel);
	paintSel.end();

	if(dir == QFileInfo(f).absolutePath())
		allPixImg[QFileInfo(f).fileName()]->setPixmap(imgNorm, imgHov);

}


// Enabling styling of widget
void Open::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

Open::~Open() { }
