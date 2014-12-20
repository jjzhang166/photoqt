#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <iostream>
#include <QObject>
#include <QImageReader>
#include <QFileInfo>
#include <QDir>
#include <QTextDocument>
#include <QPixmap>
#include <QPainter>
#include <QtSvg/QtSvg>

#ifdef GM
#include <GraphicsMagick/Magick++/Image.h>
#endif

class ImageReader : public QObject {

	Q_OBJECT

public:
	ImageReader(bool v);

	QString qtfiles;
	QString gmfiles;
	QString extrasfiles;

	float scaleImg1;
	float scaleImg2;

	bool animatedImg;
	QString fileformat;
	QSize origSize;

	QImage readImage(QString filename, int rotation, bool zoomed, bool fitinwindow, QSize maxSize, bool dontscale = false);

private:
	bool verbose;

	QImage readImage_QT(QString filename, int rotation, bool zoomed, bool fitinwindow, QSize maxSize, bool dontscale = false);
	QImage readImage_GM(QString filename, int rotation, bool zoomed, bool fitinwindow, QSize maxSize, bool dontscale = false);
	QImage readImage_XCF(QString filename, int rotation, bool zoomed, bool fitinwindow, QSize maxSize, bool dontscale = false);

	bool doIUseMagick(QString filename);
	QString whatDoIUse(QString filename);

};



#endif // IMAGEREADER_H
