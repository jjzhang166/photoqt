#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, bool verbose) : QMainWindow(parent) {

	// Needed to catch the Alt+F4. In fact we only catch Alt+closeEvent (see eventFilter())
	this->installEventFilter(this);

	// Make a screenshot
	screenshot = QPixmap::grabWindow(QApplication::desktop()->winId());

	// Black Background
	this->setObjectName("mainwindow");
	this->setStyleSheet("#mainwindow { background: black; }");
	this->setMouseTracking(true);

	// Instance for global variables
	globVar = new GlobalVariables;
	globVar->verbose = verbose;
	globVar->setVariables();

	// Instance for global settings
	globSet = new GlobalSettings;
	globSet->verbose = verbose;
	globSet->readSettings();

	// Central Widget
	QWidget *central = new QWidget;
	central->setStyleSheet("border: none; background:transparent");
	QVBoxLayout *bglayout = new QVBoxLayout;
	bglayout->setMargin(0);
	bglabel = new QLabel;
	bglabel->setMargin(0);
	bglayout->addWidget(bglabel);
	central->setLayout(bglayout);
	this->setCentralWidget(central);
	QVBoxLayout *centralLayout = new QVBoxLayout;
	centralLayout->setMargin(0);
	bglabel->setLayout(centralLayout);

	// The Item holding the big pixmap item
	graphItem = new GraphicsItem;
	sceneBig.addItem(graphItem);

	// The two GraphicViews
	viewBig = new GraphicsView(globSet->toSignalOut(),bglabel);
	viewBig->setAlignment(Qt::AlignTop);
	viewThumbs = new Thumbnails(viewBig,globSet->verbose,globSet->toSignalOut());

	// This widget is the front widget
	viewBig->raise();

	// A mouse action triggered a shortcut
	connect(viewBig, SIGNAL(shMouseDo(QString,bool)), this, SLOT(shortcutDO(QString,bool)));
	connect(viewBig, SIGNAL(loadContextMenuAction(QString)), this, SLOT(shortcutDO(QString)));

	// The settings widget
	set = new Settings(viewBig,globSet->toSignalOut());

	// The file handling dialog
	filehandling = new FileHandling(viewBig);
	connect(filehandling, SIGNAL(reloadDir(QString)), this, SLOT(reloadDir(QString)));
	connect(filehandling, SIGNAL(stopThbCreation()), viewThumbs, SLOT(stopThbCreation()));

	// Connect some globSet signals
	connect(globSet, SIGNAL(updateSettingsData(QMap<QString,QVariant>)), this, SLOT(updateSettings(QMap<QString,QVariant>)));
	connect(globSet, SIGNAL(updateSettingsToApply(QMap<QString,bool>)), this, SLOT(applySettings(QMap<QString,bool>)));

	// The drop-down menu
	menu = new DropDownMenu(viewBig);
	connect(menu, SIGNAL(itemClicked(QString,int)), this, SLOT(menuClicked(QString,int)));

	// Adjust some properties of the big GraphicsView for the main image
	viewBig->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
	viewBig->setObjectName("viewBig");
	viewBig->setScene(&sceneBig);
	viewBig->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	viewBig->setDragMode(QGraphicsView::ScrollHandDrag);
	viewBig->setMouseTracking(true);
	CustomScrollbar *vertScrollbar = new CustomScrollbar;
	CustomScrollbar *horScrollbar = new CustomScrollbar;
	horScrollbar->setOrientation(Qt::Horizontal);
	viewBig->setVerticalScrollBar(vertScrollbar);
	viewBig->setHorizontalScrollBar(horScrollbar);
	connect(viewBig, SIGNAL(mousePos(int,int)), this, SLOT(mouseMoved(int,int)));
	connect(viewBig, SIGNAL(clicked(QPoint)), this, SLOT(gotViewBigClick(QPoint)));

	// Adjust some properties of the small GraphicsView for Thumbnails
	viewThumbs->setObjectName("viewThumbs");
	viewThumbs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	viewThumbs->view->setDragMode(QGraphicsView::ScrollHandDrag);
	viewThumbs->view->setMouseTracking(true);
	if(globSet->thumbnailKeepVisible) {
		viewThumbs->setGeometry(viewThumbs->rectShown);
		viewThumbs->isShown = true;
	} else
		viewThumbs->setGeometry(viewThumbs->rectHidden);

	connect(viewThumbs, SIGNAL(loadNewImg(QString)), this, SLOT(loadNewImgFromThumbs(QString)));


	// The main GraphicsView fills the whole widget
	centralLayout->addWidget(viewBig);

	// This widget is shown after an update/fresh install
	startup = new StartUpWidget(viewBig);

	// A startup-timer (started from main.cpp)
	startUpTimer = new QTimer;
	startUpTimer->setInterval(500);
	connect(startUpTimer, SIGNAL(timeout()), this, SLOT(startuptimer()));

	// Set the shortcut version (needed for saving shortcuts
	set->sh->version = globSet->version;

	// These shortcut are needed for the widgets like settings, open, etc., thus they need to be set up always
	for(int i = 0; i < globVar->systemSh.size(); ++i) {
		QShortcut *sysSh = new QShortcut(QKeySequence(globVar->systemSh.at(i)),this);
		systemKeySH.insert(globVar->systemSh.at(i),sysSh);
		QList<QVariant> list;
		list << false << globVar->systemSh.at(i) << "";
		systemKeySHdo.value(globVar->systemSh.at(i),list);
		QSignalMapper *mapper = new QSignalMapper;
		mapper->setMapping(sysSh,globVar->systemSh.at(i));
		connect(sysSh, SIGNAL(activated()), mapper, SLOT(map()));
		connect(mapper, SIGNAL(mapped(QString)), this, SLOT(systemShortcutDO(QString)));
	}

	// The about widget
	about = new About(viewBig);
	about->setLicense(globSet->version);

	wallpaper = new Wallpaper(viewBig);

	// The slideshow settings widget
	slideshow = new SlideShow(globSet->toSignalOut(),viewBig, globVar->verbose);
	connect(slideshow, SIGNAL(startSlideShow()), this, SLOT(startSlideShow()));

	// The slideshowbar (shown as slide-in at top edge during slideshows)
	slideshowbar = new SlideShowBar(globSet->toSignalOut(), viewBig, globVar->verbose);
	connect(slideshowbar, SIGNAL(moveInDirectory(int)), this, SLOT(moveInDirectory(int)));
	connect(slideshowbar->cancel, SIGNAL(clicked()), this, SLOT(stopSlideShow()));

	// The exif widget
	exif = new Exif(viewBig,globSet->toSignalOut());
	connect(exif, SIGNAL(setOrientation(int,bool)), this, SLOT(getOrientationFromExif(int,bool)));
	connect(exif, SIGNAL(updateSettings(QMap<QString,QVariant>)), globSet, SLOT(settingsUpdated(QMap<QString,QVariant>)));

	// Setup the shortcuts
	setupShortcuts();

	// Setup the tray icon
	setupTrayIcon();

	// Setup the quickinfo labels. We have two sets of labels, for the top and for the bottom. This way we can change the position without having to restart photo.
	QVBoxLayout *viewBigLay = new QVBoxLayout;
	QHBoxLayout *quickInfoTOP = new QHBoxLayout;

	// We have labels for the top and for the bottom. So there's no need to restart Photo when switching thumbnails from top to bottom or vice versa
	quickInfoCounterTOP = new QuickInfoLabel(0,"quickinfoCounterTOP");
	quickInfoCounterTOP->setStyleSheet("color: white");
	quickInfoCounterTOP->hide();
	quickInfoSepTOP = new QLabel("--");
	quickInfoSepTOP->setStyleSheet("color:white");
	quickInfoSepTOP->setShown((globSet->hidefilename == globSet->hidecounter) && !globSet->hidecounter);
	quickInfoSepTOP->hide();
	quickInfoFilenameTOP = new QuickInfoLabel(0,"quickinfoFilenameTOP");
	quickInfoFilenameTOP->setText(tr("Open File to Begin."));
	quickInfoFilenameTOP->setStyleSheet("color: white");
	quickInfoFilenameTOP->hide();
	quickInfoFilenameTOP->globSet = globSet->toSignalOut();
	closeWindowX = new QuickInfoLabel(0,"closewindowXTOP");
	closeWindowX->setText("x");
	closeWindowX->setStyleSheet("color: white; padding: 5px");
	closeWindowX->setCursor(Qt::PointingHandCursor);
	closeWindowX->setShown(!globSet->hidex);
	QSignalMapper *mapperXTOP = new QSignalMapper;
	mapperXTOP->setMapping(closeWindowX,"0:::::__hide");
	connect(closeWindowX, SIGNAL(clicked()), mapperXTOP, SLOT(map()));
	connect(mapperXTOP, SIGNAL(mapped(QString)), this, SLOT(shortcutDO(QString)));
	connect(quickInfoCounterTOP->dohide, SIGNAL(triggered()), this, SLOT(hideQuickInfo()));
	connect(quickInfoFilenameTOP->dohide, SIGNAL(triggered()), this, SLOT(hideQuickInfo()));
	connect(quickInfoFilenameTOP->dohideFilepath, SIGNAL(triggered()), this, SLOT(hideQuickInfo()));
	connect(closeWindowX->dohide, SIGNAL(triggered()), this, SLOT(hideQuickInfo()));

	quickInfoTOP->addWidget(quickInfoCounterTOP);
	quickInfoTOP->addWidget(quickInfoSepTOP);
	quickInfoTOP->addWidget(quickInfoFilenameTOP);
	quickInfoTOP->addStretch();
	quickInfoTOP->addWidget(closeWindowX);

	viewBigLay->addLayout(quickInfoTOP);

	QHBoxLayout *quickInfoBOT = new QHBoxLayout;

	quickInfoCounterBOT = new QuickInfoLabel(0,"quickinfoCounterBOT");
	quickInfoCounterBOT->setStyleSheet("color: white");
	quickInfoCounterBOT->hide();
	quickInfoSepBOT = new QLabel("--");
	quickInfoSepBOT->setStyleSheet("color:white");
	quickInfoSepBOT->setShown((globSet->hidefilename == globSet->hidecounter) && !globSet->hidecounter);
	quickInfoSepBOT->hide();
	quickInfoFilenameBOT = new QuickInfoLabel(0,"quickinfoFilenameBOT");
	quickInfoFilenameBOT->setText(tr("Open File to Begin."));
	quickInfoFilenameBOT->setStyleSheet("color: white");
	quickInfoFilenameBOT->hide();
	quickInfoFilenameBOT->globSet = globSet->toSignalOut();
	connect(quickInfoCounterBOT->dohide, SIGNAL(triggered()), this, SLOT(hideQuickInfo()));
	connect(quickInfoFilenameBOT->dohide, SIGNAL(triggered()), this, SLOT(hideQuickInfo()));
	connect(quickInfoFilenameBOT->dohideFilepath, SIGNAL(triggered()), this, SLOT(hideQuickInfo()));

	quickInfoBOT->addWidget(quickInfoCounterBOT);
	quickInfoBOT->addWidget(quickInfoSepBOT);
	quickInfoBOT->addWidget(quickInfoFilenameBOT);
	quickInfoBOT->addStretch();

	viewBigLay->addStretch();
	viewBigLay->addLayout(quickInfoBOT);

	viewBig->setLayout(viewBigLay);

	if(globSet->thumbnailposition == "Bottom") {
		quickInfoCounterTOP->hide();
		quickInfoFilenameTOP->hide();
		quickInfoSepTOP->hide();
		closeWindowX->hide();
	} else if(globSet->thumbnailposition == "Top") {
		quickInfoCounterBOT->hide();
		quickInfoFilenameBOT->hide();
		quickInfoSepBOT->hide();
		closeWindowX->hide();
	}


	// Connect some of the blockFunc signals
	connect(about, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));
	connect(set, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));
	connect(slideshow, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));
	connect(filehandling, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));
	connect(exif->rotConf, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));

	// The settings have been changed by the user
	connect(set, SIGNAL(updateSettings(QMap<QString,QVariant>)), globSet, SLOT(settingsUpdated(QMap<QString,QVariant>)));

	// Pass the current settings on to all the subclasses
	updateSettings(globSet->toSignalOut());
	applySettings(QMap<QString,bool>(),true);

	// The settings have been changed by the user
	connect(set->sh, SIGNAL(updatedShortcuts()), this, SLOT(setupShortcuts()));

	// The global timer ensuring only one instance of Photo runs at a time
	globalRunningProgTimer = new QTimer;
	globalRunningProgTimer->setInterval(500);
	connect(globalRunningProgTimer, SIGNAL(timeout()), this, SLOT(globalRunningProgTimerTimeout()));
	globalRunningProgTimer->start();

	// Restore the default settings again
	connect(set, SIGNAL(restoreDefault()), this, SLOT(restoreDefaultSettings()));

	// Update the scene rect (called from there to make transition work)
	connect(graphItem, SIGNAL(updateSceneBigRect()), this, SLOT(updateSceneBigRect()));

	// Set initial "Open File to Begin" message
	updateQuickInfo();

}

void MainWindow::adjustGeometries() {

	if(globVar->verbose) qDebug() << "Adjusting geometries";

	// The thumbnail hight for later use
	int thbHeight = globSet->thumbnailsize + globSet->thumbnailLiftUp + 30;

	// If the thumbnail bar is shown at the bottom
	if(globSet->thumbnailposition == "Bottom") {

		if(globVar->verbose) qDebug() << "Thumbnails at the bottom";

		// Adjust the thumbnail geometry
		viewThumbs->rectShown = QRect(0,viewBig->height()-thbHeight,viewBig->width(),thbHeight);
		viewThumbs->rectHidden = QRect(0,viewBig->height(),viewBig->width(),thbHeight);
		if(viewThumbs->isShown)
			viewThumbs->setGeometry(viewThumbs->rectShown);
		else
			viewThumbs->setGeometry(viewThumbs->rectHidden);

		// Adjust the menu geometry
		menu->rectShown = QRect(viewBig->width()-750,0,700,350);
		menu->rectHidden = QRect(menu->rectShown.x(),-menu->rectShown.height(),menu->rectShown.width(),menu->rectShown.height());
		if(menu->isShown)
			menu->setGeometry(menu->rectShown);
		else
			menu->setGeometry(menu->rectHidden);

	// And if the thumbnail bar is shown at the top
	} else if(globSet->thumbnailposition == "Top") {

		if(globVar->verbose) qDebug() << "Thumbnails at the top";

		viewThumbs->rectShown = QRect(0,0,viewBig->width(),thbHeight);
		viewThumbs->rectHidden = QRect(0,0,viewBig->width(),-thbHeight);
		if(viewThumbs->isShown)
			viewThumbs->setGeometry(viewThumbs->rectShown);
		else
			viewThumbs->setGeometry(viewThumbs->rectHidden);

		menu->rectShown = QRect(viewBig->width()-750,viewBig->height()-350,700,350);
		menu->rectHidden = QRect(menu->rectShown.x(),viewBig->height(),menu->rectShown.width(),menu->rectShown.height());
		if(menu->isShown)
			menu->setGeometry(menu->rectShown);
		else
			menu->setGeometry(menu->rectHidden);

	}

	// Fullscreen Rects
	QRect fullscreenRectHidden = QRect(0,-10,10,10);
	QRect fullscreenRectAni = QRect(viewBig->width()/2.0,viewBig->height()/2.0,1,1);
	QRect fullscreenRectShown = QRect(viewBig->x(),viewBig->y(),viewBig->width(),viewBig->height());

	if(globVar->verbose) qDebug() << "AdjustingGeometries (1):" << fullscreenRectHidden;
	if(globVar->verbose) qDebug() << "AdjustingGeometries (2):" << fullscreenRectShown;
	if(globVar->verbose) qDebug() << "AdjustingGeometries (3):" << fullscreenRectAni;

	// These QRects are set to all the widgets that have a fullscreen element

	exif->rotConf->rectShown = fullscreenRectShown;
	exif->rotConf->rectHidden = fullscreenRectHidden;
	exif->rotConf->rectAni = fullscreenRectAni;

	set->rectHidden = fullscreenRectHidden;
	set->aniStart = fullscreenRectAni;
	set->rectShown = fullscreenRectShown;

	set->restoreDefaultConfirm->rectShown = fullscreenRectShown;
	set->restoreDefaultConfirm->rectHidden = fullscreenRectHidden;
	set->restoreDefaultConfirm->rectAni = fullscreenRectAni;

	set->tabShortcuts->detect->rectShown = fullscreenRectShown;
	set->tabShortcuts->detect->rectHidden = fullscreenRectHidden;
	set->tabShortcuts->detect->rectAni = fullscreenRectAni;

	set->tabShortcuts->setDefaultConfirm->rectShown = fullscreenRectShown;
	set->tabShortcuts->setDefaultConfirm->rectHidden = fullscreenRectHidden;
	set->tabShortcuts->setDefaultConfirm->rectAni = fullscreenRectAni;

	set->tabShortcuts->changeCommand->rectShown = fullscreenRectShown;
	set->tabShortcuts->changeCommand->rectHidden = fullscreenRectHidden;
	set->tabShortcuts->changeCommand->rectAni = fullscreenRectAni;

	set->tabThumb->confirmClean->rectShown = fullscreenRectShown;
	set->tabThumb->confirmClean->rectHidden = fullscreenRectHidden;
	set->tabThumb->confirmClean->rectAni = fullscreenRectAni;

	set->tabThumb->confirmErase->rectShown = fullscreenRectShown;
	set->tabThumb->confirmErase->rectHidden = fullscreenRectHidden;
	set->tabThumb->confirmErase->rectAni = fullscreenRectAni;

	filehandling->rectHidden = fullscreenRectHidden;
	filehandling->rectShown = fullscreenRectShown;
	filehandling->rectAni = fullscreenRectAni;

	about->rectHidden = fullscreenRectHidden;
	about->rectShown = fullscreenRectShown;
	about->rectAni = fullscreenRectAni;

	slideshow->rectHidden = fullscreenRectHidden;
	slideshow->rectShown = fullscreenRectShown;
	slideshow->rectAni = fullscreenRectAni;

	startup->rectHidden = fullscreenRectHidden;
	startup->aniStart = fullscreenRectAni;
	startup->rectShown = fullscreenRectShown;

	wallpaper->kdeUnsupported->rectShown = fullscreenRectShown;
	wallpaper->kdeUnsupported->rectHidden = fullscreenRectHidden;
	wallpaper->kdeUnsupported->rectAni = fullscreenRectAni;

	// And adjust the current geometries of all the widgets
	if(set->isShown)
		set->setGeometry(set->rectShown);
	else
		set->setGeometry(set->rectHidden);

	if(startup->isShown)
		startup->setGeometry(startup->rectShown);
	else
		startup->setGeometry(startup->rectHidden);

	if(about->isShown) {
		about->setGeometry(about->rectShown);
		about->center->setGeometry(QRect(100,50,about->rectShown.width()-200,about->rectShown.height()-100));
	} else {
		about->setGeometry(about->rectHidden);
	}

	if(filehandling->isShown)
		filehandling->setGeometry(filehandling->rectShown);
	else
		filehandling->setGeometry(filehandling->rectHidden);

	if(globSet->thumbnailKeepVisible) {
		if(!globVar->zoomedImgAtLeastOnce)
			viewThumbs->setGeometry(viewThumbs->rectShown);
	}

	// Adjust the slideshow geometry
	slideshow->adjustGeometries();

	// And adjust the slideshow bar
	slideshowbar->rectShown = QRect(0,0,this->width(),50);
	slideshowbar->rectHidden = QRect(0,-50,this->width(),50);

}

// This is called, whenever the settings have changed, and updates all sub-widgets
// This function is needed together with the updateSettings() function to avoid a crash on startup
void MainWindow::applySettings(QMap<QString, bool> applySet, bool justApplyAllOfThem) {

	if(globVar->verbose) qDebug() << "Applying settings:" << applySet;

	// If all the settings have to be (re-)applied
	if(justApplyAllOfThem) {
		if(globVar->verbose) qDebug() << "Simply apply all settings";
		QMap<QString, bool> newApplySet;
		QMapIterator<QString, bool> i(applySet);
		while (i.hasNext()) {
			i.next();
			newApplySet.insert(i.key(),true);
		}
		applySet = newApplySet;
	}

	// Adjust/Set the background
	if(applySet["background"])
		setBackground();


	// Update the thumbnails
	if(applySet["thumb"]) {

		// The max thumbnail height is
		int thbHeight = globSet->thumbnailsize + globSet->thumbnailLiftUp + 30;

		// If the thumbnails are supposed to be shown at the bottom
		if(globSet->thumbnailposition == "Bottom") {

			// Hide top quickinfo labels and show bottom ones
			quickInfoCounterBOT->hide();
			quickInfoSepBOT->hide();
			quickInfoFilenameBOT->hide();
			closeWindowX->show();
			quickInfoCounterTOP->show();
			quickInfoSepTOP->show();
			quickInfoFilenameTOP->show();

			// Adjust the thumbnail geometry
			viewThumbs->rectShown = QRect(0,viewBig->height()-thbHeight,viewBig->width(),thbHeight);
			viewThumbs->rectHidden = QRect(0,viewBig->height(),viewBig->width(),thbHeight);
			if(viewThumbs->isShown)
				viewThumbs->setGeometry(viewThumbs->rectShown);
			else
				viewThumbs->setGeometry(viewThumbs->rectHidden);

			// Adjust the menu geometry
			menu->rectShown = QRect(viewBig->width()-750,0,700,350);
			menu->rectHidden = QRect(menu->rectShown.x(),-menu->rectShown.height(),menu->rectShown.width(),menu->rectShown.height());

			if(menu->isShown)
				menu->setGeometry(menu->rectShown);
			else
				menu->setGeometry(menu->rectHidden);

			// Adjust the look (i.e. rounded corners) of the menu
			menu->setStyleSheet("border-radius: 8px; border-top-right-radius: 0px; border-top-left-radius: 0px; background-color: rgba(0, 0, 0, 220)");

		// If the thumbnails are supposed to be shown at the top
		} else if(globSet->thumbnailposition == "Top") {

			// Hide bottom quickinfo labels and show top ones
			quickInfoCounterBOT->show();
			quickInfoSepBOT->show();
			quickInfoFilenameBOT->show();
			closeWindowX->show();
			quickInfoCounterTOP->hide();
			quickInfoSepTOP->hide();
			quickInfoFilenameTOP->hide();

			// Adjust the thumbnail geometry
			viewThumbs->rectShown = QRect(0,0,viewBig->width(),thbHeight);
			viewThumbs->rectHidden = QRect(0,-thbHeight,viewBig->width(),thbHeight);
			if(viewThumbs->isShown)
				viewThumbs->setGeometry(viewThumbs->rectShown);
			else
				viewThumbs->setGeometry(viewThumbs->rectHidden);

			// Adjust the menu geometry
			menu->rectShown = QRect(viewBig->width()-750,viewBig->height()-350,700,350);
			menu->rectHidden = QRect(menu->rectShown.x(),viewBig->height(),menu->rectShown.width(),menu->rectShown.height());
			if(menu->isShown)
				menu->setGeometry(menu->rectShown);
			else
				menu->setGeometry(menu->rectHidden);

			// Adjust the look (i.e. rounded corners) of the menu
			menu->setStyleSheet("border-radius: 8px; border-bottom-right-radius: 0px; border-bottom-left-radius: 0px; background-color: rgba(0, 0, 0, 220)");

		}

		if(globVar->currentfile != "")
			viewThumbs->loadDir();

		if(globSet->thumbnailKeepVisible && !viewThumbs->isShown)
			viewThumbs->animate();


	}

	if(applySet["menu"]) {
		// Adjusting the menu sensitivity
		menu->xSensitivity = globSet->menusensitivity*3;
		menu->ySensitivity = globSet->menusensitivity*10;
	}

	if(applySet["thumb"] || applySet["quickinfo"])
		// Update the quickinfo labels
		updateQuickInfo();


	if(applySet["redrawimg"])
		drawImage();

}

// If a widget (like about or settings) is opened, all other functions are suspended
void MainWindow::blockFunc(bool bl) {

	if(globVar->verbose) qDebug() << "Blocking Interface:" << bl;

	viewThumbs->thumbThread->breakme = true;

	globVar->blocked = bl;
}

// The close event
void MainWindow::closeEvent(QCloseEvent *e) {

	// If a widget is opened and Alt was just pressed, then this close event probably results from the Alt+F4 combination
	if(globVar->blocked) {

		if(globVar->verbose) qDebug() << "Ignoring event, sending 'Escape' shortcut";

		e->ignore();

		systemShortcutDO("Escape");

	} else {

		// Stop the thumbnail thread
		viewThumbs->thumbThread->breakme = true;

		// Hide to system tray
		if(globSet->trayicon && !globVar->skipTrayIcon) {

			if(set->isShown)
				set->animate();
			globVar->restoringFromTrayNoResize = QDateTime::currentDateTime().toTime_t();
			this->hide();
			if(globVar->verbose) qDebug() << "Hiding to System Tray.";
			e->ignore();
		// Quit
		} else {
			sceneBig.clear();
			viewThumbs->view->scene.clear();

			e->accept();

			qDebug() << "Goodbye.";
		}

	}

}

// Draw the big main image
void MainWindow::drawImage() {

	// The slideshow also sets the globVar->blocked bool, but nevertheless, the image has to be drawn in that specific case
	if(!globVar->blocked || slideshowbar->enabled) {

		if(globVar->verbose) qDebug() << "Drawing Image";

		// Adjust the alignment. If the thumbnail are shwn permanently, then we need to align the image at the top, otherwise it overlaps with the thumbnails
		if(globSet->thumbnailKeepVisible && !globVar->zoomedImgAtLeastOnce) {
			if(globSet->thumbnailposition == "Bottom")
				viewBig->setAlignment(Qt::AlignTop);
			else if(globSet->thumbnailposition == "Top")
				viewBig->setAlignment(Qt::AlignBottom);
		} else if(!globSet->thumbnailKeepVisible || globVar->zoomedImgAtLeastOnce)
			viewBig->setAlignment(Qt::AlignCenter);

		if(globVar->currentfile == "") {

			if(globVar->verbose) qDebug() << "Ask for filename";

			// No file loaded yet, open "open file" widget
			openFile();

		} else {

			if(globVar->verbose) qDebug() << "Got filename:" << globVar->currentfile;

			// Tell the filehandling widget the new filename
			filehandling->currentfile = globVar->currentfile;

			// If the current directory info hasn't been loaded yet
			if(viewThumbs->counttot == 0) {
				viewThumbs->currentfile = globVar->currentfile;
				viewThumbs->noThumbs = globSet->thumbnailDisable;
				viewThumbs->loadDir();
			}

			// Scaling and adding the image to display (using QImageReader for better performance)
			QImageReader reader(globVar->currentfile);

			// Store the width/height for later use
			QSize origSize = reader.size();
			// Store the fileformat for later use
			QString fileformat = reader.format().toLower();

			// The image width/height
			int readerWidth = origSize.width();
			int readerHeight = origSize.height();

			int dispWidth = readerWidth;
			int dispHeight = readerHeight;

			// If the image is rotated to the left or right, the image dimensions are swapped
			if(globVar->rotation == 90 || globVar->rotation == 270) {
				int tmp = dispWidth;
				dispWidth = dispHeight;
				dispHeight = tmp;
			}


			// Calculate the factor by which the main image (view) has to be zoomed
			float q = 1;

			if(dispWidth > viewBig->width()-2*globSet->borderAroundImg) {
					q = (viewBig->width()-globSet->borderAroundImg*2)/(dispWidth*1.0);
					dispWidth *= q;
					dispHeight *= q;
			}

			if(globVar->zoomed)
				viewBig->scale(q,q);

			// If thumbnails are kept visible, then we need to subtract their height from the absolute height otherwise they overlap with the main image
			int subtractThumbnailHeight = 0;
			if(globSet->thumbnailKeepVisible)
				subtractThumbnailHeight = viewThumbs->height();

			if(dispHeight > viewBig->height()-2*globSet->borderAroundImg-subtractThumbnailHeight) {
				q = (viewBig->height()-2*globSet->borderAroundImg-subtractThumbnailHeight)/(dispHeight*1.0);
				dispWidth *= q;
				dispHeight *= q;
			}

			if(globVar->zoomed && dispWidth < viewBig->width()-2*globSet->borderAroundImg)
				viewBig->scale(q,q);


			// If image is rotated left or right, then we set the right image dimensions again
			if(globVar->rotation == 90 || globVar->rotation == 270) {
				int tmp = dispHeight;
				dispHeight = dispWidth;
				dispWidth = tmp;
			}

			if(!globVar->zoomed)
				reader.setScaledSize(QSize(dispWidth,dispHeight));

			// Eventually load the image
			QImage img = reader.read();

			// If the image is animated, set it as movie
			if(reader.supportsAnimation() && reader.imageCount() > 1)
				graphItem->setMovie(globVar->currentfile,readerWidth,readerHeight);
			// Otherwise do the normal setPixmap()
			else
				graphItem->setPixmap(QPixmap::fromImage(img));

			// Set the right position of the main image
			graphItem->setPos(QPoint((viewBig->width()-img.width())/2.0,(viewBig->height()-img.height())/2.0));

			// These formats known by Photo are supported by exiv2
			QStringList formats;
			formats << "bmp" << "gif" << "tiff" << "jpg" << "jpeg" << "png";

			// Update the current position of the image in the directory
			viewThumbs->countpos = viewThumbs->allImgsPath.indexOf(globVar->currentfile);

			// Adjust scene rect
			sceneBig.setSceneRect(sceneBig.itemsBoundingRect());

			if(!globVar->exifRead) {

				if(globVar->verbose) qDebug() << "Requesting Exif Info";

				globVar->exifRead = true;

				// If supported, load exiv2 data
				if(formats.contains(fileformat))
					// Update the exif data widget
					exif->updateData(globVar->currentfile,origSize);
				// Otherwise set exiv2 to "unsupported"
				else
					// Update the exif data widget
					exif->updateData(globVar->currentfile,origSize,false);

			}


			// Ensure the active thumbnail is shown
			// We only do that when the thumbnail was NOT loaded through a click on it. The reason is, that otherwise the thumbnailview might move a little (ensuring the thumbnail is visible) although it already IS visible.
			if(viewThumbs->allImgsPath.indexOf(globVar->currentfile) != -1 && !globSet->thumbnailDisable && !viewThumbs->thumbLoadedThroughClick) {
				if(viewThumbs->newlyLoadedDir)
					viewThumbs->view->centerOn(viewThumbs->allPixmaps.at(viewThumbs->allImgsPath.indexOf(globVar->currentfile)));
				else
					viewThumbs->view->ensureVisible(viewThumbs->allPixmaps.at(viewThumbs->allImgsPath.indexOf(globVar->currentfile)));
				if(!globSet->thumbnailKeepVisible && !viewThumbs->isShown)
					viewThumbs->setGeometry(viewThumbs->rectHidden);
				viewThumbs->startThread();
			} else if(viewThumbs->thumbLoadedThroughClick)
				viewThumbs->thumbLoadedThroughClick = false;

		}

	}

}

// When the Exif data dictates an orientation
void MainWindow::getOrientationFromExif(int degree, bool flipHor) {

	if(globVar->verbose) qDebug() << "Exif Rotate/Flip:" << degree << "-" << flipHor;

	// And rotate
	if(degree > 0) {
		while(degree > 0) {
			globVar->rotation += 90;
			viewBig->rotate(-90);
			globVar->rotation %= 360;
			degree -= 90;
		}
		drawImage();
	}

	// Flip Horizontally
	if(flipHor) {
		viewBig->scale(-1,1);
		globVar->flipHor = !globVar->flipHor;
	}

	// A vertical flip is just a horizontal flip plus 180° turn

}

// The global timer to ensure only one instance of Photo running and to make Photo remotely controllable
void MainWindow::globalRunningProgTimerTimeout() {

	// Updates the "running" file with current timestamp
	QFile t(QDir::homePath() + "/.photo/running");
	if(t.open(QIODevice::WriteOnly)) {
		QTextStream out(&t);
		out << QDateTime::currentMSecsSinceEpoch();
		t.close();
	} else
		qDebug() << "ERROR! Unable to write to file '~/.photo/running' - unable to control this instance through command line.";

	// Checks the "cmd" file for commands to be executed
	QFile cmd(QDir::homePath() + "/.photo/cmd");

	// Switches to identify options set
	bool doOpen = false;
	bool doShow = false;
	bool doHide = false;
	bool doToggle = false;
	bool doNoThumbs = false;
	bool doThumbs = false;
	QString doNewFile = "";

	// If the file exists
	if(cmd.exists() && cmd.open(QIODevice::ReadWrite)) {

		if(globVar->verbose) qDebug() << "Got passed on commands";

		QTextStream in(&cmd);

		QString line;
		do {

			// Check all the possivle options
			line = in.readLine();
			if(line == "--open")
				doOpen = true;
			if(line == "--show")
				doShow = true;
			if(line == "--hide")
				doHide = true;
			if(line == "--toggle")
				doToggle = true;
			if(line == "--no-thumbs")
				doNoThumbs = true;
			if(line == "--thumbs")
				doThumbs = true;
			if(line.startsWith("-f-"))
				doNewFile = line.remove(0,3).toUtf8();

			qDebug() << line;

		} while (!line.isNull());

		cmd.close();

	} else if(cmd.exists())
		qDebug() << "ERROR! Can't read '~/.photo/cmd'.";

	// Remove file after command is read in
	cmd.remove();

	if(doNewFile != "") {
		if(this->isHidden())
			doShow = true;
		doOpen = false;
	}


	// Toggle Photo
	if(doToggle) {

		if(this->isVisible()) {
			doHide = true;
			doShow = false;
		} else if(this->isHidden()) {
			doShow = true;
			doHide = false;
		}

	}

	// Open a new file
	if(doOpen) {
		if(this->isHidden())
			this->show();
		doShow = false;
		doHide = false;
		openFile();
	}

	// Show Photo
	if(doShow) {
		if(this->isHidden())
			this->showFullScreen();
		this->activateWindow();
		this->raise();
		doHide = false;
		if(globVar->currentfile == "" && doNewFile == "")
			openFile();
	}

	// Hide Photo
	if(doHide) {

		if(!globSet->trayicon) {
			globSet->trayicon = true;
			globSet->saveSettings();
		}

		if(this->isVisible())
			this->hide();
	}

	// Disable thumbnails
	if(doNoThumbs && !globSet->thumbnailDisable) {

		QMap<QString,QVariant> upd;
		upd.insert("ThumbnailDisable",true);
		globSet->settingsUpdated(upd);
		doThumbs = false;

	}

	// Enable thumbnails
	if(doThumbs && globSet->thumbnailDisable) {
		QMap<QString,QVariant> upd;
		upd.insert("ThumbnailDisable",false);
		globSet->settingsUpdated(upd);
	}


	// Load new file
	if(doNewFile != "")
		loadNewImgFromOpen(doNewFile);


}

// A click onto the main graphicsview
void MainWindow::gotViewBigClick(QPoint p) {

	if(globVar->verbose) qDebug() << "Click received:" << p;

	QPointF point = viewBig->mapToScene(p);

	int x = point.x();
	int y = point.y();

	int xg = graphItem->pos().x();
	int yg = graphItem->pos().y();
	int wg = graphItem->pixmap().width();
	int hg = graphItem->pixmap().height();

	// If click was on grey area outside image and corresponding option is set, then close Photo
	if(x < xg || x > xg+wg || y < yg || y > yg+hg || globVar->currentfile.trimmed().length() == 0) {
		if(globSet->closeongrey) {
			globVar->skipTrayIcon = true;
			this->close();
		}
	}

}

// This is called by the quickinfo context menus to hide the quickinfo labels
void MainWindow::hideQuickInfo() {

	QString objName = ((QAction *) sender())->objectName();
	if(globVar->verbose) qDebug() << "Hide quickinfo:" << objName;

	if(objName == "quickinfoCounter") {
		quickInfoCounterTOP->hide();
		quickInfoCounterBOT->hide();
		quickInfoSepTOP->hide();
		quickInfoSepBOT->hide();
		globSet->hidecounter = true;
	}
	if(objName == "quickinfoFilepath")
		globSet->hidefilepathshowfilename = true;
	if(objName == "quickinfoFilename") {
		quickInfoSepTOP->hide();
		quickInfoSepBOT->hide();
		quickInfoFilenameTOP->hide();
		quickInfoFilenameBOT->hide();
		globSet->hidefilename = true;
	}
	if(objName == "closewindowX") {
//		closeWindowX->hide();
		globSet->hidex = true;
	}
	globSet->saveSettings();

}

// Enabling the key detection by the shortcuts
void MainWindow::keyReleaseEvent(QKeyEvent *e) {

	if(globVar->verbose) qDebug() << "Got Key Event:" << e->key();

	if(set->tabShortcuts->detect->isShown && set->tabShortcuts->detect->keyShortcut->isChecked())
		set->tabShortcuts->detect->analyseKeyEvent(e);

	QMainWindow::keyPressEvent(e);

}

// Load a new image from the open file dialog
void MainWindow::loadNewImgFromOpen(QString path) {

	if(globVar->verbose) qDebug() << "Load from Open:" << path;

	// We need this later to update the thumbnail view
	QString temp = globVar->currentfile;

	// Update the currentfile
	globVar->currentfile = path;
	viewThumbs->currentfile = path;

	// Load thumbnails
	viewThumbs->counttot = 0;
	viewThumbs->countpos = 0;
	viewThumbs->allImgsInfo.clear();
	viewThumbs->allImgsPath.clear();
	viewThumbs->noThumbs = globSet->thumbnailDisable;
	viewThumbs->loadDir();
	globVar->exifRead = false;

	// When a new image is loaded we reset any zooming, rotation, flipping
	zoom(true,"resetNoDraw");
	rotateFlip(true,"resetNoDraw");
	rotateFlip(false, "reset");

	// Draw new image
	drawImage();

	// If thumbnails are not disabled
	if(!globSet->thumbnailDisable)
		viewThumbs->updateThbViewHoverNormPix(temp,globVar->currentfile);

	// Update quickinfo labels
	updateQuickInfo();

}

// Load a new image in current dir from the thumbnail view
void MainWindow::loadNewImgFromThumbs(QString path) {

	if(globVar->verbose) qDebug() << "Load from Thumbs:" << path;

	// Reset zooming parameter
	globVar->zoomedImgAtLeastOnce = false;

	// When a new image is loaded we reset any zooing, rotation, flipping
	zoom(true,"resetNoDraw");
	rotateFlip(true,"resetNoDraw");
	rotateFlip(false, "reset");

	viewBig->absoluteScaleFactor = 0;
	globVar->zoomed = false;
	globVar->exifRead = false;

	globVar->currentfile = path;
	viewThumbs->countpos = viewThumbs->allImgsPath.indexOf(path);
	viewThumbs->currentfile = globVar->currentfile;
	drawImage();

	updateQuickInfo();

}

// A menuitem has been clicked
void MainWindow::menuClicked(QString txt, int close) {

	if(globVar->verbose) qDebug() << "Menu clicked:" << close << "-" << txt;

	globVar->zoomedImgAtLeastOnce = false;

	// If this item was set up so that the menu closes on executing, then do it before executing the actual command (some commands could hold the menu open until the function call is finished)
	if(close)
		menu->animate();

	// Also calls this function (same as executeShortcut())
	shortcutDO(QString("0:::::__%2").arg(txt));

}

// The mouse has been moved over the main image view (called from viewBig)
void MainWindow::mouseMoved(int x, int y) {

	if(!globVar->blocked) {

		int h = globSet->thumbnailsize;

		if(globSet->thumbnailposition == "Bottom") {

			// Animate thumbnail bar
			if(!globSet->thumbnailDisable) {
				if(y < viewBig->height()-h-20 && viewThumbs->isShown && (!globSet->thumbnailKeepVisible || globVar->zoomedImgAtLeastOnce))
					viewThumbs->animate();

				if(y > viewBig->height()-20 && !viewThumbs->isShown)
					viewThumbs->animate();
			}

			// Animate menu
			if(x >= menu->rectShown.x()-globSet->menusensitivity*3 && x <= menu->rectShown.x()+menu->rectShown.width()+globSet->menusensitivity*3 && y <= globSet->menusensitivity*10 && !menu->isShown) {
				menu->animate();
			}

			if((x < menu->rectShown.x()-globSet->menusensitivity*3 || x > menu->rectShown.x()+menu->rectShown.width()+globSet->menusensitivity*3 || y > menu->rectShown.height()+globSet->menusensitivity*10) && menu->isShown) {
				menu->animate();
			}


		} else if(globSet->thumbnailposition == "Top") {

			// Animate thumbnail bar
			if(!globSet->thumbnailDisable) {
				if(y < 20 && !viewThumbs->isShown)
					viewThumbs->animate();

				if(y > viewThumbs->rectShown.height()+20 && (!globSet->thumbnailKeepVisible || globVar->zoomedImgAtLeastOnce) && viewThumbs->isShown)
					viewThumbs->animate();
			}

			// Animate menu
			if(x >= menu->rectShown.x()-globSet->menusensitivity*3 && x <= menu->rectShown.x()+menu->rectShown.width()+globSet->menusensitivity*3 && y >= menu->rectHidden.y()-globSet->menusensitivity*10 && !menu->isShown)
				menu->animate();

			if((x < menu->rectShown.x()-globSet->menusensitivity*3 || x > menu->rectShown.x()+menu->rectShown.width()+globSet->menusensitivity*3 || y < menu->rectShown.y()-globSet->menusensitivity*10) && menu->isShown)
				menu->animate();

		}


		// Animate exif widget
		if(x < 10*globSet->menusensitivity && y > exif->rectShown.y()-3*globSet->menusensitivity && y < exif->rectShown.y()+exif->rectShown.height()+globSet->menusensitivity*3 && !exif->isShown && globSet->exifenablemousetriggering)
			exif->animate();

		if((x > exif->rectShown.width()+10*globSet->menusensitivity || y < exif->rectShown.y()-3*globSet->menusensitivity || y > exif->rectShown.y()+exif->rectShown.height()+3*globSet->menusensitivity) && exif->isShown && !exif->stay->isChecked())
			exif->animate();


	// If globVar->blocked is set, but slideshow is running, animate slideshowbar
	} else if(slideshowbar->enabled) {

		if(globSet->thumbnailposition == "Bottom") {

			if(y < 30 && !slideshowbar->isShown)
				slideshowbar->animate();

			if(y > slideshowbar->height() && slideshowbar->isShown)
				slideshowbar->animate();

		} else {

			if(y > this->height()-30 && !slideshowbar->isShown)
				slideshowbar->animate();

			if(y < this->height()-slideshowbar->height() && slideshowbar->isShown)
				slideshowbar->animate();

		}

	}

}

// Move in the current directory (1=right, 0=left)
void MainWindow::moveInDirectory(int direction) {

	if(globVar->verbose) qDebug() << "Move in directory:" << direction;

	// When a new image is loaded we reset any zooing, rotation, flipping
	zoom(true,"resetNoDraw");
	rotateFlip(true,"resetNoDraw");
	rotateFlip(false, "reset");

	// Reset these parameters
	viewBig->absoluteScaleFactor = 0;
	globVar->zoomed = false;
	globVar->rotation = 0;
	globVar->exifRead = false;

	// Move to right, not the end of directory
	if(direction == 1 && viewThumbs->countpos < viewThumbs->counttot-1) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->allImgsPath.at(viewThumbs->countpos+1);
		++viewThumbs->countpos;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->currentfile,globVar->currentfile);
		viewThumbs->currentfile = globVar->currentfile;
		drawImage();
	// Move to right, end of directory
	} else if(direction == 1 && viewThumbs->countpos == viewThumbs->counttot-1 && globSet->loopthroughfolder && viewThumbs->counttot > 0 && !slideshowbar->enabled) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->allImgsPath.at(0);
		viewThumbs->countpos = 1;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->currentfile,globVar->currentfile);
		viewThumbs->currentfile = globVar->currentfile;
		drawImage();
	// Move to left, not the end of directory
	} else if(direction == 0 && viewThumbs->countpos > 0) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->allImgsPath.at(viewThumbs->countpos-1);
		--viewThumbs->countpos;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->currentfile,globVar->currentfile);
		viewThumbs->currentfile = globVar->currentfile;
		drawImage();
	// Move to left, end of directory
	} else if(direction == 0 && viewThumbs->countpos == 0 && globSet->loopthroughfolder && viewThumbs->counttot > 0) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->allImgsPath.at(viewThumbs->counttot-1);
		viewThumbs->countpos = viewThumbs->counttot-1;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->currentfile,globVar->currentfile);
		viewThumbs->currentfile = globVar->currentfile;
		drawImage();
	// If a slideshow is running and we're at the end of the directory, then we stop
	} else if(slideshowbar->enabled)
		stopSlideShow();

	// Update quickinfo labels
	updateQuickInfo();

}

// Open a new file. I had to remove (temporarily) Photo's custom Open File Dialog, because it just wasn't working right yet. It has to be completely re-done from scratch in a different way (using a model/view)
void MainWindow::openFile() {

	if(globVar->verbose) qDebug() << "Request to open new file";

	// Stopping a possibly running thread
	viewThumbs->stopThbCreation();

	// Open the FileDialog in the dir of last image. If Photo just was started (i.e. no current image), then open in home dir
	QString opendir = QDir::homePath();
	if(globVar->currentfile != "")
		opendir = QFileInfo(globVar->currentfile).absoluteDir().absolutePath();

	// Get new filename
	QString known = globSet->knownFileTypes;
	known = known.replace(","," ");
	QString file = QFileDialog::getOpenFileName(this,tr("Open image file"),opendir,tr("Images") + " (" + known + ")");

	// If a file was chosen (cancel returns an empty string)
	if(file.trimmed() != "")
		loadNewImgFromOpen(file);


}

// After a file has been manipulated (renamed, deleted, moved), the current dir is reloaded
void MainWindow::reloadDir(QString t) {

	if(globVar->verbose) qDebug() << "Reload current directory:" << t;

	// If file was renamed, simply reload renamed file
	if(t == "rename") {
		loadNewImgFromOpen(filehandling->currentfile);

	// If file was deleted
	} else if(t == "delete") {

		// If it was the last file in the directory
		if(viewThumbs->counttot == 1) {
			sceneBig.clear();
			viewThumbs->view->scene.clear();
			viewThumbs->counttot = 0;
			viewThumbs->countpos = 0;
			viewThumbs->allImgsInfo.clear();
			viewThumbs->allImgsPath.clear();
			globVar->currentfile = "";
			viewThumbs->currentfile = "";
			filehandling->currentfile = "";
			updateQuickInfo();
			drawImage();
		// If it wasn't the last file, then load file to left/right
		} else {
			QString newfile = "";
			if(viewThumbs->countpos+1 == viewThumbs->counttot)
				newfile = viewThumbs->allImgsPath.at(viewThumbs->counttot-2);
			else if(viewThumbs->countpos == 0)
				newfile = viewThumbs->allImgsPath.at(1);
			else
				newfile = viewThumbs->allImgsPath.at(viewThumbs->allImgsPath.indexOf(globVar->currentfile)+1);
			// We use this function to reload the current directory at the same time as loading a new image
			loadNewImgFromOpen(newfile);
		}

	}

}

// The resize event
void MainWindow::resizeEvent(QResizeEvent *) {

	if(globVar->verbose) qDebug() << "Window resized";

	// When photo is minimised and is restored, then the widget actually isn't resized, so the stuff in this function doesn't need to be done

	// Set an empty QPixmap to avoid any possible graphical artefacts
	if(globVar->currentfile == "") {
		QPixmap empty(viewBig->size());
		empty.fill(Qt::transparent);
		graphItem->setPixmap(empty,false,false);
	}

	// Adjust the geometries
	adjustGeometries();

	// Adjust the background
	setBackground();

	// Adjust scene rect
	sceneBig.setSceneRect(sceneBig.itemsBoundingRect());

	// And if an image is loaded, redraw it
	if(globVar->currentfile != "" && !globVar->zoomed && (QDateTime::currentDateTime().toTime_t() - globVar->restoringFromTrayNoResize) > 1)
		drawImage();

}

// Restore the default settings
void MainWindow::restoreDefaultSettings() {

	if(globVar->verbose) qDebug() << "Restoring default settings";

	globSet->setDefault();
	globSet->saveSettings();

}

// This function flips the current big image vertically/horizontally
void MainWindow::rotateFlip(bool rotateNotFlipped, QString direction) {

	if(globVar->verbose) qDebug() << "Rotate and Flip:" << rotateNotFlipped << "-" << direction;

	if(rotateNotFlipped) {

		// ROTATE

		if(direction == "clock") {

			globVar->rotation += 90;

			viewBig->rotate(90);

			globVar->rotation %= 360;

			if(!globVar->zoomed)
				drawImage();

		} else if(direction == "anticlock") {

			globVar->rotation -= 90;
			globVar->rotation += 360;

			viewBig->rotate(-90);

			globVar->rotation %= 360;

			if(!globVar->zoomed)
				drawImage();

		} else if(direction.startsWith("reset")) {

			viewBig->rotate(-globVar->rotation);
			globVar->rotation = 0;

			if(direction != "resetNoDraw" && !globVar->zoomed)
				drawImage();

		}


	} else {

		// FLIP

		if(direction == "hor") {
			viewBig->scale(-1,1);
			globVar->flipHor = !globVar->flipHor;
		} else if(direction == "ver") {
			viewBig->scale(1,-1);
			globVar->flipVer = !globVar->flipVer;
		} else if(direction == "reset") {
			if(globVar->flipHor)
				viewBig->scale(-1,1);
			if(globVar->flipVer)
				viewBig->scale(1,-1);
			globVar->flipHor = false;
			globVar->flipVer = false;
		}


	}


}

// Set the background of Photo
void MainWindow::setBackground() {

	if(globVar->verbose) qDebug() << "Set the background";

	// If compositing is enabled, then we'll use it (for the mainwindow background)
	if(globSet->composite) {

		if(globVar->verbose) qDebug() << "Composite enabled";

		this->setAttribute(Qt::WA_TranslucentBackground);
		bglabel->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(globSet->bgColorRed).arg(globSet->bgColorGreen).arg(globSet->bgColorBlue).arg(globSet->bgColorAlpha));

		// We set to bglabel an empty pixmap. We can't simply hide it, because it contains every widget in the app
		bglabel->setPixmap(QPixmap());

	// If compositing is disabled, then we'll simply take a screenshot and simulate some sort of compositing
	} else {

		this->setAttribute(Qt::WA_TranslucentBackground, false);
		this->setAttribute(Qt::WA_NoSystemBackground,false);

		// SET SCREENSHOT AS BACKGROUND (single-screen setups only)
		if(globSet->backgroundImageScreenshot && QApplication::desktop()->numScreens() == 1) {

			if(globVar->verbose) qDebug() << "Setting screenshot as background";

			QPixmap bg(screenshot.size());
			bg.fill(Qt::transparent);

			// That's the background image
			QPixmap base = screenshot;

			QPixmap overlay(screenshot.size());
			overlay.fill(Qt::transparent);
			QPainter overlayPaint(&overlay);
			overlayPaint.setCompositionMode(QPainter::CompositionMode_SourceOver);
			QColor col;
			col.setRgba(qRgba(globSet->bgColorRed,globSet->bgColorGreen,globSet->bgColorBlue,globSet->bgColorAlpha));
			overlayPaint.fillRect(overlay.rect(),col);

			// Let's start
			QPainter painter(&bg);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			int x = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).x();
			int y = 0;
			int w = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).width();
			int h = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).height();

			painter.drawPixmap(QRect(0,0,QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).width(),QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).height()), base, QRect(x,y,w,h));
			painter.drawPixmap(overlay.rect(), overlay);

			painter.end();

			// Setting the background image as background of label
			bglabel->setPixmap(bg);

		// Set a background image
		} else if(globSet->backgroundImageUse) {

			if(globVar->verbose) qDebug() << "Use background image";

			// That's the background image
			QImageReader reader(globSet->backgroundImagePath);

			if(globSet->backgroundImageStretch) {
				reader.setScaledSize(viewBig->size());
			} else if(globSet->backgroundImageScale){

				int w = reader.size().width();
				int h = reader.size().height();

				if(w != viewBig->width()) {
					float f = float(viewBig->width())/float(w);
					w *= f;
					h *= f;
				}
				reader.setScaledSize(QSize(w,h));

			} else {

				int w = reader.size().width();
				int h = reader.size().height();
				if(w > viewBig->width())
					w = viewBig->width();
				if(h > viewBig->height())
					h = viewBig->height();

				reader.setClipRect(QRect(0,0,w,h));

			}

			QPixmap base = QPixmap::fromImage(reader.read());

			QPixmap overlay(base.size());
			overlay.fill(Qt::transparent);
			QPainter overlayPaint(&overlay);
			overlayPaint.setCompositionMode(QPainter::CompositionMode_SourceOver);
			QColor col;
			col.setRgba(qRgba(globSet->bgColorRed,globSet->bgColorGreen,globSet->bgColorBlue,globSet->bgColorAlpha));
			overlayPaint.fillRect(overlay.rect(),col);

			QPixmap bg(base.size());
			bg.fill(Qt::transparent);

			// Let's start
			QPainter painter(&bg);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

			painter.drawPixmap(overlay.rect(), base,overlay.rect());
			painter.drawPixmap(overlay.rect(), overlay);

			painter.end();

			// Center Background Image?
			if(globSet->backgroundImageCenter)
				bglabel->setAlignment(Qt::AlignCenter);
			else {
				bglabel->setAlignment(Qt::AlignLeft);
				bglabel->setAlignment(Qt::AlignTop);
			}

			// Setting the background image as background of label
			bglabel->setPixmap(bg);

		} else {

			if(globVar->verbose) qDebug() << "Use background colour";

			bglabel->setPixmap(QPixmap());

			bglabel->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(globSet->bgColorRed).arg(globSet->bgColorGreen).arg(globSet->bgColorBlue).arg(globSet->bgColorAlpha));

		}

		bglabel->show();
	}

}

// Setup the shortcuts
void MainWindow::setupShortcuts() {

	if(globVar->verbose) qDebug() << "Setup Shortcuts";

	// Leave the systemkeys enabled (needed!), but set them to an empty list
	QMapIterator<QString, QList<QVariant> > i0(systemKeySHdo);
	while (i0.hasNext()) {
		i0.next();
		QList<QVariant> tmp;
		tmp.clear();
		systemKeySHdo.remove(i0.key());
		systemKeySHdo.insert(i0.key(),tmp);
	}

	// Disable all normal key shortcuts
	QMapIterator<QString, QList<QVariant> > i1(allKeySHdo);
	while (i1.hasNext()) {
		i1.next();
		allKeySH.value(i1.key())->setEnabled(false);
	}

	// Iterate through all the key shortcuts
	QMapIterator<QString,QList<QVariant> > s(set->sh->allKeyShortcuts);
	while(s.hasNext()) {
		s.next();
		// A systemkey is to be used
		if(globVar->systemSh.contains(s.key())) {
			systemKeySHdo.remove(s.key());
			systemKeySHdo.insert(s.key(),s.value());
		// If this key was already previously used
		} else if(allKeySHdo.keys().contains(s.key())) {
			allKeySHdo.remove(s.key());
			allKeySHdo.insert(s.key(),s.value());
			QString sendkey = QString("%1:::::%2").arg(s.value().at(0).toInt()).arg(s.value().at(1).toString());
			allKeySH.value(s.key())->setObjectName(sendkey);
			allKeySH.value(s.key())->setEnabled(true);
		// A comletely new key combination
		} else {

			QString sendkey = QString("%1:::::%2").arg(s.value().at(0).toInt()).arg(s.value().at(1).toString());

			QShortcut *dyn = new QShortcut(QKeySequence(s.key()),this);
			dyn->setObjectName(sendkey);
			connect(dyn, SIGNAL(activated()), this, SLOT(shortcutDO()));

			allKeySH.insert(s.key(),dyn);
			allKeySHdo.insert(s.key(),s.value());

		}
	}

	// Set all mouse shortcuts
	QMapIterator<QString,QList<QVariant> > m(set->sh->allMouseShortcuts);
	while(m.hasNext()) {
		m.next();
		viewBig->setShortcut(m.key(),m.value().at(0).toBool(),m.value().at(1).toString());
	}

}

// Setup the Tray Icon
void MainWindow::setupTrayIcon() {

	if(globVar->verbose) qDebug() << "Setup Tray Icon";

	// The Tray Icon
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/img/logo.png"));
	trayIcon->setToolTip(tr("Photo - Image Viewer"));

	// A context menu for the tray icon
	trayIconMenu = new QMenu(this);
	trayIconMenu->setStyleSheet("background-color: rgb(67,67,67); color: white; border-radius: 5px;");

	// A new action for the menu
	QAction *trayAcToggle = new QAction(QIcon(":/img/logo.png"),tr("Hide/Show Photo"),this);
	trayAcToggle->setObjectName("toggle");
	trayIconMenu->addAction(trayAcToggle);

	// A new action for the menu
	QAction *trayAcLoad = new QAction(QIcon(":/img/open.png"),tr("Open New Image"),this);
	trayAcLoad->setObjectName("open");
	trayIconMenu->addAction(trayAcLoad);

	// A seperator for the menu
	trayIconMenu->addSeparator();

	// A new action for the menu
	QAction *trayAcQuit = new QAction(QIcon(":/img/quit.png"),tr("Quit Photo"),this);
	trayAcQuit->setObjectName("quit");
	trayIconMenu->addAction(trayAcQuit);

	// Set the menu to the tray icon
	trayIcon->setContextMenu(trayIconMenu);

	// Connect signals
	connect(trayAcToggle, SIGNAL(triggered()), this, SLOT(trayAcDo()));
	connect(trayAcLoad, SIGNAL(triggered()), this, SLOT(trayAcDo()));
	connect(trayAcQuit, SIGNAL(triggered()), this, SLOT(trayAcDo()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayAcDo(QSystemTrayIcon::ActivationReason)));

	// The tray icon is always shown, but Photo is not necessarily always minimised to it (depending on user settings)
	trayIcon->show();

}

// Called by shortcuts to execute something
void MainWindow::shortcutDO(QString key, bool mouseSH) {

	if(!globVar->blocked) {

		if(key == "")
			key = ((QShortcut *) sender())->objectName();

		if(globVar->verbose) qDebug() << "DO shortcut:" << mouseSH << "-" << key;


		QString c = "";

		if(key.contains(":::::")) {
			c = key.split(":::::").at(0);
			key = key.split(":::::").at(1);
		}

		if(!key.startsWith("__")) {

			QProcess *p = new QProcess;
			key = key.replace("%f",'"' + globVar->currentfile + '"');
			key = key.replace("%d",'"' + QFileInfo(globVar->currentfile).absoluteDir().absolutePath() + '"');
			p->start(key);
			while(!p->waitForStarted()) { }
			if(c == "1")
				this->close();

		} else {

			if(key == "__stopThb")
				viewThumbs->stopThbCreation();
			if(key == "__close") {
				globVar->skipTrayIcon = true;
				this->close();
			}
			if(key == "__hide")
				this->close();
			if(key == "__settings") {
				set->animate();
				set->raise();
			}
			if(key == "__next")
				moveInDirectory(1);
			if(key == "__prev")
				moveInDirectory(0);
			if(key == "__reloadThb")
				viewThumbs->loadDir();
			if(key == "__about")
				about->animate();
			if(key == "__slideshow") {
				if(globVar->currentfile != "")
					slideshow->animate();
			}
			if(key == "__slideshowQuick") {
				if(globVar->currentfile != "")
					startSlideShow();
			}
			if(key == "__open" || key == "__openOld")
				openFile();
			if(key == "__zoomIn") {
				globVar->zoomedByMouse = mouseSH;
				zoom(true);
			}
			if(key == "__zoomOut") {
				globVar->zoomedByMouse = mouseSH;
				zoom(false);
			}
			if(key == "__zoomReset") {
				viewBig->absoluteScaleFactor = 0;
				zoom(false,"reset");
			}
			if(key == "__zoomActual")
				zoom(true,"actualsize");
			if(key == "__rotateL")
				rotateFlip(true,"anticlock");
			if(key == "__rotateR")
				rotateFlip(true,"clock");
			if(key == "__rotate0")
				rotateFlip(true,"reset");
			if(key == "__flipH")
				rotateFlip(false, "hor");
			if(key == "__flipV")
				rotateFlip(false, "ver");
			if(key == "__flipR")
				rotateFlip(false, "reset");
			if(key == "__rename")
				filehandling->openDialog("rename");
			if(key == "__delete")
				filehandling->openDialog("delete");
			if(key == "__copy")
				filehandling->openDialog("copy");
			if(key == "__move")
				filehandling->openDialog("move");
			if(key == "__hideMeta") {
				if(!exif->isShown) {
					exif->stay->setChecked(true);
					exif->animate();
				} else {
					exif->stay->setChecked(false);
					exif->animate();
				}
			}
			if(key == "__gotoFirstThb")
				viewThumbs->gotoFirstLast("first");
			if(key == "__gotoLastThb")
				viewThumbs->gotoFirstLast("last");

			if(key == "__wallpaper")
				wallpaper->setWallpaper(globVar->currentfile);

		}
	} else
		if(globVar->verbose) qDebug() << "DO shortcut (blocked)";

}

// Show the update/fresh install message
void MainWindow::showStartupUpdateInstallMsg() {

	if(globVar->startupMessageInstallUpdateShown == 1) {

		if(globVar->verbose) qDebug() << "Show Update Message";

		startup->setUpdateMsg();

		connect(startup, SIGNAL(finished()), this, SLOT(startupInstallUpdateMsgClosed()));

		startup->animate();

		blockFunc(true);

	} else if(globVar->startupMessageInstallUpdateShown == 2) {

		if(globVar->verbose) qDebug() << "Show Install Message";

		startup->setInstallMsg();

		connect(startup, SIGNAL(finished()), this, SLOT(startupInstallUpdateMsgClosed()));

		startup->animate();

		blockFunc(true);

	}

}

// The startup/fresh install message has been closed
void MainWindow::startupInstallUpdateMsgClosed() {

	if(globVar->verbose) qDebug() << "Startup Message closed";

	blockFunc(false);
	globVar->startupMessageInstallUpdateShown = 0;

}

// On startup, this timer calls drawImage() as soon as the mainwindow finished setup
void MainWindow::startuptimer() {


	if(this->centralWidget()->height()-viewBig->height() < 25 && this->centralWidget()->width()-viewBig->width() < 25 && viewBig->width() > 500 && viewBig->height() > 500) {

		// Show startup message (if it has to be shown and isn't shown yet)
		if(globVar->startupMessageInstallUpdateShown != 0 && !startup->isShown) {
			if(globVar->verbose) qDebug() << "Startup timer ended (message)";
			showStartupUpdateInstallMsg();
		// Start Photo
		} else if(globVar->startupMessageInstallUpdateShown == 0) {

			if(globVar->verbose) qDebug() << "Startup timer ended (load)";

			startUpTimer->stop();

			drawImage();

			viewThumbs->updateThbViewHoverNormPix("",globVar->currentfile);

			updateQuickInfo();

		}

	}

}

// Start slideshow
void MainWindow::startSlideShow() {

	if(globVar->verbose) qDebug() << "Start slideshow";

	// Set some global parameters
	globSet->slideShowTime = slideshow->timeSlider->value();
	globSet->slideShowTransition = slideshow->trans->value();
	QString musicFilePath = slideshow->musicPath->text();
	if(!slideshow->musicEnable->isChecked())
		musicFilePath = "";
	globSet->slideShowMusicFile = musicFilePath;
	globSet->saveSettings();

	// Block all functions
	blockFunc(true);

	// Set the transition
	graphItem->transitionSetChange(slideshow->trans->value());

	// Activate slideshow bar and animate it in and out to signalise start
	slideshowbar->enabled = true;
	slideshowbar->animateInAndOut = true;
	slideshowbar->animate();

	// update the quickinfo
	updateQuickInfo();

	// set the music file to bar
	slideshowbar->musicFile = musicFilePath;

	// set the interval
	slideshowbar->nextImg->setInterval(slideshow->timeSlider->value()*1000);

	// and start slideshow
	slideshowbar->startSlideShow();

	// Hide these possibly shown widgets
	if(viewThumbs->isShown)
		viewThumbs->animate();
	if(exif->isShown)
		exif->animate();
	if(menu->isShown)
		menu->animate();


}

// Stop Slideshowbar
void MainWindow::stopSlideShow() {

	if(globVar->verbose) qDebug() << "Stop slideshow";

	// Animate slideshowbar in (if not shown already) and out
	slideshowbar->animateInAndOut = true;
	slideshowbar->animate();

	// Disable slideshowbar
	slideshowbar->enabled = false;

	// Unblock all functions again
	blockFunc(false);

	// Update quickinfo
	updateQuickInfo();

	// Stop slideshow
	slideshowbar->stopSlideShow();

	// Re-set original transition value
	graphItem->transitionSetChange(globSet->transition);

}

// A system shortcut has been called (needed for instance for dialogs like about or settings)
void MainWindow::systemShortcutDO(QString todo) {

	if(globVar->blocked) {

		if(globVar->verbose) qDebug() << "Shortcut received (blockd):" << todo;

		if(set->isShown && !set->tabShortcuts->detect->isShown) {

			if(todo == "Escape") {
				if(set->tabShortcuts->changeCommand->isShown)
					set->tabShortcuts->changeCommand->animate();
				else if(set->tabThumb->confirmClean->isShown)
					set->tabThumb->confirmClean->no->animateClick();
				else if(set->tabThumb->confirmErase->isShown)
					set->tabThumb->confirmErase->no->animateClick();
				else {
					set->loadSettings();
					set->animate();
				}
			}
			if(todo == "Ctrl+s" && !set->tabShortcuts->changeCommand->isShown) {
				set->animate();
				set->saveSettings();
			}
			if(todo == "Alt+1")
				set->tabs->setCurrentIndex(0);
			if(todo == "Alt+2")
				set->tabs->setCurrentIndex(1);
			if(todo == "Alt+3")
				set->tabs->setCurrentIndex(2);
			if(todo == "Alt+4")
				set->tabs->setCurrentIndex(3);
			if(todo == "Alt+5")
				set->tabs->setCurrentIndex(4);
			if(todo == "Ctrl+Tab")
				set->nextTab();
			if(todo == "Ctrl+Shift+Tab")
				set->prevTab();

		}

		if(startup->isShown) {

			if(todo == "Escape" || todo == "Return" || todo == "Enter")
				startup->animate();

		}

		if(filehandling->isShown) {

			if(todo == "Escape") {
				if(filehandling->dialogType == "rename")
					filehandling->renameCancel->animateClick();
				else if(filehandling->dialogType == "delete")
					filehandling->deleteNo->animateClick();
				else if(filehandling->dialogType == "move")
					filehandling->moveCancel->animateClick();
				else if(filehandling->dialogType == "copy")
					filehandling->copyCancel->animateClick();
			}
			if(todo == "Return" || todo == "Enter") {
				if(filehandling->dialogType == "rename" && filehandling->renameSave->isEnabled())
					filehandling->renameSave->animateClick();
				else if(filehandling->dialogType == "delete")
					filehandling->deleteYes->animateClick();
				else if(filehandling->dialogType == "move" && filehandling->moveSave->isEnabled())
					filehandling->moveSave->animateClick();
				else if(filehandling->dialogType == "copy")
					filehandling->copySave->animateClick();
			}

		}

		if(about->isShown) {

			if(todo == "Escape")
				about->close->animateClick();

		}

		if(exif->rotConf->isShown) {

			if(todo == "Enter" || todo == "Return")
				exif->rotConf->yes->animateClick();
			else if(todo == "Escape")
				exif->rotConf->no->animateClick();

		}

		if(slideshow->isShown) {

			if(todo == "Escape")
				slideshow->animate();
			if(todo == "Enter" || todo == "Return")
				slideshow->andStart();

		}

		if(slideshowbar->enabled) {

			if(todo == "Escape")
				stopSlideShow();

		}

	// If functions are not blocked, then check if there's a user shortcut set for it
	} else {

		if(globVar->verbose) qDebug() << "Shortcut received";

		QList<QVariant> emp;
		emp.clear();
		if(systemKeySHdo.value(todo) != emp) {
			QString key = QString("%1:::::%2").arg(systemKeySHdo.value(todo).at(0).toInt()).arg(systemKeySHdo.value(todo).at(1).toString());
			shortcutDO(key);
		}

	}

}

// Click on a tray icon menu item
void MainWindow::trayAcDo(QSystemTrayIcon::ActivationReason rsn) {

	if(globVar->verbose) qDebug() << "Tray Action triggered:" << rsn;

	QAction *s = (QAction *) sender();
	if(s->objectName() == "open") {
		globVar->restoringFromTrayNoResize = QDateTime::currentDateTime().toTime_t();
		if(this->isHidden())
			this->showFullScreen();
		openFile();
	} else if(s->objectName() == "quit") {
		globVar->skipTrayIcon = true;
		this->close();
	} else if(rsn == QSystemTrayIcon::Trigger) {
		if(this->isVisible())
			this->hide();
		else {
			globVar->restoringFromTrayNoResize = QDateTime::currentDateTime().toTime_t();
			this->showFullScreen();
			if(globVar->currentfile == "--start-in-tray" || globVar->currentfile == "") {
				openFile();
			}
		}
	}

}

// Update the quickinfo labels top bar
void MainWindow::updateQuickInfo() {

	if(globVar->verbose) qDebug() << "Update Quickinfo labels (show/hide)";

	// If a slideshow is running and the user disabled all the quickinfos for that
	if(slideshowbar->enabled && slideshow->hideQuickInfo->isChecked()) {

		quickInfoCounterTOP->hide();
		quickInfoFilenameTOP->hide();
		quickInfoSepTOP->hide();

		quickInfoCounterBOT->hide();
		quickInfoSepBOT->hide();
		quickInfoFilenameBOT->hide();

		closeWindowX->hide();

	} else if(globSet->thumbnailposition == "Bottom") {

		if(globVar->currentfile != "") {

			quickInfoCounterTOP->setText(QString("%1/%2").arg(viewThumbs->countpos+1).arg(viewThumbs->counttot));

			if(globSet->hidefilepathshowfilename)
				quickInfoFilenameTOP->setText(QFileInfo(globVar->currentfile).fileName());
			else
				quickInfoFilenameTOP->setText(globVar->currentfile);

			quickInfoCounterTOP->setShown(!globSet->hidecounter);
			quickInfoFilenameTOP->setShown(!globSet->hidefilename);
			closeWindowX->setShown(!globSet->hidex);
			quickInfoSepTOP->setShown((globSet->hidefilename == globSet->hidecounter) && !globSet->hidecounter);
		} else {
			quickInfoFilenameTOP->setText(tr("Open File to Begin."));
			quickInfoCounterTOP->hide();
			quickInfoSepTOP->hide();
			quickInfoFilenameTOP->show();
		}

	} else if(globSet->thumbnailposition == "Top") {

		if(globVar->currentfile != "") {

			quickInfoCounterBOT->setText(QString("%1/%2").arg(viewThumbs->countpos+1).arg(viewThumbs->counttot));

			if(globSet->hidefilepathshowfilename)
				quickInfoFilenameBOT->setText(QFileInfo(globVar->currentfile).fileName());
			else
				quickInfoFilenameBOT->setText(globVar->currentfile);

			quickInfoCounterBOT->setShown(!globSet->hidecounter);
			quickInfoFilenameBOT->setShown(!globSet->hidefilename);
			closeWindowX->setShown(!globSet->hidex);
			quickInfoSepBOT->setShown(!globSet->hidefilename && !globSet->hidecounter);
		} else {
			quickInfoFilenameBOT->setText(tr("Open File to Begin."));
			quickInfoCounterBOT->hide();
			quickInfoSepBOT->hide();
			quickInfoFilenameBOT->show();
		}

	}

}

// Update scene rect (called from graphicsitem.cpp)
void MainWindow::updateSceneBigRect() {
	if(globVar->verbose) qDebug() << "Update Scene Rect";
	sceneBig.setSceneRect(sceneBig.itemsBoundingRect());
}

// The settings have been updated, so the map is passed to sub-widgets
void MainWindow::updateSettings(QMap<QString, QVariant> settings) {

	if(globVar->verbose) qDebug() << "Pass updated settings to subclasses";

	viewThumbs->globSet = settings;
	viewThumbs->view->globSet = settings;
	viewBig->globSet = settings;

	quickInfoFilenameBOT->globSet = settings;
	quickInfoFilenameTOP->globSet = settings;

	set->globSet = settings;
	set->tabLookFeel->globSet = settings;
	set->tabThumb->globSet = settings;
	set->tabExif->globSet = settings;
	set->tabOther->globSet = settings;

	exif->globSet = settings;
	exif->updateFontsize();

	graphItem->transitionSetChange(globSet->transition);

	slideshow->globSet = settings;

	set->loadSettings();

}

// Zoom the current image; if a string is set, the boolean is ignored
void MainWindow::zoom(bool zoomin, QString ignoreBoolean) {

	if(globVar->verbose) qDebug() << "Zoom:" << zoomin << "-" << ignoreBoolean;

	// Reset zoom
	if(ignoreBoolean.startsWith("reset")) {

		if(globVar->verbose) qDebug() << "Reset Zoom";

		globVar->zoomToActualSize = false;
		globVar->zoomed = false;
		viewBig->resetMatrix();
		viewBig->rotate(globVar->rotation);
		if(globVar->flipHor)
			viewBig->scale(-1,1);
		if(globVar->flipVer)
			viewBig->scale(1,-1);
		globVar->zoomedImgAtLeastOnce = false;
		if(globSet->thumbnailKeepVisible) {
			viewThumbs->setGeometry(viewThumbs->rectShown);
			viewThumbs->isShown = true;
		}
		if(ignoreBoolean != "resetNoDraw" || globSet->transition != 0)
			drawImage();

		graphItem->itemZoomed = false;
	// zoom to actual size (toggle)
	} else if(ignoreBoolean == "actualsize") {

		if(globVar->verbose) qDebug() << "Zoom to actual size";

		// If at actual size -> reset
		if(globVar->zoomToActualSize)
			zoom(true,"reset");
		else {

			viewBig->resetMatrix();
			// These need to be set to true for drawImg() not to scale the image
			globVar->zoomed = true;
			graphItem->itemZoomed = true;
			globVar->zoomedImgAtLeastOnce = true;

			drawImage();

			globVar->zoomToActualSize = true;

			viewBig->resetMatrix();
			globVar->zoomed = false;
			viewBig->rotate(globVar->rotation);
			if(globVar->flipHor)
				viewBig->scale(-1,1);
			if(globVar->flipVer)
				viewBig->scale(1,-1);

			if(globSet->thumbnailKeepVisible && viewThumbs->isShown) {
				viewThumbs->animate();
			}

		}

	} else {

		// Else Adjust alignment of graphicsview
		globVar->zoomToActualSize = false;

		viewBig->setAlignment(Qt::AlignCenter);
		globVar->zoomedImgAtLeastOnce = true;

		// Set anchor
		if(globVar->zoomedByMouse)
			viewBig->setTransformationAnchor(viewBig->AnchorUnderMouse);
		else
			viewBig->setTransformationAnchor(viewBig->AnchorViewCenter);

		// Set them to true
		globVar->zoomed = true;
		graphItem->itemZoomed = true;

		// And possibly draw full unscaled image (if image isn't zoomed and user wants to zoom in)
		if(!globVar->zoomed && zoomin) {
			drawImage();
		}

		// Zoom in
		if(zoomin && viewBig->absoluteScaleFactor == 0) {

			if(globVar->verbose) qDebug() << "Zoom In";

			viewBig->setAlignment(Qt::AlignCenter);
			viewBig->resetMatrix();
			viewBig->rotate(globVar->rotation);
			if(globVar->flipHor)
				viewBig->scale(-1,1);
			if(globVar->flipVer)
				viewBig->scale(1,-1);
			drawImage();

			QCursor::setPos(QCursor::pos().x()+1,QCursor::pos().y());

		// Zoom out
		} else if(!zoomin && (viewBig->absoluteScaleFactor == 0 || viewBig->absoluteScaleFactor == 1)) {

			if(globVar->verbose) qDebug() << "Zoom Out";

			if(viewBig->absoluteScaleFactor == 1)
				graphItem->itemZoomed = true;
			else
				graphItem->itemZoomed = false;

			globVar->zoomed = false;
			viewBig->resetMatrix();
			viewBig->rotate(globVar->rotation);
			if(globVar->flipHor)
				viewBig->scale(-1,1);
			if(globVar->flipVer)
				viewBig->scale(1,-1);
			drawImage();
			if(viewBig->absoluteScaleFactor == 1) {
				viewBig->scale(1.1,1.1);
				globVar->zoomed = true;
			}

			QCursor::setPos(QCursor::pos().x()+1,QCursor::pos().y());

		}

		if(zoomin) {
			viewBig->scale(1.1,1.1);
			viewBig->absoluteScaleFactor += 1;
		} else {
			viewBig->scale(0.90909090,0.90909090);
			viewBig->absoluteScaleFactor -= 1;
		}

	}

}

MainWindow::~MainWindow() { }


/*####################################################################################
  ####################################################################################
  ####################################################################################
  ####################################################################################
  ####################################################################################*/


// A custom label for enabling a right click on them, and adding a clicked() event
QuickInfoLabel::QuickInfoLabel(QWidget *parent, QString objName) : QLabel(parent) {

	this->setObjectName(objName);

	c = new QMenu;
	if(objName.startsWith("quickinfoFilename")) {
		dohideFilepath = new QAction(tr("Hide Filepath, leave Filename"),c);
		dohideFilepath->setObjectName("quickinfoFilepath");
		c->addAction(dohideFilepath);
	}
	dohide = new QAction(tr("Hide this item"),c);
	dohide->setObjectName(objName);
	c->setStyleSheet("QMenu { background-color: black; margin: 2px; } QMenu::item { color: grey; } QMenu::item:selected { color: white; } QMenu::item::disabled { color: black; }");
	c->addAction(dohide);

}

// The label on the top right (the little "x") for closing the app
void QuickInfoLabel::mouseReleaseEvent(QMouseEvent *e) {

	if(e->button() == Qt::LeftButton)
		emit clicked();

}

// Showing the context menu
void QuickInfoLabel::contextMenuEvent(QContextMenuEvent*) {
	if(this->objectName() == "quickinfoFilename")
		dohideFilepath->setVisible(!globSet.value("HideFilepathShowFilename").toBool());
	c->popup(QCursor::pos());
}

