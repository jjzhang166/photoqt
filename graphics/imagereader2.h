#ifndef IMAGEREADER2_H
#define IMAGEREADER2_H

#include <QObject>
#include <QImage>
#include <QImageReader>
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QTextDocument>

#ifdef GM
#include <GraphicsMagick/Magick++/Image.h>
#endif


class ImageReader2 : public QObject {

	Q_OBJECT

public:
	ImageReader2(bool v);
	~ImageReader2();

	bool verbose;

	QString qtfiles;
	QString gmfiles;

	QImage loadedImage;
	QString loadedImagePath;
	QString loadedImageFileFormat;
	QSize loadedImageSize;
	QSize loadedImageSizeCurrentlyDisplayed;
	bool loadedImageAnimated;

	void loadImage(QString filename);

	QImage getImageWithSize(QSize screen, QSize img);



	bool doIUseMagick(QString filename);

};






#endif	// IMAGEREADER2_H
