#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <QObject>
#include <GraphicsMagick/Magick++/Image.h>
#include <QImage>
#include <QImageReader>
#include <QtDebug>

class ImageReader : public QObject {

public:
	ImageReader();
	~ImageReader();

	QImage imageRead;

//	void readThisImage(QString filename);

	QImage readImage(QString filename, int rotation, bool zoomed, QSize maxSize);


	float scaleImg1;
	float scaleImg2;

	bool animatedImg;
	QString fileformat;
	QSize origSize;

	bool doIUseMagick(QString filename);
};

#endif // IMAGEREADER_H
