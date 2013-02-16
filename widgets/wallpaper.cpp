#include "wallpaper.h"

Wallpaper::Wallpaper(QWidget *parent) : QWidget(parent) {


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
	scroll->setStyleSheet("QWidget#scrollWidget { background: transparent;border-bottom: 3px solid black; padding-bottom: 3px; border-radius: 0px; }");
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


	QLabel *title = new QLabel("<center><span style=\"font-size: 30pt; font-weight: bold\">Set as Wallpaper</span></center>");
	title->setStyleSheet("color: white");
	central->addWidget(title);
	central->addSpacing(10);


	filenameLabel = new QLabel("<center>---</center>");
	filenameLabel->setStyleSheet("color: lightgrey; font-style: italic");
	central->addWidget(filenameLabel);
	central->addSpacing(20);



	QLabel *alignLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Image Alignment") + "</span></b> " + "<br><br>" + tr("You can set the image alignment: Left, Right, Top, Bottom, or Center."));
	alignLabel->setWordWrap(true);
	alignLabel->setStyleSheet("color: white");
	central->addWidget(alignLabel);
	central->addSpacing(10);

	imgCenter = new CustomRadioButton(tr("Center image"));
	imgCenter->setStyleSheet("background-color: rgba(255,255,255,150); border-radius: 5px; padding: 20px");
	imgAlignBottom = new CustomRadioButton(tr("Align at bottom edge"));
	imgAlignBottom->setStyleSheet("background-color: rgba(255,255,255,150); border-radius: 5px; padding: 20px");
	imgAlignLeft = new CustomRadioButton(tr("Align at left edge"));
	imgAlignLeft->setStyleSheet("background-color: rgba(255,255,255,150); border-radius: 5px; padding: 20px");
	imgAlignRight = new CustomRadioButton(tr("Align at right edge"));
	imgAlignRight->setStyleSheet("background-color: rgba(255,255,255,150); border-radius: 5px; padding: 20px");
	imgAlignTop = new CustomRadioButton(tr("Align at top edge"));
	imgAlignTop->setStyleSheet("background-color: rgba(255,255,255,150); border-radius: 5px; padding: 20px");

	QButtonGroup alignGrp;
	alignGrp.addButton(imgCenter);
	alignGrp.addButton(imgAlignBottom);
	alignGrp.addButton(imgAlignLeft);
	alignGrp.addButton(imgAlignRight);
	alignGrp.addButton(imgAlignTop);


	QGridLayout *alignLay = new QGridLayout;
//	alignLay->setHorizontalSpacing(10);
//	alignLay->setVerticalSpacing(20);
	alignLay->addWidget(imgAlignTop,0,1);
	alignLay->addWidget(imgAlignLeft,1,0);
	alignLay->addWidget(imgCenter,1,1);
	alignLay->setAlignment(imgCenter,Qt::AlignCenter);
	alignLay->addWidget(imgAlignRight,1,2);
	alignLay->addWidget(imgAlignBottom,2,1);

	QHBoxLayout *alignLayH = new QHBoxLayout;
	alignLayH->addStretch();
	alignLayH->addLayout(alignLay);
	alignLayH->addStretch();

	central->addLayout(alignLayH);
	central->addSpacing(20);



	imgScaleToFit = new CustomRadioButton(tr("Scale to fit (keep proportions)"));
	imgScaleToWidth = new CustomRadioButton(tr("Scale to Width (keep proportions)"));
	imgScaleToHeight = new CustomRadioButton(tr("Scale to Height (keep proportions)"));
	imgFitIntoView = new CustomRadioButton(tr("Fit into view (ignore proportions)"));

	// SCALE TO FIT (KEEP PROPORTIONS)
	// SCALE TO WIDTH (KEEP PROPORTIONS)
	// SCALE TO HEIGHT (KEEP PROPORTIONS)
	// FIT INTO VIEW




	central->addStretch();


	filename = "";

	kdeUnsupported = new CustomConfirm("KDE not (yet) supported","Currenlty, it's not possible to change the Wallpaper in KDE other than from the KDE settings dialogs. This is said to change in KDE 4.10 (though I don't know for certain).","Oh, okay...","",QSize(450,200),this->parentWidget());
	kdeUnsupported->showBorder("red",2);

}

void Wallpaper::setWallpaper(QString file) {

	if(file != "") {

		filename = file;
		filenameLabel->setText("<center>'" + QFileInfo(file).fileName() + "'</file>");
		animate();

	}

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

//	if(!center->geometry().contains(e->pos()))
//		close->animateClick();

}


void Wallpaper::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}













void Wallpaper::detectWM() {

	if(QString(getenv("KDE_FULL_SESSION")).toLower() == "true")
		setKDE();
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "gnome" || QString(getenv("XDG_CURRENT_DESKTOP")).toLower() == "unity" || QString(getenv("DESKTOP_SESSION")).toLower() == "ubuntu")
		setGNOME();
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "xfce4")
		setXFCE();

}


void Wallpaper::setKDE() {

	qDebug() << "KDE";

	kdeUnsupported->animate();

}

void Wallpaper::setGNOME() {

	qDebug() << "GNOME:" << QString("gsettings set org.gnome.desktop.background picture-uri file://%1").arg(filename);

	QProcess proc;
	proc.execute(QString("gsettings set org.gnome.desktop.background picture-uri file://%1").arg(filename));

}

void Wallpaper::setXFCE() {

	qDebug() << "XFCE";

	QProcess proc;
	if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/image-path -s \"%1\"").arg(filename)) == 1) {
		qDebug() << "Property not found, trying to create it";
		if(proc.execute("xfconf-query -c xfce4-desktop -n -p /backdrop/screen0/monitor0/image-path -t string -s ''") == 1)
			qDebug() << "ERROR: Unable to create property '/backdrop/screen0/monitor0/image-path'!";
		else {
			if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/image-path -s \"%1\"").arg(filename)) == 1)
				qDebug() << "ERROR: Unable to create property '/backdrop/screen0/monitor0/image-path' needed to set background!";
			else
				qDebug() << "Image set...";
		}
	} else
		qDebug() << "Property found, image set...";

}

void Wallpaper::setOTHER() {

}










Wallpaper::~Wallpaper() { }
