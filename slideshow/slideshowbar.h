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

#ifndef SLIDESHOWBAR_H
#define SLIDESHOWBAR_H

#include "../customelements/custompushbutton.h"
#include "../customelements/customslider.h"
#include "../customelements/customlabel.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>
#include <QFile>

#include <QtMultimedia/QMediaPlayer>

class SlideShowBar : public QWidget {

	Q_OBJECT

public:
	SlideShowBar(QMap<QString,QVariant> set, QWidget *parent = 0, bool verbose = false);
	~SlideShowBar();

	bool verbose;

	// All the global settings
	QMap<QString,QVariant> globSet;

	void makeShow();
	void makeHide();
	bool isVisible() { return isShown; }
	bool isEnabled() { return enabled; }
	void setWidth(int w);
	void setEnabled(bool e) { enabled = e; }

	// The timer to load the next image
	QTimer *nextImg;

	// Exiting the slideshow
	CustomPushButton *cancel;

	// At the start and end of a slideshow, the bar is shortly sliding in and out again (after a very short delay)
	bool animateInAndOut;

	QString musicFile;

private:

	// The animation framework
	QPropertyAnimation *ani;

	// The QRects for the two end states
	QRect rectHidden;
	QRect rectShown;

	// Is widget shown?
	bool isShown;

	// Is a slideshow running?
	bool enabled;


	QMediaPlayer *player;

	// A button to play/pause the slideshow
	CustomPushButton *playPause;
	CustomSlider *volume;
	CustomLabel *volumeLabel;
	CustomLabel *volumePercentage;

public slots:
	// Animation functions
	void animate();
	void aniFinished();

	// Start and Stop the Slideshow
	void startSlideShow();
	void stopSlideShow();

	// Load the next Image
	void loadNextImg();

	// Play/Pause Slideshow
	void togglePlay();

	// When the music is at the end, but the slideshow isn't, then the music is restarted
	void mediaStateChanged(QMediaPlayer::MediaStatus state);

	// When the volume slider is moved, we update the percentage label
	void volumeChanged();

protected:
	void paintEvent(QPaintEvent *);

signals:
	// To load next image
	void moveInDirectory(int direction);

};

#endif // SLIDESHOWBAR_H
