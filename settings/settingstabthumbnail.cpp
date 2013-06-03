#include "settingstabthumbnail.h"

SettingsTabThumbnail::SettingsTabThumbnail(QWidget *parent, QMap<QString, QVariant> set, bool v) : QWidget(parent) {

	// The global settings
	globSet = set;

	verbose = v;

	// Opening the thumbnail database
	db = QSqlDatabase::database("thumbDB");

	// Style the widget
	this->setStyleSheet("background: transparent; color: white;");

	// The main scroll area
	scrollbar = new CustomScrollbar;
	scroll = new QScrollArea;
	QVBoxLayout *lay = new QVBoxLayout(scroll);
	scrollWidg = new QWidget(scroll);
	scrollWidg->setLayout(lay);
	scroll->setWidget(scrollWidg);
	scroll->setWidgetResizable(true);
	QVBoxLayout *scrollLay = new QVBoxLayout;
	scrollLay->addWidget(scroll);
	this->setLayout(scrollLay);
	scroll->setVerticalScrollBar(scrollbar);

	// The title
	QLabel *title = new QLabel("<center><h1>" + tr("Thumbnails") + "</h1></center>");
	lay->addWidget(title);
	lay->addSpacing(20);

	// OPTION TO CHANGE THUMBNAIL SIZE
	QLabel *thumbSizeLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Thumbnail Size") + "</span></b><br><br>" + tr("Here you can adjust the thumbnail size. You can set it to any size between 20 and 256 pixel. Per default it is set to 80 pixel, but with different screen resolutions it might be nice to have them larger/smaller."));
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
	lay->addWidget(thumbSizeLabel);
	lay->addSpacing(5);
	lay->addLayout(thumbSizeLay);
	lay->addSpacing(20);
	connect(thumbSizeSlider, SIGNAL(valueChanged(int)), thumbSizeSpin, SLOT(setValue(int)));
	connect(thumbSizeSpin, SIGNAL(valueChanged(int)), thumbSizeSlider, SLOT(setValue(int)));



	// OPTION TO SET BORDER AROUND THUMBNAILS
	widgetSpacing = new QWidget;
	widgetSpacing->setObjectName("widgetSpacing");
	widgetSpacing->setStyleSheet("QWidget#widgetSpacing { background: rgba(255,255,255,20); border-radius: 20px; padding: 20px; }");
	QLabel *extSpacing = new QLabel(tr("Extended Setting"));
	extSpacing->setStyleSheet("font-weight: bold; font-size: 10px; font-style: italic");
	QHBoxLayout *extSpacingLay = new QHBoxLayout;
	extSpacingLay->addWidget(extSpacing);
	extSpacingLay->addStretch();
	borderAroundSlider = new CustomSlider;
	borderAroundSlider->setMinimum(0);
	borderAroundSlider->setMaximum(30);
	borderAroundSlider->setTickInterval(4);
	borderAroundSpin = new CustomSpinBox;
	borderAroundSpin->setMinimum(0);
	borderAroundSpin->setMaximum(30);
	borderAroundSpin->setSuffix(" px");
	QLabel *thbBorderAroundLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Spacing Between Thumbnail Images") + "</span></b><br><br>" + tr("The thumbnails are shown in a row at the lower or upper edge (depending on your setup). They are lined up side by side. Here you can adjust the spacing in between (or remove spacing altogether)."));
	thbBorderAroundLabel->setWordWrap(true);
	QHBoxLayout *thbBorderLay = new QHBoxLayout;
	thbBorderLay->addStretch();
	thbBorderLay->addWidget(borderAroundSlider);
	thbBorderLay->addWidget(borderAroundSpin);
	thbBorderLay->addStretch();
	QVBoxLayout *widgetSpacingLay = new QVBoxLayout;
	widgetSpacingLay->addLayout(extSpacingLay);
	widgetSpacingLay->addWidget(thbBorderAroundLabel);
	widgetSpacingLay->addSpacing(5);
	widgetSpacingLay->addLayout(thbBorderLay);
	widgetSpacingLay->addSpacing(20);
	widgetSpacing->setLayout(widgetSpacingLay);
	lay->addWidget(widgetSpacing);
	widgetSpacing->hide();
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
	QLabel *thbLiftUpLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Lift-up of Thumbnail Images on Hovering") + "</span></b><br><br>" + tr("When a thumbnail is hovered, it is lifted up some pixels (default 10). Here you can increase/decrease this value according to your personal preference."));
	thbLiftUpLabel->setWordWrap(true);
	QHBoxLayout *thbLiftUpLay = new QHBoxLayout;
	thbLiftUpLay->addStretch();
	thbLiftUpLay->addWidget(thbLiftUpSlider);
	thbLiftUpLay->addWidget(thbLiftUpSpin);
	thbLiftUpLay->addStretch();
	lay->addWidget(thbLiftUpLabel);
	lay->addSpacing(5);
	lay->addLayout(thbLiftUpLay);
	lay->addSpacing(20);
	connect(thbLiftUpSlider, SIGNAL(valueChanged(int)), thbLiftUpSpin, SLOT(setValue(int)));
	connect(thbLiftUpSpin, SIGNAL(valueChanged(int)), thbLiftUpSlider, SLOT(setValue(int)));



	// ADJUST THE POSITION OF THE THUMBNAILS
	QLabel *thbPosLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Change Thumbnail Position") + "</span></b><br><bR>" + tr("Per default the bar with the thumbnails is shown at the lower edge. However, some might find it nice and handy to have the thumbnail bar at the upper edge, so that's what can be changed here."));
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
	lay->addWidget(thbPosLabel);
	lay->addSpacing(5);
	lay->addLayout(thbPosLay);
	lay->addSpacing(20);



	// OPTION TO KEEP THUMBNAILS VISIBLE OR FADE THEM OUT
	QLabel *thbKeepVisibleLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Keep Thumbnails Visible") + "</span></b><br><bR>" + tr("Per default the Thumbnails slide out over the edge of the screen. Here you can force them to stay visible. The big image is shrunk to fit into the empty space. Note, that the thumbnails will be hidden (and shown on mouse hovering) once you zoomed the image in/out. Resetting the zoom restores the original visibility of the thumbnails."));
	thbKeepVisibleLabel->setWordWrap(true);
	QHBoxLayout *thbKeepLay = new QHBoxLayout;
	keepVisible = new CustomCheckBox(tr("Keep Thumbnails Visible"));
	thbKeepLay->addStretch();
	thbKeepLay->addWidget(keepVisible);
	thbKeepLay->addStretch();
	lay->addWidget(thbKeepVisibleLabel);
	lay->addSpacing(5);
	lay->addLayout(thbKeepLay);
	lay->addSpacing(20);


	// OPTION TO ENABLE DYNAMIC THUMBNAIL CREATION (handy for faster harddrives)
	widgetDynamic = new QWidget;
	widgetDynamic->setObjectName("widgetDynamic");
	widgetDynamic->setStyleSheet("QWidget#widgetDynamic { background: rgba(255,255,255,20); border-radius: 20px; padding: 20px; }");
	QLabel *extDyn = new QLabel(tr("Extended Setting"));
	extDyn->setStyleSheet("font-weight: bold; font-size: 10px; font-style: italic");
	QHBoxLayout *extDynLay = new QHBoxLayout;
	extDynLay->addWidget(extDyn);
	extDynLay->addStretch();
	QLabel *dynamicThumbnailsLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Dynamic Thumbnail Creation") + "</span></b><br><bR>" + tr("Enable dynamic thumbnail creation."));
	dynamicThumbnailsLabel->setWordWrap(true);
	QHBoxLayout *dynamicThbLay = new QHBoxLayout;
	dynamicThumbnails = new CustomCheckBox(tr("Enable Dynamic Thumbnails"));
	dynamicThbLay->addStretch();
	dynamicThbLay->addWidget(dynamicThumbnails);
	dynamicThbLay->addStretch();
	QVBoxLayout *widgetDynLay = new QVBoxLayout;
	widgetDynLay->addLayout(extDynLay);
	widgetDynLay->addWidget(dynamicThumbnailsLabel);
	widgetDynLay->addSpacing(5);
	widgetDynLay->addLayout(dynamicThbLay);
	widgetDynLay->addSpacing(20);
	widgetDynamic->setLayout(widgetDynLay);
	lay->addWidget(widgetDynamic);
	widgetDynamic->hide();



	// OPTION TO ONLY USE FILENAME AND NO ACTUAL THUMBNAIL
	QLabel *filenameInsteadThbLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Use file-name-only Thumbnails") + "</span></b><br><bR>" + tr("If you don't want PhotoQt to always load the thumbnails in the background, but you still want to have something for better navigating, then you can set a file-name-only thumbnail, i.e. PhotoQt wont load any thumbnail images but simply puts the file name into the box. You can also adjust the font size of this text."));
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
	QVBoxLayout *filenameLay = new QVBoxLayout;
	filenameLay->addLayout(filenameCheckLay);
	filenameLay->addLayout(filenameSpinSliderLay);
	connect(filenameInsteadThb, SIGNAL(toggled(bool)), filenameFontSizeSlider, SLOT(setEnabled(bool)));
	connect(filenameInsteadThb, SIGNAL(toggled(bool)), filenameFontSizeSpin, SLOT(setEnabled(bool)));
	connect(filenameFontSizeSlider, SIGNAL(valueChanged(int)), filenameFontSizeSpin, SLOT(setValue(int)));
	connect(filenameFontSizeSpin, SIGNAL(valueChanged(int)), filenameFontSizeSlider, SLOT(setValue(int)));




	widgetDisable = new QWidget;
	widgetDisable->setObjectName("widgetDynamic");
	widgetDisable->setStyleSheet("QWidget#widgetDynamic { background: rgba(255,255,255,20); border-radius: 20px; padding: 20px; }");
	QLabel *extDis = new QLabel(tr("Extended Setting"));
	extDis->setStyleSheet("font-weight: bold; font-size: 10px; font-style: italic");
	QHBoxLayout *extDisLay = new QHBoxLayout;
	extDisLay->addWidget(extDis);
	extDisLay->addStretch();
	QLabel *thumbnailDisableLabel = new QLabel("<b><span style=\"font-size: 12pt\">" + tr("Disable Thumbnails") + "</span></b><br><bR>" + tr("If you just don't need or don't want any thumbnails whatsoever, then you can disable them here completely. This option can also be toggled remotely via command line (run 'photoqt --help' for more information on that)."));
	thumbnailDisableLabel->setWordWrap(true);
	thumbDisable = new CustomCheckBox(tr("Disable Thumbnails altogether"));
	thumbDisable->setChecked(false);
	QHBoxLayout *thumbDisableLay = new QHBoxLayout;
	thumbDisableLay->addStretch();
	thumbDisableLay->addWidget(thumbDisable);
	thumbDisableLay->addStretch();
	QVBoxLayout *widgetDisLay = new QVBoxLayout;
	widgetDisLay->addLayout(extDisLay);
	widgetDisLay->addWidget(thumbnailDisableLabel);
	widgetDisLay->addSpacing(5);
	widgetDisLay->addLayout(thumbDisableLay);
	widgetDisLay->addSpacing(20);
	widgetDisable->setLayout(widgetDisLay);
	lay->addWidget(widgetDisable);
	widgetDisable->hide();


	// OPTION FOR THUMBNAIL CACHE
	widgetCache = new QWidget;
	widgetCache->setObjectName("widgetCache");
	widgetCache->setStyleSheet("QWidget#widgetCache { background: rgba(255,255,255,20); border-radius: 20px; padding: 20px; }");
	QLabel *extCache = new QLabel(tr("Extended Setting"));
	extCache->setStyleSheet("font-weight: bold; font-size: 10px; font-style: italic");
	QHBoxLayout *extCacheLay = new QHBoxLayout;
	extCacheLay->addWidget(extCache);
	extCacheLay->addStretch();

	QLabel *thumbCacheLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Thumbnail Cache") + "</span></b><hr>" + tr("Thumbnails can be cached in two different ways:<br>1) File Caching (following the freedesktop.org standard) or<br>2) Database Caching (better performance and management, default option).") + "<br><br>" + tr("Both ways have their advantages and disadvantages:") + "<br>" + tr("File Caching is done according to the freedesktop.org standard and thus different applications can share the same thumbnail for the same image file. However, it's not possible to check for obsolete thumbnails (thus this may lead to many unneeded thumbnail files).") + "<br>" + tr("Database Caching doesn't have the advantage of sharing thumbnails with other applications (and thus every thumbnails has to be newly created for PhotoQt), but it brings a slightly better performance, and it allows a better handling of existing thumbnails (e.g. deleting obsolete thumbnails).") + "<br><br>" + tr("PhotoQt works with either option, though the second way is set as default.") + "<br><br>" + tr("Although everybody is encouraged to use at least one of the two options, caching can be completely disabled altogether. However, that does affect the performance and usability of PhotoQt, since thumbnails have to be newly re-created every time they are needed."));
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

	QVBoxLayout *widgetCacheLay = new QVBoxLayout;
	widgetCacheLay->addLayout(extCacheLay);
	widgetCacheLay->addWidget(thumbCacheLabel);
	widgetCacheLay->addSpacing(5);
	widgetCacheLay->addLayout(thumbCacheLay);
	widgetCacheLay->addSpacing(5);
	widgetCacheLay->addLayout(cacheOption);
	widgetCacheLay->addSpacing(5);

	connect(thumbCache, SIGNAL(toggled(bool)), cacheFile, SLOT(setEnabled(bool)));
	connect(thumbCache, SIGNAL(toggled(bool)), cacheDatabase, SLOT(setEnabled(bool)));

	// Some info about database
	QHBoxLayout *dbInfoLay = new QHBoxLayout;
	dbInfo = new QLabel;
	dbInfo->setAlignment(Qt::AlignCenter);
	dbInfoLay->addStretch();
	dbInfoLay->addWidget(dbInfo);
	dbInfoLay->addStretch();
	widgetCacheLay->addLayout(dbInfoLay);

	// The database can be cleaned and erased
	cleanDatabase = new CustomPushButton(tr("CLEAN up Database"));
	eraseDatabase = new CustomPushButton(tr("ERASE Database"));
	cleanDatabase->removeRoundedCorners();
	eraseDatabase->removeRoundedCorners();
	QHBoxLayout *dbLay = new QHBoxLayout;
	dbLay->addStretch();
	dbLay->addWidget(cleanDatabase);
	dbLay->addWidget(eraseDatabase);
	dbLay->addStretch();

	widgetCacheLay->addSpacing(10);
	widgetCacheLay->addLayout(dbLay);
	widgetCacheLay->addSpacing(10);

	widgetCache->setLayout(widgetCacheLay);
	lay->addWidget(widgetCache);
	widgetCache->hide();

	connect(thumbCache, SIGNAL(toggled(bool)), cleanDatabase, SLOT(setEnabled(bool)));
	connect(thumbCache, SIGNAL(toggled(bool)), eraseDatabase, SLOT(setEnabled(bool)));
	connect(cacheDatabase, SIGNAL(toggled(bool)), cleanDatabase, SLOT(setEnabled(bool)));
	connect(cacheDatabase, SIGNAL(toggled(bool)), eraseDatabase, SLOT(setEnabled(bool)));


	// We ask for confirmation before cleaning up the database
	confirmClean = new CustomConfirm(tr("Clean Database"),tr("Do you really want to clean up the database? This removes all obsolete thumbnails, thus possibly making PhotoQt a little faster.") + "<bR><br>" + tr("This process might take a little while."),tr("Yes, clean is good"),tr("No, don't have time for that"),QSize(400,200),this->parentWidget());
	confirmClean->showBorder("white",2);
	confirmClean->show();

	// We ask for confirmation before erasing the entire database
	confirmErase = new CustomConfirm(tr("Erase Database"),tr("Do you really want to ERASE the entire database? This removes every single item in the database! This step should never really be necessarily. After that, every thumbnail has to be newly re-created. This step cannot be reversed!"),tr("Yes, get rid of it all"),tr("No, I want to keep it"),QSize(400,200),this->parentWidget());
	confirmErase->showBorder("white",2);
	confirmErase->show();

	connect(cleanDatabase, SIGNAL(clicked()), confirmClean, SLOT(animate()));
	connect(eraseDatabase, SIGNAL(clicked()), confirmErase, SLOT(animate()));

	connect(confirmClean, SIGNAL(confirmed()), this, SLOT(doCleanDatabase()));
	connect(confirmErase, SIGNAL(confirmed()), this, SLOT(doEraseDatabase()));


	lay->addStretch();

}

// Load all settings
void SettingsTabThumbnail::loadSettings() {

	if(verbose) qDebug() << "Load Settings (Thumb)";

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

	dynamicThumbnails->setChecked(globSet.value("ThumbnailDynamic").toBool());
	defaults.insert("ThumbnailDynamic",globSet.value("ThumbnailDynamic").toBool());

	filenameInsteadThb->setChecked(globSet.value("ThumbnailFilenameInstead").toBool());
	filenameFontSizeSlider->setEnabled(filenameInsteadThb->isChecked());
	filenameFontSizeSpin->setEnabled(filenameInsteadThb->isChecked());
	defaults.insert("ThumbnailFilenameInstead",globSet.value("ThumbnailFilenameInstead").toBool());

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

	if(verbose) qDebug() << "Save Settings (Thumb)";

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
	if(defaults.value("ThumbnailDynamic").toBool() != dynamicThumbnails->isChecked()) {
		updatedSet.insert("ThumbnailDynamic",dynamicThumbnails->isChecked());
		defaults.remove("ThumbnailDynamic");
		defaults.insert("ThumbnailDynamic",dynamicThumbnails->isChecked());
	}

	if(defaults.value("ThumbnailFilenameInstead").toBool() != filenameInsteadThb->isChecked()) {
		updatedSet.insert("ThumbnailFilenameInstead",filenameInsteadThb->isChecked());
		defaults.remove("ThumbnailFilenameInstead");
		defaults.insert("ThumbnailFilenameInstead",filenameInsteadThb->isChecked());
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

	if(verbose) qDebug() << "thb: Setting Database Info";

	QSqlQuery query(db);
	query.exec("SELECT COUNT(filepath) AS c FROM Thumbnails");
	if(query.lastError().text().trimmed().length())
		qDebug() << "ERROR: (Count)" << query.lastError().text().trimmed();
	query.next();

	noOfEntriesInDb = query.value(query.record().indexOf("c")).toInt();

	dbInfo->setText(tr("Current database filesize: %1 KB").arg(QFileInfo(QDir::homePath() + "/.photoqt/thumbnails").size()/1024) + "<br>" + tr("Entries in database: %2").arg(noOfEntriesInDb));

	query.clear();

}

// Clean the database
void SettingsTabThumbnail::doCleanDatabase() {

	if(verbose) qDebug() << "thb: Clean database";

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
			qDebug() << "ERROR (del):" << query2.lastError().text().trimmed();
		query2.clear();

	}

	// Error catching
	if(db.lastError().text().trimmed().length())
		qDebug() << "ERROR (after del):" << db.lastError().text().trimmed();


	// Compress database
	QSqlQuery query3(db);
	query3.prepare("VACUUM");
	query3.exec();
	if(query3.lastError().text().trimmed().length())
		qDebug() << "ERROR: (Vacuum)" << query3.lastError().text().trimmed();
	query3.clear();

	// Update database info
	setDatabaseInfo();

}

// Erase the entire database
void SettingsTabThumbnail::doEraseDatabase() {

	if(verbose) qDebug() << "thb: Erase database";

	QFile(QDir::homePath() + "/.photoqt/thumbnails").remove();

	QSqlDatabase::removeDatabase("thumbDB");

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "thumbDB");
	db.setDatabaseName(QDir::homePath() + "/.photoqt/thumbnails");
	if(!db.open())
		qDebug() << "ERROR: Couldn't open thumbnail database:" << db.lastError().text().trimmed();
	QSqlQuery query(db);
	query.prepare("CREATE TABLE Thumbnails (filepath TEXT,thumbnail BLOB, filelastmod INT, thumbcreated INT)");
	query.exec();
	if(query.lastError().text().trimmed().length())
		qDebug() << "ERROR (Creating Thumbnail Datbase):" << query.lastError().text().trimmed();
	query.clear();

	// Update database info
	setDatabaseInfo();

}

void SettingsTabThumbnail::toggleExtended(bool extended) {

	widgetSpacing->setVisible(extended);
	widgetDynamic->setVisible(extended);
	widgetDisable->setVisible(extended);
	widgetCache->setVisible(extended);

}

void SettingsTabThumbnail::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabThumbnail::~SettingsTabThumbnail() { }
