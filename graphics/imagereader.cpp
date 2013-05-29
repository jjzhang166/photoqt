#include "imagereader.h"

ImageReader::ImageReader() : QObject() {

}

bool ImageReader::doIUseMagick(QString filename) {

#ifdef WITH_GRAPHICSMAGICK
	QStringList qtFiles;
	qtFiles << ".bmp";
	qtFiles << ".gif";
	qtFiles << ".tif";
	qtFiles << ".tiff";
	qtFiles << ".jpg";
	qtFiles << ".jpeg";
	qtFiles << ".jpeg2000";
	qtFiles << ".pbm";
	qtFiles << ".pgm";
	qtFiles << ".ppm";
	qtFiles << ".xbm";
	qtFiles << ".xpm";

	for(int i = 0; i < qtFiles.length(); ++i) {
		if(filename.toLower().endsWith(qtFiles.at(i)))
			return false;
	}

	return true;
#else
	return false;
#endif

}


QImage ImageReader::readImage(QString filename, int rotation, bool zoomed, QSize maxSize, bool dontscale) {

#ifdef WITH_GRAPHICSMAGICK
	Magick::Image image;
#endif

	QImageReader reader;

	qDebug() << "ZOOMED:" << zoomed;

	bool useMagick = doIUseMagick(filename);
	qDebug() << "Using Graphicsengine:" << (useMagick ? "Magick" : "ImageReader");

	origSize = QSize();
	fileformat = "";
	unsigned int readerWidth = 0;
	unsigned int readerHeight = 0;

	QImage img;

	if(!useMagick) {

		// Scaling and adding the image to display (using QImageReader for better performance)
		reader.setFileName(filename);

		// Store the width/height for later use
		origSize = reader.size();
		// Store the fileformat for later use
		fileformat = reader.format().toLower();

		// The image width/height
		readerWidth = origSize.width();
		readerHeight = origSize.height();



	} else {

#ifdef WITH_GRAPHICSMAGICK

		qDebug() << "BEFORE";

		image.read(filename.toAscii().data());

		qDebug() << "AFTER";

		readerWidth = image.columns();
		readerHeight = image.rows();

		origSize = QSize(readerWidth,readerHeight);
		fileformat = QString::fromStdString(image.format());
#endif

	}



	int dispWidth = readerWidth;
	int dispHeight = readerHeight;

	// If the image is rotated to the left or right, the image dimensions are swapped
	if(rotation == 90 || rotation == 270) {
		int tmp = dispWidth;
		dispWidth = dispHeight;
		dispHeight = tmp;
	}

//	qDebug() << "D1:" << dispWidth << "-" << dispHeight;


	// Calculate the factor by which the main image (view) has to be zoomed
	float q = 1;

	if(dispWidth > maxSize.width()) {
			q = maxSize.width()/(dispWidth*1.0);
			dispWidth *= q;
			dispHeight *= q;
	}

//	qDebug() << "D2:" << dispWidth << "-" << dispHeight;

	if(zoomed)
		scaleImg1 = q;
	else
		scaleImg1 = -1;

	// If thumbnails are kept visible, then we need to subtract their height from the absolute height otherwise they overlap with the main image
	if(dispHeight > maxSize.height()) {
		q = maxSize.height()/(dispHeight*1.0);
		dispWidth *= q;
		dispHeight *= q;
	}

//	qDebug() << "D3:" << dispWidth << "-" << dispHeight;

	if(zoomed && dispWidth < maxSize.width())
//	if(zoomed)
		scaleImg2 = q;
	else
		scaleImg2 = -1;


	// If image is rotated left or right, then we set the right image dimensions again
	if(rotation == 90 || rotation == 270) {
		int tmp = dispHeight;
		dispHeight = dispWidth;
		dispWidth = tmp;
	}


	animatedImg = false;

	if(!useMagick) {

		if(!zoomed)
			reader.setScaledSize(QSize(dispWidth,dispHeight));

		// Eventually load the image
		img = reader.read();

		qDebug() << "READER:" << img.width() << "-" << img.height() << "z:" << zoomed;

		if(reader.supportsAnimation() && reader.imageCount() > 1)
			animatedImg = true;

	} else {

#ifdef WITH_GRAPHICSMAGICK

		if(!zoomed && !dontscale)
			image.zoom(Magick::Geometry(QString("%1x%2").arg(dispWidth).arg(dispHeight).toStdString()));
//					image.resize(Magick::Geometry(QString("%1x%2").arg(dispWidth).arg(dispHeight).toStdString()));

		Magick::Blob blob;
		image.depth(8);
		image.magick("XPM"); // Set XPM
		image.write( &blob );

		// Passing Image Buffer to a QPixmap
		const QByteArray imgData ((char*)(blob.data()));
		img.loadFromData(imgData);
#endif

	}

	return img;

}




ImageReader::~ImageReader() { }
