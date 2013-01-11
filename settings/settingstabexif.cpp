#include "settingstabexif.h"

SettingsTabExif::SettingsTabExif(QWidget *parent, QMap<QString, QVariant> set) : QWidget(parent) {

	globSet = set;

	this->setStyleSheet("background: transparent; color: white;");

	// the main scroll widget for all content
	scrollbar = new CustomScrollbar;
	QScrollArea *scroll = new QScrollArea;
	QVBoxLayout *lay = new QVBoxLayout(scroll);
	QWidget *scrollWidg = new QWidget(scroll);
	scrollWidg->setLayout(lay);
	scroll->setWidget(scrollWidg);
	scroll->setWidgetResizable(true);
	QVBoxLayout *scrollLay = new QVBoxLayout;
	scrollLay->addWidget(scroll);
	this->setLayout(scrollLay);
	scroll->setVerticalScrollBar(scrollbar);

	// The title labels, explaining what can be done here
	QLabel *title = new QLabel("<center><h1>" + tr("Exif Information") + "</h1></center>" + tr("Photo can display meta (exif) information for images. The widget for this information is on the left outside the screen and slides in when mouse gets close to it and/or when the set shortcut (default Ctrl+E) is triggered. On demand, the triggering by mouse movement can be disabled by checking the box below."));
	title->setWordWrap(true);
	lay->addSpacing(10);
	lay->addWidget(title);
	lay->addSpacing(15);

	// Triggering on mouse movement
	QLabel *triggerOnMouseLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Trigger Widget on Mouse Hovering") + "</span></b><br><br>" + tr("Per default the exif widget can be shown two ways: Moving the mouse cursor to the left screen edge to fade it in temporarily (as long as the mouse is hovering it), or permanently by clicking the checkbox (checkbox only stored per session, can't be saved permanently!). Alternatively the widget can also be triggered by shortcut. On demand the mouse triggering can be disabled, so that the widget would only show on shortcut. This can come in handy, if you get annoyed by accidentally opening the widget occasionally."));
	triggerOnMouseLabel->setWordWrap(true);
	triggerOnMouse = new CustomCheckBox("Turn mouse triggering OFF");
	QHBoxLayout *triggerLay = new QHBoxLayout;
	triggerLay->addStretch();
	triggerLay->addWidget(triggerOnMouse);
	triggerLay->addStretch();
	lay->addWidget(triggerOnMouseLabel);
	lay->addSpacing(10);
	lay->addLayout(triggerLay);
	lay->addSpacing(20);



	QLabel *whichItemShown = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Which items are shown?") + "</span></b><br><br>" + tr("Photo can display a number of exif information. However, you might not be interested in some of them, hence you can choose to disable some of them here."));
	whichItemShown->setWordWrap(true);
	lay->addWidget(whichItemShown);
	lay->addSpacing(10);

	CustomPushButton *enableAll = new CustomPushButton(tr("Enable ALL"));
	enableAll->setObjectName("enable");
	CustomPushButton *disableALL = new CustomPushButton(tr("Disable ALL"));
	disableALL->setObjectName("disable");
	QHBoxLayout *disEnableAllLay = new QHBoxLayout;
	disEnableAllLay->addStretch();
	disEnableAllLay->addWidget(enableAll);
	disEnableAllLay->addWidget(disableALL);
	disEnableAllLay->addStretch();
	lay->addLayout(disEnableAllLay);
	lay->addSpacing(10);
	connect(enableAll, SIGNAL(clicked()), this, SLOT(disEnableAll()));
	connect(disableALL, SIGNAL(clicked()), this, SLOT(disEnableAll()));

	// The scrollwidget for all the tiles
	CustomScrollbar *scrollbarExif = new CustomScrollbar;
	scrollbarExif->keepAlwaysVisible(true);
	QScrollArea *exifWidg = new QScrollArea;
	exifWidg->setStyleSheet("background: transparent; border-radius: 5px;");
	flow = new FlowLayout(exifWidg);
	flow->setAlignment(Qt::AlignCenter);
	exifWidg->setFixedWidth(600);
	QWidget *scrollWidgExif = new QWidget(exifWidg);
	scrollWidgExif->setLayout(flow);
	exifWidg->setWidget(scrollWidgExif);
	exifWidg->setWidgetResizable(true);
	exifWidg->setVerticalScrollBar(scrollbarExif);
	QHBoxLayout *flowLay = new QHBoxLayout;
	flowLay->addStretch();
	flowLay->addWidget(exifWidg);
	flowLay->addStretch();
	lay->addLayout(flowLay);


	allItems.clear();
	allItemsShort.clear();

	// These are all the items and their Exif key
	allItems << tr("Filename");
	allItemsShort << "Filename";

	allItems << tr("Filetype");
	allItemsShort << "Filetype";

	allItems << tr("Filesize");
	allItemsShort << "Filesize";

	allItems << tr("Dimensions");
	allItemsShort << "Dimensions";

	allItems << tr("Make");
	allItemsShort << "Make";

	allItems << tr("Model");
	allItemsShort << "Model";

	allItems << tr("Software");
	allItemsShort << "Software";

	allItems << tr("Time Photo was Taken");
	allItemsShort << "PhotoTaken";

	allItems << tr("Exposure Time");
	allItemsShort << "ExposureTime";

	allItems << tr("Flash");
	allItemsShort << "Flash";

	allItems << tr("ISO");
	allItemsShort << "Iso";

	allItems << tr("Scene Type");
	allItemsShort << "SceneType";

	allItems << tr("Focal Length");
	allItemsShort << "FLength";

	allItems << tr("F-Number");
	allItemsShort << "FNumber";

	allItems << tr("Light Source");
	allItemsShort << "LightSource";

	allItems << tr("GPS Position");
	allItemsShort << "Gps";

	for(int i = 0; i < allItemsShort.length(); ++i) {

		SettingsTabExifTiles *tile = new SettingsTabExifTiles(allItems.at(i),allItemsShort.at(i));
		allTiles.append(tile);
		flow->addWidget(tile);

	}




	// Adjust Font Size of Labels
	QLabel *fontSizelabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Adjusting Font Size") + "</span></b><br><br>" + tr("Computers can have very different resolutions. On some of them, it might be nice to increase the font size of the labels to have them easier readable. Often, a size of 8 or 9 should be working quite well..."));
	fontSizelabel->setWordWrap(true);
	setFontSizeSlider = new CustomSlider;
	setFontSizeSlider->setMinimum(7);
	setFontSizeSlider->setMaximum(12);
	setFontSizeSlider->setValue(globSet.value("ExifFontSize").toInt());
	setFontSizeSlider->setTickPosition(QSlider::TicksBelow);
	setFontSizeSlider->setTickInterval(1);
	QLabel *fsizeleft = new QLabel(tr("Smallest size"));
	QLabel *fsizeright = new QLabel(tr("Biggest size"));
	QHBoxLayout *fsizeLay = new QHBoxLayout;
	fsizeLay->addStretch();
	fsizeLay->addWidget(fsizeleft);
	fsizeLay->addWidget(setFontSizeSlider);
	fsizeLay->addWidget(fsizeright);
	fsizeLay->addStretch();
	lay->addWidget(fontSizelabel);
	lay->addSpacing(5);
	lay->addLayout(fsizeLay);
	lay->addSpacing(20);


	// ALWAYS/NEVER/POSSIBLY ROTATE/FLIP ACCORDING TO EXIF DATA
	QLabel *rotateFlipLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Rotating/Flipping Image according to Exif Info") + "</span></b><br><br>" + tr("Some cameras can detect - while taking the photo - whether the camera was turned and might store this information in the image exif data. If Photo finds this information, it can rotate the image accordingly. Per default it's not doing it, but it can be enabled here as desired."));
	rotateFlipLabel->setWordWrap(true);
	QButtonGroup *groupRot = new QButtonGroup;
	exifRotNev = new CustomRadioButton(tr("Never rotate images"));
	exifRotAlw = new CustomRadioButton(tr("Always rotate images"));
	exifRotAsk = new CustomRadioButton(tr("Always ask"));
	groupRot->addButton(exifRotNev);
	groupRot->addButton(exifRotAlw);
	groupRot->addButton(exifRotAsk);
	QHBoxLayout *rotFlipLay = new QHBoxLayout;
	rotFlipLay->addStretch();
	rotFlipLay->addWidget(exifRotNev);
	rotFlipLay->addWidget(exifRotAlw);
	rotFlipLay->addWidget(exifRotAsk);
	rotFlipLay->addStretch();
	lay->addWidget(rotateFlipLabel);
	lay->addSpacing(5);
	lay->addLayout(rotFlipLay);
	lay->addSpacing(20);


	// Select the online map service to be used for displaying GPS location
	QLabel *exifGpsLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Online map for GPS") + "</span></b><br><br>" + tr("If you're image includes a GPS location, then a click on the location text will load this location in an online map using your default external browser. Here you can choose which online service to use (suggestions for other online maps always welcome)."));
	exifGpsLabel->setWordWrap(true);
	QButtonGroup *groupMap = new QButtonGroup;
	radioGoogle = new CustomRadioButton("maps.google.com");
	radioBing = new CustomRadioButton("bing.com/maps");
	groupMap->addButton(radioGoogle);
	groupMap->addButton(radioBing);
	QHBoxLayout *tileGpsLay = new QHBoxLayout;
	tileGpsLay->addStretch();
	tileGpsLay->addWidget(radioGoogle);
	tileGpsLay->addWidget(radioBing);
	tileGpsLay->addStretch();

	lay->addWidget(exifGpsLabel);
	lay->addSpacing(5);
	lay->addLayout(tileGpsLay);
	lay->addSpacing(20);


	lay->addStretch();

}

// Disable or Enable all tiles
void SettingsTabExif::disEnableAll() {

	QString pushed = (sender())->objectName();

	bool en = true;
	if(pushed == "disable")
		en = false;

	for(int i = 0; i < allTiles.length(); ++i)
		allTiles.at(i)->setChecked(en);

}

// Load the settings
void SettingsTabExif::loadSettings() {

	defaults.clear();

	triggerOnMouse->setChecked(!globSet.value("ExifEnableMouseTriggering").toBool());
	defaults.insert("ExifEnableMouseTriggering",!globSet.value("ExifEnableMouseTriggering").toBool());

	for(int i = 0; i < allTiles.length(); ++i) {

		QString id = allTiles.at(i)->intern;

		allTiles.at(i)->setChecked(globSet.value("Exif" + id).toBool());
		defaults.insert("Exif" + id,globSet.value("Exif" + id).toBool());

	}

	setFontSizeSlider->setValue(globSet.value("ExifFontSize").toInt());
	defaults.insert("ExifFontSize",globSet.value("ExifFontSize").toInt());

	if(globSet.value("ExifRotation").toString() == "Always") {
		exifRotAlw->setChecked(true);
		defaults.insert("ExifRotation","Always");
	} else if(globSet.value("ExifRotation").toString() == "Ask") {
		exifRotAsk->setChecked(true);
		defaults.insert("ExifRotation","Ask");
	} else if(globSet.value("ExifRotation").toString() == "Never") {
		exifRotNev->setChecked(true);
		defaults.insert("ExifRotation","Never");
	}

	if(globSet.value("ExifGPSMapService").toString() == "maps.google.com") {
		radioGoogle->setChecked(true);
		defaults.insert("ExifGPSMapService","maps.google.com");
	} else  if(globSet.value("ExifGPSMapService").toString() == "bing.com/maps") {
		radioBing->setChecked(true);
		defaults.insert("ExifGPSMapService","bing.com/maps");
	}

}

// Save the settings (if changed)
void SettingsTabExif::saveSettings() {

	updatedSet.clear();

	if(defaults.value("ExifEnableMouseTriggering").toBool() != triggerOnMouse->isChecked()) {
		updatedSet.insert("ExifEnableMouseTriggering",!triggerOnMouse->isChecked());
		defaults.remove("ExifEnableMouseTriggering");
		defaults.insert("ExifEnableMouseTriggering",!triggerOnMouse->isChecked());
	}

	for(int i = 0; i < allTiles.length(); ++i) {

		QString id = allTiles.at(i)->intern;

		if(allTiles.at(i)->enabled->isChecked() != defaults.value("Exif" + id)) {
			updatedSet.insert("Exif" + id,allTiles.at(i)->enabled->isChecked());
			defaults.remove("Exif" + id);
			defaults.insert("Exif" + id, allTiles.at(i)->enabled->isChecked());
		}

	}

	if(defaults.value("ExifFontSize").toInt() != setFontSizeSlider->value()) {
		updatedSet.insert("ExifFontSize",setFontSizeSlider->value());
		defaults.remove("ExifFontSize");
		defaults.insert("ExifFontSize",setFontSizeSlider->value());
	}

	if(exifRotAlw->isChecked() && defaults.value("ExifRotation").toString() != "Always") {
		updatedSet.insert("ExifRotation","Always");
		defaults.remove("ExifRotation");
		defaults.insert("ExifRotation","Always");
	} else if(exifRotAsk->isChecked() && defaults.value("ExifRotation").toString() != "Ask") {
		updatedSet.insert("ExifRotation","Ask");
		defaults.remove("ExifRotation");
		defaults.insert("ExifRotation","Ask");
	} else if(exifRotNev->isChecked() && defaults.value("ExifRotation").toString() != "Never") {
		updatedSet.insert("ExifRotation","Never");
		defaults.remove("ExifRotation");
		defaults.insert("ExifRotation","Never");
	}

	if(radioGoogle->isChecked() && defaults.value("ExifGPSMapService").toString() != "maps.google.com") {
		updatedSet.insert("ExifGPSMapService","maps.google.com");
		defaults.remove("ExifGPSMapService");
		defaults.insert("ExifGPSMapService","maps.google.com");
	} else if(radioBing->isChecked() && defaults.value("ExifGPSMapService").toString() != "bing.com/maps") {
		updatedSet.insert("ExifGPSMapService","bing.com/maps");
		defaults.remove("ExifGPSMapService");
		defaults.insert("ExifGPSMapService","bing.com/maps");
	}

}

void SettingsTabExif::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabExif::~SettingsTabExif() { }
