#include "slideshowsettings.h"

SlideShow::SlideShow(QMap<QString, QVariant> set, QWidget *parent) : QWidget(parent) {

	// The global settings
	globSet = set;

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

	center = new QWidget(this);
	center->setStyleSheet("background: rgba(0,0,0,200); border-radius: 10px;");

	// The current animation framework
	ani = new QPropertyAnimation(center,"geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	// Create and set the scrollarea with main layout
	QVBoxLayout *central = new QVBoxLayout;
	central->setMargin(10);
	QScrollArea *scroll = new QScrollArea(this);
	scroll->setObjectName("scrollWidget");
	scroll->setStyleSheet("QWidget#scrollWidget { background: transparent;border-bottom: 3px solid black; padding-bottom: 3px; border-radius: 0px; } QLabel { background: transparent; color: white; } QToolTip { color: white; }");
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

	// The title of the widget
	QLabel *title = new QLabel("<center><h1>" + tr("Start a Slideshow") + "</h1></center>");
	title->setStyleSheet("color: white; background: none");
	title->setWordWrap(true);
	central->addWidget(title);
	central->addSpacing(20);

	// A short description
	QLabel *desc = new QLabel(tr("There are several settings that can be adjusted for a slideshow, like the time between the image, if and how long the transition between the images should be, and also a music file can be specified that is played in the background.") + "<br><br>" + tr("Once you have set the desired options, you can also start a slideshow the next time via 'Quickstart', i.e. skipping this settings window."));
	desc->setWordWrap(true);
	central->addWidget(desc);
	central->addSpacing(10);

	// Adjust time
	timeSlider = new CustomSlider;
	timeSlider->setMinimum(1);
	timeSlider->setMaximum(300);
	timeSlider->setSingleStep(1);
	timeSlider->setPageStep(1);
	timeSlider->setValue(globSet.value("SlideShowTime").toInt());
	CustomSpinBox *timeSpin = new CustomSpinBox;
	timeSpin->setMinimum(1);
	timeSpin->setMaximum(300);
	timeSpin->setSingleStep(1);
	timeSpin->setSuffix("s");
	timeSpin->setValue(globSet.value("SlideShowTime").toInt());
	connect(timeSlider, SIGNAL(valueChanged(int)), timeSpin, SLOT(setValue(int)));
	connect(timeSpin, SIGNAL(valueChanged(int)), timeSlider, SLOT(setValue(int)));
	QHBoxLayout *timeLay = new QHBoxLayout;
	timeLay->addStretch();
	timeLay->addWidget(timeSlider);
	timeLay->addWidget(timeSpin);
	timeLay->addStretch();
	QLabel *timeLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Time in between") + "</b></span><br><br>" + tr("Adjust the time between the images. The time specified here is the amount of time the image will be completely visible, i.e. the transitioning (if set) is not part of this time."));
	timeLabel->setWordWrap(true);
	central->addWidget(timeLabel);
	central->addSpacing(5);
	central->addLayout(timeLay);
	central->addSpacing(10);

	// Adjust transition
	trans = new CustomSlider(Qt::Horizontal);
	trans->setMinimum(0);
	trans->setMaximum(10);
	trans->setOrientation(Qt::Horizontal);
	trans->setTickPosition(QSlider::TicksBelow);
	trans->setPageStep(1);
	trans->setSingleStep(1);
	trans->setValue(globSet.value("SlideShowTransition").toInt());
	QLabel *transLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Smooth Transition") + "</span></b> " + "<br><br>" + tr("Here you can set, if you want the images to fade into each other, and how fast they shall do that."));
	transLabel->setWordWrap(true);
	QLabel *noTrans = new QLabel(tr("No Transition"));
	QLabel *longTrans = new QLabel(tr("Long Transition"));
	QHBoxLayout *transLay = new QHBoxLayout;
	transLay->addStretch();
	transLay->addWidget(noTrans);
	transLay->addWidget(trans);
	transLay->addWidget(longTrans);
	transLay->addStretch();
	central->addWidget(transLabel);
	central->addSpacing(5);
	central->addLayout(transLay);
	central->addSpacing(10);

	// Adjust quickinfo labels
	hideQuickInfo = new CustomCheckBox(tr("Hide Quickinfos"));
	hideQuickInfo->setChecked(globSet.value("SlideShowHideQuickinfo").toBool());
	QLabel *hideQuickLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Hide Quickinfo") + "</span></b> " + "<br><br>" + tr("Depending on your setup, Photo displays some information at the top edge, like position in current directory or file path/name. Here you can disable them temporarily for the slideshow."));
	hideQuickLabel->setWordWrap(true);
	QHBoxLayout *hideQuickLay = new QHBoxLayout;
	hideQuickLay->addStretch();
	hideQuickLay->addWidget(hideQuickInfo);
	hideQuickLay->addStretch();
	central->addWidget(hideQuickLabel);
	central->addSpacing(5);
	central->addLayout(hideQuickLay);
	central->addSpacing(10);

	// Adjust music
	musicEnable = new CustomCheckBox(tr("Enable Music"));
	musicEnable->setChecked(globSet.value("SlideShowMusicFile").toString() != "");
	QHBoxLayout *musicEnableLay = new QHBoxLayout;
	musicEnableLay->addStretch();
	musicEnableLay->addWidget(musicEnable);
	musicEnableLay->addStretch();
	musicPath = new CustomLineEdit;
	musicPath->setEnabled(globSet.value("SlideShowMusicFile").toString() != "");
	musicPath->setReadOnly(true);
	musicPath->setCursor(Qt::PointingHandCursor);
	musicPath->setToolTip(tr("Click to select/change music file"));
	musicPath->setText(globSet.value("SlideShowMusicFile").toString());
	QHBoxLayout *musicPathLay = new QHBoxLayout;
	musicPathLay->addStretch();
	musicPathLay->addWidget(musicPath);
	musicPathLay->addStretch();
	QLabel *musicLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Background Music") + "</span></b> " + "<br><br>" + tr("Some might like to listen to some music while the slideshow is running. Here you can select a music file you want to be played in the background."));
	musicLabel->setWordWrap(true);
	central->addWidget(musicLabel);
	central->addSpacing(5);
	central->addLayout(musicEnableLay);
	central->addLayout(musicPathLay);
	connect(musicEnable, SIGNAL(toggled(bool)), musicPath, SLOT(setEnabled(bool)));
	connect(musicPath, SIGNAL(clicked()), this, SLOT(browseForMusic()));

	central->addStretch();


	// Start or don't slideshow
	CustomPushButton *ok = new CustomPushButton(tr("Okay, lets start"));
	CustomPushButton *cancel = new CustomPushButton(tr("Maybe later"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(ok);
	butLay->addWidget(cancel);
	butLay->addStretch();
	scCentral->addLayout(butLay);

	connect(ok, SIGNAL(clicked()), this, SLOT(andStart()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));


}

// Adjust the geometries of the widgets
void SlideShow::adjustGeometries() {

	if(this->isShown) {
		this->setGeometry(rectShown);
		QRect shown = QRect(200,200,rectShown.width()-400,rectShown.height()-400);
		center->setGeometry(shown);
	} else
		this->setGeometry(rectHidden);

}

// The animation function
void SlideShow::animate() {

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
void SlideShow::fadeStep() {

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
void SlideShow::aniFinished() {

	// Move widget out of screen
	if(!isShown)
		this->setGeometry(rectHidden);


}

// Click on background closes dialog
void SlideShow::mouseReleaseEvent(QMouseEvent *e) {

	if(!center->geometry().contains(e->pos()))
		animate();

}

// browse for a music file
void SlideShow::browseForMusic() {

	QString oldPath = QDir::homePath();
	if(musicPath->text() != "" && QFileInfo(musicPath->text()).exists())
		oldPath = musicPath->text();

	QString newFile = QFileDialog::getOpenFileName(0,"Select Music File",oldPath);

	if(newFile != "")
		musicPath->setText(newFile);

}

// Start slideshow
void SlideShow::andStart() {

	this->animate();
	emit startSlideShow();

}


void SlideShow::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


SlideShow::~SlideShow() { }
