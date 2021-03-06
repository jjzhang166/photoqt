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

#include "settingstabthumbnail.h"
#include <iostream>

SettingsTabThumbnail::SettingsTabThumbnail(QWidget *parent, QMap<QString, QVariant> set, bool v) : QWidget(parent) {

	// The global settings
	globSet = set;

	this->setObjectName("tabthumb");

	verbose = v;

	// Opening the thumbnail database
	db = QSqlDatabase::database("thumbDB");

	// Style the widget
	this->setStyleSheet("#tabthumb { background: transparent; color: white; }");


	tabs = new TabWidget(false);
	tabs->expand(false);
	tabs->setBorderTop("rgba(150,150,150,100)",2);
	tabs->setBorderBot("rgba(150,150,150,100)",2);

	QVBoxLayout *mainLay = new QVBoxLayout;
	mainLay->addWidget(tabs);
	this->setLayout(mainLay);

	// the main scroll widget for all LOOK content
	scrollbarBasic = new CustomScrollbar;
	QScrollArea *scrollBasic = new QScrollArea;
	QVBoxLayout *layBasic = new QVBoxLayout(scrollBasic);
	QWidget *scrollWidgBasic = new QWidget(scrollBasic);
	scrollWidgBasic->setLayout(layBasic);
	scrollBasic->setWidget(scrollWidgBasic);
	scrollBasic->setWidgetResizable(true);
	scrollBasic->setVerticalScrollBar(scrollbarBasic);

	// the main scroll widget for all FEEL content
	scrollbarAdvanced = new CustomScrollbar;
	QScrollArea *scrollAdvanced = new QScrollArea;
	QVBoxLayout *layAdvanced = new QVBoxLayout(scrollAdvanced);
	QWidget *scrollWidgAdvanced = new QWidget(scrollAdvanced);
	scrollWidgAdvanced->setLayout(layAdvanced);
	scrollAdvanced->setWidget(scrollWidgAdvanced);
	scrollAdvanced->setWidgetResizable(true);
	scrollAdvanced->setVerticalScrollBar(scrollbarAdvanced);

	tabBasic = new QWidget;
	tabAdvanced = new QWidget;

	QVBoxLayout *scrollLayBasic = new QVBoxLayout;
	scrollLayBasic->addWidget(scrollBasic);
	tabBasic->setLayout(scrollLayBasic);

	QVBoxLayout *scrollLayAdvanced = new QVBoxLayout;
	scrollLayAdvanced->addWidget(scrollAdvanced);
	tabAdvanced->setLayout(scrollLayAdvanced);

	tabs->addTab(tabBasic,tr("Basic"));
	tabs->addTab(tabAdvanced,tr("Advanced"));



	// The titles
	CustomLabel *titleBasic = new CustomLabel("<center><h1>" + tr("Basic Settings") + "</h1></center>");
	layBasic->addWidget(titleBasic);
	layBasic->addSpacing(20);
	CustomLabel *titleAdvanced = new CustomLabel("<center><h1>" + tr("Advanced Settings") + "</h1></center>");
	layAdvanced->addWidget(titleAdvanced);
	layAdvanced->addSpacing(20);



	///////// BASIC TAB /////////


	// OPTION TO CHANGE THUMBNAIL SIZE
	CustomLabel *thumbSizeLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Thumbnail Size") + "</span></b><br><br>" + tr("Here you can adjust the thumbnail size. You can set it to any size between 20 and 256 pixel. Per default it is set to 80 pixel, but with different screen resolutions it might be nice to have them larger/smaller."));
	thumbSizeLabel->setWordWrap(true);
	QHBoxLayout *thumbSizeLay = new QHBoxLayout;
	thumbSizeSlider = new CustomSlider;
	thumbSizeSlider->setMinimum(20);
	thumbSizeSlider->setMaximum(256);
	thumbSizeSpin = new CustomSpinBox;
	thumbSizeSpin->setMinimum(20);
	thumbSizeSpin->setMaximum(256);
	thumbSizeLay->addStretch();
	thumbSizeLay->addWidget(thumbSizeSlider);
	thumbSizeLay->addWidget(thumbSizeSpin);
	thumbSizeLay->addStretch();
	layBasic->addWidget(thumbSizeLabel);
	layBasic->addSpacing(10);
	layBasic->addLayout(thumbSizeLay);
	layBasic->addSpacing(20);
	connect(thumbSizeSlider, SIGNAL(valueChanged(int)), thumbSizeSpin, SLOT(setValue(int)));
	connect(thumbSizeSpin, SIGNAL(valueChanged(int)), thumbSizeSlider, SLOT(setValue(int)));


	// OPTION TO SET SPACING BETWEEN THUMBNAILS
	borderAroundSlider = new CustomSlider;
	borderAroundSlider->setMinimum(0);
	borderAroundSlider->setMaximum(30);
	borderAroundSlider->setTickInterval(4);
	borderAroundSpin = new CustomSpinBox;
	borderAroundSpin->setMinimum(0);
	borderAroundSpin->setMaximum(30);
	borderAroundSpin->setSuffix(" px");
	CustomLabel *thbBorderAroundLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Spacing Between Thumbnail Images") + "</span></b><br><br>" + tr("The thumbnails are shown in a row at the lower or upper edge (depending on your setup). They are lined up side by side. Per default, there's no empty space between them, however exactly that can be changed here."));
	thbBorderAroundLabel->setWordWrap(true);
	QHBoxLayout *thbBorderLay = new QHBoxLayout;
	thbBorderLay->addStretch();
	thbBorderLay->addWidget(borderAroundSlider);
	thbBorderLay->addWidget(borderAroundSpin);
	thbBorderLay->addStretch();
	layBasic->addWidget(thbBorderAroundLabel);
	layBasic->addSpacing(10);
	layBasic->addLayout(thbBorderLay);
	layBasic->addSpacing(20);
	connect(borderAroundSlider, SIGNAL(valueChanged(int)), borderAroundSpin, SLOT(setValue(int)));
	connect(borderAroundSpin, SIGNAL(valueChanged(int)), borderAroundSlider, SLOT(setValue(int)));


	// OPTION TO ADJUST THE LIFTUP OF HOVERED THUMBNAIL
	thbLiftUpSlider = new CustomSlider;
	thbLiftUpSlider->setMinimum(0);
	thbLiftUpSlider->setMaximum(40);
	thbLiftUpSlider->setTickInterval(4);
	thbLiftUpSpin = new CustomSpinBox;
	thbLiftUpSpin->setMinimum(0);
	thbLiftUpSpin->setMaximum(40);
	thbLiftUpSpin->setSuffix(" px");
	CustomLabel *thbLiftUpLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Lift-up of Thumbnail Images on Hovering") + "</span></b><br><br>" + tr("When a thumbnail is hovered, it is lifted up some pixels (default 10). Here you can increase/decrease this value according to your personal preference."));
	thbLiftUpLabel->setWordWrap(true);
	QHBoxLayout *thbLiftUpLay = new QHBoxLayout;
	thbLiftUpLay->addStretch();
	thbLiftUpLay->addWidget(thbLiftUpSlider);
	thbLiftUpLay->addWidget(thbLiftUpSpin);
	thbLiftUpLay->addStretch();
	layBasic->addWidget(thbLiftUpLabel);
	layBasic->addSpacing(10);
	layBasic->addLayout(thbLiftUpLay);
	layBasic->addSpacing(20);
	connect(thbLiftUpSlider, SIGNAL(valueChanged(int)), thbLiftUpSpin, SLOT(setValue(int)));
	connect(thbLiftUpSpin, SIGNAL(valueChanged(int)), thbLiftUpSlider, SLOT(setValue(int)));


	// OPTION TO KEEP THUMBNAILS VISIBLE OR FADE THEM OUT
	CustomLabel *thbKeepVisibleLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Keep Thumbnails Visible") + "</span></b><br><bR>" + tr("Per default the Thumbnails slide out over the edge of the screen. Here you can force them to stay visible. The big image is shrunk to fit into the empty space. Note, that the thumbnails will be hidden (and only shown on mouse hovering) once you zoomed the image in/out. Resetting the zoom restores the original visibility of the thumbnails."));
	thbKeepVisibleLabel->setWordWrap(true);
	QHBoxLayout *thbKeepLay = new QHBoxLayout;
	keepVisible = new CustomCheckBox(tr("Keep Thumbnails Visible"));
	thbKeepLay->addStretch();
	thbKeepLay->addWidget(keepVisible);
	thbKeepLay->addStretch();
	layBasic->addWidget(thbKeepVisibleLabel);
	layBasic->addSpacing(10);
	layBasic->addLayout(thbKeepLay);
	layBasic->addSpacing(20);


	// OPTION TO ENABLE DYNAMIC THUMBNAIL CREATION (handy for faster harddrives)
	CustomLabel *dynamicThumbnailsLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Dynamic Thumbnail Creation") + "</span></b><br><bR>" + tr("Dynamic thumbnail creation means, that PhotoQt only sets up those thumbnail images that are actually needed, i.e. it stops once it reaches the end of the visible area and sits idle until you scroll left/right.") + "<br>" + tr("Smart thumbnails are similar in nature. However, they make use of the fast, that once a thumbnail has been created, it can be loaded very quickly and efficiently. It also first loads all of the currently visible thumbnails, but it doesn't stop there: Any thumbnails (even if invisible at the moment) that once have been created are loaded. This is a nice compromise between efficiency and usability.") + "<br><br>" + tr("Enabling either one of these is recommended, as it increases the performance of PhotoQt significantly, while preserving the usability."));
	dynamicThumbnailsLabel->setWordWrap(true);
	normalThumbnails = new CustomRadioButton(tr("Normal thumbnails"));
	dynamicThumbnails = new CustomRadioButton(tr("Dynamic Thumbnails"));
	dynamicThumbnailsSmart = new CustomRadioButton(tr("Smart Thumbnails"));
	QButtonGroup dynamicGroup;
	dynamicGroup.addButton(normalThumbnails);
	dynamicGroup.addButton(dynamicThumbnails);
	dynamicGroup.addButton(dynamicThumbnailsSmart);
	QVBoxLayout *dynamicThbVerticalLay = new QVBoxLayout;
	dynamicThbVerticalLay->addWidget(normalThumbnails);
	dynamicThbVerticalLay->addWidget(dynamicThumbnails);
	dynamicThbVerticalLay->addWidget(dynamicThumbnailsSmart);
	QHBoxLayout *dynamicThbLay = new QHBoxLayout;
	dynamicThbLay->addStretch();
	dynamicThbLay->addLayout(dynamicThbVerticalLay);
	dynamicThbLay->addStretch();
	layBasic->addWidget(dynamicThumbnailsLabel);
	layBasic->addSpacing(10);
	layBasic->addLayout(dynamicThbLay);
	layBasic->addSpacing(20);


	// OPTION TO ALWAYS KEEP ACTIVE THUMBNAIL IN CENTER (IF POSSIBLE)
	CustomLabel *alwaysCenterThumbnailsLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Always center on Active Thumbnail") + "</span></b><br><bR>" + tr("If this option is set, then the active thumbnail (i.e., the thumbnail of the currently displayed image) will always be kept in the center of the thumbnail bar (if possible). If this option is not set, then the active thumbnail will simply be kept visible, but not necessarily in the center."));
	alwaysCenterThumbnailsLabel->setWordWrap(true);
	QHBoxLayout *alwaysCenterThbLay = new QHBoxLayout;
	alwaysCenterThumbnail = new CustomCheckBox(tr("Center on Active Thumbnails"));
	alwaysCenterThbLay->addStretch();
	alwaysCenterThbLay->addWidget(alwaysCenterThumbnail);
	alwaysCenterThbLay->addStretch();
	layBasic->addWidget(alwaysCenterThumbnailsLabel);
	layBasic->addSpacing(10);
	layBasic->addLayout(alwaysCenterThbLay);
	layBasic->addSpacing(20);


	layBasic->addStretch();



	////////// ADVANCED //////////


	// ADJUST THE POSITION OF THE THUMBNAILS
	CustomLabel *thbPosLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Change Thumbnail Position") + "</span></b><br><bR>" + tr("Per default the bar with the thumbnails is shown at the lower edge. However, some might find it nice and handy to have the thumbnail bar at the upper edge, so that's what can be changed here."));
	thbPosLabel->setWordWrap(true);
	QHBoxLayout *thbPosLay = new QHBoxLayout;
	thbPosTop = new CustomRadioButton(tr("Show Thumbnails at upper edge"));
	thbPosBot = new CustomRadioButton(tr("Show Thumbnails at lower edge"));
	QButtonGroup *thbPosGroup = new QButtonGroup;
	thbPosGroup->addButton(thbPosTop);
	thbPosGroup->addButton(thbPosBot);
	thbPosLay->addStretch();
	thbPosLay->addWidget(thbPosTop);
	thbPosLay->addWidget(thbPosBot);
	thbPosLay->addStretch();
	layAdvanced->addWidget(thbPosLabel);
	layAdvanced->addSpacing(10);
	layAdvanced->addLayout(thbPosLay);
	layAdvanced->addSpacing(20);


	// OPTION TO SWITCH BETWEEN FILENAME, DIMENSION OR BOTH FOR WRITING ON THUMBNAILS
	CustomLabel *writeFilenameDimensionsLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Filename? Dimension? Or both?") + "</span></b><br><bR>" + tr("When thumbnails are displayed at the top/bottom, PhotoQt usually writes the filename on them. But also the dimension of the image can be written on it. Or also both or none. You can use the slider below to adjust the font size."));
	writeFilename = new CustomCheckBox(tr("Write Filename"));
	writeDimensions = new CustomCheckBox(tr("Write Resolution"));
	QHBoxLayout *writeCheckLay = new QHBoxLayout;
	writeCheckLay->addStretch();
	writeCheckLay->addWidget(writeFilename);
	writeCheckLay->addWidget(writeDimensions);
	writeCheckLay->addStretch();
	CustomLabel *filenameDimensionFontSizeLower = new CustomLabel("5pt");
	CustomLabel *filenameDimensionFontSizeUpper = new CustomLabel("20pt");
	filenameDimensionFontSize = new CustomSlider;
	filenameDimensionFontSize->setMinimum(5);
	filenameDimensionFontSize->setMaximum(20);
	QHBoxLayout *filenameDimensionFontSizeLay = new QHBoxLayout;
	filenameDimensionFontSizeLay->addStretch();
	filenameDimensionFontSizeLay->addWidget(filenameDimensionFontSizeLower);
	filenameDimensionFontSizeLay->addWidget(filenameDimensionFontSize);
	filenameDimensionFontSizeLay->addWidget(filenameDimensionFontSizeUpper);
	filenameDimensionFontSizeLay->addStretch();
	layAdvanced->addWidget(writeFilenameDimensionsLabel);
	layAdvanced->addSpacing(10);
	layAdvanced->addLayout(writeCheckLay);
	layAdvanced->addSpacing(10);
	layAdvanced->addLayout(filenameDimensionFontSizeLay);
	layAdvanced->addSpacing(20);


	// OPTION TO ONLY USE FILENAME AND NO ACTUAL THUMBNAIL
	CustomLabel *filenameInsteadThbLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Use file-name-only Thumbnails") + "</span></b><br><bR>" + tr("If you don't want PhotoQt to always load the actual image thumbnail in the background, but you still want to have something for better navigating, then you can set a file-name-only thumbnail, i.e. PhotoQt wont load any thumbnail images but simply puts the file name into the box. You can also adjust the font size of this text."));
	filenameInsteadThbLabel->setWordWrap(true);
	filenameInsteadThb = new CustomCheckBox(tr("Use file-name-only Thumbnail"));
	filenameInsteadThb->setChecked(false);
	QHBoxLayout *filenameCheckLay = new QHBoxLayout;
	filenameCheckLay->addStretch();
	filenameCheckLay->addWidget(filenameInsteadThb);
	filenameCheckLay->addStretch();
	filenameFontSizeSlider = new CustomSlider;
	filenameFontSizeSpin = new CustomSpinBox;
	filenameFontSizeSlider->setEnabled(false);
	filenameFontSizeSpin->setEnabled(false);
	filenameFontSizeSlider->setMinimum(6);
	filenameFontSizeSlider->setMaximum(20);
	filenameFontSizeSlider->setValue(globSet.value("ThumbnailFilenameInsteadFontSize").toInt());
	filenameFontSizeSpin->setMinimum(6);
	filenameFontSizeSpin->setMaximum(20);
	filenameFontSizeSpin->setSuffix("pt");
	filenameFontSizeSpin->setValue(globSet.value("ThumbnailFilenameInsteadFontSize").toInt());
	QHBoxLayout *filenameSpinSliderLay = new QHBoxLayout;
	filenameSpinSliderLay->addStretch();
	filenameSpinSliderLay->addWidget(filenameFontSizeSlider);
	filenameSpinSliderLay->addWidget(filenameFontSizeSpin);
	filenameSpinSliderLay->addStretch();
	layAdvanced->addWidget(filenameInsteadThbLabel);
	layAdvanced->addSpacing(10);
	layAdvanced->addLayout(filenameCheckLay);
	layAdvanced->addLayout(filenameSpinSliderLay);
	layAdvanced->addSpacing(20);
	connect(filenameInsteadThb, SIGNAL(toggled(bool)), filenameFontSizeSlider, SLOT(setEnabled(bool)));
	connect(filenameInsteadThb, SIGNAL(toggled(bool)), filenameFontSizeSpin, SLOT(setEnabled(bool)));
	connect(filenameFontSizeSlider, SIGNAL(valueChanged(int)), filenameFontSizeSpin, SLOT(setValue(int)));
	connect(filenameFontSizeSpin, SIGNAL(valueChanged(int)), filenameFontSizeSlider, SLOT(setValue(int)));


	// OPTION TO PRELOAD FULL DIRECTORY OR ADJUST NUMBER OF PRELOADED IMAGES
	CustomLabel *preloadFullNumberLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Preloading") + "</span></b><br><bR>" + tr("Here you can adjust, how many images AT MOST will be preloaded. For example, if the directory contains 800 images, a limit of 400 (default value) means, that starting from the opened image, 200 images to the left and 200 to the right are preloaded.") + "<br><br>" + tr("If you don't want to limit PhotoQt to any number, you can simply enable the option to always preload the full directory. WARNING: This is perfectly fine for directories with a small number of images (usually anything less than 1000, depending on your computer), but can lead to performance and memory issues for larger directories. Make sure you know what you're doing before enabling this!"));
	preloadFullNumberLabel->setWordWrap(true);
	CustomSpinBox *preloadFullNumberSpin = new CustomSpinBox;
	preloadFullNumberSpin->setSuffix(" images");
	preloadFullNumberSpin->setMinimum(50);
	preloadFullNumberSpin->setMaximum(2500);
	preloadFullNumberSpin->setSingleStep(10);
	preloadFullNumber = new CustomSlider;
	preloadFullNumber->setMinimum(50);
	preloadFullNumber->setMaximum(2500);
	preloadFullNumber->setSingleStep(10);
	QHBoxLayout *preloadFullNumberSliderLay = new QHBoxLayout;
	preloadFullNumberSliderLay->addStretch();
	preloadFullNumberSliderLay->addWidget(preloadFullNumberSpin);
	preloadFullNumberSliderLay->addWidget(preloadFullNumber);
	preloadFullNumberSliderLay->addStretch();
	preloadFullNumberCheck = new CustomCheckBox(tr("Preload full directory"));
	QHBoxLayout *preloadFullNumberCheckLay = new QHBoxLayout;
	preloadFullNumberCheckLay->addStretch();
	preloadFullNumberCheckLay->addWidget(preloadFullNumberCheck);
	preloadFullNumberCheckLay->addStretch();
	QVBoxLayout *preloadFullNumberLay = new QVBoxLayout;
	preloadFullNumberLay->addLayout(preloadFullNumberSliderLay);
	preloadFullNumberLay->addSpacing(10);
	preloadFullNumberLay->addLayout(preloadFullNumberCheckLay);
	layAdvanced->addWidget(preloadFullNumberLabel);
	layAdvanced->addSpacing(10);
	layAdvanced->addLayout(preloadFullNumberLay);
	layAdvanced->addSpacing(20);
	connect(preloadFullNumber, SIGNAL(valueChanged(int)), preloadFullNumberSpin, SLOT(setValue(int)));
	connect(preloadFullNumberSpin, SIGNAL(valueChanged(int)), preloadFullNumber, SLOT(setValue(int)));
	connect(preloadFullNumberCheck, SIGNAL(toggled(bool)), preloadFullNumber, SLOT(setDisabled(bool)));
	connect(preloadFullNumberCheck, SIGNAL(toggled(bool)), preloadFullNumberSpin, SLOT(setDisabled(bool)));


	// OPTION TO DISABLE THUMBNAILS ALLTOGETHER
	CustomLabel *thumbnailDisableLabel = new CustomLabel("<b><span style=\"font-size: 12pt\">" + tr("Disable Thumbnails") + "</span></b><br><bR>" + tr("If you just don't need or don't want any thumbnails whatsoever, then you can disable them here completely. This option can also be toggled remotely via command line (run 'photoqt --help' for more information on that). This might increase the speed of PhotoQt a good bit, however, navigating through a folder might be a little harder without thumbnails."));
	thumbnailDisableLabel->setWordWrap(true);
	thumbDisable = new CustomCheckBox(tr("Disable Thumbnails altogether"));
	thumbDisable->setChecked(false);
	QHBoxLayout *thumbDisableLay = new QHBoxLayout;
	thumbDisableLay->addStretch();
	thumbDisableLay->addWidget(thumbDisable);
	thumbDisableLay->addStretch();
	layAdvanced->addWidget(thumbnailDisableLabel);
	layAdvanced->addSpacing(5);
	layAdvanced->addLayout(thumbDisableLay);
	layAdvanced->addSpacing(20);


	// OPTION FOR THUMBNAIL CACHE

	CustomLabel *thumbCacheLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Thumbnail Cache") + "</span></b><hr>" + tr("Thumbnails can be cached in two different ways:<br>1) File Caching (following the freedesktop.org standard) or<br>2) Database Caching (better performance and management, default option).") + "<br><br>" + tr("Both ways have their advantages and disadvantages:") + "<br>" + tr("File Caching is done according to the freedesktop.org standard and thus different applications can share the same thumbnail for the same image file. However, it's not possible to check for obsolete thumbnails (thus this may lead to many unneeded thumbnail files).") + "<br>" + tr("Database Caching doesn't have the advantage of sharing thumbnails with other applications (and thus every thumbnails has to be newly created for PhotoQt), but it brings a slightly better performance, and it allows a better handling of existing thumbnails (e.g. deleting obsolete thumbnails).") + "<br><br>" + tr("PhotoQt works with either option, though the second way is set as default.") + "<br><br>" + tr("Although everybody is encouraged to use at least one of the two options, caching can be completely disabled altogether. However, that does affect the performance and usability of PhotoQt, since thumbnails have to be newly re-created every time they are needed."));
	thumbCacheLabel->setWordWrap(true);
	QHBoxLayout *thumbCacheLay = new QHBoxLayout;
	thumbCache = new CustomCheckBox(tr("Enable Thumbnail Cache"));
	thumbCacheLay->addStretch();
	thumbCacheLay->addWidget(thumbCache);
	thumbCacheLay->addStretch();

	// OPTION TO SELECT CACHE TYPE
	QHBoxLayout *cacheOption = new QHBoxLayout;
	QButtonGroup *thbCacheGroup = new QButtonGroup;
	cacheFile = new CustomRadioButton(tr("File caching"));
	cacheDatabase = new CustomRadioButton(tr("Database caching"));
	thbCacheGroup->addButton(cacheFile);
	thbCacheGroup->addButton(cacheDatabase);
	cacheOption->addStretch();
	cacheOption->addWidget(cacheFile);
	cacheOption->addWidget(cacheDatabase);
	cacheOption->addStretch();
	cacheDatabase->setChecked(true);

	layAdvanced->addWidget(thumbCacheLabel);
	layAdvanced->addSpacing(10);
	layAdvanced->addLayout(thumbCacheLay);
	layAdvanced->addSpacing(5);
	layAdvanced->addLayout(cacheOption);
	layAdvanced->addSpacing(5);

	connect(thumbCache, SIGNAL(toggled(bool)), this, SLOT(updateCacheStuff()));

	// Some info about database
	QHBoxLayout *dbInfoLay = new QHBoxLayout;
	dbInfo = new CustomLabel;
	dbInfo->setAlignment(Qt::AlignCenter);
	dbInfoLay->addStretch();
	dbInfoLay->addWidget(dbInfo);
	dbInfoLay->addStretch();
	layAdvanced->addLayout(dbInfoLay);

	// The database can be cleaned and erased
	cleanDatabase = new CustomPushButton(tr("CLEAN up Database"));
	eraseDatabase = new CustomPushButton(tr("ERASE Database"));
	QHBoxLayout *dbLay = new QHBoxLayout;
	dbLay->addStretch();
	dbLay->addWidget(cleanDatabase);
	dbLay->addWidget(eraseDatabase);
	dbLay->addStretch();

	layAdvanced->addSpacing(10);
	layAdvanced->addLayout(dbLay);
	layAdvanced->addSpacing(10);

	connect(cacheDatabase, SIGNAL(toggled(bool)), cleanDatabase, SLOT(setEnabled(bool)));
	connect(cacheDatabase, SIGNAL(toggled(bool)), eraseDatabase, SLOT(setEnabled(bool)));
	connect(cacheDatabase, SIGNAL(toggled(bool)), dbInfo, SLOT(setEnabled(bool)));


	// We ask for confirmation before cleaning up the database
	confirmClean = new CustomConfirm(tr("Clean Database"),tr("Do you really want to clean up the database? This removes all obsolete thumbnails, thus possibly making PhotoQt a little faster.") + "<bR><br>" + tr("This process might take a little while."),tr("Yes, clean is good"),tr("No, don't have time for that"),QSize(450,250),"default","default",this->parentWidget());
	confirmClean->show();

	// We ask for confirmation before erasing the entire database
	confirmErase = new CustomConfirm(tr("Erase Database"),tr("Do you really want to ERASE the entire database? This removes every single item in the database! This step should never really be necessarily. After that, every thumbnail has to be newly re-created. This step cannot be reversed!"),tr("Yes, get rid of it all"),tr("Nooo, I want to keep it"),QSize(450,250),"default","default",this->parentWidget());
	confirmErase->show();

	connect(cleanDatabase, SIGNAL(clicked()), confirmClean, SLOT(animate()));
	connect(eraseDatabase, SIGNAL(clicked()), confirmErase, SLOT(animate()));

	connect(confirmClean, SIGNAL(confirmed()), this, SLOT(doCleanDatabase()));
	connect(confirmErase, SIGNAL(confirmed()), this, SLOT(doEraseDatabase()));


	layAdvanced->addStretch();

}

// Load all settings
void SettingsTabThumbnail::loadSettings() {

	if(verbose) std::clog << "Load Settings (Thumb)" << std::endl;

	defaults.clear();

	thumbSizeSlider->setValue(globSet.value("ThumbnailSize").toInt());
	thumbSizeSpin->setValue(globSet.value("ThumbnailSize").toInt());
	defaults.insert("ThumbnailSize",globSet.value("ThumbnailSize").toInt());

	borderAroundSlider->setValue(globSet.value("ThumbnailSpacingBetween").toInt());
	borderAroundSpin->setValue(globSet.value("ThumbnailSpacingBetween").toInt());
	defaults.insert("ThumbnailSpacingBetween",globSet.value("ThumbnailSpacingBetween").toInt());

	thbLiftUpSlider->setValue(globSet.value("ThumbnailLiftUp").toInt());
	thbLiftUpSpin->setValue(globSet.value("ThumbnailLiftUp").toInt());
	defaults.insert("ThumbnailLiftUp",globSet.value("ThumbnailLiftUp").toInt());

	thbPosTop->setChecked(globSet.value("ThumbnailPosition").toString() == "Top");
	thbPosBot->setChecked(globSet.value("ThumbnailPosition").toString() == "Bottom");
	defaults.insert("ThumbnailPosition",globSet.value("ThumbnailPosition").toString());

	keepVisible->setChecked(globSet.value("ThumbnailKeepVisible").toBool());
	defaults.insert("ThumbnailKeepVisible",globSet.value("ThumbnailKeepVisible").toBool());

	normalThumbnails->setChecked(globSet.value("ThumbnailDynamic").toInt() == 0);
	defaults.insert("ThumbnailNormal",globSet.value("ThumbnailDynamic").toInt() == 0);

	dynamicThumbnails->setChecked(globSet.value("ThumbnailDynamic").toInt() == 1);
	defaults.insert("ThumbnailDynamic",globSet.value("ThumbnailDynamic").toInt() == 1);

	dynamicThumbnailsSmart->setChecked(globSet.value("ThumbnailDynamic").toInt() == 2);
	defaults.insert("ThumbnailDynamicSmart",globSet.value("ThumbnailDynamic").toInt() == 2);

	preloadFullNumber->setValue(globSet.value("ThumbnailPreloadNumber").toInt());
	defaults.insert("ThumbnailPreloadNumber",globSet.value("ThumbnailPreloadNumber").toInt());
	preloadFullNumberCheck->setChecked(globSet.value("ThumbnailPreloadFullDirectory").toBool());
	defaults.insert("ThumbnailPreloadFullDirectory",globSet.value("ThumbnailPreloadFullDirectory").toBool());

	alwaysCenterThumbnail->setChecked(globSet.value("ThumbnailCenterActive").toBool());
	defaults.insert("ThumbnailCenterActive",globSet.value("ThumbnailCenterActive").toBool());

	filenameInsteadThb->setChecked(globSet.value("ThumbnailFilenameInstead").toBool());
	filenameFontSizeSlider->setEnabled(filenameInsteadThb->isChecked());
	filenameFontSizeSpin->setEnabled(filenameInsteadThb->isChecked());
	defaults.insert("ThumbnailFilenameInstead",globSet.value("ThumbnailFilenameInstead").toBool());

	writeFilename->setChecked(globSet.value("ThumbnailWriteFilename").toBool());
	defaults.insert("ThumbnailWriteFilename",globSet.value("ThumbnailWriteFilename").toBool());

	writeDimensions->setChecked(globSet.value("ThumbnailWriteResolution").toBool());
	defaults.insert("ThumbnailWriteResolution",globSet.value("ThumbnailWriteResolution").toBool());

	filenameDimensionFontSize->setValue(globSet.value("ThumbnailFontSize").toInt());
	defaults.insert("ThumbnailFontSize",globSet.value("ThumbnailFontSize").toInt());

	filenameFontSizeSlider->setValue(globSet.value("ThumbnailFilenameInsteadFontSize").toInt());
	filenameFontSizeSpin->setValue(globSet.value("ThumbnailFilenameInsteadFontSize").toInt());
	defaults.insert("ThumbnailFilenameInsteadFontSize",globSet.value("ThumbnailFilenameInsteadFontSize").toInt());

	thumbDisable->setChecked(globSet.value("ThumbnailDisable").toBool());
	defaults.insert("ThumbnailDisable",globSet.value("ThumbnailDisable").toBool());

	thumbCache->setChecked(globSet.value("ThumbnailCache").toBool());
	cacheFile->setEnabled(globSet.value("ThumbnailCache").toBool());
	cacheDatabase->setEnabled(globSet.value("ThumbnailCache").toBool());
	defaults.insert("ThumbnailCache",globSet.value("ThumbnailCache").toBool());

	cacheFile->setChecked(globSet.value("ThbCacheFile").toBool());
	cacheDatabase->setChecked(!globSet.value("ThbCacheFile").toBool());
	defaults.insert("ThbCacheFile",globSet.value("ThbCacheFile").toBool());

}

// Save all settings
void SettingsTabThumbnail::saveSettings() {

	if(verbose) std::clog << "Save Settings (Thumb)" << std::endl;

	updatedSet.clear();

	if(defaults.value("ThumbnailSize").toInt() != thumbSizeSlider->value()) {
		updatedSet.insert("ThumbnailSize",thumbSizeSlider->value());
		defaults.remove("ThumbnailSize");
		defaults.insert("ThumbnailSize",thumbSizeSlider->value());
	}

	if(defaults.value("ThumbnailSpacingBetween").toInt() != borderAroundSlider->value()) {
		updatedSet.insert("ThumbnailSpacingBetween",borderAroundSlider->value());
		defaults.remove("ThumbnailSpacingBetween");
		defaults.insert("ThumbnailSpacingBetween",borderAroundSlider->value());
	}
	if(defaults.value("ThumbnailLiftUp").toInt() != thbLiftUpSlider->value()) {
		updatedSet.insert("ThumbnailLiftUp",thbLiftUpSlider->value());
		defaults.remove("ThumbnailLiftUp");
		defaults.insert("ThumbnailLiftUp",thbLiftUpSlider->value());
	}
	if(defaults.value("ThumbnailPosition").toString() == "Top" && thbPosBot->isChecked()) {
		updatedSet.insert("ThumbnailPosition","Bottom");
		defaults.remove("ThumbnailPosition");
		defaults.insert("ThumbnailPosition","Bottom");
	}
	if(defaults.value("ThumbnailPosition").toString() == "Bottom" && thbPosTop->isChecked()) {
		updatedSet.insert("ThumbnailPosition","Top");
		defaults.remove("ThumbnailPosition");
		defaults.insert("ThumbnailPosition","Top");
	}

	if(defaults.value("ThumbnailKeepVisible").toBool() != keepVisible->isChecked()) {
		updatedSet.insert("ThumbnailKeepVisible",keepVisible->isChecked());
		defaults.remove("ThumbnailKeepVisible");
		defaults.insert("ThumbnailKeepVisible",keepVisible->isChecked());
	}

	if(defaults.value("ThumbnailNormal").toBool() != normalThumbnails->isChecked()
		|| defaults.value("ThumbnailDynamic").toBool() != dynamicThumbnails->isChecked()
		|| defaults.value("ThumbnailDynamicSmart").toBool() != dynamicThumbnails->isChecked()) {
		updatedSet.insert("ThumbnailDynamic",(dynamicThumbnailsSmart->isChecked() ? 2 : (dynamicThumbnails->isChecked() ? 1 : 0)));
		defaults.remove("ThumbnailNormal");
		defaults.remove("ThumbnailDynamic");
		defaults.remove("ThumbnailDynamicSmart");
		defaults.insert("ThumbnailNormal",normalThumbnails->isChecked());
		defaults.insert("ThumbnailDynamic",dynamicThumbnails->isChecked());
		defaults.insert("ThumbnailDynamicSmart",dynamicThumbnailsSmart->isChecked());
	}

	if(defaults.value("ThumbnailPreloadNumber").toInt() != preloadFullNumber->value()) {
		updatedSet.insert("ThumbnailPreloadNumber",preloadFullNumber->value());
		defaults.remove("ThumbnailPreloadNumber");
		defaults.insert("ThumbnailPreloadNumber",preloadFullNumber->value());
	}
	if(defaults.value("ThumbnailPreloadFullDirectory").toBool() != preloadFullNumberCheck->isChecked()) {
		updatedSet.insert("ThumbnailPreloadFullDirectory",preloadFullNumberCheck->isChecked());
		defaults.remove("ThumbnailPreloadFullDirectory");
		defaults.insert("ThumbnailPreloadFullDirectory",preloadFullNumberCheck->isChecked());
	}

	if(defaults.value("ThumbnailCenterActive").toBool() != alwaysCenterThumbnail->isChecked()) {
		updatedSet.insert("ThumbnailCenterActive",alwaysCenterThumbnail->isChecked());
		defaults.remove("ThumbnailCenterActive");
		defaults.insert("ThumbnailCenterActive",alwaysCenterThumbnail->isChecked());
	}

	if(defaults.value("ThumbnailFilenameInstead").toBool() != filenameInsteadThb->isChecked()) {
		updatedSet.insert("ThumbnailFilenameInstead",filenameInsteadThb->isChecked());
		defaults.remove("ThumbnailFilenameInstead");
		defaults.insert("ThumbnailFilenameInstead",filenameInsteadThb->isChecked());
	}

	if(defaults.value("ThumbnailWriteFilename").toBool() != writeFilename->isChecked()) {
		updatedSet.insert("ThumbnailWriteFilename",writeFilename->isChecked());
		defaults.remove("ThumbnailWriteFilename");
		defaults.insert("ThumbnailWriteFilename",writeFilename->isChecked());
	}

	if(defaults.value("ThumbnailWriteResolution").toBool() != writeDimensions->isChecked()) {
		updatedSet.insert("ThumbnailWriteResolution",writeDimensions->isChecked());
		defaults.remove("ThumbnailWriteResolution");
		defaults.insert("ThumbnailWriteResolution",writeDimensions->isChecked());
	}

	if(defaults.value("ThumbnailFontSize").toInt() != filenameDimensionFontSize->value()) {
		updatedSet.insert("ThumbnailFontSize",filenameDimensionFontSize->value());
		defaults.remove("ThumbnailFontSize");
		defaults.insert("ThumbnailFontSize",filenameDimensionFontSize->value());
	}

	if(defaults.value("ThumbnailFilenameInsteadFontSize").toInt() != filenameFontSizeSlider->value()) {
		updatedSet.insert("ThumbnailFilenameInsteadFontSize",filenameFontSizeSlider->value());
		defaults.remove("ThumbnailFilenameInsteadFontSize");
		defaults.insert("ThumbnailFilenameInsteadFontSize",filenameFontSizeSlider->value());
	}

	if(defaults.value("ThumbnailDisable").toBool() != thumbDisable->isChecked()) {
		updatedSet.insert("ThumbnailDisable",thumbDisable->isChecked());
		defaults.remove("ThumbnailDisable");
		defaults.insert("ThumbnailDisable",thumbDisable->isChecked());
	}

	if(defaults.value("ThumbnailCache").toBool() != thumbCache->isChecked()) {
		updatedSet.insert("ThumbnailCache",thumbCache->isChecked());
		defaults.remove("ThumbnailCache");
		defaults.insert("ThumbnailCache",thumbCache->isChecked());
	}
	if(defaults.value("ThbCacheFile").toBool() != cacheFile->isChecked()) {
		updatedSet.insert("ThbCacheFile",cacheFile->isChecked());
		defaults.remove("ThbCacheFile");
		defaults.insert("ThbCacheFile",cacheFile->isChecked());
	}

}

// Set some basic database info (is loaded when tab is activated)
void SettingsTabThumbnail::setDatabaseInfo() {

	if(verbose) std::clog << "thb: Setting Database Info" << std::endl;

	QSqlQuery query(db);
	query.exec("SELECT COUNT(filepath) AS c FROM Thumbnails");
	if(query.lastError().text().trimmed().length())
		std::cerr << "ERROR: (Count) " << query.lastError().text().trimmed().toStdString() << std::endl;
	query.next();

	noOfEntriesInDb = query.value(query.record().indexOf("c")).toInt();

	dbInfo->setText(tr("Current database filesize: %1 KB").arg(QFileInfo(QDir::homePath() + "/.photoqt/thumbnails").size()/1024) + "<br>" + tr("Entries in database: %2").arg(noOfEntriesInDb));

	query.clear();

}

void SettingsTabThumbnail::updateCacheStuff() {

	if(thumbCache->isChecked()) {
		cacheDatabase->setEnabled(true);
		cacheFile->setEnabled(true);
		dbInfo->setEnabled(cacheDatabase->isChecked());
		cleanDatabase->setEnabled(cacheDatabase->isChecked());
		eraseDatabase->setEnabled(cacheDatabase->isChecked());
	} else {
		dbInfo->setEnabled(false);
		cleanDatabase->setEnabled(false);
		eraseDatabase->setEnabled(false);
		cacheDatabase->setEnabled(false);
		cacheFile->setEnabled(false);
	}

}

// Clean the database
void SettingsTabThumbnail::doCleanDatabase() {

	if(verbose) std::clog << "thb: Clean database" << std::endl;

	QSqlQuery query(db);

	// First, we remove all entries with empty filepath (something went wrong there)
	query.prepare("DELETE FROM Thumbnails WHERE filepath=''");
	query.exec();
	query.clear();

	// Then lets look at the remaining entries
	query.prepare("SELECT * FROM Thumbnails");
	query.exec();

	// First we create a list of items that are to be deleted
	QList<QStringList> toDel;
	while(query.next()) {
		QString path = query.value(query.record().indexOf("filepath")).toString();
		int mtime = query.value(query.record().indexOf("filelastmod")).toInt();

		if(!QFile(path).exists() || mtime != int(QFileInfo(path).lastModified().toTime_t())) {

			QStringList l;
			l << path << QString("%1").arg(mtime);
			toDel << l;

		}

	}
	query.clear();

	// Then we actually delete all the items
	for(int i = 0; i < toDel.length(); ++i) {

		QSqlQuery query2(db);
		query2.prepare("DELETE FROM Thumbnails WHERE filepath=:path AND filelastmod=:mod");
		query2.bindValue(":mod",toDel.at(i).at(1));
		query2.bindValue(":path",toDel.at(i).at(0));
		query2.exec();
		if(query2.lastError().text().trimmed().length())
			std::cerr << "ERROR (del): " << query2.lastError().text().trimmed().toStdString() << std::endl;
		query2.clear();

	}

	// Error catching
	if(db.lastError().text().trimmed().length())
		std::cerr << "ERROR (after del): " << db.lastError().text().trimmed().toStdString() << std::endl;


	// Compress database
	QSqlQuery query3(db);
	query3.prepare("VACUUM");
	query3.exec();
	if(query3.lastError().text().trimmed().length())
		std::cerr << "ERROR: (Vacuum) " << query3.lastError().text().trimmed().toStdString() << std::endl;
	query3.clear();

	// Update database info
	setDatabaseInfo();

}

// Erase the entire database
void SettingsTabThumbnail::doEraseDatabase() {

	if(verbose) std::clog << "thb: Erase database" << std::endl;

	QFile(QDir::homePath() + "/.photoqt/thumbnails").remove();

	QSqlDatabase::removeDatabase("thumbDB");

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "thumbDB");
	db.setDatabaseName(QDir::homePath() + "/.photoqt/thumbnails");
	if(!db.open())
		std::cerr << "ERROR: Couldn't open thumbnail database: " << db.lastError().text().trimmed().toStdString() << std::endl;
	QSqlQuery query(db);
	query.prepare("CREATE TABLE Thumbnails (filepath TEXT,thumbnail BLOB, filelastmod INT, thumbcreated INT, origwidth INT, origheight INT)");
	query.exec();
	if(query.lastError().text().trimmed().length())
		std::cerr << "ERROR (Creating Thumbnail Datbase): " << query.lastError().text().trimmed().toStdString() << std::endl;
	query.clear();

	// Update database info
	setDatabaseInfo();

}

SettingsTabThumbnail::~SettingsTabThumbnail() { }
