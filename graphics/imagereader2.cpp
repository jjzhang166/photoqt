#include "imagereader2.h"
#include <iostream>

ImageReader2::ImageReader2(bool v) : QObject() {

	verbose = v;

	gmfiles = "";
	qtfiles = "";

}


void ImageReader2::loadImage(QString filename) {

	loadedImagePath = filename;

	loadedImage.fill(Qt::transparent);


#ifdef GM
	Magick::Image image;
	QString faultyImage = "";
#endif

	QImageReader reader;

//	if(verbose) std::clog << "[reader] zoomed: " << zoomed << std::endl;

	bool useMagick = doIUseMagick(filename);
	if(verbose) std::clog << "Using Graphicsengine: " << (useMagick ? "Magick" : "ImageReader") << std::endl;

	loadedImageSize = QSize();
	loadedImageFileFormat = "";
//	unsigned int readerWidth = 0;
//	unsigned int readerHeight = 0;

//	QImage img;

	if(!useMagick) {

		// Scaling and adding the image to display (using QImageReader for better performance)
		reader.setFileName(filename);

		// Store the width/height for later use
		loadedImageSize = reader.size();
		// Store the fileformat for later use
		loadedImageFileFormat = reader.format().toLower();

		// The image width/height
//		readerWidth = origSize.width();
//		readerHeight = origSize.height();



	} else {

#ifdef GM

		try {
			if(QFileInfo(filename).suffix() == "x" || QFileInfo(filename).suffix() == "avs") image.magick("AVS");
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

		loadedImageSize = QSize(image.columns(),image.rows());

		if(faultyImage == "") {

//			loadedImageSize = QSize(readerWidth,readerHeight);
			try {
				loadedImageFileFormat = QString::fromStdString(image.format());
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



//	int dispWidth = readerWidth;
//	int dispHeight = readerHeight;

	// If the image is rotated to the left or right, the image dimensions are swapped
//	if(rotation == 90 || rotation == 270)
//		loadedImageSize = QSize(loadedImageSize.height(),loadedImageSize.width());


	// Calculate the factor by which the main image (view) has to be zoomed
//	float q = 1;

//	if(dispWidth > maxSize.width()) {
//			q = maxSize.width()/(dispWidth*1.0);
//			dispWidth *= q;
//			dispHeight *= q;
//	}

//	if(zoomed)
//		scaleImg1 = q;
//	else
//		scaleImg1 = -1;

	// If thumbnails are kept visible, then we need to subtract their height from the absolute height otherwise they overlap with the main image
//	if(dispHeight > maxSize.height()) {
//		q = maxSize.height()/(dispHeight*1.0);
//		dispWidth *= q;
//		dispHeight *= q;
//	}

//	if(zoomed && dispWidth < maxSize.width())
//		scaleImg2 = q;
//	else
//		scaleImg2 = -1;


	// If image is rotated left or right, then we set the right image dimensions again
//	if(rotation == 90 || rotation == 270) {
//		int tmp = dispHeight;
//		dispHeight = dispWidth;
//		dispWidth = tmp;
//	}


	loadedImageAnimated = false;

	if(!useMagick) {

//		if(!zoomed)
//			reader.setScaledSize(QSize(dispWidth,dispHeight));

		// Eventually load the image
		loadedImage = reader.read();

		if(verbose) std::clog << "[read] image: " << loadedImageSize.width() << " - " << loadedImageSize.height() << std::endl;

		if(reader.supportsAnimation() && reader.imageCount() > 1)
			loadedImageAnimated = true;

	} else {

#ifdef GM


		Magick::Blob blob;

		if(faultyImage == "") {

//			if(!zoomed && !dontscale) {

//				try {
//					image.zoom(Magick::Geometry(QString("%1x%2").arg(dispWidth).arg(dispHeight).toStdString()));
//				} catch(Magick::Warning &warning) {
//					std::cerr << "[zoom] CAUGHT Magick++ WARNING: " << warning.what() << std::endl;
//				} catch(Magick::Exception &error) {
//					std::cerr << "[zoom] CAUGHT Magick++ EXCEPTION: " << error.what() << std::endl;
//					faultyImage = error.what();
//				} catch(std::exception &error) {
//					std::cerr << "[zoom] CAUGHT C++ STD EXCEPTION: " << error.what() << std::endl;
//					faultyImage = error.what();
//				} catch( ... ) {
//					std::cerr << "[zoom] CRITICAL ERROR: unknown reason" << std::endl;
//					faultyImage = "unknown error";
//				}
//			}

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
			loadedImage.loadFromData(imgData);

		} else {

			QPixmap pix(":/img/plainerrorimg.png");
			QPainter paint(&pix);
			QTextDocument txt;
			txt.setHtml("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\">ERROR LOADING IMAGE<br><br><bR>" + faultyImage + "</div></center>");
			paint.translate(100,150);
			txt.setTextWidth(440);
			txt.drawContents(&paint);
			paint.end();
			loadedImage = pix.toImage();

		}

#endif

	}


}


QImage ImageReader2::getImageWithSize(QSize screen, QSize img) {

//	img.setWidth(img.width()+500);
//	img.setHeight(img.height()+500);

	QImage resizedImageRead(loadedImage);

//	bool scaleheight = false;
//	if(abs(screen.width()-loadedImageSize.width()) > abs(screen.height()-loadedImageSize.height()))
//		scaleheight = true;



	if(resizedImageRead.height() > img.height())
		resizedImageRead = resizedImageRead.scaledToHeight(img.height(),Qt::SmoothTransformation);

	if(resizedImageRead.width() > img.width())
		resizedImageRead = resizedImageRead.scaledToWidth(img.width(),Qt::SmoothTransformation);

	loadedImageSizeCurrentlyDisplayed = resizedImageRead.size();

	return resizedImageRead;

}


bool ImageReader2::doIUseMagick(QString filename) {

#ifdef GM
	QStringList qtFiles = qtfiles.replace("*","").split(",");

	for(int i = 0; i < qtFiles.length(); ++i) {
		if(filename.toLower().endsWith(qtFiles.at(i)))
			return false;
	}

	return true;
#else
	return false;
#endif

}


ImageReader2::~ImageReader2() { }
