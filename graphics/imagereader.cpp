#include "imagereader.h"
#include <QtDebug>

ImageReader::ImageReader(bool v) : QObject() {

	verbose = v;

	gmfiles = "";
	qtfiles = "";

}

QImage ImageReader::readImage(QString filename, int rotation, bool zoomed, QSize maxSize, bool dontscale) {


	if(verbose) std::clog << "[reader] zoomed: " << zoomed << std::endl;

	bool useMagick = doIUseMagick(filename);
	if(verbose) std::clog << "Using Graphicsengine: " << (useMagick ? "Magick" : "ImageReader") << std::endl;
	std::clog << "Using Graphicsengine: " << (useMagick ? "Magick" : "ImageReader") << std::endl;

	if(useMagick)
		return readImage_GM(filename, rotation, zoomed, maxSize, dontscale);
	return readImage_QT(filename, rotation, zoomed, maxSize, dontscale);

}

QImage ImageReader::readImage_QT(QString filename, int rotation, bool zoomed, QSize maxSize, bool dontscale) {

	QImageReader reader;
	QImage img;

	// Scaling and adding the image to display (using QImageReader for better performance)
	reader.setFileName(filename);

	// Store the width/height for later use
	origSize = reader.size();
	// Store the fileformat for later use
	if(QFileInfo(filename).fileName() != "photoqt_tmp.png") fileformat = reader.format().toLower();
	else fileformat = "";

	// Sometimes the size returned by reader.size() is <= 0 (observed for .jp2 files)
	// -> then we need to load the actual image to get dimensions
	if(origSize.width() <= 0 || origSize.height() <= 0) {
		QImageReader r;
		r.setFileName(filename);
		origSize = r.read().size();
	}

	// The image width/height
	unsigned int readerWidth = origSize.width();
	unsigned int readerHeight = origSize.height();

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

	std::clog << "max = " << maxSize.width() << "|" << maxSize.height() << " -- w = " << dispWidth << " -- h = " << dispHeight << std::endl;

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

	if(!zoomed)
		reader.setScaledSize(QSize(dispWidth,dispHeight));

	std::clog << "SIZE: " << dispWidth << " -- " <<dispHeight << std::endl;

	// Eventually load the image
	img = reader.read();

	if(verbose) std::clog << "[read] image: " << img.width() << " - " << img.height() << " - z: " << zoomed << std::endl;

	if(reader.supportsAnimation() && reader.imageCount() > 1)
		animatedImg = true;

	return img;

}

// If GraphicsMagick supports the file format,
QImage ImageReader::readImage_GM(QString filename, int rotation, bool zoomed, QSize maxSize, bool dontscale) {

#ifdef GM

	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	char *data = new char[file.size()];
	qint64 s = file.read(data, file.size());
	if (s < file.size()) {
		delete[] data;
		if(verbose) std::cerr << "[reader gm] ERROR reading image file data" << std::endl;
		return QImage();
	}

	Magick::Blob blob(data, file.size());
	try {
		Magick::Image image;

		QString suf = QFileInfo(filename).suffix().toLower();

		if(suf == "x" || suf == "avs")

			image.magick("AVS");

		// ART IMAGE -- NOT WORKING (?)
		else if(suf == "art")

			image.magick("ART");

		else if(suf == "cals" || suf == "cal" || suf == "dcl"  || suf == "ras")

			image.magick("CALS");

		else if(suf == "cut")

			image.magick("CUT");

		else if(suf == "cur")

			image.magick("CUR");

		else if(suf == "acr" || suf == "dcm" || suf == "dicom" || suf == "dic")

			image.magick("DCM");

		else if(suf == "fax")

			image.magick("FAX");

		else if(suf == "ico")

			image.magick("ICO");

		else if(suf == "mtv")

			image.magick("MTV");

		else if(suf == "otb")

			image.magick("OTB");

		else if(suf == "palm")

			image.magick("PALM");

		else if(suf == "pict" || suf == "pct" || suf == "pic")

			image.magick("PICT");

		else if(suf == "pix"
			|| suf == "pal")

			image.magick("PIX");

		else if(suf == "tga")

			image.magick("TGA");

		else if(suf == "wbm"
			|| suf == "wbmp")

			image.magick("WBMP");

		else if(suf == "cmyk" || suf == "arw" || suf == "raw") {

			image.magick("CMYK");
			image.depth(8);
			image.size(Magick::Geometry(2000,1500));
			image.endian(Magick::LSBEndian);

		}


		image.read(blob);
		Magick::Blob ob;
		image.magick("XPM");
		image.write(&ob);

		QFile out(QDir::tempPath() + "/photoqt_tmp.xpm");
		out.open(QIODevice::WriteOnly);
		out.write(static_cast<const char*>(ob.data()), ob.length());

		return readImage_QT(QDir::tempPath() + "/photoqt_tmp.xpm",rotation,zoomed,maxSize,dontscale);

	} catch(Magick::Exception &error_) {
		delete[] data;
		std::cerr << "[reader gm] Error: " << error_.what() << std::endl;
		QPixmap pix(":/img/plainerrorimg.png");
		QPainter paint(&pix);
		QTextDocument txt;
		txt.setHtml("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\">ERROR LOADING IMAGE<br><br><bR>" + QString(error_.what()) + "</div></center>");
		paint.translate(100,150);
		txt.setTextWidth(440);
		txt.drawContents(&paint);
		paint.end();
		pix.save(QDir::tempPath() + "/photoqt_tmp.png");
		return readImage_QT(QDir::tempPath() + "/photoqt_tmp.png",rotation,zoomed,maxSize,dontscale);
	}

#endif

	return QImage();

}



bool ImageReader::doIUseMagick(QString filename) {

#ifdef GM
	QStringList qtFiles = qtfiles.replace("*","").split(",");

	for(int i = 0; i < qtFiles.length(); ++i) {
		if(filename.toLower().endsWith(qtFiles.at(i)))
			return false;
	}

	return true;
#endif
	return false;

}
