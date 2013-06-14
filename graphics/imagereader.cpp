#include "imagereader.h"

#include <iostream>

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
	qtFiles << ".png";
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
	QString faultyImage = "";
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

		try {
			if(QFileInfo(filename).suffix() == "x")
				filename = "avs:" + filename;
			image.read(filename.toAscii().data());
		} catch( Magick::ErrorFileOpen &error ) {
			std::cerr << "[read] ERROR: " << error.what() << std::endl;
			faultyImage = error.what();
		} catch( Magick::Warning &warning ) {
			std::cerr << "[read] WARNING: " << warning.what() << std::endl;
		} catch(Magick::Exception &error) {
			std::cerr << "[read] ERROR: " << error.what() << std::endl;
			faultyImage = error.what();
		} catch(std::exception &error) {
			std::cerr << "CAUGHT C++ STD EXCEPTION: " << error.what() << std::endl;
			faultyImage = error.what();
		} catch( ... ) {
			std::cerr << "[read] CRITICAL ERROR: unknown reason" << std::endl;
			faultyImage = "unknown error";
		}

		readerWidth = image.columns();
		readerHeight = image.rows();

		if(faultyImage == "") {

			origSize = QSize(readerWidth,readerHeight);
			try {
				fileformat = QString::fromStdString(image.format());
			} catch(Magick::Warning &warning) {
				std::cerr << "[format] ERROR: " << warning.what() << std::endl;
			} catch(Magick::Exception &error) {
				std::cerr << "[format] ERROR: " << error.what() << std::endl;
				faultyImage = error.what();
			} catch(std::exception &error) {
				std::cerr << "CAUGHT C++ STD EXCEPTION: " << error.what() << std::endl;
				faultyImage = error.what();
			} catch(...) {
				std::cerr << "[format] ERROR: unknown error" << std::endl;
				faultyImage = "unknown error";
			}
		}

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


	// Calculate the factor by which the main image (view) has to be zoomed
	float q = 1;

	if(dispWidth > maxSize.width()) {
			q = maxSize.width()/(dispWidth*1.0);
			dispWidth *= q;
			dispHeight *= q;
	}

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

	if(zoomed && dispWidth < maxSize.width())
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


		Magick::Blob blob;

		if(faultyImage == "") {

			if(!zoomed && !dontscale) {

				try {
					image.zoom(Magick::Geometry(QString("%1x%2").arg(dispWidth).arg(dispHeight).toStdString()));
//					image.depth(8);
//					image.magick("XPM"); // Set XPM
//					image.write( &blob );
				} catch(Magick::Warning &warning) {
					std::cerr << "[zoom] CAUGHT Magick++ WARNING: " << warning.what() << std::endl;
				} catch(Magick::Exception &error) {
					std::cerr << "[zoom] CAUGHT Magick++ EXCEPTION: " << error.what() << std::endl;
					faultyImage = error.what();
				} catch(std::exception &error) {
					std::cerr << "[zoom] CAUGHT C++ STD EXCEPTION: " << error.what() << std::endl;
					faultyImage = error.what();
				} catch( ... ) {
					std::cerr << "[zoom] CRITICAL ERROR: unknown reason" << std::endl;
					faultyImage = "unknown error";
				}
			}

			try {
				image.depth(8);
				image.magick("XPM"); // Set XPM
				image.write( &blob );
			} catch(Magick::Warning &warning) {
				std::cerr << "[blob] CAUGHT Magick++ WARNING: " << warning.what() << std::endl;
			} catch(Magick::Exception &error) {
				std::cerr << "[blob] CAUGHT Magick++ EXCEPTION: " << error.what() << std::endl;
				faultyImage = error.what();
			} catch(std::exception &error) {
				std::cerr << "[blob] CAUGHT C++ STD EXCEPTION: " << error.what() << std::endl;
				faultyImage = error.what();
			} catch( ... ) {
				std::cerr << "[blob] CRITICAL ERROR: unknown reason" << std::endl;
				faultyImage = "unknown error";
			}


			// Passing Image Buffer to a QPixmap
			const QByteArray imgData ((char*)(blob.data()));
			img.loadFromData(imgData);

		} else {

			QPixmap pix(":/img/plainerrorimg.png");
			QPainter paint(&pix);
			QTextDocument txt;
			txt.setHtml("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\">ERROR LOADING IMAGE<br><br><bR>" + faultyImage + "</div></center>");
			paint.translate(100,150);
			txt.setTextWidth(440);
			txt.drawContents(&paint);
			paint.end();
			img = pix.toImage();

		}

#endif

	}

	return img;

}




ImageReader::~ImageReader() { }
