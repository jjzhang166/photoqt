/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "scale.h"

Scale::Scale(bool verbose, QWidget *parent) : MyWidget(parent) {

	this->setVisibleArea(QSize(550,300));

	// Some variables
	ignoreSizeChange = false;
	this->verbose = verbose;

	// A title label
	CustomLabel *title = new CustomLabel("<center><h1>" + tr("Scale Image") + "</h1></center>");
	// A label to display the current size
	curSize = new CustomLabel("<center>" + tr("Current Size:") + "</center>");

	// Layout for the spinboxes and their labels
	QGridLayout *spinLay = new QGridLayout;

	// Width label and spinbox
	CustomLabel *widthLabel = new CustomLabel("     " + tr("New Width:"));
	widthLabel->setAlignment(Qt::AlignRight);
	widthSpin = new CustomSpinBox;
	widthSpin->setObjectName("w");
	widthSpin->setMaximum(999999);
	widthSpin->setFixedWidth(70);
	widthSpin->setBackground("white", "black");
	widthSpin->setBorder("black",1);
	widthSpin->setFontColor("black");
	widthSpin->setEnabled(false);
	spinLay->addWidget(widthLabel,0,0);
	spinLay->addWidget(widthSpin,0,1);
	connect(widthSpin, SIGNAL(valueChanged(int)), this, SLOT(sizeChanged()));

	// Height label and spinbox
	CustomLabel *heightLabel = new CustomLabel("     " + tr("New Height:"));
	heightLabel->setAlignment(Qt::AlignRight);
	heightSpin = new CustomSpinBox;
	heightSpin->setObjectName("h");
	heightSpin->setMaximum(999999);
	heightSpin->setFixedWidth(70);
	heightSpin->setBackground("white", "black");
	heightSpin->setBorder("black",1);
	heightSpin->setFontColor("black");
	heightSpin->setEnabled(false);
	spinLay->addWidget(heightLabel,1,0);
	spinLay->addWidget(heightSpin,1,1);
	connect(heightSpin, SIGNAL(valueChanged(int)), this, SLOT(sizeChanged()));

	// Checkbox to keep aspect ratio
	keepratio = new CustomCheckBox(tr("Aspect Ratio"));
	keepratio->setIndicatorImage(":/img/ratioKeep.png",":/img/ratioDontKeep.png");
	keepratio->setChecked(true);
	keepRatioClicked();	// Set tooltip etc.
	connect(keepratio, SIGNAL(clicked()), this, SLOT(keepRatioClicked()));
	connect(keepratio, SIGNAL(clicked()), this, SLOT(sizeChanged()));

	// Layout to put the above in horizontal order
	QHBoxLayout *spinCheckLay = new QHBoxLayout;
	spinCheckLay->addStretch();
	spinCheckLay->addLayout(spinLay);
	spinCheckLay->addWidget(keepratio);
	spinCheckLay->addStretch();


	// Set the quality of the scaling
	CustomLabel *qualityLabel = new CustomLabel(tr("Quality:"));
	CustomSpinBox *qualityValue = new CustomSpinBox;
	qualityValue->setMinimum(0);
	qualityValue->setMaximum(100);
	qualityValue->setValue(90);
	quality = new CustomSlider;
	quality->setMinimum(0);
	quality->setMaximum(100);
	quality->setFixedWidth(150);
	quality->setValue(90);
	QHBoxLayout *qualityLay = new QHBoxLayout;
	qualityLay->addStretch();
	qualityLay->addWidget(qualityLabel);
	qualityLay->addWidget(quality);
	qualityLay->addWidget(qualityValue);
	qualityLay->addStretch();
	connect(qualityValue, SIGNAL(valueChanged(int)), quality, SLOT(setValue(int)));
	connect(quality, SIGNAL(valueChanged(int)), qualityValue, SLOT(setValue(int)));


	// Pushbuttons to go ahead or cancel
	enterInPlace = new CustomPushButton(tr("Scale in place"));
	enterInPlace->setObjectName("enterInPlace");
	CustomPushButton *enterNew = new CustomPushButton(tr("Scale in new file"));
	enterNew->setObjectName("enterNew");
	CustomPushButton *cancel = new CustomPushButton(tr("Don't Scale"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(enterInPlace);
	butLay->addWidget(enterNew);
	butLay->addWidget(cancel);
	butLay->addStretch();
	connect(cancel, SIGNAL(clicked()), this, SLOT(disableAllSpinBoxAndClose()));
	connect(enterInPlace, SIGNAL(clicked()), this, SLOT(enterClicked()));
	connect(enterNew, SIGNAL(clicked()), this, SLOT(enterClicked()));


	// Put everything into layout and set to widget
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget(title);
	lay->addSpacing(15);
	lay->addWidget(curSize);
	lay->addSpacing(20);
	lay->addLayout(spinCheckLay);
	lay->addSpacing(10);
	lay->addLayout(qualityLay);
	lay->addSpacing(15);
	lay->addLayout(butLay);
	lay->addStretch();
	this->setWidgetLayout(lay);

	// We need to initialise them here, otherwise the system shortcut in mainwindow.cpp might cause a crash
	confirmNotSupported = new CustomConfirm("","");
	confirmInPlace = new CustomConfirm("","");
	confirmNew = new CustomConfirm("","");

}

// This disables the spinboxes and closes the widget
// We need to disable the spinboxes, otherwise the keyboard will be caught by them and other shortcuts (like 'o', ...) wont work anymore
void Scale::disableAllSpinBoxAndClose() {

	widthSpin->setEnabled(false);
	heightSpin->setEnabled(false);

	makeHide();

}

// Open widget for scaling
void Scale::scale(QString filename, QSize s) {

	if(verbose) std::clog << "scale: Open Widget for Scaling: " << filename.toStdString() << std::endl;

	// Scaling currently only works for Qt-supported image formats - GraphicsMagick support not yet implemented
	if(!QImageReader::supportedImageFormats().contains(QFileInfo(filename).suffix().toLower().toLatin1())) {

		confirmNotSupported = new CustomConfirm(tr("Filetype Not Supported"),tr("Sorry, scaling is currently only available for filetypes natively supported by Qt..."),tr("Oh, okay..."),"",QSize(350,200), "default", "default", this->parentWidget());
		confirmNotSupported->show();
		confirmNotSupported->setRects(getRectShown(),getRectHidden(),getRectAni());
		connect(confirmNotSupported, SIGNAL(blockFunc(bool)), this, SIGNAL(blockFunc(bool)));
		confirmNotSupported->makeShow();

		return;

	}

	// Save the current filename
	currentfile = filename;

	// This makes it easier and quicker to work with
	widthByHeightRatio = (1.0*s.width())/(1.0*s.height());

	// Set some values
	curSize->setText("<center>" + tr("Current Size:") + QString(" %1 x %2").arg(s.width()).arg(s.height()) + "</center>");
	widthSpin->setValue(s.width());
	heightSpin->setValue(s.height());
	keepratio->setChecked(true);

	// And show widget
	makeShow();

	// Focus and select width dimension
	widthSpin->setFocus();
	widthSpin->selectAll();

}

// When either dimension has been changed, then we possibly need to update the other (in order to preserve aspect ratio
void Scale::sizeChanged() {

	// If keep aspect ratio, and if current size change event is not caused by setting size programmatically
	if(keepratio->isChecked() && !ignoreSizeChange) {

		// Which dimension has been edited (if none saved, then aspect ratio possibly was disabled before, so we take the dimension that was edited last)
		QString obj = ((CustomSpinBox *)sender())->objectName();
		if(obj != "w" && obj != "h" && lastEdit != "")
			obj = lastEdit;
		lastEdit = "";

		if(verbose) std::clog << "scale: Adjust dimension (received '" << obj.toStdString() << "') to keep aspect ratio" << std::endl;

		// WHile setting the new size to the other spinbox, a valuechanged event will be emitted -> ignore it
		ignoreSizeChange = true;
		if(obj == "w")
			heightSpin->setValue(widthSpin->value()/widthByHeightRatio);
		else if(obj == "h")
			widthSpin->setValue(heightSpin->value()*widthByHeightRatio);
		ignoreSizeChange = false;

	} else if(!keepratio->isChecked()) {

		// Keep Ratio disabled -> if it will be enabled again, the last edited dimension will be the dominant one
		lastEdit = ((CustomSpinBox *)sender())->objectName();

		if(verbose) std::clog << "scale: Store last edited: " << lastEdit.toStdString() << std::endl;
	}

}

// Toggle Keep Ratio
void Scale::keepRatioClicked() {

	if(verbose) std::clog << "scale: Toggle Checkbox for keeping aspect ratio" << std::endl;

	keepratio->setFontColor(keepratio->isChecked() ? "white" : "grey");
	keepratio->setToolTip(!keepratio->isChecked() ? tr("Don't keep aspect ratio") : tr("Keep aspect ratio"));

}

// Click on enter (either one)
void Scale::enterClicked() {

	// Get info on which one was clicked
	QString obj = ((CustomPushButton *) sender())->objectName();

	if(verbose) std::clog << "scale: Click on enter: " << obj.toStdString() << std::endl;

	// If the image is to be scaled in place
	if(obj == "enterInPlace") {

		// Ask for confirmation
		confirmInPlace = new CustomConfirm(tr("Overwrite Image?"),
					    tr("This will replace the current image with the scaled version... Continue?"),
					    tr("Continue"), tr("Stop"),
					    QSize(300,200), "default", "rgba(0,0,0,240)", this->parentWidget());
		confirmInPlace->show();
		confirmInPlace->setRects(getRectShown(),getRectHidden(),getRectAni());
		// If confirmed, we automatically call the scale function with the current filename
		QSignalMapper *map = new QSignalMapper;
		map->setMapping(confirmInPlace,currentfile);
		connect(confirmInPlace, SIGNAL(confirmed()), map, SLOT(map()));
		connect(map, SIGNAL(mapped(QString)), this, SLOT(doScale(QString)));
		confirmInPlace->makeShow();
		// And if confirmed, we also close this widget
		connect(confirmInPlace, SIGNAL(confirmed()), this, SLOT(disableAllSpinBoxAndClose()));

	// If the image is to be scaled to a new file
	} else {

		// Get a new filename
		QString fname = QFileDialog::getSaveFileName(0,tr("Save as..."),currentfile, tr("Images") + " (*." + QFileInfo(currentfile).suffix() + ")", 0, QFileDialog::DontConfirmOverwrite);
		if(fname == "") return;

		// If the file exists already ask for confirmation
		if(QFileInfo(fname).exists()) {

			confirmNew = new CustomConfirm(tr("Overwrite Image?"),
						    tr("This file exists already... Continue?"),
						    tr("Continue"), tr("Stop"),
						    QSize(300,200), "default", "rgba(0,0,0,240)", this->parentWidget());
			confirmNew->show();
			confirmNew->setRects(getRectShown(),getRectHidden(),getRectAni());
			QSignalMapper *map = new QSignalMapper;
			map->setMapping(confirmNew,fname);
			connect(confirmNew, SIGNAL(confirmed()), map, SLOT(map()));
			connect(map, SIGNAL(mapped(QString)), this, SLOT(doScale(QString)));
			connect(confirmNew, SIGNAL(confirmed()), this, SLOT(disableAllSpinBoxAndClose()));
			confirmNew->makeShow();

		} else {

			// If it's a new file, scale
			disableAllSpinBoxAndClose();
			doScale(fname);

		}

	}

}

// Scaling an image
void Scale::doScale(QString filename) {

	if(verbose) std::clog << "scale: Scaling image '" << filename.toStdString() << "'" << std::endl;

	// These image formats known by exiv2 are also supported by PhotoQt
	QStringList formats;
	formats << "jpeg"
		<< "jpg"
		<< "tif"
		<< "tiff"
		<< "png"
		<< "psd"
		<< "jpeg2000"
		<< "jp2"
		<< "jpc"
		<< "j2k"
		<< "jpf"
		<< "jpx"
		<< "jpm"
		<< "mj2"
		<< "bmp"
		<< "bitmap"
		<< "gif"
		<< "tga";

#ifdef EXIV2

	// This will store all the exif data
	Exiv2::ExifData exifData;
	bool gotExifData = false;

	if(formats.contains(QFileInfo(currentfile).suffix().toLower()) && formats.contains(QFileInfo(filename).suffix().toLower())) {

		if(verbose) std::clog << "scale: image format supported by exiv2" << std::endl;

		try {

			// Open image for exif reading
			Exiv2::Image::AutoPtr image_read = Exiv2::ImageFactory::open(currentfile.toStdString());

			if(image_read.get() != 0) {

				// YAY, WE FOUND SOME!!!!!
				gotExifData = true;

				// read exif
				image_read->readMetadata();
				exifData = image_read->exifData();

				// Update dimensions
				exifData["Exif.Photo.PixelXDimension"] = int32_t(widthSpin->value());
				exifData["Exif.Photo.PixelYDimension"] = int32_t(heightSpin->value());

			}

		}

		catch (Exiv2::Error& e) {
			std::cerr << "scale: ERROR reading exif data (caught exception): " << e.what() << std::endl;
		}

	} else
		if(verbose) std::clog << "scale: image format NOT supported by exiv2" << std::endl;


#endif

	// We need to do the actual scaling in between reading the exif data above and writing it below,
	// since we might be scaling the image in place and thus would overwrite old exif data
	QImageReader reader(currentfile);
	reader.setScaledSize(QSize(widthSpin->value(),heightSpin->value()));
	QImage img = reader.read();
	img.save(filename,0,quality->value());

#ifdef EXIV2

	// We don't need to check again, if both files are actually supported formats, since if either one isn't supported, this bool cannot be true
	if(gotExifData) {

		try {

			// And write exif data to new image file
			Exiv2::Image::AutoPtr image_write = Exiv2::ImageFactory::open(filename.toStdString());
			image_write->setExifData(exifData);
			image_write->writeMetadata();

		}

		catch (Exiv2::Error& e) {
			std::cerr << "scale: ERROR writing exif data (caught exception): " << e.what() << std::endl;
		}

	}

#endif

}


Scale::~Scale() { }
