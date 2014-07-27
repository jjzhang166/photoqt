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

	QHBoxLayout *central = new QHBoxLayout;

	// Play and pause the whole slideshow (pictures and music)
	playPause = new CustomPushButton(tr("Pause Slideshow"),this);
	connect(playPause, SIGNAL(clicked()), this, SLOT(togglePlay()));

	// A mediaplayer to play an audiofile
	player = new QMediaPlayer;
	connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));
	connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(mediaStateChanged(QMediaPlayer::MediaStatus)));
	player->setVolume(80);

	// volume slider and percentage display
	volume = new CustomSlider;
	volume->setMinimum(0);
	volume->setMaximum(100);
	volume->setValue(80);
	volume->setMaximumWidth(200);
	connect(volume, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged()));
	volumeLabel = new CustomLabel(tr("Music Volume:"));
	volumePercentage = new CustomLabel("80%");
	volumePercentage->setFixedWidth(50);

	// button to quit the slideshow
	cancel = new CustomPushButton(tr("Exit Slideshow"));

	// Add all elements to layout
	central->addWidget(playPause);
	central->addStretch();
	central->addWidget(volumeLabel);
	central->addWidget(volume);
	central->addWidget(volumePercentage);
	central->addStretch();
	central->addWidget(cancel);

	this->setLayout(central);

	// Every x seconds (set by user) we show the next image
	nextImg = new QTimer;
	connect(nextImg, SIGNAL(timeout()), this, SLOT(loadNextImg()));

}

// Play/Pause Slideshow
void SlideShowBar::togglePlay() {

	if(nextImg->isActive()) {
		if(verbose) std::clog << "sldb: Toggle Playback (Play)" << std::endl;
		playPause->setText(tr("Play Slideshow"));
		nextImg->stop();
		if(musicFile != "")
			player->pause();
	} else {
		if(verbose) std::clog << "sldb: Toggle Playback (Pause)" << std::endl;
		playPause->setText(tr("Pause Slideshow"));
		nextImg->start();
		if(musicFile != "")
			player->play();
	}

}

// Load next image in directory
void SlideShowBar::loadNextImg() {

	if(verbose) std::clog << "sldb: Load next img" << std::endl;

	emit moveInDirectory(1);

}

// When music is played and the file comes to an end (but the images not)
void SlideShowBar::mediaStateChanged(QMediaPlayer::MediaStatus state) {

	if(verbose) std::clog << "sldb: Media State changed... New state: " << state << std::endl;

	if(state == QMediaPlayer::EndOfMedia) {
		player->stop();
		player->play();
	}

}

// Update the volume
void SlideShowBar::volumeChanged() {

	volumePercentage->setText(QString("%1%").arg(volume->value()));
	player->setVolume(volume->value());

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

	if(musicFile != "") {
		player->setMedia(QUrl::fromLocalFile(musicFile));
		player->play();
	}

	nextImg->start();

}


void SlideShowBar::stopSlideShow() {

	if(verbose) std::clog << "sldb: Stop slideshow" << std::endl;

	player->stop();

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

