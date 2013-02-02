#include "imagemagick.h"

ImageMagick::ImageMagick() : QObject() {

}

void ImageMagick::readThisImage(QString filename) {

	image.read(filename.toStdString());
	imageRead = QImage(image.columns(), image.rows(), QImage::Format_RGB32);

}

ImageMagick::~ImageMagick() { }
