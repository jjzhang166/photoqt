#include "settingstablookandfeel.h"

SettingsTabLookAndFeel::SettingsTabLookAndFeel(QWidget *parent, QMap<QString, QVariant> set, bool v) : QWidget(parent) {

	// The global settings
	globSet = set;

	verbose = v;

	// Style widget
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

	// The title
	QLabel *title = new QLabel("<center><h1>" + tr("Look and Feel") + "</h1></center>");
	lay->addWidget(title);
	lay->addSpacing(20);


	// OPTION FOR COMPOSITE
	compositeBackground = new CustomRadioButton(tr("Enable Composite"));
	backgroundImageUseScreenshot = new CustomRadioButton(tr("Use Screenshot"));
	backgroundImageUseCustom = new CustomRadioButton(tr("Use custom background image"));
	noBackgroundImage = new CustomRadioButton(tr("Don't use either"));
	QLabel *compositeImageLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Composite or Background Image") + "</span></b><br><br>" + tr("If composite is enabled, then it causes the background of PhotoQt to be real half transparent. Of course, using composite is not a must. When disabled PhotoQt by default uses a screenshot and sets it as a background. However, this is only possible for single-screen setups. On multi-head setups you can only set either a background image (with overlay) or just set some background colour. Of course these two options are also available for setups with a single monitor only."));
	compositeImageLabel->setWordWrap(true);
	QHBoxLayout *compLay = new QHBoxLayout;
	compLay->addStretch();
	compLay->addWidget(compositeBackground);
	compLay->addWidget(backgroundImageUseScreenshot);
	compLay->addWidget(backgroundImageUseCustom);
	compLay->addWidget(noBackgroundImage);
	compLay->addStretch();
	lay->addWidget(compositeImageLabel);
	lay->addSpacing(10);
	lay->addLayout(compLay);

	// OPTION TO SET BACKGROUND IMAGE
	backgroundImage = new ClickLabel;
	backgroundImage->setStyleSheet("background-color: rgba(0,0,0,150); border: 1px solid white");
	backgroundImage->setPixmap(QPixmap(":/img/emptybackground.png"));
	backgroundImage->setFixedSize(200,150);
	QString noPath = " " + tr("no image has been selected");
	backgroundImage->setToolTip("<p style=\"background: black\">" + noPath.split(" ").join("/") + "</p>");
	backgroundImage->setCursor(Qt::PointingHandCursor);

	backgroundImgScaleToFit = new CustomCheckBox(tr("Scale To Fit"));
	backgroundImgScaleToFit->setObjectName("scale");
	backgroundImgStretchToFit = new CustomCheckBox(tr("Stretch To Fit"));
	backgroundImgStretchToFit->setObjectName("stretch");
	backgroundImgCenter = new CustomCheckBox(tr("Center Image"));
	backgroundImgCenter->setObjectName("center");

	connect(backgroundImage, SIGNAL(clicked()), this, SLOT(changeBackgroundImage()));
	connect(backgroundImageUseCustom, SIGNAL(toggled(bool)), backgroundImage, SLOT(setEnabled(bool)));
	connect(backgroundImageUseCustom, SIGNAL(toggled(bool)), backgroundImgScaleToFit, SLOT(setEnabled(bool)));
	connect(backgroundImageUseCustom, SIGNAL(toggled(bool)), backgroundImgStretchToFit, SLOT(setEnabled(bool)));
	connect(backgroundImageUseCustom, SIGNAL(toggled(bool)), backgroundImgCenter, SLOT(setEnabled(bool)));
	connect(backgroundImgScaleToFit, SIGNAL(clicked()), this, SLOT(backgroundDispType()));
	connect(backgroundImgStretchToFit, SIGNAL(clicked()), this, SLOT(backgroundDispType()));
	connect(backgroundImgCenter, SIGNAL(clicked()), this, SLOT(backgroundDispType()));

	QVBoxLayout *stretchScaleLay = new QVBoxLayout;
	stretchScaleLay->addStretch();
	stretchScaleLay->addWidget(backgroundImgScaleToFit);
	stretchScaleLay->addWidget(backgroundImgStretchToFit);
	stretchScaleLay->addWidget(backgroundImgCenter);
	stretchScaleLay->addStretch();

	QHBoxLayout *backgroundImageLay = new QHBoxLayout;
	backgroundImageLay->addStretch();
	backgroundImageLay->addWidget(backgroundImage);
	backgroundImageLay->addSpacing(20);
	backgroundImageLay->addLayout(stretchScaleLay);
	backgroundImageLay->addStretch();

	lay->addSpacing(10);
	lay->addLayout(backgroundImageLay);
	lay->addSpacing(20);




	// OPTION TO ADJUST BACKGROUND COLOR
	background = new QColorDialog;
	background->setOption(QColorDialog::ShowAlphaChannel);
	QLabel *backgroundLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Background/Overlay Color") + "</span></b><br><br>" + tr("Here you can adjust the background colour of PhotoQt. When using compositing or a background image, then you can also specify an alpha value, i.e. the transparency of the coloured overlay layer. When neither compositing is enabled nor a background image is set, then this colour will be the background of PhotoQt."));
	backgroundLabel->setWordWrap(true);
	selectCol = new CustomPushButton(tr("Click to change color!"));
	QHBoxLayout *backgroundLay = new QHBoxLayout;
	backgroundLay->addStretch();
	backgroundLay->addWidget(selectCol);
	backgroundLay->addStretch();
	connect(selectCol, SIGNAL(clicked()), background, SLOT(exec()));
	connect(background, SIGNAL(colorSelected(QColor)), this, SLOT(newBgColorSelected(QColor)));
	lay->addWidget(backgroundLabel);
	lay->addSpacing(5);
	lay->addLayout(backgroundLay);
	lay->addSpacing(20);


	// OPTION FOR TRAY ICON USAGE
	trayIcon = new CustomCheckBox(tr("Hide to Tray Icon"));
	QLabel *trayIconLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Hide to Tray Icon") + "</span></b><br><br>" + tr("When started PhotoQt creates a tray icon in the system tray. If desired, you can set PhotoQt to minimise to the tray instead of quitting. This causes PhotoQt to be almost instantaneously available when an image is opened.") + "<br>" + tr("It is also possible to start PhotoQt already minimised to the tray (e.g. at system startup) when called with \"--start-in-tray\"."));
	trayIconLabel->setWordWrap(true);
	QHBoxLayout *trayIconLay = new QHBoxLayout;
	trayIconLay->addStretch();
	trayIconLay->addWidget(trayIcon);
	trayIconLay->addStretch();
	lay->addWidget(trayIconLabel);
	lay->addSpacing(5);
	lay->addLayout(trayIconLay);
	lay->addSpacing(20);



	// OPTION FOR LOOPING THROUGH FOLDER
	loopThroughFolder = new CustomCheckBox(tr("Loop Through Folder"));
	QLabel *loopLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Looping Through Folder") + "</span></b><hr>" + tr("When you load the last image in a directory and select \"Next\", PhotoQt automatically jumps to the first image (and vice versa: if you select \"Previous\" while having the first image loaded, PhotoQt jumps to the last image). Disabling this option makes PhotoQt stop at the first/last image (i.e. selecting \"Next\"/\"Previous\" will have no effect in these two special cases)."));
	loopLabel->setWordWrap(true);
	QHBoxLayout *loopLay = new QHBoxLayout;
	loopLay->addStretch();
	loopLay->addWidget(loopThroughFolder);
	loopLay->addStretch();
	lay->addWidget(loopLabel);
	lay->addSpacing(5);
	lay->addLayout(loopLay);
	lay->addSpacing(20);


	// OPTION FOR TRANSITIONING BETWEEN IMAGES
	transition = new CustomSlider;
	transition->setMinimum(0);
	transition->setMaximum(10);
	transition->setOrientation(Qt::Horizontal);
	transition->setTickPosition(QSlider::TicksBelow);
	transition->setPageStep(1);
	QLabel *transLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Smooth Transition") + "</span></b> " + "<hr>" + tr("Switching between images can be done smoothly, the new image can be set to fade into the old image."));
	transLabel->setWordWrap(true);
	QLabel *noTrans = new QLabel(tr("No Fading"));
	QLabel *longTrans = new QLabel(tr("Long Transition"));
	QHBoxLayout *transLay = new QHBoxLayout;
	transLay->addStretch();
	transLay->addWidget(noTrans);
	transLay->addWidget(transition);
	transLay->addWidget(longTrans);
	transLay->addStretch();
	lay->addWidget(transLabel);
	lay->addSpacing(5);
	lay->addLayout(transLay);
	lay->addSpacing(20);



	// OPTION FOR MENU SENSITIVITY
	QLabel *menuLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Menu Sensitivity") + "</span></b><hr>" + tr("Here you can adjust the sensitivity of the drop-down menu. The menu opens when your mouse cursor gets close to the right side of the upper edge. Here you can adjust how close you need to get for it to open."));
	menuLabel->setWordWrap(true);
	QHBoxLayout *menuLay = new QHBoxLayout;
	QLabel *menuLabelLeft = new QLabel(tr("Low Sensitivity"));
	QLabel *menuLabelRight = new QLabel(tr("High Sensitivity"));
	menu = new CustomSlider;
	menu->setMinimum(1);
	menu->setMaximum(10);
	menu->setPageStep(1);
	menuLay->addStretch();
	menuLay->addWidget(menuLabelLeft);
	menuLay->addWidget(menu);
	menuLay->addWidget(menuLabelRight);
	menuLay->addStretch();
	lay->addWidget(menuLabel);
	lay->addSpacing(5);
	lay->addLayout(menuLay);
	lay->addSpacing(20);



	// OPTION FOR CLOSING ON CLICK ON GREY
	grey = new CustomCheckBox(tr("Close on Click in empty area"));
	QLabel *greyLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Close on Click in empty area") + "</span></b><hr>" + tr("This option makes PhotoQt a bit like the JavaScript image viewers you find on many websites. A click outside of the image on the empty background will close the application. It can be a nice feature, PhotoQt will feel even more like a \"floating layer\". However, you might at times close PhotoQt accidentally.") + "<br><br>" + tr("Note: If you use a mouse click for a shortcut already, then this option wont have any effect!"));
	greyLabel->setWordWrap(true);
	QHBoxLayout *greyLay = new QHBoxLayout;
	greyLay->addStretch();
	greyLay->addWidget(grey);
	greyLay->addStretch();
	lay->addWidget(greyLabel);
	lay->addSpacing(5);
	lay->addLayout(greyLay);
	lay->addSpacing(20);



	// OPTION FOR ADJUSTING BORDER AROUND MAIN IMAGE
	borderAroundImgSlider = new CustomSlider;
	borderAroundImgSlider->setMinimum(0);
	borderAroundImgSlider->setMaximum(50);
	borderAroundImgSlider->setTickInterval(5);
	borderAroundImgSpinBox = new CustomSpinBox;
	borderAroundImgSpinBox->setMinimum(0);
	borderAroundImgSpinBox->setMaximum(50);
	borderAroundImgSpinBox->setSuffix(" px");
	QLabel *borderLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Border Around Image") + "</span></b><br><hr>" + tr("Whenever you load an image, the image is per default not shown completely in fullscreen, i.e. it's not stretching from screen edge to screen edge. Instead there is a small margin around the image of a couple pixels (for optical reasons). Here you can adjust the width of this border or disable it altogether. Note, that this only works when the thumbnails are floating."));
	borderLabel->setWordWrap(true);
	QHBoxLayout *borderLay = new QHBoxLayout;
	borderLay->addStretch();
	borderLay->addWidget(borderAroundImgSlider);
	borderLay->addWidget(borderAroundImgSpinBox);
	borderLay->addStretch();
	lay->addWidget(borderLabel);
	lay->addSpacing(5);
	lay->addLayout(borderLay);
	lay->addSpacing(20);
	connect(borderAroundImgSlider, SIGNAL(valueChanged(int)), borderAroundImgSpinBox, SLOT(setValue(int)));
	connect(borderAroundImgSpinBox, SIGNAL(valueChanged(int)), borderAroundImgSlider, SLOT(setValue(int)));



	// OPTION FOR HIDING SOME/ALL QUICKINFO
	QLabel *hideQuickInfoLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Hide Quickinfo (Text Labels)") + "</span></b><hr>" + tr("Here you can hide the text labels shown in the main area: The Counter in the top left corner, the file path/name following the counter, and the \"X\" displayed in the top right corner. The labels can also be hidden by simply right-clicking on them and selecting \"Hide\"."));
	hideQuickInfoLabel->setWordWrap(true);
	QHBoxLayout *hideQuickInfoLay = new QHBoxLayout;
	QVBoxLayout *hideQuickInfoColLay = new QVBoxLayout;
	hideCounter = new CustomCheckBox(tr("Hide Counter"));
	hideFilePATH = new CustomCheckBox(tr("Hide Filepath (Shows only file name)"));
	hideFilename = new CustomCheckBox(tr("Hide Filename (Including file path)"));
	hideX = new CustomCheckBox(tr("Hide \"X\" (Closing)"));
	hideQuickInfoColLay->addWidget(hideCounter);
	hideQuickInfoColLay->addWidget(hideFilePATH);
	hideQuickInfoColLay->addWidget(hideFilename);
	hideQuickInfoColLay->addWidget(hideX);
	hideQuickInfoLay->addStretch();
	hideQuickInfoLay->addLayout(hideQuickInfoColLay);
	hideQuickInfoLay->addStretch();
	lay->addWidget(hideQuickInfoLabel);
	lay->addSpacing(5);
	lay->addLayout(hideQuickInfoLay);
	lay->addSpacing(20);


	lay->addStretch();

}

// Load the settings
void SettingsTabLookAndFeel::loadSettings() {

	if(verbose) qDebug() << "Load Settings (Look And Feel)";

	defaults.clear();


	compositeBackground->setChecked(globSet.value("Composite").toBool());
	defaults.insert("Composite",globSet.value("Composite").toBool());
	backgroundImageUseScreenshot->setChecked(globSet.value("BackgroundImageScreenshot").toBool());
	defaults.insert("BackgroundImageScreenshot",globSet.value("BackgroundImageScreenshot").toBool());
	backgroundImageUseCustom->setChecked(globSet.value("BackgroundImageUse").toBool());
	defaults.insert("BackgroundImageUse",globSet.value("BackgroundImageUse").toBool());

	if(!globSet.value("Composite").toBool() && !globSet.value("BackgroundImageScreenshot").toBool() && !globSet.value("BackgroundImageUse").toBool()) {
		noBackgroundImage->setChecked(true);
		defaults.insert("NoBackgroundImage",true);
	} else {
		noBackgroundImage->setChecked(false);
		defaults.insert("NoBackgroundImage",false);
	}


	if(globSet.value("BackgroundImagePath").toString() == "") {
		setBackgroundImage(":/img/emptybackground.png",true);
		defaults.insert("BackgroundImagePath",backgroundImage->toolTip().remove("<p style=\"background: black\">").remove("</p>"));
	} else {
		setBackgroundImage(globSet.value("BackgroundImagePath").toString());
		defaults.insert("BackgroundImagePath",globSet.value("BackgroundImagePath").toString());
	}
	backgroundImgScaleToFit->setChecked(globSet.value("BackgroundImageScale").toBool());
	defaults.insert("BackgroundImageScale",globSet.value("BackgroundImageScale").toBool());
	backgroundImgStretchToFit->setChecked(globSet.value("BackgroundImageStretch").toBool());
	defaults.insert("BackgroundImageStretch",globSet.value("BackgroundImageStretch").toBool());
	backgroundImgCenter->setChecked(globSet.value("BackgroundImageCenter").toBool());
	defaults.insert("BackgroundImageCenter",globSet.value("BackgroundImageCenter").toBool());

	if(backgroundImageUseCustom->isChecked()) {
		backgroundImage->setEnabled(true);
		backgroundImgScaleToFit->setEnabled(true);
		backgroundImgStretchToFit->setEnabled(true);
		backgroundImgCenter->setEnabled(true);
	} else {
		backgroundImage->setEnabled(false);
		backgroundImgScaleToFit->setEnabled(false);
		backgroundImgStretchToFit->setEnabled(false);
		backgroundImgCenter->setEnabled(false);
	}


	background->setOption(QColorDialog::ShowAlphaChannel);
	int r = globSet.value("BgColorRed").toInt();
	int g = globSet.value("BgColorGreen").toInt();
	int b = globSet.value("BgColorBlue").toInt();
	int a = globSet.value("BgColorAlpha").toInt();
	background->setCurrentColor(QColor::fromRgba(qRgba(r,g,b,a)));
	defaults.insert("BgColorRed",globSet.value("BgColorRed").toInt());
	defaults.insert("BgColorGreen",globSet.value("BgColorGreen").toInt());
	defaults.insert("BgColorBlue",globSet.value("BgColorBlue").toInt());
	defaults.insert("BgColorAlpha",globSet.value("BgColorAlpha").toInt());

	selectCol->setStyleSheet(QString("background: rgba(%1,%2,%3,%4); border-radius: 10px; padding: 25px; border: 1px solid white;").arg(r).arg(g).arg(b).arg(a));

	trayIcon->setChecked(globSet.value("TrayIcon").toBool());
	defaults.insert("TrayIcon",globSet.value("TrayIcon").toBool());

	loopThroughFolder->setChecked(globSet.value("LoopThroughFolder").toBool());
	defaults.insert("LoopThroughFolder",globSet.value("LoopThroughFolder").toBool());

	transition->setValue(globSet.value("Transition").toInt());
	defaults.insert("Transition",globSet.value("Transition").toInt());

	borderAroundImgSlider->setValue(globSet.value("BorderAroundImg").toInt());
	defaults.insert("BorderAroundImg",globSet.value("BorderAroundImg").toInt());

	menu->setValue(globSet.value("MenuSensitivity").toInt());
	defaults.insert("MenuSensitivity",globSet.value("MenuSensitivity").toInt());

	grey->setChecked(globSet.value("CloseOnGrey").toBool());
	defaults.insert("CloseOnGrey",globSet.value("CloseOnGrey").toBool());

	hideCounter->setChecked(globSet.value("HideCounter").toBool());
	hideFilePATH->setChecked(globSet.value("HideFilepathShowFilename").toBool());
	hideFilename->setChecked(globSet.value("HideFilename").toBool());
	hideX->setChecked(globSet.value("HideX").toBool());
	defaults.insert("HideCounter",globSet.value("HideCounter").toBool());
	defaults.insert("HideFilepathShowFilename",globSet.value("HideFilepathShowFilename").toBool());
	defaults.insert("HideFilename",globSet.value("HideFilename").toBool());
	defaults.insert("HideX",globSet.value("HideX").toBool());


}

// Save all settings
void SettingsTabLookAndFeel::saveSettings() {

	if(verbose) qDebug() << "Save Settings (Look and Feel)";

	updatedSet.clear();

	if(defaults.value("Composite").toBool() != compositeBackground->isChecked()) {
		updatedSet.insert("Composite",compositeBackground->isChecked());
		defaults.remove("Composite");
		defaults.insert("Composite",compositeBackground->isChecked());
	}

	if(defaults.value("BgColorRed").toInt() != background->currentColor().red()
			|| defaults.value("BgColorGreen").toInt() != background->currentColor().green()
			|| defaults.value("BgColorBlue").toInt() != background->currentColor().blue()
			|| defaults.value("BgColorAlpha").toInt() != background->currentColor().alpha()) {
		updatedSet.insert("BgColorRed",background->currentColor().red());
		updatedSet.insert("BgColorGreen",background->currentColor().green());
		updatedSet.insert("BgColorBlue",background->currentColor().blue());
		updatedSet.insert("BgColorAlpha",background->currentColor().alpha());
		defaults.remove("BgColorRed");
		defaults.remove("BgColorGreen");
		defaults.remove("BgColorBlue");
		defaults.remove("BgColorAlpha");
		defaults.insert("BgColorRed",background->currentColor().red());
		defaults.insert("BgColorGreen",background->currentColor().green());
		defaults.insert("BgColorBlue",background->currentColor().blue());
		defaults.insert("BgColorAlpha",background->currentColor().alpha());
	}

	if(defaults.value("BackgroundImageScreenshot").toBool() != backgroundImageUseScreenshot->isChecked()) {
		updatedSet.insert("BackgroundImageScreenshot",backgroundImageUseScreenshot->isChecked());
		defaults.remove("BackgroundImageScreenshot");
		defaults.insert("BackgroundImageScreenshot",backgroundImageUseScreenshot->isChecked());
	}

	if(defaults.value("BackgroundImageUse").toBool() != backgroundImageUseCustom->isChecked()) {
		updatedSet.insert("BackgroundImageUse",backgroundImageUseCustom->isChecked());
		defaults.remove("BackgroundImageUse");
		defaults.insert("BackgroundImageUse",backgroundImageUseCustom->isChecked());
	}

	QString path = backgroundImage->toolTip().remove("<p style=\"background: black\">").remove("</p>");
	if(defaults.value("BackgroundImagePath").toString() != path) {
		updatedSet.insert("BackgroundImagePath",path);
		defaults.remove("BackgroundImagePath");
		defaults.insert("BackgroundImagePath",path);
	}

	if(defaults.value("BackgroundImageScale").toBool() != backgroundImgScaleToFit->isChecked()) {
		updatedSet.insert("BackgroundImageScale",backgroundImgScaleToFit->isChecked());
		defaults.remove("BackgroundImageScale");
		defaults.insert("BackgroundImageScale",backgroundImgScaleToFit->isChecked());
	}

	if(defaults.value("BackgroundImageStretch").toBool() != backgroundImgStretchToFit->isChecked()) {
		updatedSet.insert("BackgroundImageStretch",backgroundImgStretchToFit->isChecked());
		defaults.remove("BackgroundImageStretch");
		defaults.insert("BackgroundImageStretch",backgroundImgStretchToFit->isChecked());
	}

	if(defaults.value("BackgroundImageCenter").toBool() != backgroundImgCenter->isChecked()) {
		updatedSet.insert("BackgroundImageCenter",backgroundImgCenter->isChecked());
		defaults.remove("BackgroundImageCenter");
		defaults.insert("BackgroundImageCenter",backgroundImgCenter->isChecked());
	}



	if(defaults.value("TrayIcon").toBool() != trayIcon->isChecked()) {
		updatedSet.insert("TrayIcon",trayIcon->isChecked());
		defaults.remove("TrayIcon");
		defaults.insert("TrayIcon",trayIcon->isChecked());
	}

	if(defaults.value("LoopThroughFolder").toBool() != loopThroughFolder->isChecked()) {
		updatedSet.insert("LoopThroughFolder",loopThroughFolder->isChecked());
		defaults.remove("LoopThroughFolder");
		defaults.insert("LoopThroughFolder",loopThroughFolder->isChecked());
	}

	if(defaults.value("Transition").toInt() != transition->value()) {
		updatedSet.insert("Transition",transition->value());
		defaults.remove("Transition");
		defaults.insert("Transition",transition->value());
	}

	if(defaults.value("BorderAroundImg").toInt() != borderAroundImgSlider->value()) {
		updatedSet.insert("BorderAroundImg",borderAroundImgSlider->value());
		defaults.remove("BorderAroundImg");
		defaults.insert("BorderAroundImg",borderAroundImgSlider->value());
	}

	if(defaults.value("MenuSensitivity").toInt() != menu->value()) {
		updatedSet.insert("MenuSensitivity",menu->value());
		defaults.remove("MenuSensitivity");
		defaults.insert("MenuSensitivity",menu->value());
	}

	if(defaults.value("CloseOnGrey") != grey->isChecked()) {
		updatedSet.insert("CloseOnGrey",grey->isChecked());
		defaults.remove("CloseOnGrey");
		defaults.insert("CloseOnGrey",grey->isChecked());
	}

	if(defaults.value("HideCounter").toBool() != hideCounter->isChecked()) {
		updatedSet.insert("HideCounter",hideCounter->isChecked());
		defaults.remove("HideCounter");
		defaults.insert("HideCounter",hideCounter->isChecked());
	}
	if(defaults.value("HideFilepathShowFilename").toBool() != hideFilePATH->isChecked()) {
		updatedSet.insert("HideFilepathShowFilename",hideFilePATH->isChecked());
		defaults.remove("HideFilepathShowFilename");
		defaults.insert("HideFilepathShowFilename",hideFilePATH->isChecked());
	}
	if(defaults.value("HideFilename").toBool() != hideFilename->isChecked()) {
		updatedSet.insert("HideFilename",hideFilename->isChecked());
		defaults.remove("HideFilename");
		defaults.insert("HideFilename",hideFilename->isChecked());
	}
	if(defaults.value("HideX").toBool() != hideX->isChecked()) {
		updatedSet.insert("HideX",hideX->isChecked());
		defaults.remove("HideX");
		defaults.insert("HideX",hideX->isChecked());
	}

}

// Set a background image into the preview square
void SettingsTabLookAndFeel::setBackgroundImage(QString path, bool empty) {

	if(verbose) qDebug() << "lnf: Set background image:" << path << "-" << empty;

	// Empty background
	if(empty) {
		backgroundImage->setToolTip("<p style=\"background: black\">" + QString(" " + tr("no image has been selected")).split(" ").join("/") + "</p>");
		backgroundImage->setPixmap(QPixmap(":/img/emptybackground.png"));

	// Non-empty background
	} else {

		// The path is set as tooltip
		backgroundImage->setToolTip("<p style=\"background: black\">" + path + "</p>");
		QImageReader reader(path);

		// The original image sizes
		int iw = reader.size().width();
		int ih = reader.size().height();

		// Stretch Image to fit screen dimensions
		if(backgroundImgStretchToFit->isChecked()) {
			if(verbose) qDebug() << "lnf: Set background image: StretchToFit";
			backgroundImage->setAlignment(Qt::AlignLeft);
			backgroundImage->setAlignment(Qt::AlignTop);
			iw = 200;
			ih = 150;
		// Scale image to fit width of screen
		} else if(backgroundImgScaleToFit->isChecked()) {
			if(verbose) qDebug() << "lnf: Set background image: ScaleToFit";
			backgroundImage->setAlignment(Qt::AlignLeft);
			backgroundImage->setAlignment(Qt::AlignTop);
			if(iw != 200) {
				float f = 200.0/iw;
				iw *= f;
				ih *= f;
				if(ih*f > 150)
					reader.setClipRect(QRect(0,0,reader.size().width(),150/f));
			}
		// Center image
		} else {
			if(verbose) qDebug() << "lnf: Set background image: Center";
			if(backgroundImgCenter->isChecked())
				backgroundImage->setAlignment(Qt::AlignCenter);
			else {
				backgroundImage->setAlignment(Qt::AlignLeft);
				backgroundImage->setAlignment(Qt::AlignTop);
			}
			float w = 200.0;
			float h = 150.0;
			if(iw != w) {
				float f = w/iw;
				iw *= f;
				ih *= f;
			}
			if(ih > h) {
				float f = h/ih;
				iw *= f;
				ih *= f;
			}
		}
		reader.setScaledSize(QSize(iw,ih));
		backgroundImage->setPixmap(QPixmap::fromImage(reader.read()));
	}

	backgroundImage->setCursor(Qt::PointingHandCursor);

}

// A new background color selected
void SettingsTabLookAndFeel::newBgColorSelected(QColor col) {

	if(verbose) qDebug() << "lnf: New background color selected:" << col;

	selectCol->setStyleSheet(QString("background: rgba(%1,%2,%3,%4); border-radius: 10px; padding: 25px; border: 1px solid white;").arg(col.red()).arg(col.green()).arg(col.blue()).arg(col.alpha()));

}

// Browse for a new background image
void SettingsTabLookAndFeel::changeBackgroundImage() {

	if(verbose) qDebug() << "lnf: Change background image.";

	QString dir = QDir::homePath();

	QString noPath = " " + tr("no image has been selected");
	if(backgroundImage->toolTip() != "<p style=\"background: black\">" + noPath.split(" ").join("/") + "</p>")
		dir = backgroundImage->toolTip().remove("<p style=\"background: black\">").remove("</p>");

	QString newBG = QFileDialog::getOpenFileName(0,tr("Select Background Image"),dir,globSet.value("KnownFileTypes").toString());

	if(newBG != "") {

		setBackgroundImage(newBG);

	}

}

// The checkboxes are mutually exclusive
void SettingsTabLookAndFeel::backgroundDispType() {

	QString objName = ((CustomCheckBox *)sender())->objectName();

	if(backgroundImgScaleToFit->isChecked() && objName == "scale") {
		if(verbose) qDebug() << "lnf: Set background image: scale";
		backgroundImgStretchToFit->setChecked(false);
		backgroundImgCenter->setChecked(false);
	}

	if(backgroundImgStretchToFit->isChecked() && objName == "stretch") {
		if(verbose) qDebug() << "lnf: Set background image: stretch";
		backgroundImgScaleToFit->setChecked(false);
		backgroundImgCenter->setChecked(false);
	}

	if(backgroundImgCenter->isChecked() && objName == "center") {
		if(verbose) qDebug() << "lnf: Set background image: center";
		backgroundImgScaleToFit->setChecked(false);
		backgroundImgStretchToFit->setChecked(false);
	}

	setBackgroundImage(backgroundImage->toolTip().remove("<p style=\"background: black\">").remove("</p>"));

}

void SettingsTabLookAndFeel::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabLookAndFeel::~SettingsTabLookAndFeel() { }



// A label that emits a clicked() signal
ClickLabel::ClickLabel(const QString &text, QWidget *parent) : QLabel(text, parent) {

	css = "QLabel {color: white; } QLabel:disabled { color: grey; }";

	this->setStyleSheet(css);

}

void ClickLabel::setEnabled(bool e) {

	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect;

	if(e)
		eff->setOpacity(1);
	else
		eff->setOpacity(0.3);

	this->setGraphicsEffect(eff);


	QLabel::setEnabled(e);

}

void ClickLabel::setCSS(QString c) {

	css = css;
	if(this->isEnabled())
		this->setStyleSheet(css + "QLabel { background: rgba(255,255,255,200); }");
	else
		this->setStyleSheet(css + "QLabel { background: transparent; }");

}

void ClickLabel::mouseReleaseEvent(QMouseEvent*) {

	emit clicked();

}

ClickLabel::~ClickLabel() { }
