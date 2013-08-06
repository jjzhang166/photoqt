#ifndef SLIDESHOWBAR_H
#define SLIDESHOWBAR_H

#include "../customelements/custompushbutton.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>

#ifdef PHONON
#include <phonon/AudioOutput>
#include <phonon/MediaObject>
#include <phonon/VolumeSlider>
#endif

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

#ifdef PHONON
	// The Phonon Objects for the possibly set music file
	Phonon::AudioOutput *audio;
	Phonon::MediaObject *media;
#endif

	// A button to play/pause the slideshow
	CustomPushButton *playPause;

#ifdef PHONON
	// Adjusting the volume
	Phonon::VolumeSlider *volume;
	QLabel *volumeLabel;
#endif

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
	void endOfMusicFile();

protected:
	void paintEvent(QPaintEvent *);

signals:
	// To load next image
	void moveInDirectory(int direction);

};

#endif // SLIDESHOWBAR_H
