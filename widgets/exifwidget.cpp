#include "exifwidget.h"

// This class provides a widget for displaying meta (Exif) information
Exif::Exif(QWidget *parent, QMap<QString, QVariant> set): QWidget(parent) {

	// The global settings
	globSet = set;

	// style widget
	this->setObjectName("exif");
	this->setStyleSheet("QWidget#exif { border-radius: 8px; border-bottom-left-radius: 0px; border-top-left-radius: 0px; background-color: rgba(0, 0, 0, 200); }");
	this->setContentsMargins(10,10,10,10);

	onlineservice = "maps.google.com";

	// Initiate and set layout
	central = new QVBoxLayout;
	this->setLayout(central);

	// Setup animation
	ani = new QPropertyAnimation(this,"geometry");

	// A boolean storing if widget is shown or hidden
	isShown = false;

	// block widget in certain circumstances
	blockAll = false;

	// Store if widget shall be triggered my mouse movement
	mouseTrickerEnable = true;

	// The dimensions of the widget (shown and hidden)
	rectShown = QRect(0,50,300,600);
	rectHidden = QRect(-rectShown.width(),rectShown.y(),rectShown.width(),rectShown.height());

	// Initially the widget it hidden
	this->setGeometry(rectHidden);

	// This widget is shown when no file is loaded
	empty = new QLabel("<center><h1>" + tr("No file loaded") + "</h1></center>");
	empty->setStyleSheet("background: transparent; color: rgb(135,135,135);");
	empty->setWordWrap(true);
	central->addWidget(empty);

	// Setup the labels
	setupLabels();

	// Confirm a rotation
	rotConf = new CustomConfirm(tr("Rotate Image?"), tr("The Exif data of this image says, that this image is supposed to be rotated.") + "<br><br>" + tr("Do you want to apply the rotation?"), tr("Okay, do it"), tr("What? No!"),QSize(450,210), this->parentWidget());
	rotConf->setDontShowAgain();
	connect(rotConf, SIGNAL(confirmed()), this, SLOT(rotConfYes()));
	connect(rotConf, SIGNAL(rejected()), this, SLOT(rotConfNo()));


	// These QMaps hold the translation for some fields
	mapAll.clear();

	// The light source "translated" into a human-readable format
	QMap<QString,QString> mapLightSource;
	//: This string refers to the light source
	mapLightSource.insert("0",tr("Unknown"));
	//: This string refers to the light source
	mapLightSource.insert("1",tr("Daylight"));
	//: This string refers to the light source
	mapLightSource.insert("2",tr("Fluorescent"));
	//: This string refers to the light source
	mapLightSource.insert("3",tr("Tungsten (incandescent light)"));
	//: This string refers to the light source
	mapLightSource.insert("4",tr("Flash"));
	//: This string refers to the light source
	mapLightSource.insert("9",tr("Fine weather"));
	//: This string refers to the light source
	mapLightSource.insert("10",tr("Cloudy Weather"));
	//: This string refers to the light source
	mapLightSource.insert("11",tr("Shade"));
	//: This string refers to the light source
	mapLightSource.insert("12",tr("Daylight fluorescent (D 5700 - 7100K)"));
	//: This string refers to the light source
	mapLightSource.insert("13",tr("Day white fluorescent (N 4600 - 5400K)"));
	//: This string refers to the light source
	mapLightSource.insert("14",tr("Cool white fluorescent") + "(W 3900 - 4500K)");
	//: This string refers to the light source
	mapLightSource.insert("15",tr("White fluorescent") + "(WW 3200 - 3700K)");
	//: This string refers to the light source
	mapLightSource.insert("17",tr("Standard light") + " A");
	mapLightSource.insert("18",tr("Standard light") + " B");
	mapLightSource.insert("19",tr("Standard light") + " C");
	mapLightSource.insert("20","D55");
	mapLightSource.insert("21","D65");
	mapLightSource.insert("22","D75");
	mapLightSource.insert("23","D50");
	//: This string refers to the light source
	mapLightSource.insert("24",tr("ISO studio tungsten"));
	//: This string refers to the light source
	mapLightSource.insert("255",tr("Other light source"));

	//: This string identifies that flash was fired
	QString fYes = tr("yes");
	//: This string identifies that flash wasn't fired
	QString fNo = tr("no");
	//: This string refers to a flash mode
	QString fNoStrobe = tr("strobe return light not detected");
	//: This string refers to a flash mode
	QString fYesStrobe = tr("strobe return light detected");
	//: This string refers to a flash mode
	QString fComp = tr("compulsory flash mode");
	//: This string refers to a flash mode
	QString fAuto = tr("auto mode");
	//: This string refers to a flash mode
	QString fRed = tr("red-eye reduction mode");
	//: This string refers to a flash mode
	QString fYesReturn = tr("return light detected");
	//: This string refers to a flash mode
	QString fNoReturn = tr("return light not detected");

	// The flash "translated" into a human-readable format
	QMap<QString,QString> mapFlash;
	mapFlash.insert("0",fYes);
	mapFlash.insert("1",fNo);
	mapFlash.insert("5",fNoStrobe);
	mapFlash.insert("6",fYesStrobe);
	mapFlash.insert("9",fYes + " (" + fComp + ")");
	mapFlash.insert("13",fYes + " (" + fComp + ", " + fNoReturn + ")");
	mapFlash.insert("15",fYes + " (" + fComp + ", " + fYesReturn + ")");
	mapFlash.insert("16",fNo + " (" + fComp + ")");
	mapFlash.insert("24",fNo + " (" + fAuto + ")");
	mapFlash.insert("25",fYes + " (" + fAuto + ")");
	mapFlash.insert("29",fYes + " (" + fAuto + ", " + fNoReturn + ")");
	mapFlash.insert("31",fYes + " (" + fAuto + ", " + fYesReturn + ")");
	//: This string refers to a flash mode
	mapFlash.insert("32",tr("No flash function"));
	mapFlash.insert("65",fYes + " (" + fRed + ")");
	mapFlash.insert("69",fYes + " (" + fRed + ", " + fNoReturn + ")");
	mapFlash.insert("71",fYes + " (" + fRed + ", " + fYesReturn + ")");
	mapFlash.insert("73",fYes + " (" + fComp + ", " + fRed + ")");
	mapFlash.insert("77",fYes + " (" + fComp + ", " + fRed + ", " + fNoReturn + ")");
	mapFlash.insert("79",fYes + " (" + fComp + ", " + fRed + ", " + fYesReturn + ")");
	mapFlash.insert("89",fYes + " (" + fAuto + ", " + fRed + ")");
	mapFlash.insert("93",fYes + " (" + fAuto + ", " + fNoReturn + ", " + fRed + ")");
	mapFlash.insert("95",fYes + " (" + fAuto + ", " + fYesReturn + ", " + fRed + ")");

	// The scene type "translated" into a human-readable format
	QMap<QString,QString> mapSceneType;
	//: This string refers to a scene type
	mapSceneType.insert("0",tr("Standard"));
	//: This string refers to a scene type
	mapSceneType.insert("1",tr("Landscape"));
	//: This string refers to a scene type
	mapSceneType.insert("2",tr("Portrait"));
	//: This string refers to a scene type
	mapSceneType.insert("3",tr("Night Scene"));

	// Store these maps in a global map with the internal ids as accessor
	mapAll.insert("LightSource",mapLightSource);
	mapAll.insert("Flash",mapFlash);
	mapAll.insert("SceneType",mapSceneType);


}

// Setup the labels
void Exif::setupLabels() {

	// This QMap stores the exiv2 key and the connected internal ids
	keyVal.clear();

	labels.clear();
	labelsId.clear();

	// Initiate the labels
	labels << tr("Filename");
	labelsId << "Filename";

	labels << tr("Filetype");
	labelsId << "Filetype";

	labels << tr("Filesize");
	labelsId << "Filesize";

	labels << tr("Dimensions");
	labelsId << "Dimensions";

	labels << "";
	labelsId << "";

	labels << tr("Make");
	labelsId << "Make";
	keyVal.insert("Exif.Image.Make","Make");

	labels << tr("Model");
	labelsId << "Model";
	keyVal.insert("Exif.Image.Model","Model");

	labels << tr("Software");
	labelsId << "Software";
	keyVal.insert("Exif.Image.Software","Software");

	labels << tr("Time photo was taken");
	labelsId << "PhotoTaken";
	keyVal.insert("Exif.Photo.DateTimeOriginal","PhotoTaken");

	labels << "";
	labelsId << "";

	labels << tr("Exposure Time");
	labelsId << "ExposureTime";
	keyVal.insert("Exif.Photo.ExposureTime","ExposureTime");
	units.insert("ExposureTime","1 s");

	labels << tr("Flash");
	labelsId << "Flash";
	keyVal.insert("Exif.Photo.Flash","Flash");

	labels << tr("ISO");
	labelsId << "Iso";
	keyVal.insert("Exif.Photo.ISOSpeedRatings","Iso");

	labels << "";
	labelsId << "";

	labels << tr("Scene Type");
	labelsId << "SceneType";
	keyVal.insert("Exif.Photo.SceneCaptureType","SceneType");

	labels << tr("Focal Length");
	labelsId << "FLength";
	keyVal.insert("Exif.Photo.FocalLength","FLength");
	units.insert("FLength","1 mm");

	labels << tr("F Number");
	labelsId << "FNumber";
	keyVal.insert("Exif.Photo.FNumber","FNumber");
	units.insert("FNumber","0F");

	labels << tr("Light Source");
	labelsId << "LightSource";
	keyVal.insert("Exif.Photo.LightSource","LightSource");

	labels << "";
	labelsId << "";

	labels << tr("GPS Position");
	labelsId << "Gps";

	// The GPS position is composed of these 4 data sets
	keyVal.insert("Exif.GPSInfo.GPSLatitudeRef","gpslatref");
	keyVal.insert("Exif.GPSInfo.GPSLatitude","gpslat");
	keyVal.insert("Exif.GPSInfo.GPSLongitudeRef","gpslonref");
	keyVal.insert("Exif.GPSInfo.GPSLongitude","gpslon");

	labelCSS = "color: white; background: rgba(0,0,0,100); padding: 2px; border-radius: 4px;";
	labelCSSfontsize = QString("font-size: %1pt;").arg(globSet.value("ExifFontSize").toInt());

	// Eventually set up the labels
	for(int i = 0; i < labels.length(); ++i) {

		if(labels.at(i) != "") {
			if(labelsId.at(i) != "Gps") {
				QLabel *l = new QLabel("<b>" + labels.at(i) + "</b>");
				l->setStyleSheet(labelCSS + labelCSSfontsize);
				items.insert(labelsId.at(i),l);
				central->addWidget(l);
				l->hide();
			} else {
				LabelClick *l = new LabelClick;
				l->setCursor(Qt::PointingHandCursor);
				l->setText("<b>" + labels.at(i) + "</b>");
				connect(l, SIGNAL(clicked()), this, SLOT(gpsClick()));
				l->setStyleSheet(labelCSS + labelCSSfontsize);
				items.insert(labelsId.at(i),l);
				central->addWidget(l);
				l->hide();
			}

		} else
			central->addSpacing(8);

	}

	central->addSpacing(8);

	// This pushbutton enabled an option for triggering "stay open" state
	stay = new CustomCheckBox("Keep Open");
	stay->setIndicatorSize(10);
	stay->setFontSize(8);
	QHBoxLayout *stayLay = new QHBoxLayout;
	stayLay->addStretch();
	stayLay->addWidget(stay);
	stayLay->addStretch();
	central->addLayout(stayLay);
	stay->hide();

}

// Update the labels data
void Exif::updateData(QString currentfile, QSize origSize, bool exiv2Supported) {

	// This button is only shown when mouse triggering is enabled
	if(mouseTrickerEnable)
		stay->show();
	else
		stay->hide();

	// As soon as data is loaded, the empty label is hidden
	empty->hide();

	// Setup some data
	QFileInfo info(currentfile);
	QImageReader file(currentfile);

	// This string is re-used again and again
	QString temp = "";

	// Set the filename and show label
	temp = items["Filename"]->text();
	if(items["Filename"]->text().contains(":"))
		temp = items["Filename"]->text().split(":").at(0);
	items["Filename"]->setText(temp + ": " + info.fileName());
	items["Filename"]->setToolTip(items["Filename"]->text());
	if(globSet.value("ExifFilename").toBool())
		items["Filename"]->show();
	else
		items["Filename"]->hide();

	// Set the filetype and show label
	temp = items["Filetype"]->text();
	if(items["Filetype"]->text().contains(":"))
		temp = items["Filetype"]->text().split(":").at(0);
	items["Filetype"]->setText(temp + ": " + file.format());
	items["Filetype"]->setToolTip(items["Filetype"]->text());
	if(globSet.value("ExifFiletype").toBool())
		items["Filetype"]->show();
	else
		items["Filetype"]->hide();

	// Set the filesize and show label
	temp = items["Filesize"]->text();
	if(items["Filesize"]->text().contains(":"))
		temp = items["Filesize"]->text().split(":").at(0);
	items["Filesize"]->setText(temp + ": " + QString("%1").arg(info.size()/1024) + " KB");
	items["Filesize"]->setToolTip(items["Filesize"]->text());
	if(globSet.value("ExifFilesize").toBool())
		items["Filesize"]->show();
	else
		items["Filesize"]->hide();

	// Set the filesize and show label
	temp = items["Dimensions"]->text();
	if(items["Dimensions"]->text().contains(":"))
		temp = items["Dimensions"]->text().split(":").at(0);
	items["Dimensions"]->setText(temp + ": " + QString("%1x%2").arg(origSize.width()).arg(origSize.height()) + " px");
	items["Dimensions"]->setToolTip(items["Dimensions"]->text());
	if(globSet.value("ExifDimensions").toBool())
		items["Dimensions"]->show();
	else
		items["Dimensions"]->hide();


	// If image format is supported
	if(exiv2Supported) {


		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(currentfile.toUtf8().constData());
		image->readMetadata();
		Exiv2::ExifData &exifData = image->exifData();

		QString gpsLonRef = "";
		QString gpsLon = "";
		QString gpsLatRef = "";
		QString gpsLat = "";

		QMapIterator<QString,QString> i(keyVal);
		while(i.hasNext()) {
			i.next();

			try {
				Exiv2::ExifKey k(i.key().toStdString());
				Exiv2::ExifData::const_iterator it = exifData.findKey(k);

				if(it != exifData.end()) {

					QString val = QString::fromStdString(Exiv2::toString(it->value()));

					// Check if value is known
					if(mapAll.contains(i.value())) {
						QMap<QString,QString> m = mapAll[i.value()];
						if(m.keys().contains(val))
							val = m[val];
						// If data is in unknown format
						else
							val = "[" + tr("Unknown data") + "]";
					}

					// Some values need to be formatted a little

					if(i.value() == "ExposureTime")
						val = exifExposureTime(val);

					if(i.value() == "FLength" || i.value() == "FNumber")
						val = exifFNumberFLength(val);

					if(i.value() == "PhotoTaken")
						val = exifPhotoTaken(val);

					// The GPS data is stored seperately and composed to one at end
					if(i.value() == "gpslonref")
						gpsLonRef = val;
					else if(i.value() == "gpslon")
						gpsLon = val;
					else if(i.value() == "gpslatref")
						gpsLatRef = val;
					else if(i.value() == "gpslat")
						gpsLat = val;


					// Store the unit for this data (set up in setupLabels() function)
					QString unit1 = "";
					QString unit2 = "";
					temp = units.value(i.value());
					if(temp != "") {
						if(temp.startsWith("1"))
							unit2 = temp.remove(0,1);
						else
							unit1 = temp.remove(0,1);
					}

					// Ignore GPS, set data to rest
					if(!i.value().startsWith("gpsl")) {

						temp = items[i.value()]->text();
						if(items[i.value()]->text().contains(":"))
							temp = items[i.value()]->text().split(":").at(0);
						items[i.value()]->setText(temp + ": " + unit1 + val + unit2);
						items[i.value()]->setToolTip(items[i.value()]->text());
						if(globSet.value("Exif" + i.value()).toBool())
							items[i.value()]->show();
						else
							items[i.value()]->hide();

					}


				} else {

					// If function gets here, then the corresponding value hasn't been set

					QString iValue = i.value();

					if(iValue == "gpslatref" || iValue == "gpslat" || iValue == "gpslonref" || iValue == "gpslon")
						iValue = "Gps";

					temp = items[iValue]->text();
					if(items[iValue]->text().contains(":"))
						temp = items[iValue]->text().split(":").at(0);
					items[iValue]->setText(temp + ": <i>[" + tr("not set") + "]</i>");
					items[iValue]->setToolTip(items[iValue]->text());
					if(globSet.value("Exif" + iValue).toBool())
						items[iValue]->show();
					else
						items[iValue]->hide();

				}

			} catch(const Exiv2::AnyError&) { }


		}

		// Default: not set
		QString gps = "<i>[" + tr("not set") + "]</i>";

		// If however set, compose it and store in 'gps'
		if(gpsLonRef != "" && gpsLon != "" && gpsLatRef != "" && gpsLat != "")
			gps = exifGps(gpsLonRef, gpsLon, gpsLatRef, gpsLat);
		temp = items["Gps"]->text();
		if(items["Gps"]->text().contains(":"))
			temp = items["Gps"]->text().split(":").at(0);
		items["Gps"]->setText(temp + ": " + gps);
		items["Gps"]->setToolTip(items["Gps"]->text());
		if(globSet.value("ExifGps").toBool())
			items["Gps"]->show();
		else
			items["Gps"]->hide();




		// Look for orientation (not displayed, but could rotate/flip image)
		Exiv2::ExifKey k("Exif.Image.Orientation");
		Exiv2::ExifData::const_iterator it = exifData.findKey(k);
		if(it != exifData.end()) {

			QString val = QString::fromStdString(Exiv2::toString(it->value()));

			flipHor = false;
			// 1 = No rotation/flipping
			if(val == "1")
				rotationDeg = 0;
			// 2 = Horizontally Flipped
			if(val == "2") {
				rotationDeg = 0;
				flipHor = true;
			// 3 = Rotated by 180 degrees
			} else if(val == "3")
				rotationDeg = 180;
			// 4 = Rotated by 180 degrees and flipped horizontally
			else if(val == "4") {
				rotationDeg = 180;
				flipHor = true;
			// 5 = Rotated by 270 degrees and flipped horizontally
			} else if(val == "5") {
				rotationDeg = 270;
				flipHor = true;
			// 6 = Rotated by 270 degrees
			} else if(val == "6")
				rotationDeg = 270;
			// 7 = Flipped Horizontally and Rotated by 90 degrees
			else if(val == "7") {
				rotationDeg = 90;
				flipHor = true;
			// 8 = Rotated by 90 degrees
			} else if(val == "8")
				rotationDeg = 90;

			if(rotationDeg != 0 || flipHor) {
				if(globSet.value("ExifRotation").toString() == "Always")
					emit setOrientation(rotationDeg,flipHor);
				else if(globSet.value("ExifRotation").toString() == "Ask")
					rotConf->animate();
			}

		}


	// If image format isn't supported
	} else {

		QMapIterator<QString,QString> i(keyVal);

		while(i.hasNext()) {
			i.next();

			QString val = i.value();
			if(val.startsWith("gpsl"))
				val = "Gps";

			temp = items[val]->text();
			if(items[val]->text().contains(":"))
				temp = items[val]->text().split(":").at(0);
			items[val]->setText(temp + ": <i>[" + tr("unsupported format") + "]</i>");
			items[val]->setToolTip(items[val]->text());
			if(globSet.value("Exif" + val).toBool())
				items[val]->show();
			else
				items[val]->hide();

		}

	}

	adjustHeight();

}

// Format exposure time
QString Exif::exifExposureTime(QString value) {

	if(value.contains("/")) {
		QStringList split = value.split("/");
		if(split.at(0) != "1") {
			int t1 = split.at(0).toInt();
			float t2 = split.at(1).toFloat();
			// I got a bug report of Photo crashing for certain images that have an exposure time of "0/1". So we have to check for it, or we get a division by zero, i.e. crash
			if(t1 == 0) {
				t1 = 0;
				t2 = 0;
				value = "0";
			} else if(t1 != 1) {
				t1 = t1/t1;
				t2 = t2/t1;
				value = QString("%1/%2").arg(t1).arg(t2);
			} else {
				value = QString("%1/%2").arg(t1).arg(t2);
			}
		}

	}

	return value;

}

// Format Focal Length
QString Exif::exifFNumberFLength(QString value) {

	if(value.contains("/")) {
		QStringList split = value.split("/");
		float t1 = split.at(0).toFloat();
		float t2 = split.at(1).toFloat();
		t1 = t1/t2;
		value = QString("%1").arg(t1);
	}

	return value;

}

// Format time the photo was taken
QString Exif::exifPhotoTaken(QString value) {

       QStringList split = value.split(" ");
       QStringList split2 = split.at(0).split(":");
       if(split.length() > 1 && split2.length() > 2)
	       value = split2.at(2) + "/" + split2.at(1) + "/" + split2.at(0) + ", " + split.at(1);

       return value;

}

// Compose GPS data
QString Exif::exifGps(QString gpsLonRef, QString gpsLon, QString gpsLatRef, QString gpsLat) {

	// Format the latitude string
	QStringList split = gpsLat.split(" ");
	for(int i = 0; i < split.length(); ++i) {
		if(split.at(i).contains("/")) {
			float t1 = split.at(i).split("/").at(0).toFloat();
			float t2 = split.at(i).split("/").at(1).toFloat();
			split.replace(i,QString("%1").arg(t1/t2));
		}

	}
	gpsLat = split.at(0) + "&deg;" + split.at(1) + "'" + split.at(2) + "''";

	// Format the longitude string
	split = gpsLon.split(" ");
	for(int i = 0; i < split.length(); ++i) {
		if(split.at(i).contains("/")) {
			float t1 = split.at(i).split("/").at(0).toFloat();
			float t2 = split.at(i).split("/").at(1).toFloat();
			split.replace(i,QString("%1").arg(t1/t2));
		}
	}
	gpsLon = split.at(0) + "&deg;" + split.at(1) + "'" + split.at(2) + "''";

	// Compose all the gps data into one string
	return gpsLat + " " + gpsLatRef + ", " + gpsLon + " " + gpsLonRef;

}

// Click on yes
void Exif::rotConfYes() {

	emit setOrientation(rotationDeg, flipHor);

	if(rotConf->dontShowAgain->isChecked()) {
		QMap<QString,QVariant> set;
		set.insert("ExifRotation","Always");
		emit updateSettings(set);
	}

}

// Click on no
void Exif::rotConfNo() {

	if(rotConf->dontShowAgain->isChecked()) {
		QMap<QString,QVariant> set;
		set.insert("ExifRotation","Never");
		emit updateSettings(set);
	}

}

// Animate open/close the widget
void Exif::animate() {

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

	}

}

// Adjust the height of the widget
void Exif::adjustHeight() {

	// The height depends on the amount of labels shown
	int count = 4;
	for(int i = 0; i < labelsId.length(); ++i) {
		if(globSet.value("Exif" + labelsId.at(i)).toBool())
			++count;
	}

	int fsize = globSet.value("ExifFontSize").toInt();

	// Adjust the QRects
	rectShown = QRect(rectShown.x(),(this->parentWidget()->height()-count*(200/fsize))/4,41*fsize,count*fsize*2.8);
	rectHidden = QRect(-rectShown.width(),rectShown.y(),rectShown.width(),rectShown.height());

	// Set the appropriate QRect
	if(isShown)
		this->setGeometry(rectShown);
	else if(!isShown)
		this->setGeometry(rectHidden);


}

// Click on GPS location (opens in online map (google/bing))
void Exif::gpsClick() {

	QStringList temp = items["Gps"]->text().split(":");
	temp.removeFirst();
	QString loc = temp.join(":").trimmed();

	if(!loc.contains("[") && !loc.contains("]")) {

		loc.replace("&deg;",QString::fromUtf8("°"));

		QUrl url;
		if(onlineservice == "bing.com/maps")
			url.setUrl("http://www.bing.com/maps/?sty=b&q=" + loc);
		else
			url.setUrl("http://maps.google.com/maps?t=h&q=" + loc);


		if(!QDesktopServices::openUrl(QUrl(url)))
			qDebug() << "ERROR: Couldn't open URL...";

	}

}

// Change the fontsize of all the labels
void Exif::updateFontsize() {

	labelCSSfontsize = QString("font-size: %1pt;").arg((globSet.value("ExifFontSize").toInt()));

	for(int i = 0; i < labelsId.size(); ++i) {

		if(labelsId.at(i) != "")
			items[labelsId.at(i)]->setStyleSheet(labelCSS + labelCSSfontsize);

	}

}


// Make the widget styleable
void Exif::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


Exif::~Exif() { }



void LabelClick::mouseReleaseEvent(QMouseEvent *) {
	emit clicked();
}
