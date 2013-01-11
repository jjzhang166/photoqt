#include "dropdownmenu.h"

DropDownMenu::DropDownMenu(QWidget *parent) : QWidget(parent) {

	this->setStyleSheet("border-radius: 8px; border-top-right-radius: 0px; border-top-left-radius: 0px; background-color: rgba(0, 0, 0, 220)");

	// The sensitivity of the menu
	ySensitivity = 10;
	xSensitivity = 3;

	// Per default, the menu is shown at the top
	posAtBottom = true;

	// This is the dimension of the menu
	rectShown = QRect(0,0,10,10);
	rectHidden = QRect(rectShown.x(),-rectShown.height(),rectShown.width(),rectShown.height());
	this->setGeometry(rectHidden);

	// The animation
	ani = new QPropertyAnimation(this,"geometry");

	// Is the menu shown?
	isShown = false;

	// The layout holding all the labels
	mLayL = new QVBoxLayout;
	mLayR = new QVBoxLayout;

	// Set the specialised functions
	setGoTo("left");
	setJumpTo("left");
	setZoom("left");
	setSlideshow("left");
	setRotate("left");
	setFlip("left");
	setFileActions("left");

	// And set the standard functions
	setHeading(tr("General Functions"),"right");
	setStandard("right", tr("Open File"), ":/img/open.png","open", 1);
	setStandard("right", tr("Settings"), ":/img/setup.png","settings", 1);
	setStandard("right", tr("Interrupt Thumbnail Creation"), ":/img/interrupt.png","stopThb", 1);
	setStandard("right", tr("Reload Thumbnails"), ":/img/reload.png","reloadThb", 1);
	setStandard("right", tr("Show Exif Widget"), ":/img/exif.png","hideMeta", 1);
	setStandard("right", tr("About Photo"), ":/img/about.png","about", 1);
	setStandard("right", tr("Hide (System Tray)"), ":/img/quit.png","hide", 1);
	setStandard("right", tr("Quit"), ":/img/quit.png","close", 1);

	// And set the layouts
	QHBoxLayout *mLay = new QHBoxLayout;
	mLay->addLayout(mLayL);
	mLay->addLayout(mLayR);
	this->setLayout(mLay);


}

// Go to next/previous
void DropDownMenu::setGoTo(QString pos) {

	MainMenuItem *item0 = new MainMenuItem(tr("Go To"),"","",0,false);
	MainMenuItem *item1 = new MainMenuItem(QString("<img src=\":/img/previous.png\" height=\"13\"> %1").arg(tr("Previous")),"","prev",0);
	MainMenuItem *item15 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item2 = new MainMenuItem(QString("%1 <img src=\":/img/next.png\" height=\"13\">").arg(tr("Next")),"","next",0);

	QHBoxLayout *npLay1 = new QHBoxLayout;
	QHBoxLayout *npLay2 = new QHBoxLayout;

	npLay1->addStretch();
	npLay1->addWidget(item0);
	npLay1->addStretch();

	npLay2->addStretch();
	npLay2->addWidget(item1);
	npLay2->addWidget(item15);
	npLay2->addWidget(item2);
	npLay2->addStretch();

	QVBoxLayout *npLay = new QVBoxLayout;
	npLay->addLayout(npLay1);
	npLay->addLayout(npLay2);

	connect(item1,SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item2,SIGNAL(clicked()), this, SLOT(menuClicked()));

	if(pos == "right") {
		mLayR->addSpacing(5);
		mLayR->addLayout(npLay);
	} else {
		mLayL->addSpacing(5);
		mLayL->addLayout(npLay);
	}

}

// Jump to beginning/end
void DropDownMenu::setJumpTo(QString pos) {

	MainMenuItem *item0 = new MainMenuItem(tr("Jump To"),"","",0,false);
	MainMenuItem *item1 = new MainMenuItem(QString("<img src=\":/img/first.png\" height=\"13\"> %1").arg(tr("First")),"","gotoFirstThb",0);
	MainMenuItem *item15 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item2 = new MainMenuItem(QString("%1 <img src=\":/img/last.png\" height=\"13\">").arg(tr("Last")),"","gotoLastThb",0);

	QHBoxLayout *npLay1 = new QHBoxLayout;
	QHBoxLayout *npLay2 = new QHBoxLayout;

	npLay1->addStretch();
	npLay1->addWidget(item0);
	npLay1->addStretch();

	npLay2->addStretch();
	npLay2->addWidget(item1);
	npLay2->addWidget(item15);
	npLay2->addWidget(item2);
	npLay2->addStretch();

	QVBoxLayout *npLay = new QVBoxLayout;
	npLay->addLayout(npLay1);
	npLay->addLayout(npLay2);

	connect(item1,SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item2,SIGNAL(clicked()), this, SLOT(menuClicked()));

	if(pos == "right") {
		mLayR->addSpacing(5);
		mLayR->addLayout(npLay);
	} else {
		mLayL->addSpacing(5);
		mLayL->addLayout(npLay);
	}

}

// Zoom in/out/Actual/Reset
void DropDownMenu::setZoom(QString pos) {

	MainMenuItem *item1 = new MainMenuItem(tr("Zoom:"),"","",0,false);
	MainMenuItem *item2 = new MainMenuItem(QString(" %1 ").arg(tr("In")),"","zoomIn",0);
	MainMenuItem *item25 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item3 = new MainMenuItem(QString(" %1 ").arg(tr("Out")),"","zoomOut",0);
	MainMenuItem *item35 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item4 = new MainMenuItem(QString(" %1 ").arg(tr("Actual")),"","zoomActual",1);
	MainMenuItem *item45 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item5 = new MainMenuItem(QString(" %1 ").arg(tr("Reset")),"","zoomReset",1);

	QHBoxLayout *zLay1 = new QHBoxLayout;
	QHBoxLayout *zLay2 = new QHBoxLayout;

	zLay1->addStretch();
	zLay1->addWidget(item1);
	zLay1->addStretch();

	zLay2->addStretch();
	zLay2->addWidget(item2);
	zLay2->addWidget(item25);
	zLay2->addWidget(item3);
	zLay2->addWidget(item35);
	zLay2->addWidget(item4);
	zLay2->addWidget(item45);
	zLay2->addWidget(item5);
	zLay2->addStretch();

	QVBoxLayout *zLay = new QVBoxLayout;
	zLay->addLayout(zLay1);
	zLay->addLayout(zLay2);

	connect(item2, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item3, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item4, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item5, SIGNAL(clicked()), this, SLOT(menuClicked()));

	if(pos == "right") {
		mLayR->addSpacing(5);
		mLayR->addLayout(zLay);
	} else {
		mLayL->addSpacing(5);
		mLayL->addLayout(zLay);
	}

}

// Rotate Left/Right/Reset
void DropDownMenu::setRotate(QString pos) {

	MainMenuItem *item1 = new MainMenuItem(tr("Rotate:"),"","",0,false);
	MainMenuItem *item2 = new MainMenuItem(tr("Left"),":/img/rotateLeft.png","rotateL",0);
	MainMenuItem *item25 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item3 = new MainMenuItem(tr("Reset"),"","rotate0",1);
	MainMenuItem *item35 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item4 = new MainMenuItem(QString("%1 <img src=\":/img/rotateRight.png\" height=\"13\">").arg(tr("Right")),"","rotateR",0);

	QHBoxLayout *rLay1 = new QHBoxLayout;
	QHBoxLayout *rLay2 = new QHBoxLayout;

	rLay1->addStretch();
	rLay1->addWidget(item1);
	rLay1->addStretch();

	rLay2->addStretch();
	rLay2->addWidget(item2);
	rLay2->addWidget(item25);
	rLay2->addWidget(item3);
	rLay2->addWidget(item35);
	rLay2->addWidget(item4);
	rLay2->addStretch();

	QVBoxLayout *rLay = new QVBoxLayout;
	rLay->addLayout(rLay1);
	rLay->addLayout(rLay2);

	connect(item2, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item3, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item4, SIGNAL(clicked()), this, SLOT(menuClicked()));

	if(pos == "right") {
		mLayR->addSpacing(5);
		mLayR->addLayout(rLay);
	} else {
		mLayL->addSpacing(5);
		mLayL->addLayout(rLay);
	}

}

// Flip Horizontally/Vertically/Reset
void DropDownMenu::setFlip(QString pos) {

	MainMenuItem *item1 = new MainMenuItem(tr("Flip:"),"","",0,false);
	MainMenuItem *item2 = new MainMenuItem(tr("Horizontally"),":/img/flipH.png","mirrorH",0);
	MainMenuItem *item25 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item3 = new MainMenuItem(tr("Reset"),"","mirrorR",0);
	MainMenuItem *item35 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item4 = new MainMenuItem(QString("%1 <img src=\":/img/flipV.png\" height=\"13\">").arg(tr("Vertically")),"","mirrorV",0);

	QHBoxLayout *fLay1 = new QHBoxLayout;
	QHBoxLayout *fLay2 = new QHBoxLayout;

	fLay1->addStretch();
	fLay1->addWidget(item1);
	fLay1->addStretch();

	fLay2->addStretch();
	fLay2->addWidget(item2);
	fLay2->addWidget(item25);
	fLay2->addWidget(item3);
	fLay2->addWidget(item35);
	fLay2->addWidget(item4);
	fLay2->addStretch();

	QVBoxLayout *fLay = new QVBoxLayout;
	fLay->addLayout(fLay1);
	fLay->addLayout(fLay2);

	connect(item2, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item3, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item4, SIGNAL(clicked()), this, SLOT(menuClicked()));

	if(pos == "right") {
		mLayR->addSpacing(5);
		mLayR->addLayout(fLay);
	} else {
		mLayL->addSpacing(5);
		mLayL->addLayout(fLay);
	}

}

// Rename/Copy/Move/Delete current file
void DropDownMenu::setFileActions(QString pos) {

	MainMenuItem *item1 = new MainMenuItem(tr("File:"),"","",0,false);
	MainMenuItem *item2 = new MainMenuItem(tr("Rename"),"","rename",1);
	MainMenuItem *item25 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item3 = new MainMenuItem(tr("Copy"),"","copy",1);
	MainMenuItem *item35 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item4 = new MainMenuItem(tr("Move"),"","move",1);
	MainMenuItem *item45 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item5 = new MainMenuItem(tr("Delete"),"","delete",1);

	QHBoxLayout *fLay1 = new QHBoxLayout;
	QHBoxLayout *fLay2 = new QHBoxLayout;

	fLay1->addStretch();
	fLay1->addWidget(item1);
	fLay1->addStretch();

	fLay2->addStretch();
	fLay2->addWidget(item2);
	fLay2->addWidget(item25);
	fLay2->addWidget(item3);
	fLay2->addWidget(item35);
	fLay2->addWidget(item4);
	fLay2->addWidget(item45);
	fLay2->addWidget(item5);
	fLay2->addStretch();

	QVBoxLayout *fLay = new QVBoxLayout;
	fLay->addLayout(fLay1);
	fLay->addLayout(fLay2);

	connect(item2, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item3, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item4, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item5, SIGNAL(clicked()), this, SLOT(menuClicked()));

	if(pos == "right") {
		mLayR->addSpacing(5);
		mLayR->addLayout(fLay);
	} else {
		mLayL->addSpacing(5);
		mLayL->addLayout(fLay);
	}


}

// Slideshow Settings/Quickstart
void DropDownMenu::setSlideshow(QString pos) {

	MainMenuItem *item1 = new MainMenuItem(tr("Slideshow:"),"","",0,false);
	MainMenuItem *item2 = new MainMenuItem(tr("Settings"),"","slideshow",1);
	MainMenuItem *item25 = new MainMenuItem("|","","",0,false);
	MainMenuItem *item3 = new MainMenuItem(tr("Quickstart"),"","slideshowQuick",1);

	QHBoxLayout *fLay1 = new QHBoxLayout;
	QHBoxLayout *fLay2 = new QHBoxLayout;

	fLay1->addStretch();
	fLay1->addWidget(item1);
	fLay1->addStretch();

	fLay2->addStretch();
	fLay2->addWidget(item2);
	fLay2->addWidget(item25);
	fLay2->addWidget(item3);
	fLay2->addStretch();

	QVBoxLayout *fLay = new QVBoxLayout;
	fLay->addLayout(fLay1);
	fLay->addLayout(fLay2);

	connect(item2, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item3, SIGNAL(clicked()), this, SLOT(menuClicked()));

	if(pos == "right") {
		mLayR->addSpacing(5);
		mLayR->addLayout(fLay);
	} else {
		mLayL->addSpacing(5);
		mLayL->addLayout(fLay);
	}

}

// Set a standard function
void DropDownMenu::setStandard(QString pos, QString txt, QString icon, QString data, int active) {

	QHBoxLayout *itemLay = new QHBoxLayout;

	MainMenuItem *item = new MainMenuItem(txt,icon,data,active);

	if(pos == "right") {
		itemLay->addWidget(item);
		itemLay->addSpacing(50);
	}

	if(pos == "right")
		mLayR->addLayout(itemLay);
	else
		mLayL->addWidget(item);

	connect(item, SIGNAL(clicked()), this, SLOT(menuClicked()));

}

// Add a seperator
void DropDownMenu::setSep(QString pos) {

	MainMenuItem *item = new MainMenuItem("","","",0,false);

	if(pos == "right")
		mLayR->addWidget(item);
	else
		mLayL->addWidget(item);

}

// Ser a heading
void DropDownMenu::setHeading(QString txt, QString pos) {

	MainMenuItem *item = new MainMenuItem("<center><i>" + txt + "</i></span></center>","","",0,false);

	if(pos == "right")
		mLayR->addWidget(item);
	else
		mLayL->addWidget(item);

}




// Animate the menu in/out
void DropDownMenu::animate() {

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

	}


}

// If an item was clicked
void DropDownMenu::menuClicked() {

	MainMenuItem *item = (MainMenuItem*) sender();
	QString txt = item->data;

	int close = item->close;

	emit itemClicked(txt,close);

}

// Enabling styling of widget
void DropDownMenu::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}



DropDownMenu::~DropDownMenu() { }
