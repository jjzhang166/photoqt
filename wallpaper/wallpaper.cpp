#include "wallpaper.h"

Wallpaper::Wallpaper(QMap<QString, QVariant> set, QWidget *parent) : QWidget(parent) {

	// The different QRects
	rectShown = QRect();
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect();

	// The current geometry and position
	isShown = false;
	this->setGeometry(rectHidden);

	// Some variables
	fadeBack = new QTimeLine;
	fadeBack->setLoopCount(5);
	backAlphaShow = 130;
	backAlphaCur = 0;
	fadeBackIN = true;
	connect(fadeBack, SIGNAL(valueChanged(qreal)), this, SLOT(fadeStep()));

	// The current widget look
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaShow));

	// the central widget containing all the information
	center = new QWidget(this);
	center->setStyleSheet("background: rgba(0,0,0,200); border-radius: 10px; font-size: 12pt;");

	// The current animation framework
	ani = new QPropertyAnimation(center,"geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	// Create and set the scrollarea with main layout
	QVBoxLayout *central = new QVBoxLayout;
	central->setMargin(10);
	QScrollArea *scroll = new QScrollArea(this);
	scroll->setObjectName("scrollWidget");
	scroll->setStyleSheet("QWidget#scrollWidget { background: transparent;border-bottom: 1px solid white; padding-bottom: 3px; border-radius: 0px; }");
	QWidget *scrollWidget = new QWidget(scroll->widget());
	scrollWidget->setStyleSheet("background: transparent;");
	scroll->setWidgetResizable(true);
	scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	scrollWidget->setLayout(central);
	scroll->setWidget(scrollWidget);
	QVBoxLayout *scCentral = new QVBoxLayout;
	center->setLayout(scCentral);
	scCentral->addWidget(scroll);

	CustomScrollbar *scrollbar = new CustomScrollbar;
	scroll->setVerticalScrollBar(scrollbar);

	ok = new CustomPushButton(tr("Okay, do it!"));
	ok->removeRoundedCorners();
	cancel = new CustomPushButton(tr("Nooo, don't!"));
	cancel->removeRoundedCorners();
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(ok);
	butLay->addWidget(cancel);
	butLay->addStretch();
	scCentral->addSpacing(10);
	scCentral->addLayout(butLay);
	scCentral->addSpacing(10);

	connect(ok, SIGNAL(clicked()), this, SLOT(goAheadAndSetWallpaper()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));



	QLabel *title = new QLabel("<center><span style=\"font-size: 30pt; font-weight: bold\">Set as Wallpaper</span></center>");
	title->setStyleSheet("color: white");
	central->addWidget(title);
	central->addSpacing(10);




	filenameLabel = new QLabel("<center>---</center>");
	filenameLabel->setStyleSheet("color: lightgrey; font-style: italic");
	central->addWidget(filenameLabel);
	central->addSpacing(20);

	QLabel *wmDetectedLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Window Manager") + "</span></b> " + "<br><br>" + tr("Photo tries to detect you window manager according to the environment variables set by your system. If it still got it wrong, you can change the window manager here manually."));
	wmDetectedLabel->setWordWrap(true);
	wmDetectedLabel->setStyleSheet("color: white");
	central->addWidget(wmDetectedLabel);
	central->addSpacing(10);

	wm = new CustomComboBox;
	wm->setFontSize(15);
	wm->setBorder(1,"white");
	wm->addItem("KDE","kde");
	wm->addItem("Gnome","gnome");
	wm->addItem("XFCE4","xfce");
	wm->addItem(tr("Other"),"other");
	QHBoxLayout *wmLay = new QHBoxLayout;
	wmLay->addStretch();
	wmLay->addWidget(wm);
	wmLay->addStretch();
	central->addLayout(wmLay);
	//	central->addWidget(wm);
	central->addSpacing(10);


	wmMessage = new QLabel;
	wmMessage->setAlignment(Qt::AlignCenter);
	wmMessage->setWordWrap(true);
	wmMessage->setVisible(false);
	central->addSpacing(10);
	central->addWidget(wmMessage);
	central->addSpacing(20);

	connect(wm, SIGNAL(currentIndexChanged(int)), this, SLOT(wmSelected()));





	//////// GNOME SETTINGS /////////////

	QVBoxLayout *gnomePicOpsLayCenter = new QVBoxLayout;

	gnomeButGrp = new QButtonGroup;
	gnomePictureOptions.clear();
	QStringList picOpts;
	picOpts << "wallpaper";
	picOpts << "centered";
	picOpts << "scaled";
	picOpts << "zoom";
	picOpts << "spanned";
	for(int i = 0; i < picOpts.size(); ++i) {
		CustomRadioButton *chk = new CustomRadioButton(picOpts.at(i));
		if(i == 3) chk->setChecked(true);
		gnomePicOpsLayCenter->addWidget(chk);
		gnomeButGrp->addButton(chk);
		gnomePictureOptions.insert(picOpts.at(i),chk);
	}

	QHBoxLayout *gnomePicOpsLay = new QHBoxLayout;
	gnomePicOpsLay->addStretch();
	gnomePicOpsLay->addLayout(gnomePicOpsLayCenter);
	gnomePicOpsLay->addStretch();

	gnomePicOpsLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Picture Options") + "</span></b> " + "<br><br>" + tr("There are several picture options that can be set for the wallpaper image."));
	gnomePicOpsLabel->setMargin(5);
	gnomePicOpsLabel->setStyleSheet("color: white");
	gnomePicOpsLabel->setWordWrap(true);

	central->addWidget(gnomePicOpsLabel);
	central->addLayout(gnomePicOpsLay);


	/////// XFCE SETTINGS /////////////

	QVBoxLayout *xfceMonitorLayCenter = new QVBoxLayout;
	xfceMonitorSelect.clear();
	QDesktopWidget desk;
	for(int i = 0; i < desk.screenCount(); ++i) {
		CustomCheckBox *mon = new CustomCheckBox(tr("Screen") + QString(" #%1").arg(i));
		mon->setChecked(true);
		xfceMonitorLayCenter->addWidget(mon);
		xfceMonitorSelect.insert(i,mon);
	}

	QHBoxLayout *xfceMonitorLay = new QHBoxLayout;
	xfceMonitorLay->addStretch();
	xfceMonitorLay->addLayout(xfceMonitorLayCenter);
	xfceMonitorLay->addStretch();

	xfceMonitorLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Select Monitors") + "</span></b> " + "<br><br>" + tr("The wallpaper can be set to either of the available monitors (or any combination)."));
	xfceMonitorLabel->setWordWrap(true);
	xfceMonitorLabel->setMargin(5);
	xfceMonitorLabel->setStyleSheet("color: white");

	central->addWidget(xfceMonitorLabel);
	central->addLayout(xfceMonitorLay);



	QVBoxLayout *xfcePicOpsLayCenter = new QVBoxLayout;

	xfceButGrp = new QButtonGroup;
	xfcePictureOptions.clear();
	QStringList xfcePicOpts;
	xfcePicOpts << "automatic";
	xfcePicOpts << "centered";
	xfcePicOpts << "tiled";
	xfcePicOpts << "spanned";
	xfcePicOpts << "scaled";
	xfcePicOpts << "magnified";
	for(int i = 0; i < xfcePicOpts.size(); ++i) {
		CustomRadioButton *chk = new CustomRadioButton(xfcePicOpts.at(i));
		if(i == 0) chk->setChecked(true);
		xfcePicOpsLayCenter->addWidget(chk);
		xfceButGrp->addButton(chk);
		xfcePictureOptions.insert(xfcePicOpts.at(i),chk);
	}

	QHBoxLayout *xfcePicOpsLay = new QHBoxLayout;
	xfcePicOpsLay->addStretch();
	xfcePicOpsLay->addLayout(xfcePicOpsLayCenter);
	xfcePicOpsLay->addStretch();

	xfcePicOpsLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Picture Options") + "</span></b> " + "<br><br>" + tr("There are several picture options that can be set for the wallpaper image."));
	xfcePicOpsLabel->setMargin(5);
	xfcePicOpsLabel->setStyleSheet("color: white");
	xfcePicOpsLabel->setWordWrap(true);

	central->addWidget(xfcePicOpsLabel);
	central->addLayout(xfcePicOpsLay);





	central->addSpacing(10);
	central->addStretch();


	detectWM();
	wmSelected();


}


void Wallpaper::setWallpaper(QString file) {

	if(file != "") {

		filename = file;
		filenameLabel->setText("<center>'" + QFileInfo(file).fileName() + "'</file>");
		animate();

	}

}

void Wallpaper::dontSetWallpaper() {

	if(isShown)
		animate();

}

void Wallpaper::detectWM() {

	if(QString(getenv("KDE_FULL_SESSION")).toLower() == "true")
		wm->setCurrentIndex(0);
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "gnome" || QString(getenv("XDG_CURRENT_DESKTOP")).toLower() == "unity" || QString(getenv("DESKTOP_SESSION")).toLower() == "ubuntu")
		wm->setCurrentIndex(1);
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "xfce4")
		wm->setCurrentIndex(2);
	else
		wm->setCurrentIndex(3);

}

void Wallpaper::wmSelected() {


	bool hideGNOME = false;
	bool hideXFCE = false;



	int current = wm->currentIndex();

	// KDE
	if(current == 0) {
		wmMessage->setText(tr("Sorry, KDE doesn't (yet) offer the feature to change the wallpaper except from their own system settings. As soon as this changes (if it'll ever change), Photo will get an implementation supporting that."));
		wmMessage->setStyleSheet("color: red; font-weight: bold");

		ok->setEnabled(false);
		wmMessage->setVisible(true);

		hideGNOME = true;
		hideXFCE = true;

	// GNOME
	} else if(current == 1) {

		ok->setEnabled(true);
		wmMessage->setVisible(false);

		hideGNOME = false;
		hideXFCE = true;



	// XFCE4
	} else if(current == 2) {

		ok->setEnabled(true);
		wmMessage->setVisible(false);

		hideGNOME = true;
		hideXFCE = false;


	// OTHER
	} else if(current == 3) {
		wmMessage->setText(tr("Photo will try to change the background of the root window. This usually equates to the desktop background. However, there's no guarantee that this will work!"));
		wmMessage->setStyleSheet("color: red");
		ok->setEnabled(true);
		wmMessage->setVisible(true);

		hideGNOME = true;
		hideXFCE = true;

	}

	if(hideGNOME) {
		gnomePicOpsLabel->hide();
		QMapIterator<QString, CustomRadioButton*> i(gnomePictureOptions);
		while (i.hasNext()) {
			i.next();
			i.value()->hide();
		}
	} else {
		gnomePicOpsLabel->show();
		QMapIterator<QString, CustomRadioButton*> i(gnomePictureOptions);
		while (i.hasNext()) {
			i.next();
			i.value()->show();
		}
	}

	if(hideXFCE) {
		xfceMonitorLabel->hide();
		QMapIterator<int, CustomCheckBox*> i2(xfceMonitorSelect);
		while (i2.hasNext()) {
			i2.next();
			i2.value()->hide();
		}

		xfcePicOpsLabel->hide();
		QMapIterator<QString, CustomRadioButton*> i3(xfcePictureOptions);
		while (i3.hasNext()) {
			i3.next();
			i3.value()->hide();
		}
	} else {
		QDesktopWidget desk;
		if(desk.screenCount() > 1) {
			xfceMonitorLabel->show();
			QMapIterator<int, CustomCheckBox*> i2(xfceMonitorSelect);
			while (i2.hasNext()) {
				i2.next();
				i2.value()->show();
			}
		} else {
			xfceMonitorLabel->hide();
			QMapIterator<int, CustomCheckBox*> i2(xfceMonitorSelect);
			while (i2.hasNext()) {
				i2.next();
				i2.value()->hide();
			}
		}

		xfcePicOpsLabel->show();
		QMapIterator<QString, CustomRadioButton*> i3(xfcePictureOptions);
		while (i3.hasNext()) {
			i3.next();
			i3.value()->show();
		}
	}
}




void Wallpaper::goAheadAndSetWallpaper() {

//	QMap<QString,QVariant> updatedSet;
//	updatedSet.clear();

//	if(imgCenter->isChecked() && globSet.value("WallpaperAlignment").toString() != "center")
//		updatedSet.insert("WallpaperAlignment","center");
//	if(imgAlignLeft->isChecked() && globSet.value("WallpaperAlignment").toString() != "left")
//		updatedSet.insert("WallpaperAlignment","left");
//	if(imgAlignRight->isChecked() && globSet.value("WallpaperAlignment").toString() != "right")
//		updatedSet.insert("WallpaperAlignment","right");
//	if(imgAlignTop->isChecked() && globSet.value("WallpaperAlignment").toString() != "top")
//		updatedSet.insert("WallpaperAlignment","top");
//	if(imgAlignBottom->isChecked() && globSet.value("WallpaperAlignment").toString() != "bottom")
//		updatedSet.insert("WallpaperAlignment","bottom");

//	if(imgDontScale->isChecked() && globSet.value("WallpaperScale").toString() != "noscale")
//		updatedSet.insert("WallpaperScale","noscale");
//	if(imgScaleToFit->isChecked() && globSet.value("WallpaperScale").toString() != "scalefit")
//		updatedSet.insert("WallpaperScale","scalefit");
//	if(imgScaleToWidth->isChecked() && globSet.value("WallpaperScale").toString() != "scalewidth")
//		updatedSet.insert("WallpaperScale","scalewidth");
//	if(imgScaleToHeight->isChecked() && globSet.value("WallpaperScale").toString() != "scaleheight")
//		updatedSet.insert("WallpaperScale","scaleheight");
//	if(imgFitIntoView->isChecked() && globSet.value("WallpaperScale").toString() != "fitview")
//		updatedSet.insert("WallpaperScale","fitview");


//	if(!updatedSet.isEmpty())
//		emit wallpaperSet(updatedSet);

	animate();

	if(wm->currentIndex() == 0)
		setKDE();
	else if(wm->currentIndex() == 1)
		setGNOME();
	else if(wm->currentIndex() == 2)
		setXFCE();
	else if(wm->currentIndex() == 3)
		setOTHER();


}



// The program should never reach this function (since a selection of "KDE" will disable the OK button, but it is included as a placeholder for a future implementation
void Wallpaper::setKDE() {

	qDebug() << "KDE -- not yet supported";

}

void Wallpaper::setGNOME() {

	qDebug() << "GNOME";

	QProcess proc;
	proc.execute(QString("gsettings set org.gnome.desktop.background picture-options %1").arg(gnomeButGrp->checkedButton()->text()));
	proc.execute(QString("gsettings set org.gnome.desktop.background picture-uri file://%1").arg(filename));

}

void Wallpaper::setXFCE() {

	qDebug() << "XFCE";

	QStringList xfcePicOpts;
	xfcePicOpts << "automatic";
	xfcePicOpts << "centered";
	xfcePicOpts << "tiled";
	xfcePicOpts << "spanned";
	xfcePicOpts << "scaled";
	xfcePicOpts << "magnified";

	QMapIterator<int, CustomCheckBox*> i3(xfceMonitorSelect);
	int i = 0;
	while (i3.hasNext()) {
		i3.next();


		if(i3.value()->isChecked()) {

			QProcess proc;

			// Trying to set image style
			if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-style -s %2").arg(i).arg(xfcePicOpts.indexOf(xfceButGrp->checkedButton()->text())))) {

				// If we don't succeed, then the property most likely doesn't exist
				qDebug() << "image-style property not found, trying to create it";


				// Create Property
				if(proc.execute(QString("xfconf-query -c xfce4-desktop -n -p /backdrop/screen0/monitor%1/image-style -t int -s 0").arg(i)) == 1)
					qDebug() << QString("ERROR: Unable to create property '/backdrop/screen0/monitor%1/image-style'!").arg(i);
				else {

					// Trying to set image style again
					if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-style -s %2").arg(i).arg(i3.key())) == 1)
						qDebug() << "ERROR: Couldn't set image-style!";
					else
						qDebug() << "Image style set...";

				}

			} else
				qDebug() << "image-style set";




			if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-path -s \"%2\"").arg(i).arg(filename)) == 1) {

				// If we don't succeed, then the property most likely doesn't exist
				qDebug() << "image-path property not found, trying to create it";

				// Create Property
				if(proc.execute(QString("xfconf-query -c xfce4-desktop -n -p /backdrop/screen0/monitor%1/image-path -t string -s ''").arg(i)) == 1)
					qDebug() << QString("ERROR: Unable to create property '/backdrop/screen0/monitor%1/image-path'!").arg(i);
				else {

					// And try again setting wallpaper
					if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-path -s \"%2\"").arg(i).arg(filename)) == 1)
						qDebug() << QString("ERROR: Unable to create property '/backdrop/screen0/monitor%1image-path' needed to set background!").arg(i);
					else
						qDebug() << "Image set...";
				}
			}
				qDebug() << "Property found, image set...";

		} else
			qDebug() << "Monitor #" << i << "not checked...";

		++i;

	}

}

void Wallpaper::setOTHER() {

#ifdef Q_WS_X11

	Display *disp2 = XOpenDisplay(NULL);
	Window root2 = RootWindow(disp2, DefaultScreen(disp2));
	// Generation of random number to set window color
	srand(time(NULL));
	int i = rand();
	// Setting background
	// I forgot to change 256 to i
	// XSetWindowBackground(disp2, root2, 256 );
//	XSetWindowBackground(disp2, root2, i);
//	Pixmap *pix;
//	unsigned int *w = 4000;
//	unsigned int *h = 3000;
//	unsigned int *xh = 0;
//	unsigned int *yh = 0;
//	XCreatePixmapFromBitmapData(disp2,root2,)
//	XReadBitmapFile(disp2,root2,filename.toLocal8Bit(),w,h,pix,xh,yh);
//	XSetWindowBackgroundPixmap(disp2,root2,pix);
//	XClearWindow(disp2, root2);


#else

	qDebug() << "Sorry, you're not using a Unix based system...";

#endif

}


// The animation function
void Wallpaper::animate() {

	int shownWidth = rectShown.width()-400;
	int shownHeight = rectShown.height()-400;
	int shownX = 200;
	int shownY = 200;

	if(shownWidth - shownHeight > 500) {
		shownHeight += 300;
		shownY -= 150;
	}

	QRect shown = QRect(shownX,shownY,shownWidth, shownHeight);

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		// The background is initially transparent but the geometry is full
		this->setStyleSheet(QString("background: rgba(0,0,0,0);"));
		this->setGeometry(rectShown);

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
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

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
void Wallpaper::fadeStep() {

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
void Wallpaper::aniFinished() {

	// Move widget out of screen
	if(!isShown)
		this->setGeometry(rectHidden);


}

// Click on background closes dialog
void Wallpaper::mouseReleaseEvent(QMouseEvent *e) {

	if(!center->geometry().contains(e->pos()))
		cancel->animateClick();

}

void Wallpaper::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}




Wallpaper::~Wallpaper() { }
