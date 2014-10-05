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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, bool verbose) : QMainWindow(parent) {

	// Make a screenshot of each screen
	takeScreenshots();

	// Black Background
	this->setObjectName("mainwindow");
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setStyleSheet("#mainwindow { background-color: rgba(0,0,0,0); }");
	this->setMouseTracking(true);

	// If the user switched to window mode, then we need to have a minimum window size
	this->setMinimumSize(800,600);

	// Instance for global variables
	globVar = new GlobalVariables;
	globVar->verbose = verbose;

	// Instance for global settings
	globSet = new GlobalSettings;
	globSet->verbose = verbose;
	globSet->readSettings();
	// Connect some globSet signals
	connect(globSet, SIGNAL(updateSettingsData(QMap<QString,QVariant>)), this, SLOT(updateSettings(QMap<QString,QVariant>)));
	connect(globSet, SIGNAL(updateSettingsToApply(QMap<QString,bool>)), this, SLOT(applySettings(QMap<QString,bool>)));

	// Instance for which widgets have been set up
	setupWidgets = new SetupWidgets;

	// Central Widget
	QWidget *central = new QWidget;
	central->setObjectName("central");
	central->setStyleSheet("#central { border: none; background-color:rgba(0,0,0,0); }");
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

	// The main GraphicViews
	viewBig = new GraphicsView(globSet->toSignalOut(),bglabel);
	connect(viewBig, SIGNAL(mousePos(int,int)), this, SLOT(mouseMoved(int,int)));
	connect(viewBig, SIGNAL(clicked(QPoint)), this, SLOT(gotViewBigClick(QPoint)));
	// A mouse action triggered a shortcut
	connect(viewBig, SIGNAL(shMouseDo(QString,bool)), this, SLOT(shortcutDO(QString,bool)));
	connect(viewBig, SIGNAL(loadContextMenuAction(QString)), this, SLOT(shortcutDO(QString)));
	// The main GraphicsView fills the whole widget
	centralLayout->addWidget(viewBig);

	// Setup the quickinfo labels. We have two sets of labels, for the top and for the bottom. This way we can change the position without having to restart PhotoQt.
	viewBigLay = new ViewBigLay(globSet->toSignalOut(),verbose);
	viewBig->setLayout(viewBigLay);
	viewBigLay->setPosition(globSet->thumbnailposition);
	connect(viewBigLay, SIGNAL(clickOnX(QString)), this, SLOT(shortcutDO(QString)));
	connect(viewBigLay, SIGNAL(updateSettings(QMap<QString,QVariant>)), globSet, SLOT(settingsUpdated(QMap<QString,QVariant>)));
// rotation indicator
/*	viewBigLay->updateInfo("",0,0,false);*/
	viewBigLay->updateInfo("",0,0);

	// The Item holding the big pixmap item
	graphItem = new GraphicsItem;
	connect(graphItem, SIGNAL(updateSceneBigRect()), this, SLOT(updateSceneBigRect()));
	viewBig->scene()->addItem(graphItem);


	// The thumbnail-bar instance
	viewThumbs = new Thumbnails(viewBig,globSet->verbose,globSet->toSignalOut());
	connect(viewThumbs, SIGNAL(loadNewImg(QString)), this, SLOT(loadNewImgFromThumbs(QString)));



	// An ImageReader combining QImageReader and GraphicsMagic (if not disabled)
	imageReader = new ImageReader(globVar->verbose);


	// The settings widget
	set = new Settings(globSet->toSignalOut(),verbose,viewBig);
	// Set the shortcut version (needed for saving shortcuts
	connect(set, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));
	connect(set, SIGNAL(updateSettings(QMap<QString,QVariant>)), globSet, SLOT(settingsUpdated(QMap<QString,QVariant>)));
	connect(set, SIGNAL(restoreDefault()), this, SLOT(restoreDefaultSettings()));
	set->sh->version = globSet->version;
	connect(set->sh, SIGNAL(updatedShortcuts()), this, SLOT(setupShortcuts()));
	connect(set, SIGNAL(widgetClosed()), this, SLOT(settingsClosed()));


	// The exif widget
	details = new Details(viewBig,globSet->toSignalOut());
	connect(details, SIGNAL(updateSettings(QMap<QString,QVariant>)), globSet, SLOT(settingsUpdated(QMap<QString,QVariant>)));
#ifdef EXIV2
	connect(details, SIGNAL(setOrientation(int,bool)), this, SLOT(getOrientationFromExif(int,bool)));
	connect(details->rotConf, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));
#endif


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

	// Setup the shortcuts
	setupShortcuts();

	// Setup the tray icon
	setupTrayIcon();


	// Pass the current settings on to all the subclasses
	updateSettings(globSet->toSignalOut());
	applySettings(QMap<QString,bool>(),true);

	// The global timer ensuring only one instance of Photo runs at a time
	globalRunningProgTimer = new QTimer;
	globalRunningProgTimer->setInterval(300);
	connect(globalRunningProgTimer, SIGNAL(timeout()), this, SLOT(globalRunningProgTimerTimeout()));
	globalRunningProgTimer->start();

	// A startup-timer (started from main.cpp)
	startUpTimer = new QTimer;
	startUpTimer->setInterval(300);
	connect(startUpTimer, SIGNAL(timeout()), this, SLOT(startuptimer()));


}

// Adjust the geometries (if applicable)
void MainWindow::adjustGeometries() {

	if(globVar->verbose) std::clog << "Adjusting geometries" << std::endl;

	// The thumbnail hight for later use
	int thbHeight = globSet->thumbnailsize + globSet->thumbnailLiftUp + 30;
	int viewH = viewBig->height();
	int viewW = viewBig->width();

	// If the thumbnail bar is shown at the bottom
	if(globSet->thumbnailposition == "Bottom") {

		if(globVar->verbose) std::clog << "Thumbnails at the bottom" << std::endl;

		// Adjust the thumbnail geometry
		viewThumbs->setRect(QRect(0,viewH-thbHeight,viewW,thbHeight));

		// Adjust the menu geometry
		if(setupWidgets->menu) menu->setRect(QRect(viewW-450,0,300,300));



	// And if the thumbnail bar is shown at the top
	} else if(globSet->thumbnailposition == "Top") {

		if(globVar->verbose) std::clog << "Thumbnails at the top" << std::endl;

		// Adjust the thumbnail geometry
		viewThumbs->setRect(QRect(0,0,viewBig->width(),thbHeight));

		// Adjust the menu geometry
		if(setupWidgets->menu) menu->setRect(QRect(viewBig->width()-450,viewBig->height()-300,300,300));

	}


	// Fullscreen Rect
	QRect fullscreen = QRect(0,0,viewW,viewH);

	if(globVar->verbose) std::clog << "Adjusting Geometries: " << QString("QRect, topleft: %1:%2, size: %3x%4").arg(fullscreen.left()).arg(fullscreen.top()).arg(fullscreen.width()).arg(fullscreen.height()).toStdString() << std::endl;

	// exif and set are always setup (the tabs in set aren't initially)
	details->setRect(fullscreen);
	set->setRect(fullscreen);


	// Update the rects for all set up widgets

	if(setupWidgets->filehandling) filehandling->setRect(fullscreen);

	if(setupWidgets->about) about->setRect(fullscreen);

	if(setupWidgets->slideshow) slideshow->setRect(fullscreen);

	if(setupWidgets->startup) startup->setRect(fullscreen);

	if(setupWidgets->wallpaper) wallpaper->setRect(fullscreen);

	if(setupWidgets->scaleimage) scaleimage->setRect(fullscreen);

	if(setupWidgets->slideshowbar) slideshowbar->setWidth(this->width());

	if(setupWidgets->quicksettings) quickset->setRect(fullscreen);


	if(globSet->thumbnailKeepVisible)
		viewThumbs->makeShow();

}

// This is called, whenever the settings have changed, and updates all sub-widgets
// This function is needed together with the updateSettings() function to avoid a crash on startup
void MainWindow::applySettings(QMap<QString, bool> applySet, bool justApplyAllOfThem) {

	if(globVar->verbose) std::clog << "Applying settings..." << std::endl;

	// If all the settings have to be (re-)applied
	if(justApplyAllOfThem) {
		if(globVar->verbose) std::clog << "Simply apply all settings" << std::endl;
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

	// Update window for window mode/fullscreen
	if(applySet["window"]) {
		if(globSet->windowmode) {
			this->showMaximized();
			globSet->windowDecoration ? this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint) : this->setWindowFlags(Qt::FramelessWindowHint);
			QTimer::singleShot(10,this,SLOT(showMaximized()));
			QTimer::singleShot(500,this,SLOT(showMaximized()));
		} else
			this->showFullScreen();
	}


	// Update the thumbnails
	if(applySet["thumb"]) {

		// The max thumbnail height is
		int thbHeight = globSet->thumbnailsize + globSet->thumbnailLiftUp + 30;
		int viewH = viewBig->height();
		int viewW = viewBig->width();

		// If the thumbnails are supposed to be shown at the bottom
		if(globSet->thumbnailposition == "Bottom") {

			// Adjust the thumbnail geometry
			viewThumbs->setRect(QRect(0,viewH-thbHeight,viewW,thbHeight));

			// Adjust the menu geometry
			if(setupWidgets->menu) menu->setRect(QRect(viewW-450,0,300,300));


		// If the thumbnails are supposed to be shown at the top
		} else if(globSet->thumbnailposition == "Top") {

			// Adjust the thumbnail geometry
			viewThumbs->setRect(QRect(0,0,viewBig->width(),thbHeight));

			// Adjust the menu geometry
			if(setupWidgets->menu) menu->setRect(QRect(viewBig->width()-450,viewBig->height()-300,300,300));

		}

		// (Re-)Load dir
		if(globVar->currentfile != "")
			viewThumbs->loadDir(true);

		// Keep Thumbs visible?
		if(globSet->thumbnailKeepVisible && !viewThumbs->isVisible())
			viewThumbs->makeShow();


	}

	if(applySet["menu"]) {
		// Adjusting the menu sensitivity
		if(setupWidgets->menu) menu->setSensitivity(globSet->menusensitivity*3,globSet->menusensitivity*10);
	}

	if(applySet["thumb"] || applySet["quickinfo"])
		// Update the quickinfo labels
// rotation indicator
/*		viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,globVar->tosave.value(globVar->currentfile));*/
		viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);


	if(applySet["redrawimg"])
		drawImage();


	if(applySet["exif"])
		details->updateFontsize();


	QString qtfiles = globSet->knownFileTypesQt;
	QString qtfilesextras = globSet->knownFileTypesQtExtras;
	if(qtfiles != "" && qtfilesextras != "") qtfiles += ",";
	qtfiles += qtfilesextras;

	QString gmfiles = globSet->knownFileTypesGm;
	QString gmfilesextras = globSet->knownFileTypesGmExtras;
	if(gmfiles != "" && gmfilesextras != "") gmfiles += ",";
	gmfiles += gmfilesextras;

	imageReader->gmfiles = gmfiles;
	imageReader->qtfiles = qtfiles;


	if(setupWidgets->menu) menu->allItems["hide"]->setEnabled(globSet->trayicon);

}

// If a widget (like about or settings) is opened, all other functions are suspended
void MainWindow::blockFunc(bool bl) {

	if(globVar->verbose) std::clog << "Blocking Interface:" << bl << std::endl;

	globVar->blocked = bl;
}

// The close event
void MainWindow::closeEvent(QCloseEvent *e) {

	// If a widget (like settings or about) is open, then this close event only closes this widget (like escape)
	if(globVar->blocked) {

		if(globVar->verbose) std::clog << "Ignoring closeEvent, sending 'Escape' shortcut" << std::endl;

		e->ignore();

		systemShortcutDO("Escape");

	} else {

		// Stop the thumbnail thread
		viewThumbs->stopThbCreation();

		// Hide to system tray
		if(globSet->trayicon && !globVar->skipTrayIcon) {

			if(set->isVisible())
				set->makeHide();
			globVar->restoringFromTrayNoResize = QDateTime::currentDateTime().toTime_t();
			this->hide();
			if(globVar->verbose) std::clog << "Hiding to System Tray." << std::endl;
			e->ignore();

		// Quit
		} else {
			viewBig->scene()->clear();
			viewThumbs->view->scene.clear();

			e->accept();

			std::cout << "Goodbye!" << std::endl;
		}

	}

}

// Draw the big main image
void MainWindow::drawImage() {

	if(globVar->verbose) std::clog << "Drawing Image" << std::endl;

	// the alignment of the image is always center. If the thumbnails are kept visible, we simply add a tranparent bar to the bottom of the image (happening just a couple lines below)
	viewBig->setAlignment(Qt::AlignCenter);

	// No file loaded yet,...
	if(globVar->currentfile == "") {

		if(globVar->verbose) std::clog << "Ask for filename" << std::endl;

		// ..., so open a new file
		openFile();

		// Load image
	} else {

		// Display busy cursor
		qApp->setOverrideCursor(Qt::WaitCursor);

		if(globVar->verbose) std::clog << "Got filename:" << globVar->currentfile.toStdString() << std::endl;

		// Tell the filehandling widget the new filename
		if(setupWidgets->filehandling) filehandling->currentfile = globVar->currentfile;

		// If the current directory info hasn't been loaded yet
		if(viewThumbs->counttot == 0) {
			viewThumbs->setCurrentfile(globVar->currentfile);
			viewThumbs->noThumbs = globSet->thumbnailDisable;
			viewThumbs->loadDir();
		}


		// Get the maximum possible dimensions of the main image
		int maxW = viewBig->width()-globSet->borderAroundImg*2;
		int subtractThumbnailHeight = 0;
		if(globSet->thumbnailKeepVisible)
			subtractThumbnailHeight = viewThumbs->height();
		int maxH = viewBig->height()-2*globSet->borderAroundImg-subtractThumbnailHeight;

		// In the two cases below we need to swap dimensions, otherwise the image doesn't fit
		if((globVar->rotation == -270 || globVar->rotation == -90)) {
			int t = maxW;
			maxW = maxH;
			maxH = t;
		}

		// Get the image
		QImage img = imageReader->readImage(globVar->currentfile,globVar->rotation,globVar->zoomed,QSize(maxW,maxH),true);

		if(!globVar->zoomToActualSize) {
			// The imagereader stores two possible scaling factors
			if(imageReader->scaleImg1 != -1)
				viewBig->scale(imageReader->scaleImg1,imageReader->scaleImg1);
			if(imageReader->scaleImg2 != -1)
				viewBig->scale(imageReader->scaleImg2,imageReader->scaleImg2);
		}

		// Get the fileformat and the original size
		QString fileformat = imageReader->fileformat;
		QSize origSize = imageReader->origSize;
		globVar->originalImageSize = origSize;

		// If the thumbnails are kept visible we need to add a transparent bar to the main image so that they don't overlap
		graphItem->setTransBarHeight((!globVar->zoomed && globSet->thumbnailKeepVisible) ? viewThumbs->height() : 0);
		// Is it an animated image?
		if(imageReader->animatedImg)
			graphItem->setMovie(globVar->currentfile,origSize.width(),origSize.height());
		// Otherwise do the normal setPixmap()
		else
			graphItem->setPixmap(QPixmap::fromImage(img));

		int imgWidth = img.width();
		int imgHeight = img.height();
		if((globVar->rotation == -270 || globVar->rotation == -90)) {
			int t = imgWidth;
			imgWidth = imgHeight;
			imgHeight = t;
		}

		if(imgWidth < viewBig->width()) {
			// Set the right position of the main image
			int graphItemX = (viewBig->width()-imgWidth)/2.0;
			graphItem->setX(graphItemX);
		}
		if(imgHeight < viewBig->height()) {
			int graphItemY = (viewBig->height()-imgHeight)/2.0;
			graphItem->setY(graphItemY);
		}

		// Update the current position of the image in the directory
		viewThumbs->countpos = viewThumbs->getImageFilePathIndexOf(globVar->currentfile);

		// Adjust scene rect
		viewBig->scene()->setSceneRect(viewBig->scene()->itemsBoundingRect());

		// If exif isn't read yet AND we also don't need to get this data while a slideshow is running
		// (if we check, then it asking if user wants photo rotated can lead to undefined behaviour)
		if(!globVar->exifRead && !globVar->slideshowRunning) {

			if(globVar->verbose) std::clog << "Requesting Exif Info" << std::endl;

			// Exif info read
			globVar->exifRead = true;

			// These formats known by Photo are supported by exiv2
			QStringList formats;
			formats << "bmp" << "gif" << "tiff" << "jpg" << "jpeg" << "png";

			// If supported, load exiv2 data
			if(formats.contains(fileformat))
				// Update the exif data widget
				details->updateData(globVar->currentfile,origSize);
			// Otherwise set exiv2 to "unsupported"
			else
				// Update the exif data widget
				details->updateData(globVar->currentfile,origSize,false);

		}

		// The rotation/flipping is stored temporarily for each session
		// We reset the rotation/flipping after the image has changed
		if(globVar->store_rotation.keys().contains(globVar->currentfile) && globVar->store_rotation[globVar->currentfile] != 0 && globVar->store_rotation[globVar->currentfile] != globVar->rotation) {
			int val = globVar->store_rotation[globVar->currentfile];
			QString clock = "anticlock";
			if(val < 0) {
				clock = "clock";
				val *= -1;
			}
			rotateFlip(true,clock,val);
		}
		if(globVar->store_flipHor[globVar->currentfile] && globVar->store_flipHor[globVar->currentfile] != globVar->flipHor)
			rotateFlip(false,"hor");
		if(globVar->store_flipVer[globVar->currentfile] && globVar->store_flipVer[globVar->currentfile] != globVar->flipVer)
			rotateFlip(false,"ver");

		// Restore normal cursor
		qApp->restoreOverrideCursor();

		// Ensure the active thumbnail is shown
		// We only do that when the thumbnail was NOT loaded through a click on it. The reason is, that otherwise the thumbnailview might move a little (ensuring the thumbnail is visible) although it already IS visible.
		if(viewThumbs->getImageFilePathIndexOf(globVar->currentfile) != -1 && !globSet->thumbnailDisable && !viewThumbs->thumbLoadedThroughClick) {
			// We center on the image if it's a newly loaded dir
			if(viewThumbs->newlyLoadedDir)
				viewThumbs->view->centerOn(viewThumbs->allPixmaps.at(viewThumbs->getImageFilePathIndexOf(globVar->currentfile)));
			else
				viewThumbs->view->ensureVisible(viewThumbs->allPixmaps.at(viewThumbs->getImageFilePathIndexOf(globVar->currentfile)));
			// We also have to check here where the cursor is, cause sometimes the app reaches these statements here when auto rotate is active (exif)
			if(!globSet->thumbnailKeepVisible && viewThumbs->isVisible() && !viewThumbs->thumbLoadedThroughClick && !viewThumbs->areaShown().contains(QCursor::pos()))
				viewThumbs->makeHide();
			viewThumbs->startThread();
		} else if(viewThumbs->thumbLoadedThroughClick)
			viewThumbs->thumbLoadedThroughClick = false;

	}

}

// When the Exif data dictates an orientation
void MainWindow::getOrientationFromExif(int degree, bool flipHor) {

	if(globVar->verbose) std::clog << "Exif Rotate/Flip:" << degree << "-" << flipHor << std::endl;

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

// The global timer to ensure only one instance of PhotoQt running and to make PhotoQt remotely controllable
void MainWindow::globalRunningProgTimerTimeout() {

	// Updates the "running" file with current timestamp
	QFile t(QDir::homePath() + "/.photoqt/running");
	if(t.open(QIODevice::WriteOnly)) {
		QTextStream out(&t);
		out << QDateTime::currentMSecsSinceEpoch();
		t.close();
	} else
		std::cerr << "ERROR! Unable to write to file '~/.photoqt/running' - unable to control this instance through command line." << std::endl;

	// Checks the "cmd" file for commands to be executed
	QFile cmd(QDir::homePath() + "/.photoqt/cmd");

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

		if(globVar->verbose) std::clog << "Got passed on commands" << std::endl;

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

		} while (!line.isNull());

		cmd.close();

	} else if(cmd.exists())
		std::cerr << "ERROR! Can't read '~/.photoqt/cmd'." << std::endl;

	// Remove file after command is read in
	cmd.remove();

	if(doNewFile != "") {
		if(this->isHidden())
			doShow = true;
		doOpen = false;
	}


	// Toggle PhotoQt
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
		if(this->isHidden()) {
			takeScreenshots();	// Refresh screenshots
			if(!globSet->windowmode) {
				this->showFullScreen();
			} else if(globVar->windowMaximised) {
				this->showMaximized();
				QTimer::singleShot(100,this,SLOT(showMaximized()));
				globVar->windowMaximised = true;
			} else {
				this->showNormal();
				globVar->windowMaximised = false;
			}
			setBackground();	// Refresh background
		}
		doShow = false;
		doHide = false;
		openFile();
	}

	// Show PhotoQt
	if(doShow) {
		if(this->isHidden()) {
			takeScreenshots();	// Refresh screenshots
			if(!globSet->windowmode) {
				this->showFullScreen();
			} else if(globVar->windowMaximised) {
				this->showMaximized();
				QTimer::singleShot(100,this,SLOT(showMaximized()));
				globVar->windowMaximised = true;
			} else {
				this->showNormal();
				globVar->windowMaximised = false;
			}
			setBackground();	// Refresh background
		}
		this->activateWindow();
		this->raise();
		doHide = false;
		if(globVar->currentfile == "" && doNewFile == "")
			openFile();
	}

	// Hide PhotoQt
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

	if(globVar->verbose) std::clog << "Click received:" << QString("x:y: %1:%2").arg(p.x()).arg(p.y()).toStdString() << std::endl;

	QPointF point = viewBig->mapToScene(p);

	int x = point.x();
	int y = point.y();

	int xg = graphItem->pos().x();
	int yg = graphItem->pos().y();
	int wg = graphItem->pixmap().width();
	int hg = graphItem->pixmap().height();

	// If click was on grey area outside image and corresponding option is set, then close PhotoQt
	if(x < xg || x > xg+wg || y < yg || y > yg+hg || globVar->currentfile.trimmed().length() == 0) {
		if(globSet->closeongrey) {
			globVar->skipTrayIcon = true;
			this->close();
		}
	}

}

// Enabling the key detection by the shortcuts
void MainWindow::keyReleaseEvent(QKeyEvent *e) {

	if(globVar->verbose) std::clog << "Got Key Event:" << e->key() << std::endl;

	if(set->tabsSetup && set->tabShortcuts->detect->isVisible() && set->tabShortcuts->detect->keyShortcut->isChecked())
		set->tabShortcuts->detect->analyseKeyEvent(e);

	QMainWindow::keyPressEvent(e);

}

// Load a new image from the open file dialog
void MainWindow::loadNewImgFromOpen(QString path, bool hideImageFilterLabel) {

	if(globVar->verbose) std::clog << "Load from Open:" << path.toStdString() << std::endl;

	if(setupWidgets->filterimages && hideImageFilterLabel) filterImagesDisplay->setVisible(false);

	// We need this later to update the thumbnail view
	QString temp = globVar->currentfile;

	// Update the currentfile
	globVar->currentfile = path;
	viewThumbs->setCurrentfile(path);

	// Load thumbnails
	if(hideImageFilterLabel) viewThumbs->removeFilter();
	viewThumbs->counttot = 0;
	viewThumbs->countpos = 0;
	viewThumbs->allImgsInfo.clear();
	viewThumbs->noThumbs = globSet->thumbnailDisable;
	viewThumbs->loadDir();
	globVar->exifRead = false;

	// When a new image is loaded we reset any zooming, rotation, flipping
	zoom(true,((globVar->zoomToActualSize || globVar->zoomed) && globSet->transition != 0) ? "reset" : "resetNoDraw");
	rotateFlip(true,"resetNoDraw");
	rotateFlip(false, "reset");
	globVar->store_rotation.clear();
	globVar->store_flipHor.clear();
	globVar->store_flipVer.clear();

	// Remove "to-save" property
// rotation indicator
/*	globVar->tosave.clear();*/

	// Draw new image
	drawImage();

	viewBig->imgLoaded = true;

	// If thumbnails are not disabled
	if(!globSet->thumbnailDisable)
		viewThumbs->updateThbViewHoverNormPix(temp,globVar->currentfile);

	// Update quickinfo labels
// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,false);*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

}

// Load a new image in current dir from the thumbnail view
void MainWindow::loadNewImgFromThumbs(QString path) {

	if(globVar->verbose) std::clog << "Load from Thumbs:" << path.toStdString() << std::endl;

	// Reset zooming parameter
	globVar->zoomedImgAtLeastOnce = false;

	// When a new image is loaded we reset any zooing, rotation, flipping
	QMap<QString,int> tmp_rotation = globVar->store_rotation;
	QMap<QString,bool> tmp_flipHor = globVar->store_flipHor;
	QMap<QString,bool> tmp_flipVer = globVar->store_flipVer;
	zoom(true,((globVar->zoomToActualSize || globVar->zoomed) && globSet->transition != 0) ? "reset" : "resetNoDraw");
	rotateFlip(true,"resetNoDraw");
	rotateFlip(false, "reset");
	globVar->store_rotation = tmp_rotation;
	globVar->store_flipHor = tmp_flipHor;
	globVar->store_flipVer = tmp_flipVer;

	viewBig->absoluteScaleFactor = 0;
	globVar->zoomed = false;
	globVar->exifRead = false;

	globVar->currentfile = path;
	viewThumbs->countpos = viewThumbs->getImageFilePathIndexOf(path);
	viewThumbs->setCurrentfile(globVar->currentfile);
	drawImage();

	viewBig->imgLoaded = true;

// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,globVar->tosave.value(globVar->currentfile));*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

}

// A menuitem has been clicked
void MainWindow::menuClicked(QString txt, int close) {

	if(globVar->verbose) std::clog << "Menu clicked: " << close << " - " << txt.toStdString() << std::endl;

	globVar->zoomedImgAtLeastOnce = false;

	// If this item was set up so that the menu closes on executing, then do it before executing the actual command (some commands could hold the menu open until the function call is finished)
	if(close) menu->animate();

	if(txt == "hideMeta") {
		menu->allItems["hideMeta"]->setText(details->isVisible() ? tr("Show Details") : tr("Hide Details"));
		menu->allItems["hideMeta"]->setIcon(":/img/exif.png");
	}


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
				if(y < viewBig->height()-h-20 && viewThumbs->isVisible() && (!globSet->thumbnailKeepVisible || (globVar->zoomedImgAtLeastOnce && globVar->zoomed && viewBig->absoluteScaleFactor > 0)))
					viewThumbs->makeHide();

				if(y > viewBig->height()-20 && !viewThumbs->isVisible()) {
					viewThumbs->makeShow();
					QTimer::singleShot(520,viewThumbs,SLOT(scrolledView()));
				}
			}

			int menuX = viewBig->width()-450;
			int menuW = 300;

			// Animate menu
			if(x >= menuX-globSet->menusensitivity*3 && x <= menuX+menuW+globSet->menusensitivity*3 && y <= globSet->menusensitivity*10 && (!setupWidgets->menu || !menu->isVisible())) {
				if(!setupWidgets->menu) setupWidget("menu");
				menu->makeShow();
			}

			if(setupWidgets->menu && (x < menu->x()-globSet->menusensitivity*3 || x > menu->x()+menu->width()+globSet->menusensitivity*3 || y > menu->height()+globSet->menusensitivity*10) && menu->isVisible()) {
				menu->makeHide();
			}


		} else if(globSet->thumbnailposition == "Top") {

			// Animate thumbnail bar
			if(!globSet->thumbnailDisable) {

				if(y < 20 && !viewThumbs->isVisible())
					viewThumbs->makeShow();

				int viewThbH = globSet->thumbnailsize + globSet->thumbnailLiftUp + 30;
				if(y > viewThbH+20 && (!globSet->thumbnailKeepVisible || globVar->zoomedImgAtLeastOnce) && viewThumbs->isVisible())
					viewThumbs->makeHide();
			}

			int menuX = viewBig->width()-450;
			int menuW = 300;
			int menuY = viewBig->height()-250;

			// Animate menu
			if(x >= menuX-globSet->menusensitivity*3 && x <= menuX+menuW+globSet->menusensitivity*3 && y >= menuY-globSet->menusensitivity*10 && (!setupWidgets->menu || !menu->isVisible())) {
				if(!setupWidgets->menu) setupWidget("menu");
				menu->makeShow();
			}

			if(setupWidgets->menu && (x < menu->x()-globSet->menusensitivity*3 || x > menu->x()+menu->width()+globSet->menusensitivity*3 || y < menu->y()-globSet->menusensitivity*10) && menu->isVisible())
				menu->makeHide();

		}


		// Animate details widget
		if(x < 10*globSet->menusensitivity && y > details->y()-3*globSet->menusensitivity && y < details->y()+details->height()+globSet->menusensitivity*3 && !details->isVisible() && globSet->exifenablemousetriggering) {
			details->makeShow();
			if(setupWidgets->menu) {
				menu->allItems["hideMeta"]->setText(tr("Hide Details"));
				menu->allItems["hideMeta"]->setIcon(":/img/exif.png");
			}
		}

		if((x > details->width()+10*globSet->menusensitivity || y < details->y()-3*globSet->menusensitivity || y > details->y()+details->height()+3*globSet->menusensitivity) && details->isVisible() && !details->stay->isChecked()) {
			details->makeHide();
			if(setupWidgets->menu) {
				menu->allItems["hideMeta"]->setText(tr("Show Details"));
				menu->allItems["hideMeta"]->setIcon(":/img/exif.png");
			}
		}


		// Animate Quicksettings widget
		if(x > viewBig->width()-10*globSet->menusensitivity && globSet->quickSettings) {
			if(!setupWidgets->quicksettings) setupWidget("quicksetting");
			quickset->makeShow();
		}
		if(setupWidgets->quicksettings && quickset->isVisible() && x < viewBig->width()-10*globSet->menusensitivity && (x < viewBig->width()-quickset->width()-10*globSet->menusensitivity || y < quickset->y()-10*globSet->menusensitivity || y> quickset->y()+quickset->height()+10*globSet->menusensitivity))
			quickset->makeHide();


	// If globVar->blocked is set, but slideshow is running, animate slideshowbar
	} else if(setupWidgets->slideshowbar && slideshowbar->isEnabled()) {

		if(globSet->thumbnailposition == "Bottom") {

			if(y < 30 && !slideshowbar->isVisible())
				slideshowbar->makeShow();

			if(y > slideshowbar->height() && slideshowbar->isVisible())
				slideshowbar->makeHide();

		} else {

			if(y > this->height()-30 && !slideshowbar->isVisible())
				slideshowbar->makeShow();

			if(y < this->height()-slideshowbar->height() && slideshowbar->isVisible())
				slideshowbar->makeHide();

		}

	}

}

// Move in the current directory (1=right, 0=left)
void MainWindow::moveInDirectory(int direction) {

	if(setupWidgets->filterimages && filterImagesDisplay->isVisible() && viewThumbs->counttot == 0) return;

	if(globVar->verbose) std::clog << "Move in directory: " << direction << std::endl;

	// When a new image is loaded we reset any zooing, rotation, flipping
	QMap<QString,int> tmp_rotation = globVar->store_rotation;
	QMap<QString,bool> tmp_flipHor = globVar->store_flipHor;
	QMap<QString,bool> tmp_flipVer = globVar->store_flipVer;
	zoom(true,((globVar->zoomToActualSize || globVar->zoomed) && globSet->transition != 0) ? "reset" : "resetNoDraw");
	rotateFlip(true,"resetNoDraw");
	rotateFlip(false, "reset");
	globVar->store_rotation = tmp_rotation;
	globVar->store_flipHor = tmp_flipHor;
	globVar->store_flipVer = tmp_flipVer;


	// Reset these parameters
	viewBig->absoluteScaleFactor = 0;
	globVar->zoomed = false;
	globVar->rotation = 0;
	globVar->exifRead = false;

	// Move to right, not the end of directory
	if(direction == 1 && viewThumbs->countpos < viewThumbs->counttot-1) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->getImageFilePathAt(viewThumbs->countpos+1);
		++viewThumbs->countpos;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->getCurrentfile(),globVar->currentfile);
		viewThumbs->setCurrentfile(globVar->currentfile);
		drawImage();
	// Move to right, end of directory
	} else if(direction == 1 && viewThumbs->countpos == viewThumbs->counttot-1 && globSet->loopthroughfolder && viewThumbs->counttot > 0 && (!setupWidgets->slideshowbar || !slideshowbar->isEnabled())) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->getImageFilePathAt(0);
		viewThumbs->countpos = 1;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->getCurrentfile(),globVar->currentfile);
		viewThumbs->setCurrentfile(globVar->currentfile);
		drawImage();
	// Move to left, not the end of directory
	} else if(direction == 0 && viewThumbs->countpos > 0) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->getImageFilePathAt(viewThumbs->countpos-1);
		--viewThumbs->countpos;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->getCurrentfile(),globVar->currentfile);
		viewThumbs->setCurrentfile(globVar->currentfile);
		drawImage();
	// Move to left, end of directory
	} else if(direction == 0 && viewThumbs->countpos == 0 && globSet->loopthroughfolder && viewThumbs->counttot > 0) {
		globVar->zoomedImgAtLeastOnce = false;
		globVar->currentfile = viewThumbs->getImageFilePathAt(viewThumbs->counttot-1);
		viewThumbs->countpos = viewThumbs->counttot-1;
		viewThumbs->updateThbViewHoverNormPix(viewThumbs->getCurrentfile(),globVar->currentfile);
		viewThumbs->setCurrentfile(globVar->currentfile);
		drawImage();
	// If a slideshow is running and we're at the end of the directory, then we stop
	} else if(setupWidgets->slideshowbar && slideshowbar->isEnabled())
		stopSlideShow();

	// Update quickinfo labels
// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,globVar->tosave.value(globVar->currentfile));*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

}

// Open a new file. I had to remove (temporarily) PhotoQt's custom Open File Dialog, because it just wasn't working right yet. It has to be completely re-done from scratch in a different way (using a model/view)
void MainWindow::openFile() {

	if(globVar->verbose) std::clog << "Got request to open new file" << std::endl;

	// Stopping a possibly running thread
	viewThumbs->stopThbCreation();

	// Open the FileDialog in the dir of last image. If PhotoQt just was started (i.e. no current image), then open in home dir
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

	if(globVar->verbose) std::clog << "Reload current directory:" << t.toStdString() << std::endl;

	// If file was renamed, simply reload renamed file
	if(t == "rename" && setupWidgets->filehandling) {
		loadNewImgFromOpen(filehandling->currentfile);

	// If file was deleted
	} else if(t == "delete" && setupWidgets->filehandling) {

		// If it was the last file in the directory
		if(viewThumbs->counttot == 1) {
			viewBig->scene()->clear();
			viewThumbs->view->scene.clear();
			viewThumbs->counttot = 0;
			viewThumbs->countpos = 0;
			viewThumbs->allImgsInfo.clear();
			globVar->currentfile = "";
			viewThumbs->setCurrentfile("");
			filehandling->currentfile = "";
// rotation indicator
/*			viewBigLay->updateInfo("",0,0,false);*/
			viewBigLay->updateInfo("",0,0);
			drawImage();
		// If it wasn't the last file, then load file to left/right
		} else {
			QString newfile = "";
			if(viewThumbs->countpos+1 == viewThumbs->counttot)
				newfile = viewThumbs->getImageFilePathAt(viewThumbs->counttot-2);
			else if(viewThumbs->countpos == 0)
				newfile = viewThumbs->getImageFilePathAt(1);
			else
				newfile = viewThumbs->getImageFilePathAt(viewThumbs->getImageFilePathIndexOf(globVar->currentfile)+1);
			// We use this function to reload the current directory at the same time as loading a new image
			loadNewImgFromOpen(newfile);
		}

	}

}

// The resize event
void MainWindow::resizeEvent(QResizeEvent *) {

	if(globVar->verbose) std::clog << "Window resized" << std::endl;

	// When PhotoQt is minimised and is restored, then the widget actually isn't resized, so the stuff in this function doesn't need to be done

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
	viewBig->scene()->setSceneRect(viewBig->scene()->itemsBoundingRect());

	// And if an image is loaded, redraw it
	if(globVar->currentfile != "" && !globVar->zoomed && (QDateTime::currentDateTime().toTime_t() - globVar->restoringFromTrayNoResize) > 1)
		drawImage();

	globVar->windowMaximised = this->isMaximized();

}

// Restore the default settings
void MainWindow::restoreDefaultSettings() {

	if(globVar->verbose) std::clog << "Restoring default settings" << std::endl;

	globSet->setDefault();
	globSet->saveSettings();

}

// This function flips the current big image vertically/horizontally
void MainWindow::rotateFlip(bool rotateNotFlipped, QString direction, int rotateSpecificAmount) {

	if(globVar->verbose) std::clog << "Rotate and Flip: " << rotateNotFlipped << " - " << direction.toStdString() << std::endl;

	if(rotateNotFlipped) {

		// We need to reverse direction if image was flipped once
		// (since the flipping also reverts direction once)
		if((globVar->flipHor || globVar->flipVer) && !(globVar->flipHor && globVar->flipVer)) {
			if(direction == "clock")
				direction = "anticlock";
			else if(direction == "anticlock")
				direction = "clock";
		}

		// ROTATE

		int rot = (rotateSpecificAmount == 0 ? 90 : rotateSpecificAmount);

		if(direction == "clock") {

// rotation indicator
/*			globVar->tosave.insert(globVar->currentfile,true);*/

			globVar->rotation -= rot;
			viewBig->rotate(rot);
			globVar->rotation %= 360;

			globVar->store_rotation[globVar->currentfile] = globVar->rotation;

			if(!globVar->zoomed)
				drawImage();

		} else if(direction == "anticlock") {

// rotation indicator
/*			globVar->tosave.insert(globVar->currentfile,true);*/

			globVar->rotation += rot;
			viewBig->rotate(-rot);
			globVar->rotation %= 360;

			globVar->store_rotation[globVar->currentfile] = globVar->rotation;

			if(!globVar->zoomed)
				drawImage();

		} else if(direction.startsWith("reset")) {

// rotation indicator
/*			globVar->tosave.remove(globVar->currentfile);*/

			viewBig->rotate(globVar->rotation);
			globVar->rotation = 0;

			globVar->store_rotation[globVar->currentfile] = globVar->rotation;

			if(direction != "resetNoDraw" && !globVar->zoomed)
				drawImage();

		}


	} else {

		// FLIP

		if(direction == "hor") {
// rotation indicator
/*			globVar->tosave.insert(globVar->currentfile,true);*/
			viewBig->scale(-1,1);
			globVar->flipHor = !globVar->flipHor;
			globVar->store_flipHor[globVar->currentfile] = globVar->flipHor;
		} else if(direction == "ver") {
// rotation indicator
/*			globVar->tosave.insert(globVar->currentfile,true);*/
			viewBig->scale(1,-1);
			globVar->flipVer = !globVar->flipVer;
			globVar->store_flipVer[globVar->currentfile] = globVar->flipVer;
		} else if(direction == "reset") {
// rotation indicator
/*			globVar->tosave.remove(globVar->currentfile);*/
			if(globVar->flipHor)
				viewBig->scale(-1,1);
			if(globVar->flipVer)
				viewBig->scale(1,-1);
			globVar->flipHor = false;
			globVar->flipVer = false;
			globVar->store_flipHor[globVar->currentfile] = false;
			globVar->store_flipVer[globVar->currentfile] = false;
		}


	}

// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,globVar->tosave.value(globVar->currentfile));*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);


}

// Set the background of PhotoQt
void MainWindow::setBackground() {

	if(globVar->verbose) std::clog << "Set the background" << std::endl;

	// If compositing is enabled, then we'll use it (for the mainwindow background)
	if(globSet->composite) {

		if(globVar->verbose) std::clog << "Composite enabled" << std::endl;

		this->setAttribute(Qt::WA_TranslucentBackground);
		bglabel->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(globSet->bgColorRed).arg(globSet->bgColorGreen).arg(globSet->bgColorBlue).arg(globSet->bgColorAlpha));

		// We set to bglabel an empty pixmap. We can't simply hide it, because it contains every widget in the app
		bglabel->setPixmap(QPixmap());


	// If compositing is disabled, then we'll simply take a screenshot and simulate some sort of compositing
	} else {

		this->setAttribute(Qt::WA_TranslucentBackground, false);
		this->setAttribute(Qt::WA_NoSystemBackground,false);

		// SET SCREENSHOT AS BACKGROUND
		if(globSet->backgroundImageScreenshot) {

			if(globVar->verbose) std::clog << "Setting screenshot as background" << std::endl;

			// That's the background image
			QPixmap base;
			QRect curRect;
			for(int i = 0; i < QGuiApplication::screens().count(); ++i) {
				if(QGuiApplication::screens().at(i)->geometry().contains(this->x(),this->y())) {
					base = screenshots.at(i);
					curRect = QGuiApplication::screens().at(i)->geometry();
				}
			}

			QPixmap bg(base.size());
			bg.fill(Qt::transparent);

			QPixmap overlay(base.size());
			overlay.fill(Qt::transparent);
			QPainter overlayPaint(&overlay);
			overlayPaint.setCompositionMode(QPainter::CompositionMode_SourceOver);
			QColor col;
			col.setRgba(qRgba(globSet->bgColorRed,globSet->bgColorGreen,globSet->bgColorBlue,globSet->bgColorAlpha));
			overlayPaint.fillRect(overlay.rect(),col);

			// Let's start
			QPainter painter(&bg);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			painter.drawPixmap(QRect(0,0,curRect.width(),curRect.height()), base, QRect(0,0,curRect.width(),curRect.height()));
			painter.drawPixmap(overlay.rect(), overlay);

			painter.end();

			// Setting the background image as background of label
			bglabel->setPixmap(bg);

		// SET BACKGROUND IMAGE
		} else if(globSet->backgroundImageUse) {

			if(globVar->verbose) std::clog << "Use background image" << std::endl;

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

			if(globVar->verbose) std::clog << "Use background colour" << std::endl;

			bglabel->setPixmap(QPixmap());

			bglabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(globSet->bgColorRed).arg(globSet->bgColorGreen).arg(globSet->bgColorBlue));

		}

		bglabel->show();
	}

}

void MainWindow::removeImageFilter() {

	viewThumbs->removeFilter();

	globVar->curDir = "";

	if(globVar->currentfile == "")
		loadNewImgFromOpen(globVar->fileBeforeEmptyFilter);
	else
		loadNewImgFromOpen(globVar->currentfile);
	globVar->fileBeforeEmptyFilter = "";

}

// Filter images in current directory
void MainWindow::setImageFilter(QString curDir, QStringList filter) {

	if(filter.isEmpty()) return;

	viewThumbs->setFilter(filter);

	// Get filepath
	QString path = "";
	// If there is a current filter set, we need to look at the actual directory content
	if(filterImagesDisplay->isVisible()) {
		QStringList filterMe;
		for(int i = 0; i < filter.length(); ++i)
			filterMe.append("*" + filter.at(i) + (filter.at(i).startsWith(".") ? "" : "*"));
		QDir dir;
		dir.setPath(curDir);
		QFileInfoList l = dir.entryInfoList(filterMe);
		if(l.length() != 0)
			path = l.at(0).absoluteFilePath();
	// If no filter is currently set, we can look at the list in the Thumbnails class
	} else {
		for(int i = 0; i < viewThumbs->allImgsInfo.length(); ++i) {
			for(int j = 0; j < filter.length(); ++j) {
				if((!filter.at(j).startsWith(".") && viewThumbs->allImgsInfo.at(i).fileName().contains(filter.at(j)))
					|| (filter.at(j).startsWith(".") && viewThumbs->allImgsInfo.at(i).fileName().endsWith(filter.at(j)))) {
					path = viewThumbs->allImgsInfo.at(i).absoluteFilePath();
					break;
				}
			}
			if(path != "") break;
		}
	}

	// Display filter on top of thumbnails
	filterImagesDisplay->showFilter(filter);


	// Load image (if any result found)
	if(path != "")
		loadNewImgFromOpen(path, false);
	// Show 'empty' image
	else {
		if(globVar->currentfile != "")
			globVar->fileBeforeEmptyFilter = globVar->currentfile;

		viewThumbs->view->scene.clear();
		viewThumbs->counttot = 0;
		viewThumbs->countpos = 0;
		viewThumbs->allImgsInfo.clear();
		globVar->currentfile = "";
		viewThumbs->setCurrentfile("");
		if(setupWidgets->filehandling) filehandling->currentfile = "";
// rotation indicator
/*		viewBigLay->updateInfo("",-1,-1,false);*/
		viewBigLay->updateInfo("",-1,-1);
		viewBig->scene()->setSceneRect(viewBig->scene()->itemsBoundingRect());
		graphItem->setPixmap(QPixmap(":/img/noresults.png"));
		rotateFlip(true,"resetNoDraw");
		rotateFlip(false,"reset");
	}

}

// Setup the shortcuts
void MainWindow::setupShortcuts() {

	if(globVar->verbose) std::clog << "Setup Shortcuts" << std::endl;

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

	if(globVar->verbose) std::clog << "Setup Tray Icon" << std::endl;

	// The Tray Icon
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/img/icon.png"));
	trayIcon->setToolTip("PhotoQt - " + tr("Image Viewer"));

	// A context menu for the tray icon
	trayIconMenu = new QMenu(this);
	trayIconMenu->setStyleSheet("background-color: rgb(67,67,67); color: white; border-radius: 5px;");

	// A new action for the menu
	QAction *trayAcToggle = new QAction(QIcon(":/img/logo.png"),tr("Hide/Show PhotoQt"),this);
	trayAcToggle->setObjectName("toggle");
	trayIconMenu->addAction(trayAcToggle);

	// A new action for the menu
	QAction *trayAcLoad = new QAction(QIcon(":/img/open.png"),tr("Open New Image"),this);
	trayAcLoad->setObjectName("open");
	trayIconMenu->addAction(trayAcLoad);

	// A seperator for the menu
	trayIconMenu->addSeparator();

	// A new action for the menu
	QAction *trayAcQuit = new QAction(QIcon(":/img/quit.png"),tr("Quit PhotoQt"),this);
	trayAcQuit->setObjectName("quit");
	trayIconMenu->addAction(trayAcQuit);

	// Set the menu to the tray icon
	trayIcon->setContextMenu(trayIconMenu);

	// Connect signals
	connect(trayAcToggle, SIGNAL(triggered()), this, SLOT(trayAcDo()));
	connect(trayAcLoad, SIGNAL(triggered()), this, SLOT(trayAcDo()));
	connect(trayAcQuit, SIGNAL(triggered()), this, SLOT(trayAcDo()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayAcDo(QSystemTrayIcon::ActivationReason)));

	// The tray icon is always shown, but PhotoQt is not necessarily always minimised to it (depending on user settings)
	trayIcon->show();

}

// Some widgets aren't setup initially (faster startup)
void MainWindow::setupWidget(QString what) {

	// Set up Menu
	if(what == "menu" && !setupWidgets->menu) {

		if(globVar->verbose) std::clog << "Setting up menu" << std::endl;

		setupWidgets->menu = true;

		menu = new DropDownMenu(viewBig);
		connect(menu, SIGNAL(itemClicked(QString,int)), this, SLOT(menuClicked(QString,int)));

		if(globSet->thumbnailposition == "Bottom")
			menu->setRect(QRect(viewBig->width()-450,0,300,300));
		else if(globSet->thumbnailposition == "Top")
			menu->setRect(QRect(viewBig->width()-450,viewBig->height()-300,300,300));

		menu->show();

		menu->setSensitivity(globSet->menusensitivity*3,globSet->menusensitivity*10);

		if(details->isVisible() && details->stay->isChecked()) {
			menu->allItems["hideMeta"]->setText(tr("Hide Details"));
			menu->allItems["hideMeta"]->setIcon(":/img/exif.png");
		}

		menu->allItems["hide"]->setEnabled(globSet->trayicon);


	}

	// Set up filehandling
	if(what == "filehandling" && !setupWidgets->filehandling) {

		if(globVar->verbose) std::clog << "Setting up filehandling" << std::endl;

		setupWidgets->filehandling = true;

		filehandling = new FileHandling(viewBig,globVar->verbose,globVar->currentfile);
		filehandling->setRect(QRect(0,0,viewBig->width(),viewBig->height()));
		filehandling->show();

		connect(filehandling, SIGNAL(reloadDir(QString)), this, SLOT(reloadDir(QString)));
		connect(filehandling, SIGNAL(stopThbCreation()), viewThumbs, SLOT(stopThbCreation()));

		connect(filehandling, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));



	}

	// Set up about
	if(what == "about" && !setupWidgets->about) {

		if(globVar->verbose) std::clog << "Setting up about" << std::endl;

		setupWidgets->about = true;

		about = new About(viewBig);
		about->setLicense(globSet->version);
		about->setRect(QRect(0,0,viewBig->width(),viewBig->height()));
		about->show();

		connect(about, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));

	}

	// Set up wallpaper
	if(what == "wallpaper" && !setupWidgets->wallpaper) {

		if(globVar->verbose) std::clog << "Setting up wallpaper" << std::endl;

		setupWidgets->wallpaper = true;

		wallpaper = new Wallpaper(globSet->toSignalOut(),globVar->verbose,viewBig);
		wallpaper->globSet = globSet->toSignalOut();
		wallpaper->setRect(QRect(0,0,viewBig->width(),viewBig->height()));
		wallpaper->show();

		connect(wallpaper, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));

	}

	if(what == "filterimages" && !setupWidgets->filterimages) {

		if(globVar->verbose) std::clog << "Setting up FilterImages" << std::endl;

		setupWidgets->filterimages = true;

		filterImagesSetup = new FilterImagesSetup(viewBig);
		filterImagesSetup->setRect(QRect(0,0,viewBig->width(),viewBig->height()));
		filterImagesSetup->show();

		filterImagesDisplay = new FilterImagesDisplay(viewThumbs);
		filterImagesDisplay->show();
		filterImagesDisplay->setVisible(false);

		connect(filterImagesSetup, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));
		connect(filterImagesSetup, SIGNAL(setFilter(QString,QStringList)), this, SLOT(setImageFilter(QString,QStringList)));
		connect(filterImagesSetup, SIGNAL(removeFilter()), this, SLOT(removeImageFilter()));
		connect(filterImagesDisplay, SIGNAL(removeImageFilter()), this, SLOT(removeImageFilter()));
		connect(filterImagesDisplay, SIGNAL(changeFilter()), filterImagesSetup, SLOT(animate()));

	}

	// Set up slideshow
	if(what == "slideshow" && !setupWidgets->slideshow) {

		if(globVar->verbose) std::clog << "Setting up slideshow" << std::endl;

		setupWidgets->slideshow = true;

		slideshow = new SlideShow(globSet->toSignalOut(), globVar->verbose, viewBig);
		slideshow->setRect(QRect(0,0,viewBig->width(),viewBig->height()));
		slideshow->show();
		slideshow->globSet = globSet->toSignalOut();

		connect(slideshow, SIGNAL(startSlideShow()), this, SLOT(startSlideShow()));
		connect(slideshow, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));

	}

	// Set up slideshowbar
	if(what == "slideshowbar" && !setupWidgets->slideshowbar) {

		if(globVar->verbose) std::clog << "Setting up slideshowbar" << std::endl;

		setupWidgets->slideshowbar = true;

		slideshowbar = new SlideShowBar(globSet->toSignalOut(), viewBig, globVar->verbose);
		slideshowbar->setWidth(this->width());
		slideshowbar->show();

		connect(slideshowbar, SIGNAL(moveInDirectory(int)), this, SLOT(moveInDirectory(int)));
		connect(slideshowbar->cancel, SIGNAL(clicked()), this, SLOT(stopSlideShow()));

	}

	if(what == "scaleimage" && !setupWidgets->scaleimage) {

		if(globVar->verbose) std::clog << "Setting up scale class" << std::endl;

		setupWidgets->scaleimage = true;

		scaleimage = new Scale(globVar->verbose, viewBig);
		scaleimage->setRect(QRect(0,0,viewBig->width(),viewBig->height()));
		scaleimage->show();

		connect(scaleimage, SIGNAL(blockFunc(bool)), this, SLOT(blockFunc(bool)));

	}

	if(what == "quicksetting" && !setupWidgets->quicksettings) {

		if(globVar->verbose) std::clog << "Setting up quick setting widget" << std::endl;

		setupWidgets->quicksettings = true;

		quickset = new QuickSettings(globSet->toSignalOut(), globVar->verbose, viewBig);
		quickset->setRect(QRect(0,0,viewBig->width(),viewBig->height()));
		quickset->show();

		connect(quickset, SIGNAL(updateSettings(QMap<QString,QVariant>)), globSet, SLOT(settingsUpdated(QMap<QString,QVariant>)));
		connect(quickset, SIGNAL(emulateShortcut(QString)), this, SLOT(shortcutDO(QString)));

	}

}

// Called by shortcuts to exec
void MainWindow::shortcutDO(QString key, bool mouseSH) {

	if(!globVar->blocked) {

		// These are sent from the context menu
		if(key.startsWith("__CTX__")) {

			if(key == "__CTX__openinfm")
				QDesktopServices::openUrl(QUrl("file:///" + QFileInfo(globVar->currentfile).absolutePath()));
			else if(key == "__CTX__delete") {
				if(!setupWidgets->filehandling) setupWidget("filehandling");
				filehandling->openDialog("delete");
			} else if(key == "__CTX__rename") {
				if(!setupWidgets->filehandling) setupWidget("filehandling");
				filehandling->openDialog("rename");
			} else if(key == "__CTX__rotateleft")
				rotateFlip(true,"anticlock");
			else if(key == "__CTX__rotateright")
				rotateFlip(true,"clock");
			else if(key == "__CTX__fliph")
				rotateFlip(false,"hor");
			else if(key == "__CTX__flipv")
				rotateFlip(false,"ver");
			else if(key == "__CTX__zoomin")
				zoom(true);
			else if(key == "__CTX__zoomout")
				zoom(false);
			else if(key == "__CTX__zoomreset")
				zoom(true,"reset");
			else if(key == "__CTX__zoomactual")
				zoom(true,"actualsize");
			else if(key == "__CTX__scaleimage") {
				if(!setupWidgets->scaleimage) setupWidget("scaleimage");
				scaleimage->scale(globVar->currentfile, globVar->originalImageSize);
			} else if(key == "__CTX__movefirst")
				viewThumbs->gotoFirstLast("first");
			else if(key == "__CTX__moveprev")
				moveInDirectory(0);
			else if(key == "__CTX__movenext")
				moveInDirectory(1);
			else if(key == "__CTX__movelast")
				viewThumbs->gotoFirstLast("last");

			return;

		}

		// If key is empty, then we get the info from the sender#s object name
		if(key == "")
			key = ((QShortcut *) sender())->objectName();

		if(globVar->verbose) std::clog << "Execute shortcut: " << mouseSH << " - " << key.toStdString() << std::endl;

		QString c = "";

		if(key.contains(":::::")) {
			c = key.split(":::::").at(0);
			key = key.split(":::::").at(1);
		}

		// external command
		if(!key.startsWith("__")) {

			QProcess *p = new QProcess;
			key = key.replace("%f",'"' + globVar->currentfile + '"');
			key = key.replace("%d",'"' + QFileInfo(globVar->currentfile).absoluteDir().absolutePath() + '"');
			p->start(key);
			while(!p->waitForStarted()) { }
			if(c == "1")
				this->close();

		// internal command
		} else {

			if(key == "__stopThb")
				viewThumbs->stopThbCreation();
			if(key == "__close") {
				globVar->skipTrayIcon = true;
				this->close();
			}
			if(key == "__hide")
				this->close();
			if(key == "__settings")
				set->makeShow();
			if(key == "__next")
				moveInDirectory(1);
			if(key == "__prev")
				moveInDirectory(0);
			if(key == "__reloadThb")
				viewThumbs->loadDir(true);
			if(key == "__about") {
				if(!setupWidgets->about)
					setupWidget("about");
				about->makeShow();
			}
			if(key == "__slideshow") {
				if(globVar->currentfile != "") {
					if(!setupWidgets->slideshow)
						setupWidget("slideshow");
					slideshow->makeShow();
				}
			}
			if(key == "__filterImages") {
				if(globVar->currentfile != "" || filterImagesDisplay->isVisible()) {
					if(!setupWidgets->filterimages) setupWidget("filterimages");
					if(globVar->curDir == "")
						globVar->curDir = QFileInfo(globVar->currentfile).absolutePath();
					filterImagesSetup->makeShow(globVar->curDir);
				}
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
			if(key == "__rotate0") {
			// Since rotation and flipping overlap each other, they both are reset at the same time
				rotateFlip(false, "reset");
				rotateFlip(true,"reset");
			}
			if(key == "__flipH")
				rotateFlip(false, "hor");
			if(key == "__flipV")
				rotateFlip(false, "ver");
			if(key == "__rename") {
				if(!setupWidgets->filehandling)
					setupWidget("filehandling");
				filehandling->openDialog("rename");
			}
			if(key == "__delete") {
				if(!setupWidgets->filehandling) setupWidget("filehandling");
				filehandling->openDialog("delete");
			}
			if(key == "__copy") {
				if(!setupWidgets->filehandling) setupWidget("filehandling");
				filehandling->openDialog("copy");
			}
			if(key == "__move") {
				if(!setupWidgets->filehandling) setupWidget("filehandling");
				filehandling->openDialog("move");
			}
			if(key == "__hideMeta") {
				if(!details->isVisible()) {
					details->stay->setChecked(true);
					details->makeShow();
				} else {
					details->stay->setChecked(false);
					details->makeHide();
				}
			}
			if(key == "__showContext")
				viewBig->showContext();
			if(key == "__gotoFirstThb")
				viewThumbs->gotoFirstLast("first");
			if(key == "__gotoLastThb")
				viewThumbs->gotoFirstLast("last");

			if(key == "__wallpaper") {
				if(!setupWidgets->wallpaper) setupWidget("wallpaper");
				wallpaper->setWallpaper(globVar->currentfile);
			}
			if(key == "__scale") {
				if(!setupWidgets->scaleimage) setupWidget("scaleimage");
				scaleimage->scale(globVar->currentfile, globVar->originalImageSize);
			}

		}
	} else
		if(globVar->verbose) std::clog << "Execution of shortcuts blocked" << std::endl;

}

// Show the update/fresh install message
void MainWindow::showStartupUpdateInstallMsg() {

	if(globVar->startupMessageInstallUpdateShown == 1) {

		// This widget is shown after an update/fresh install
		startup = new StartUpWidget(viewBig);
		setupWidgets->startup = true;
		startup->setRect(QRect(0,0,viewBig->width(),viewBig->height()));

		if(globVar->verbose) std::clog << "Show Update Message" << std::endl;

		startup->setUpdateMsg();

		connect(startup, SIGNAL(widgetClosed()), this, SLOT(startupInstallUpdateMsgClosed()));

		startup->show();
		startup->makeShow();

		blockFunc(true);

	} else if(globVar->startupMessageInstallUpdateShown == 2) {

		// This widget is shown after an update/fresh install
		startup = new StartUpWidget(viewBig);
		setupWidgets->startup = true;
		startup->setRect(QRect(0,0,viewBig->width(),viewBig->height()));

		if(globVar->verbose) std::clog << "Show Install Message" << std::endl;

		startup->setInstallMsg();

		connect(startup, SIGNAL(widgetClosed()), this, SLOT(startupInstallUpdateMsgClosed()));

		startup->show();
		startup->makeShow();

		blockFunc(true);

	}

}

// The startup/fresh install message has been closed
void MainWindow::startupInstallUpdateMsgClosed() {

	if(globVar->verbose) std::clog << "Startup Message closed" << std::endl;

	blockFunc(false);
	globVar->startupMessageInstallUpdateShown = 0;

}

// On startup, this timer calls drawImage() as soon as the mainwindow finished setup
void MainWindow::startuptimer() {


	if(this->centralWidget()->height()-viewBig->height() < 25 && this->centralWidget()->width()-viewBig->width() < 25 && viewBig->width() > 500 && viewBig->height() > 500) {

		// Show startup message (if it has to be shown and isn't shown yet)
		if(globVar->startupMessageInstallUpdateShown != 0 && !setupWidgets->startup) {
			if(globVar->verbose) std::clog << "Startup timer ended (message)" << std::endl;
			showStartupUpdateInstallMsg();
		// Start PhotoQt
		} else if(globVar->startupMessageInstallUpdateShown == 0) {

			if(globVar->verbose) std::clog << "Startup timer ended (load)" << std::endl;

			startUpTimer->stop();

			drawImage();

			viewThumbs->updateThbViewHoverNormPix("",globVar->currentfile);

// rotation indicator
/*			viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,globVar->tosave.value(globVar->currentfile));*/
			viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

		}

	}

}

// Start slideshow
void MainWindow::startSlideShow() {

	if(!setupWidgets->slideshowbar)
		setupWidget("slideshowbar");

	if(!setupWidgets->slideshow)
		setupWidget("slideshow");

	if(globVar->verbose) std::clog << "Start slideshow" << std::endl;

	globVar->slideshowRunning = true;

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
	slideshowbar->setEnabled(true);
	slideshowbar->animateInAndOut = true;
	slideshowbar->animate();

	// update the quickinfo
// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,false);*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

	// set the music file to bar
	slideshowbar->musicFile = musicFilePath;

	// set the interval
	slideshowbar->nextImg->setInterval(slideshow->timeSlider->value()*1000);

	// and start slideshow
	slideshowbar->startSlideShow();

	// Hide these possibly shown widgets
	if(viewThumbs->isVisible())
		viewThumbs->makeHide();
	if(details->isVisible())
		details->makeHide();
	if(setupWidgets->menu && menu->isVisible())
		menu->makeHide();

	viewBigLay->slideshowHide = slideshow->hideQuickInfo->isChecked();
	viewBigLay->slideshowRunning = true;
// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,false);*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

}

// Stop Slideshowbar
void MainWindow::stopSlideShow() {

	if(globVar->verbose) std::clog << "Stop slideshow" << std::endl;

	globVar->slideshowRunning = false;

	// Animate slideshowbar in (if not shown already) and out
	slideshowbar->animateInAndOut = true;
	slideshowbar->animate();

	// Disable slideshowbar
	slideshowbar->setEnabled(false);

	// Unblock all functions again
	blockFunc(false);

	// Update quickinfo
// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,globVar->tosave.value(globVar->currentfile));*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

	// Stop slideshow
	slideshowbar->stopSlideShow();

	// Re-set original transition value
	graphItem->transitionSetChange(globSet->transition);

	viewBigLay->slideshowRunning = false;
// rotation indicator
/*	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot,globVar->tosave.value(globVar->currentfile));*/
	viewBigLay->updateInfo(globVar->currentfile,viewThumbs->countpos,viewThumbs->counttot);

	// We simply redraw the image for, e.g., getting the exif data
	drawImage();

}

// A system shortcut has been called (needed for instance for dialogs like about or settings)
void MainWindow::systemShortcutDO(QString todo) {

	if(globVar->blocked) {

		if(globVar->verbose) std::clog << "Shortcut received (blockd):" << todo.toStdString() << std::endl;

		if(set->isVisible() && !set->tabShortcuts->detect->isVisible()) {

			if(todo == "Escape") {
				if(set->tabShortcuts->changeCommand->isVisible())
					set->tabShortcuts->changeCommand->makeHide();
				else if(set->tabShortcuts->setDefaultConfirm->isVisible())
					set->tabShortcuts->setDefaultConfirm->makeHide();
				else if(set->tabThumb->confirmClean->isVisible())
					set->tabThumb->confirmClean->no->animateClick();
				else if(set->tabThumb->confirmErase->isVisible())
					set->tabThumb->confirmErase->no->animateClick();
				else {
					set->loadSettings();
					set->makeHide();
				}
			}
			if(todo == "Ctrl+s" && !set->tabShortcuts->changeCommand->isVisible()) {
				set->makeHide();
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

		if(setupWidgets->startup && startup->isVisible()) {

			if(todo == "Escape" || todo == "Return" || todo == "Enter")
				startup->makeHide();

		}

		if(setupWidgets->filehandling && filehandling->isVisible()) {

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

		if(setupWidgets->about && about->isVisible()) {

			if(todo == "Escape")
				about->makeHide();

		}

#ifdef EXIV2
		if(details->rotConf->isVisible()) {

			if(todo == "Enter" || todo == "Return")
				details->rotConf->yes->animateClick();
			else if(todo == "Escape")
				details->rotConf->no->animateClick();

		}
#endif

		if(setupWidgets->slideshow && slideshow->isVisible()) {

			if(todo == "Escape")
				slideshow->makeHide();
			if(todo == "Enter" || todo == "Return")
				slideshow->andStart();

		}

		if(setupWidgets->slideshowbar && slideshowbar->isEnabled()) {

			if(todo == "Escape")
				stopSlideShow();

		}

		if(setupWidgets->wallpaper && wallpaper->isVisible()) {

			if(todo == "Escape")
				wallpaper->dontSetWallpaper();
			if(todo == "Enter" || todo == "Return")
				wallpaper->accept();
		}

		if(setupWidgets->filterimages && filterImagesSetup->isVisible()) {

			if(todo == "Escape")
				filterImagesSetup->cancel->animateClick();
			if(todo == "Enter" || todo == "Return")
				filterImagesSetup->enter->animateClick();

		}

		if(setupWidgets->scaleimage && scaleimage->isVisible()) {

			if(todo == "Escape") {
				if(scaleimage->confirmInPlace->isVisible())
					scaleimage->confirmInPlace->no->animateClick();
				else if(scaleimage->confirmNew->isVisible())
					scaleimage->confirmNew->no->animateClick();
				else if(scaleimage->confirmNotSupported->isVisible())
					scaleimage->confirmNotSupported->yes->animateClick();
				else
					scaleimage->disableAllSpinBoxAndClose();
			}
			if(todo == "Enter" || todo == "Return")
				scaleimage->enterInPlace->animateClick();

		}

	// If functions are not blocked, then check if there's a user shortcut set for it
	} else {

		if(globVar->verbose) std::clog << "(Possible) system shortcut received" << std::endl;

		QList<QVariant> emp;
		emp.clear();
		if(systemKeySHdo.value(todo) != emp) {
			QString key = QString("%1:::::%2").arg(systemKeySHdo.value(todo).at(0).toInt()).arg(systemKeySHdo.value(todo).at(1).toString());
			shortcutDO(key);
		}

	}

}

// Take screenshots (called at startup, and when PhotoQt is minimised to system tray and re-opened)
void MainWindow::takeScreenshots() {

	screenshots.clear();

	for(int i = 0; i < QGuiApplication::screens().count(); ++i) {
		QScreen *screen = QGuiApplication::screens().at(i);
		QRect r = screen->geometry();
		QPixmap pix = screen->grabWindow(0,r.x(),r.y(),r.width(),r.height());
		screenshots.append(pix);
	}

}

// Click on a tray icon menu item
void MainWindow::trayAcDo(QSystemTrayIcon::ActivationReason rsn) {

	if(globVar->verbose) std::clog << "Tray Action triggered: " << rsn << std::endl;

	QAction *s = (QAction *) sender();
	if(s->objectName() == "open") {
		globVar->restoringFromTrayNoResize = QDateTime::currentDateTime().toTime_t();
		if(this->isHidden()) {
			takeScreenshots();	// Refresh background
			if(globSet->windowmode) {
				this->showMaximized();
				globSet->windowDecoration ? this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint) : this->setWindowFlags(Qt::FramelessWindowHint);
				QTimer::singleShot(10,this,SLOT(showMaximized()));
				QTimer::singleShot(500,this,SLOT(showMaximized()));
			} else
				this->showFullScreen();
			setBackground();	// Refresh background
		}
		openFile();
	} else if(s->objectName() == "quit") {
		globVar->skipTrayIcon = true;
		this->close();
	} else if(rsn == QSystemTrayIcon::Trigger) {
		if(this->isVisible())
			this->hide();
		else {
			takeScreenshots();	// Refresh background
			globVar->restoringFromTrayNoResize = QDateTime::currentDateTime().toTime_t();
			if(globSet->windowmode) {
				globSet->windowDecoration ? this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint) : this->setWindowFlags(Qt::FramelessWindowHint);
				if(globVar->windowMaximised) {
					this->showMaximized();
					QTimer::singleShot(100,this,SLOT(showMaximized()));
					globVar->windowMaximised = true;
				} else {
					this->showNormal();
					globVar->windowMaximised = false;
				}
			} else
				this->showFullScreen();
			setBackground();	// Refresh background
			if(globVar->currentfile == "--start-in-tray" || globVar->currentfile == "") {
				openFile();
			}
		}
	}

}

// Update scene rect (called from graphicsitem.cpp)
void MainWindow::updateSceneBigRect() {
	if(globVar->verbose) std::clog << "Update Scene Rect" << std::endl;
	viewBig->scene()->setSceneRect(viewBig->scene()->itemsBoundingRect());
}

// The settings have been updated, so the map is passed to sub-widgets
void MainWindow::updateSettings(QMap<QString, QVariant> settings) {

	if(globVar->verbose) std::clog << "Passing updated settings to subclasses" << std::endl;

	viewThumbs->globSet = settings;
	viewThumbs->view->globSet = settings;
	viewBig->globSet = settings;

	viewBigLay->setSettings(settings);

	set->globSet = settings;
	if(set->tabsSetup) {
		set->tabLookFeel->globSet = settings;
		set->tabThumb->globSet = settings;
		set->tabExif->globSet = settings;
		set->tabOther->globSet = settings;
	}

	details->globSet = settings;
	details->updateFontsize();

	graphItem->transitionSetChange(globSet->transition);

	if(setupWidgets->slideshow) slideshow->globSet = settings;

	if(setupWidgets->wallpaper) wallpaper->globSet = settings;

	if(set->tabsSetup) set->loadSettings();

	if(setupWidgets->quicksettings) {
		quickset->globSet = settings;
		quickset->loadSettings();
	}

}

// Zoom the current image; if a string is set, the boolean is ignored
void MainWindow::zoom(bool zoomin, QString ignoreBoolean) {

	if(globVar->verbose) std::clog << "Zoom: " << zoomin << " - " << ignoreBoolean.toStdString() << std::endl;

	// Reset zoom
	if(ignoreBoolean.startsWith("reset")) {

		if(globVar->verbose) std::clog << "Reset Zoom" << std::endl;

		bool wasZoomToActual = globVar->zoomToActualSize;
		globVar->zoomToActualSize = false;
		globVar->zoomed = false;
		viewBig->resetMatrix();
		globVar->zoomedImgAtLeastOnce = false;
		if(globSet->thumbnailKeepVisible)
			viewThumbs->makeShow();


		graphItem->itemZoomed = false;

		viewBig->rotate(-globVar->rotation);
		if(globVar->flipVer)
			viewBig->scale(1,-1);
		if(globVar->flipHor)
			viewBig->scale(-1,1);

		// We need to disable temporarily transition globally if image was zoomed to actual size, since in that case the scene scale, etc. would mess everything up
		if(wasZoomToActual) {
			QMap<QString, QVariant> s = globSet->toSignalOut();
			s["Transition"] = 0;
			updateSettings(s);
		}

		if(ignoreBoolean != "resetNoDraw")
			drawImage();

		if(wasZoomToActual)
			updateSettings(globSet->toSignalOut());

	// zoom to actual size (toggle)
	} else if(ignoreBoolean == "actualsize") {

		if(globVar->verbose) std::clog << "Zoom to actual size" << std::endl;

		// If at actual size -> reset
		if(globVar->zoomToActualSize || globVar->zoomed)
			zoom(true,"reset");
		else {
			viewBig->resetMatrix();
			viewBig->resetTransform();
			// These need to be set to true for drawImg() not to scale the image
			globVar->zoomed = true;
			graphItem->itemZoomed = true;
			globVar->zoomedImgAtLeastOnce = true;

			globVar->zoomToActualSize = true;

			drawImage();

			viewBig->centerOn(viewBig->scene()->sceneRect().center());

			viewBig->rotate(-globVar->rotation);
			if(globVar->flipHor)
				viewBig->scale(-1,1);
			if(globVar->flipVer)
				viewBig->scale(1,-1);

			if(globSet->thumbnailKeepVisible && viewThumbs->isVisible())
				viewThumbs->makeHide();

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

			if(globVar->verbose) std::clog << "Zoom In" << std::endl;

			viewBig->setAlignment(Qt::AlignCenter);
			viewBig->resetMatrix();
			viewBig->rotate(globVar->rotation%180 != 0 ? globVar->rotation+180 : globVar->rotation);

			if(globVar->flipHor)
				viewBig->scale(-1,1);
			if(globVar->flipVer)
				viewBig->scale(1,-1);
			drawImage();

			QCursor::setPos(QCursor::pos().x()+1,QCursor::pos().y());

			// Zoom out
		} else if(!zoomin && (viewBig->absoluteScaleFactor == 0 || viewBig->absoluteScaleFactor == 1)) {

			if(globVar->verbose) std::clog << "Zoom Out" << std::endl;

			if(viewBig->absoluteScaleFactor == 1)
				graphItem->itemZoomed = true;
			else
				graphItem->itemZoomed = false;

			globVar->zoomed = false;
			viewBig->resetMatrix();
			viewBig->rotate(globVar->rotation%180 != 0 ? globVar->rotation+180 : globVar->rotation);

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

		if(viewBig->absoluteScaleFactor == 0 && globSet->thumbnailKeepVisible && !viewThumbs->isVisible()) {
			globVar->zoomToActualSize = false;
			globVar->zoomed = false;
			viewBig->resetMatrix();
			viewBig->rotate(globVar->rotation);
			globVar->rotation = 0;
			if(globVar->flipHor)
				viewBig->scale(-1,1);
			globVar->flipHor = false;
			if(globVar->flipVer)
				viewBig->scale(1,-1);
			globVar->flipVer = false;
			globVar->zoomedImgAtLeastOnce = false;
			if(globSet->thumbnailKeepVisible)
				// We need to use timer, since it might not yet be faded out completely
				QTimer::singleShot(500,viewThumbs, SLOT(makeShow()));

		}

	}

}

MainWindow::~MainWindow() { }


