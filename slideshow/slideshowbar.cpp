#include "slideshowbar.h"

SlideShowBar::SlideShowBar(QMap<QString, QVariant> set, QWidget *parent, bool v) : QWidget(parent) {

	verbose = v;

	this->setStyleSheet("background: rgba(0,0,0,200)");

	globSet = set;

	// We manipulate the geometry property
	ani = new QPropertyAnimation(this, "geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	// Bar is not shown and no slideshow is running
	isShown = false;
	enabled = false;
	animateInAndOut = false;

	rectShown = QRect(0,0,10,10);
	rectHidden = QRect(0,-10,10,10);

	this->setGeometry(rectHidden);

	// No music file as default
	musicFile = "";

	// The stylesheet for the volume slider
	QString css = "QSlider::groove:horizontal {";
		css += "border: 1px solid #000000;";
		css += "height: 6px;";
		css += "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ededed, stop:1 #ffffff);";
		css += "margin: 2px 0;";
	css += "}";

	css += "QSlider::groove:disabled {";
		css += "background: grey;";
	css += "}";

	css += "QSlider::handle:horizontal {";
		css += "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #000000, stop:1 #282828);";
		css += "border: 1px solid grey;";
		css += "width: 18px;";
		css += "margin: -2px 0;";
		css += "border-radius: 3px;";
	css += "}";


	QHBoxLayout *central = new QHBoxLayout;

	playPause = new CustomPushButton(tr("Pause Slideshow"),this);
	connect(playPause, SIGNAL(clicked()), this, SLOT(togglePlay()));

	audio = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	media = new Phonon::MediaObject(this);
	Phonon::createPath(media,audio);
	volume = new Phonon::VolumeSlider(audio,this);
	volume->setStyleSheet(css);
	volume->setCursor(Qt::PointingHandCursor);
	volume->setOrientation(Qt::Horizontal);
	volume->setMaximumWidth(200);
	volume->setTracking(true);
	volume->setMuteVisible(false);

	volumeLabel = new QLabel(tr("Music Volume:"));
	volumeLabel->setStyleSheet("QLabel { color: white; background: none; } QLabel:disabled { color: grey; } ");


	cancel = new CustomPushButton(tr("Exit Slideshow"));

	central->addStretch();
	central->addWidget(playPause);
	central->addSpacing(20);
	central->addWidget(volumeLabel);
	central->addWidget(volume);
	central->addStretch();
	central->addWidget(cancel);

	this->setLayout(central);


	nextImg = new QTimer;
	connect(nextImg, SIGNAL(timeout()), this, SLOT(loadNextImg()));

	// At the end of the music file we restart it if the slideshow is still running
	connect(media, SIGNAL(aboutToFinish()), this, SLOT(endOfMusicFile()));


}

// Play/Pause Slideshow
void SlideShowBar::togglePlay() {

	if(nextImg->isActive()) {
		if(verbose) qDebug() << "sldb: Toggle Playback (Play)";
		playPause->setText(tr("Play Slideshow"));
		nextImg->stop();
		if(musicFile != "")
			media->pause();
	} else {
		if(verbose) qDebug() << "sldb: Toggle Playback (Pause)";
		playPause->setText(tr("Pause Slideshow"));
		nextImg->start();
		if(musicFile != "")
			media->play();
	}

}


void SlideShowBar::loadNextImg() {

	if(verbose) qDebug() << "sldb: Load next img";

	emit moveInDirectory(1);

}


void SlideShowBar::endOfMusicFile() {

	if(verbose) qDebug() << "sldb: End of music file";

	media->stop();
	media->play();

}

void SlideShowBar::aniFinished() {

	if(animateInAndOut && isShown)
		QTimer::singleShot(400, this, SLOT(animate()));

	animateInAndOut = false;

}

void SlideShowBar::animate() {


	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown && enabled) {

		if(verbose) qDebug() << "sldb: Animate in";

		ani->setDuration(500);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		if(verbose) qDebug() << "sldb: Animate out";

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();
	}

}


void SlideShowBar::startSlideShow() {

	if(verbose) qDebug() << "sldb: Start Slideshow";

	if(musicFile != "") {
		volume->setEnabled(true);
		volumeLabel->setEnabled(true);
		media->setCurrentSource(Phonon::MediaSource(musicFile));
		media->play();
	} else {
		volume->setEnabled(false);
		volumeLabel->setEnabled(false);
	}

	nextImg->start();

}


void SlideShowBar::stopSlideShow() {

	if(verbose) qDebug() << "sldb: Stop slideshow";

	media->stop();

	nextImg->stop();

}


// Enabling styling of widget
void SlideShowBar::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SlideShowBar::~SlideShowBar() { }

