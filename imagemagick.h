#ifndef IMAGEMAGICK_H
#define IMAGEMAGICK_H

#include <QObject>
#include <Magick++.h>
#include <QImage>

class ImageMagick : public QObject {

public:
	ImageMagick();
	~ImageMagick();

	Magick::Image image;

	QImage imageRead;

	void readThisImage(QString filename);
};

#endif // IMAGEMAGICK_H
