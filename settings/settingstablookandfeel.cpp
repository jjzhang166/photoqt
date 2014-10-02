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

#include "settingstablookandfeel.h"
#include <iostream>

SettingsTabLookAndFeel::SettingsTabLookAndFeel(QWidget *parent, QMap<QString, QVariant> set, bool v) : QWidget(parent) {

	// The global settings
	globSet = set;

	verbose = v;

	// Style widget
	this->setObjectName("tablookfeel");
	this->setStyleSheet("#tablookfeel { background: transparent; color: white; }");

	tabs = new TabWidget;
	tabs->expand(false);
	tabs->setBorderTop("rgba(150,150,150,100)",2);
	tabs->setBorderBot("rgba(150,150,150,100)",2);

	QVBoxLayout *mainLay = new QVBoxLayout;
	mainLay->addWidget(tabs);
	this->setLayout(mainLay);

	// the main scroll widget for all LOOK content
	scrollbarLook = new CustomScrollbar;
	QScrollArea *scrollLook = new QScrollArea;
	QVBoxLayout *layLook = new QVBoxLayout(scrollLook);
	QWidget *scrollWidgLook = new QWidget(scrollLook);
	scrollWidgLook->setLayout(layLook);
	scrollLook->setWidget(scrollWidgLook);
	scrollLook->setWidgetResizable(true);
	scrollLook->setVerticalScrollBar(scrollbarLook);

	// the main scroll widget for all FEEL content
	scrollbarFeel = new CustomScrollbar;
	QScrollArea *scrollFeel = new QScrollArea;
	QVBoxLayout *layFeel = new QVBoxLayout(scrollFeel);
	QWidget *scrollWidgFeel = new QWidget(scrollFeel);
	scrollWidgFeel->setLayout(layFeel);
	scrollFeel->setWidget(scrollWidgFeel);
	scrollFeel->setWidgetResizable(true);
	scrollFeel->setVerticalScrollBar(scrollbarFeel);

	tabLook = new QWidget;
	tabFeel = new QWidget;

	QVBoxLayout *scrollLayLook = new QVBoxLayout;
	scrollLayLook->addWidget(scrollLook);
	tabLook->setLayout(scrollLayLook);

	QVBoxLayout *scrollLayFeel = new QVBoxLayout;
	scrollLayFeel->addWidget(scrollFeel);
	tabFeel->setLayout(scrollLayFeel);

	tabs->addTab(tabLook,tr("Look"));
	tabs->addTab(tabFeel,tr("Behaviour"));



	// The titles
	CustomLabel *titleLook = new CustomLabel("<center><h1>" + tr("Overall Look") + "</h1></center>");
	layLook->addWidget(titleLook);
	layLook->addSpacing(20);
	CustomLabel *titleFeel = new CustomLabel("<center><h1>" + tr("Behaviour of PhotoQt") + "</h1></center>");
	layFeel->addWidget(titleFeel);
	layFeel->addSpacing(20);


	// OPTION FOR COMPOSITE
	compositeBackground = new CustomRadioButton(tr("Use (half-)transparent background"));
	backgroundImageUseScreenshot = new CustomRadioButton(tr("Use faked transparency"));
	backgroundImageUseCustom = new CustomRadioButton(tr("Use custom background image"));
	noBackgroundImage = new CustomRadioButton(tr("Use one-colored, non-transparent background"));

	QString compTxt = tr("The background of PhotoQt is the part, that is not covered by an image. It can be made either real (half-)transparent (using a compositor), or faked transparent (instead of the actual desktop a screenshot of it is shown), or a custom background image can be set, or none of the above.");
	compTxt += "<br>";
	compTxt += tr("Note: Fake transparency currently only really works when PhotoQt is run in fullscreen/maximised!");
	CustomLabel *compositeImageLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Background of PhotoQt") + "</span></b><br><br>" + compTxt);
	compositeImageLabel->setWordWrap(true);

	QHBoxLayout *compLay1 = new QHBoxLayout;
	compLay1->addStretch();
	compLay1->addWidget(compositeBackground);
	compLay1->addWidget(backgroundImageUseScreenshot);
	compLay1->addWidget(backgroundImageUseCustom);
	compLay1->addStretch();

	QHBoxLayout *compLay2 = new QHBoxLayout;
	compLay2->addStretch();
	compLay2->addWidget(noBackgroundImage);
	compLay2->addStretch();

	layLook->addWidget(compositeImageLabel);
	layLook->addSpacing(10);
	layLook->addLayout(compLay1);
	layLook->addLayout(compLay2);

	// OPTION TO SET BACKGROUND IMAGE
	QWidget *widgetChooseBgImg = new QWidget;

	backgroundImage = new CustomLabel;
	backgroundImage->setBorder(1,"white");
	backgroundImage->setPixmap(QPixmap(":/img/emptybackground.png"));
	backgroundImage->setFixedSize(200,150);
	QString noPath = " " + tr("no image has been selected");
	backgroundImage->setToolTip(noPath.split(" ").join("/"));
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

	QVBoxLayout *widgetChooseBgImgLay = new QVBoxLayout;
	widgetChooseBgImgLay->addSpacing(10);
	widgetChooseBgImgLay->addLayout(backgroundImageLay);
	widgetChooseBgImgLay->addSpacing(20);
	widgetChooseBgImg->setLayout(widgetChooseBgImgLay);
	layLook->addWidget(widgetChooseBgImg);
	connect(backgroundImageUseCustom, SIGNAL(toggled(bool)), widgetChooseBgImg, SLOT(setVisible(bool)));
	widgetChooseBgImg->hide();

	layLook->addSpacing(20);


	// OPTION TO ADJUST BACKGROUND COLOR
	background = new QColorDialog;
	background->setOption(QColorDialog::ShowAlphaChannel);
	CustomLabel *backgroundLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Background/Overlay Color") + "</span></b><br><br>" + tr("Here you can adjust the background colour of PhotoQt (of the part not covered by an image). When using compositing or a background image, then you can also specify an alpha value, i.e. the transparency of the coloured overlay layer. When neither compositing is enabled nor a background image is set, then this colour will be the non-transparent background of PhotoQt."));
	backgroundLabel->setWordWrap(true);
	selectCol = new CustomPushButton(tr("Click to change color!"));
	selectCol->setPadding(25);
	QHBoxLayout *backgroundLay = new QHBoxLayout;
	backgroundLay->addStretch();
	backgroundLay->addWidget(selectCol);
	backgroundLay->addStretch();
	connect(selectCol, SIGNAL(clicked()), background, SLOT(exec()));
	connect(background, SIGNAL(colorSelected(QColor)), this, SLOT(newBgColorSelected(QColor)));
	layLook->addWidget(backgroundLabel);
	layLook->addSpacing(5);
	layLook->addLayout(backgroundLay);
	layLook->addSpacing(20);


	// OPTION FOR TRAY ICON USAGE
	trayIcon = new CustomCheckBox(tr("Hide to Tray Icon"));
	CustomLabel *trayIconLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Hide to Tray Icon") + "</span></b><br><br>" + tr("When started PhotoQt creates a tray icon in the system tray. If desired, you can set PhotoQt to minimise to the tray instead of quitting. This causes PhotoQt to be almost instantaneously available when an image is opened.") + "<br>" + tr("It is also possible to start PhotoQt already minimised to the tray (e.g. at system startup) when called with \"--start-in-tray\"."));
	trayIconLabel->setWordWrap(true);
	QHBoxLayout *trayIconLay = new QHBoxLayout;
	trayIconLay->addStretch();
	trayIconLay->addWidget(trayIcon);
	trayIconLay->addStretch();
	layFeel->addWidget(trayIconLabel);
	layFeel->addSpacing(5);
	layFeel->addLayout(trayIconLay);
	layFeel->addSpacing(20);



	// OPTION FOR LOOPING THROUGH FOLDER
	loopThroughFolder = new CustomCheckBox(tr("Loop Through Folder"));
	CustomLabel *loopLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Looping Through Folder") + "</span></b><hr>" + tr("When you load the last image in a directory and select \"Next\", PhotoQt automatically jumps to the first image (and vice versa: if you select \"Previous\" while having the first image loaded, PhotoQt jumps to the last image). Disabling this option makes PhotoQt stop at the first/last image (i.e. selecting \"Next\"/\"Previous\" will have no effect in these two special cases)."));
	loopLabel->setWordWrap(true);
	QHBoxLayout *loopLay = new QHBoxLayout;
	loopLay->addStretch();
	loopLay->addWidget(loopThroughFolder);
	loopLay->addStretch();
	layFeel->addWidget(loopLabel);
	layFeel->addSpacing(5);
	layFeel->addLayout(loopLay);
	layFeel->addSpacing(20);


	// OPTION FOR TRANSITIONING BETWEEN IMAGES
	transition = new CustomSlider;
	transition->setMinimum(0);
	transition->setMaximum(10);
	transition->setOrientation(Qt::Horizontal);
	transition->setTickPosition(QSlider::TicksBelow);
	transition->setPageStep(1);
	CustomLabel *transLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Smooth Transition") + "</span></b> " + "<hr>" + tr("Switching between images can be done smoothly, the new image can be set to fade into the old image."));
	transLabel->setWordWrap(true);
	CustomLabel *noTrans = new CustomLabel(tr("No Fading"));
	CustomLabel *longTrans = new CustomLabel(tr("Long Transition"));
	QHBoxLayout *transLay = new QHBoxLayout;
	transLay->addStretch();
	transLay->addWidget(noTrans);
	transLay->addWidget(transition);
	transLay->addWidget(longTrans);
	transLay->addStretch();
	layFeel->addWidget(transLabel);
	layFeel->addSpacing(5);
	layFeel->addLayout(transLay);
	layFeel->addSpacing(20);




	// OPTION FOR MENU SENSITIVITY
	CustomLabel *menuLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Menu Sensitivity") + "</span></b><hr>" + tr("Here you can adjust the sensitivity of the drop-down menu. The menu opens when your mouse cursor gets close to the right side of the upper edge. Here you can adjust how close you need to get for it to open."));
	menuLabel->setWordWrap(true);
	QHBoxLayout *menuLay = new QHBoxLayout;
	CustomLabel *menuLabelLeft = new CustomLabel(tr("Low Sensitivity"));
	CustomLabel *menuLabelRight = new CustomLabel(tr("High Sensitivity"));
	menu = new CustomSlider;
	menu->setMinimum(1);
	menu->setMaximum(10);
	menu->setPageStep(1);
	menuLay->addStretch();
	menuLay->addWidget(menuLabelLeft);
	menuLay->addWidget(menu);
	menuLay->addWidget(menuLabelRight);
	menuLay->addStretch();
	layFeel->addWidget(menuLabel);
	layFeel->addSpacing(5);
	layFeel->addLayout(menuLay);
	layFeel->addSpacing(20);



	// Adjust window mode
	CustomLabel *windowModeLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Window Mode") + "</span></b><br><br>" + tr("PhotoQt is designed with the space of a fullscreen app in mind. That's why it by default runs as fullscreen. However, some might prefer to have it as a normal window, e.g. so that they can see the panel.") + "<br><b></b>");
	windowModeLabel->setWordWrap(true);
	windowMode = new CustomCheckBox(tr("Run PhotoQt in Window Mode"));
	windowDeco = new CustomCheckBox(tr("Show Window Decoration"));
	windowDeco->setEnabled(false);
	QHBoxLayout *windowLay = new QHBoxLayout;
	windowLay->addStretch();
	windowLay->addWidget(windowMode);
	windowLay->addSpacing(10);
	windowLay->addWidget(windowDeco);
	windowLay->addStretch();
	layFeel->addWidget(windowModeLabel);
	layFeel->addSpacing(10);
	layFeel->addLayout(windowLay);
	layFeel->addSpacing(20);
	connect(windowMode, SIGNAL(toggled(bool)), windowDeco, SLOT(setEnabled(bool)));



	// OPTION FOR CLOSING ON CLICK ON GREY
	grey = new CustomCheckBox(tr("Close on Click in empty area"));
	CustomLabel *greyLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Close on Click in empty area") + "</span></b><hr>" + tr("This option makes PhotoQt behave a bit like the JavaScript image viewers you find on many websites. A click outside of the image on the empty background will close the application. It can be a nice feature, PhotoQt will feel even more like a \"floating layer\". However, you might at times close PhotoQt accidentally.") + "<br><br>" + tr("Note: If you use a mouse click for a shortcut already, then this option wont have any effect!"));
	greyLabel->setWordWrap(true);
	QHBoxLayout *greyLay = new QHBoxLayout;
	greyLay->addStretch();
	greyLay->addWidget(grey);
	greyLay->addStretch();
	layFeel->addWidget(greyLabel);
	layFeel->addSpacing(5);
	layFeel->addLayout(greyLay);
	layFeel->addSpacing(20);



	// OPTION FOR ADJUSTING BORDER AROUND MAIN IMAGE
	borderAroundImgSlider = new CustomSlider;
	borderAroundImgSlider->setMinimum(0);
	borderAroundImgSlider->setMaximum(50);
	borderAroundImgSlider->setTickInterval(5);
	borderAroundImgSpinBox = new CustomSpinBox;
	borderAroundImgSpinBox->setMinimum(0);
	borderAroundImgSpinBox->setMaximum(50);
	borderAroundImgSpinBox->setSuffix(" px");
	CustomLabel *borderLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Border Around Image") + "</span></b><br><hr>" + tr("Whenever you load an image, the image is per default not shown completely in fullscreen, i.e. it's not stretching from screen edge to screen edge. Instead there is a small margin around the image of a couple pixels (looks better). Here you can adjust the width of this margin (set to 0 to disable it)."));
	borderLabel->setWordWrap(true);
	QHBoxLayout *borderLay = new QHBoxLayout;
	borderLay->addStretch();
	borderLay->addWidget(borderAroundImgSlider);
	borderLay->addWidget(borderAroundImgSpinBox);
	borderLay->addStretch();
	connect(borderAroundImgSlider, SIGNAL(valueChanged(int)), borderAroundImgSpinBox, SLOT(setValue(int)));
	connect(borderAroundImgSpinBox, SIGNAL(valueChanged(int)), borderAroundImgSlider, SLOT(setValue(int)));
	layLook->addWidget(borderLabel);
	layLook->addSpacing(5);
	layLook->addLayout(borderLay);
	layLook->addSpacing(20);



	// OPTION FOR HIDING SOME/ALL QUICKINFO
	CustomLabel *hideQuickInfoLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Hide Quickinfo (Text Labels)") + "</span></b><hr>" + tr("Here you can hide the text labels shown in the main area: The Counter in the top left corner, the file path/name following the counter, and the \"X\" displayed in the top right corner. The labels can also be hidden by simply right-clicking on them and selecting \"Hide\"."));
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
	layLook->addWidget(hideQuickInfoLabel);
	layLook->addSpacing(5);
	layLook->addLayout(hideQuickInfoLay);
	layLook->addSpacing(20);


	layLook->addStretch();
	layFeel->addStretch();

}

// Load the settings
void SettingsTabLookAndFeel::loadSettings() {

	if(verbose) std::clog << "Load Settings (Look And Feel)" << std::endl;

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
	selectCol->setRGBA(r,g,b,a);

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

	windowMode->setChecked(globSet.value("WindowMode").toBool());
	defaults.insert("WindowMode",globSet.value("WindowMode").toBool());

	windowDeco->setChecked(globSet.value("WindowDecoration").toBool());
	defaults.insert("WindowDecoration",globSet.value("WindowDecoration").toBool());

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

	if(verbose) std::clog << "Save Settings (Look and Feel)" << std::endl;

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

	if(defaults.value("WindowMode").toBool() != windowMode->isChecked()) {
		updatedSet.insert("WindowMode",windowMode->isChecked());
		defaults.remove("WindowMode");
		defaults.insert("WindowMode",windowMode->isChecked());
	}

	if(defaults.value("WindowDecoration").toBool() != windowDeco->isChecked()) {
		updatedSet.insert("WindowDecoration",windowDeco->isChecked());
		defaults.remove("WindowDecoration");
		defaults.insert("WindowDecoration",windowDeco->isChecked());
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

	if(verbose) std::clog << "lnf: Set background image: " << path.toStdString() << " - " << empty << std::endl;

	// Empty background
	if(empty) {
		backgroundImage->setToolTip(QString(" " + tr("no image has been selected")).split(" ").join("/"));
		backgroundImage->setPixmap(QPixmap(":/img/emptybackground.png"));

	// Non-empty background
	} else {

		// The path is set as tooltip
		backgroundImage->setToolTip(path);
		QImageReader reader(path);

		// The original image sizes
		int iw = reader.size().width();
		int ih = reader.size().height();

		// Stretch Image to fit screen dimensions
		if(backgroundImgStretchToFit->isChecked()) {
			if(verbose) std::clog << "lnf: Set background image: StretchToFit" << std::endl;
			backgroundImage->setAlignment(Qt::AlignLeft);
			backgroundImage->setAlignment(Qt::AlignTop);
			iw = 200;
			ih = 150;
		// Scale image to fit width of screen
		} else if(backgroundImgScaleToFit->isChecked()) {
			if(verbose) std::clog << "lnf: Set background image: ScaleToFit" << std::endl;
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
			if(verbose) std::clog << "lnf: Set background image: Center" << std::endl;
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

	if(verbose) std::clog << "lnf: New background color selected: " << QString("RGB: %1, %2, %3").arg(col.red()).arg(col.green()).arg(col.blue()).toStdString() << std::endl;

	selectCol->setRGBA(col.red(),col.green(),col.blue(),col.alpha());

}

// Browse for a new background image
void SettingsTabLookAndFeel::changeBackgroundImage() {

	if(verbose) std::clog << "lnf: Change background image." << std::endl;

	QString dir = QDir::homePath();

	QString noPath = " " + tr("no image has been selected");
	if(backgroundImage->toolTip() != noPath.split(" ").join("/"))
		dir = backgroundImage->toolTip();

	QString newBG = QFileDialog::getOpenFileName(0,tr("Select Background Image"),dir,"Images (" + globSet.value("KnownFileTypes").toString().replace(","," ") + ")");

	if(newBG != "") {

		setBackgroundImage(newBG);

	}

}

// The checkboxes are mutually exclusive
void SettingsTabLookAndFeel::backgroundDispType() {

	QString objName = ((CustomCheckBox *)sender())->objectName();

	if(backgroundImgScaleToFit->isChecked() && objName == "scale") {
		if(verbose) std::clog << "lnf: Set background image: scale" << std::endl;
		backgroundImgStretchToFit->setChecked(false);
		backgroundImgCenter->setChecked(false);
	}

	if(backgroundImgStretchToFit->isChecked() && objName == "stretch") {
		if(verbose) std::clog << "lnf: Set background image: stretch" << std::endl;
		backgroundImgScaleToFit->setChecked(false);
		backgroundImgCenter->setChecked(false);
	}

	if(backgroundImgCenter->isChecked() && objName == "center") {
		if(verbose) std::clog << "lnf: Set background image: center" << std::endl;
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
