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

#include "slideshowbar.h"
#include <iostream>

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

#ifdef WITH_PHONON
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

#endif

	cancel = new CustomPushButton(tr("Exit Slideshow"));

	central->addStretch();
	central->addWidget(playPause);
	central->addSpacing(20);
#ifdef WITH_PHONON
	central->addWidget(volumeLabel);
	central->addWidget(volume);
#endif
	central->addStretch();
	central->addWidget(cancel);

	this->setLayout(central);


	nextImg = new QTimer;
	connect(nextImg, SIGNAL(timeout()), this, SLOT(loadNextImg()));

#ifdef WITH_PHONON
	// At the end of the music file we restart it if the slideshow is still running
	connect(media, SIGNAL(aboutToFinish()), this, SLOT(endOfMusicFile()));
#endif


}

// Play/Pause Slideshow
void SlideShowBar::togglePlay() {

	if(nextImg->isActive()) {
		if(verbose) std::clog << "sldb: Toggle Playback (Play)" << std::endl;
		playPause->setText(tr("Play Slideshow"));
		nextImg->stop();
#ifdef WITH_PHONON
		if(musicFile != "")
			media->play();
#endif
	} else {
		if(verbose) std::clog << "sldb: Toggle Playback (Pause)" << std::endl;
		playPause->setText(tr("Pause Slideshow"));
		nextImg->start();
#ifdef WITH_PHONON
		if(musicFile != "")
			media->pause();
#endif
	}

}

// Load next image in directory
void SlideShowBar::loadNextImg() {

	if(verbose) std::clog << "sldb: Load next img" << std::endl;

	emit moveInDirectory(1);

}

// When music is played and the file comes to an end (but the images not)
void SlideShowBar::endOfMusicFile() {

	if(verbose) std::clog << "sldb: End of music file" << std::endl;

#ifdef WITH_PHONON
	media->stop();
	media->play();
#endif

}

void SlideShowBar::aniFinished() {

	if(animateInAndOut && isShown)
		QTimer::singleShot(400, this, SLOT(animate()));

	animateInAndOut = false;

}


void SlideShowBar::makeShow() {

	if(!isShown) animate();

}

void SlideShowBar::makeHide() {

	if(isShown) animate();

}

void SlideShowBar::setWidth(int w) {

	rectShown = QRect(0,0,w,50);
	rectHidden = QRect(0,-50,w,50);

	if(isShown) this->setGeometry(rectShown);
	else this->setGeometry(rectHidden);

}

void SlideShowBar::animate() {


	// Open widget
	if(!isShown && enabled) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(500);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();
	}

}


void SlideShowBar::startSlideShow() {

	if(verbose) std::clog << "sldb: Start Slideshow" << std::endl;
#ifdef WITH_PHONON
	if(musicFile != "") {
		volume->setEnabled(true);
		volumeLabel->setEnabled(true);
		media->setCurrentSource(Phonon::MediaSource(musicFile));
		media->play();
	} else {
		volume->setEnabled(false);
		volumeLabel->setEnabled(false);
	}
#endif

	nextImg->start();

}


void SlideShowBar::stopSlideShow() {

	if(verbose) std::clog << "sldb: Stop slideshow" << std::endl;

#ifdef WITH_PHONON
	media->stop();
#endif

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

