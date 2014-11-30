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

#include "settingstabother.h"
#include <iostream>

SettingsTabOther::SettingsTabOther(QWidget *parent, QMap<QString, QVariant> set, bool v) : QWidget(parent) {

	// The global settings
	globSet = set;

	verbose = v;

	this->setObjectName("tabother");
	this->setStyleSheet("#tabother { background: transparent; color: white; }");

	tabs = new TabWidget;
	tabs->expand(false);
	tabs->setBorderTop("rgba(150,150,150,100)",2);
	tabs->setBorderBot("rgba(150,150,150,100)",2);

	QVBoxLayout *mainLay = new QVBoxLayout;
	mainLay->addWidget(tabs);
	this->setLayout(mainLay);

	// the main scroll widget for all LOOK content
	scrollbarOther = new CustomScrollbar;
	QScrollArea *scrollOther = new QScrollArea;
	QVBoxLayout *layOther = new QVBoxLayout(scrollOther);
	QWidget *scrollWidgOther = new QWidget(scrollOther);
	scrollWidgOther->setLayout(layOther);
	scrollOther->setWidget(scrollWidgOther);
	scrollOther->setWidgetResizable(true);
	scrollOther->setVerticalScrollBar(scrollbarOther);

	// the main scroll widget for all FEEL content
	scrollbarFile = new CustomScrollbar;
	QScrollArea *scrollFile = new QScrollArea;
	QVBoxLayout *layFile = new QVBoxLayout(scrollFile);
	QWidget *scrollWidgFile = new QWidget(scrollFile);
	scrollWidgFile->setLayout(layFile);
	scrollFile->setWidget(scrollWidgFile);
	scrollFile->setWidgetResizable(true);
	scrollFile->setVerticalScrollBar(scrollbarFile);

	tabOther = new QWidget;
	tabFile = new QWidget;

	QVBoxLayout *scrollLayOther = new QVBoxLayout;
	scrollLayOther->addWidget(scrollOther);
	tabOther->setLayout(scrollLayOther);

	QVBoxLayout *scrollLayFile = new QVBoxLayout;
	scrollLayFile->addWidget(scrollFile);
	tabFile->setLayout(scrollLayFile);

	tabs->addTab(tabOther,tr("Other"));
	tabs->addTab(tabFile,tr("File Types"));



	// The titles
	CustomLabel *titleOther = new CustomLabel("<center><h1>" + tr("Other Settings") + "</h1></center>");
	layOther->addWidget(titleOther);
	layOther->addSpacing(20);
	CustomLabel *titleFile = new CustomLabel("<center><h1>" + tr("Known File Types") + "</h1></center>");
	layFile->addWidget(titleFile);
	layFile->addSpacing(20);



	// CHOOSE A LANGUAGE
	CustomLabel *langLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Choose Language") + "</span></b><br><br>" + tr("There are a good few different languages available. Thanks to everybody who took the time to translate PhotoQt!"));
	langLabel->setWordWrap(true);
	layOther->addWidget(langLabel);
	layOther->addSpacing(15);


	// All the languages available. They are sorted according to their language code (except English)
	// A GOOD FEW OF THE TRANSLATIONS HAVEN'T BEEN UPDATED IN A LONG TIME AND ARE STANDING AT 0-5%
	// These translations are NOT included!

	langDesc << "English";
	langShort << "en";

	// Arabic
//	langDesc << QString::fromUtf8("العربية (Amar T.)");
//	langShort << "ar";

	// Czech
	langDesc << QString::fromUtf8("Čeština (Robin H. & Petr Š.)");
	langShort << "cs";

	// German
	langDesc << "Deutsch";
	langShort << "de";

	// Greek
	langDesc << QString::fromUtf8("Ελληνικά (Dimitrios G.)");
	langShort << "el";

	// Spanish
	langDesc << QString::fromUtf8("Español (Hector C. & Victoria P.)");
	langShort << "es_ES";

	// Finnish
	langDesc << QString::fromUtf8("Suomen kieli (Jiri G.)");
	langShort << "fi";

	// French
	langDesc << QString::fromUtf8("Français (Olivier D. & Tubuntu)");
	langShort << "fr";

	// Hungarian
//	langDesc << QString::fromUtf8("Magyar (Zoltan H.)");
//	langShort << "hu";

	// Hebrew
	langDesc << QString::fromUtf8("עברית (GenghisKhan)");
	langShort << "he";

	// Italian
	langDesc << "Italiano (Vincenzo C. & Fabio M.)";
	langShort << "it";

	// Japanese
	langDesc << QString::fromUtf8("日本語 (Obytetest)");
	langShort << "ja";

	// Norwegian Bokmal
//	langDesc << QString::fromUtf8("Bokmål (Ola Haugen H.)");
//	langShort << "nb_NO";

	// Norwegian Nynorsk
//	langDesc << "Nynorsk (Ola Haugen H.)";
//	langShort << "nn_NO";

	// Polish
//	langDesc << "Polski (Daniel K.)";
//	langShort << "pl";

	// Portugal (Brazil)
	langDesc << QString::fromUtf8("Português (Brasil) (Rafael N. & Everton)");
	langShort << "pt_BR";

	// Portugal (Portugal)
	langDesc << QString::fromUtf8("Português (Portugal) (Sérgio M. & Manuela S. & Willow)");
	langShort << "pt_PT";

	// Russian
	langDesc << QString::fromUtf8("Pусский (Yuriy T.)");
	langShort << "ru_RU";

	//Slovak
	langDesc << QString::fromUtf8("Slovenčina (Lukáš D.)");
	langShort << "sk";

	// Serbian
//	langDesc << QString::fromUtf8("српски екавски (Mladen Pejaković)");
//	langShort << "sr_RS";

	// Turkish
//	langDesc << QString::fromUtf8("Türkçe (Onuralp SEZER)");
//	langShort << "tr";

	// Ukrainian
	langDesc << QString::fromUtf8("Українська (neeesdfsdf & zubr139)");
	langShort << "uk_UA";

	// Viatnemese
//	langDesc << QString::fromUtf8("Tiếng Việt (Nguyễn Hữu Tài)");
//	langShort << "vi";

	// Chinese (China)
	langDesc << "Chinese (Min Zhang)";
	langShort << "zh_CN";

	langDesc << "Chinese (traditional) (Min Zhang)";
	langShort << "zh_TW";

	FlowLayout *langLay = new FlowLayout;
	QButtonGroup *langButGrp = new QButtonGroup;

	for(int i = 0; i < langDesc.length(); ++i) {

		SettingsTabOtherLanguageTiles *tile = new SettingsTabOtherLanguageTiles(langDesc.at(i), langShort.at(i));
		allLangTiles << tile;
		langButGrp->addButton(tile->button);
		langLay->addWidget(tile);

	}

	QHBoxLayout *langWidgLay = new QHBoxLayout;
	langWidgLay->addSpacing(50);
	langWidgLay->addLayout(langLay);
	langWidgLay->addSpacing(50);

	layOther->addLayout(langWidgLay);
	layOther->addSpacing(30);


	// Adjust quick settings trigering
	CustomLabel *quickSetLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Quick Settings") + "</span></b><br><br>" + tr("The 'Quick Settings' is a widget hidden on the right side of the screen. When you move the cursor there, it shows up, and you can adjust a few simple settings on the spot without having to go through this settings dialog. Of course, only a small subset of settings is available (the ones needed most often). Here you can disable the dialog so that it doesn't show on mouse movement anymore."));
	quickSet = new CustomCheckBox(tr("Show 'Quick Settings' on mouse hovering"));
	QHBoxLayout *quickSetLay = new QHBoxLayout;
	quickSetLay->addStretch();
	quickSetLay->addWidget(quickSet);
	quickSetLay->addStretch();

	layOther->addWidget(quickSetLabel);
	layOther->addSpacing(20);
	layOther->addLayout(quickSetLay);
	layOther->addSpacing(30);



	// Adjust context menu
	CustomLabel *contextMenuLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Adjust Context Menu") + "</span></b><br><br>" + tr("Here you can adjust the context menu. You can simply drag and drop the entries, edit them, add a new one and remove an existing one."));
	context = new Context;
	QHBoxLayout *contextLay = new QHBoxLayout;
	contextLay->addStretch();
	contextLay->addWidget(context);
	contextLay->addStretch();
	CustomPushButton *addNew = new CustomPushButton("+ " + tr("Add new Entry"),this);
	QHBoxLayout *addNewLay = new QHBoxLayout;
	connect(addNew, SIGNAL(clicked()), context, SLOT(addNewEntry()));
	addNewLay->addStretch();
	addNewLay->addWidget(addNew);
	addNewLay->addStretch();
	layOther->addWidget(contextMenuLabel);
	layOther->addSpacing(10);
	layOther->addLayout(contextLay);
	layOther->addLayout(addNewLay);
	layOther->addSpacing(20);



	allCheckQt.clear();
	allCheckGm.clear();
	allCheckGmUntested.clear();


	/********
	 *  QT  *
	 ********/


	// Adjust known file formats
	CustomLabel *titleQt = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - Qt") + "</span></b><br><br>" + tr("These are the file types natively supported by Qt. Make sure, that you'll have the required libraries installed (e.g., qt5-imageformats), otherwise some of them might not work on your system.") + "<br>" + tr("If a file ending for one of the formats is missing, you can add it below, formatted like '*.ending' (without single quotation marks), multiple entries seperated by commas.") + "</b>");
	titleQt->setWordWrap(true);

	FlowLayout *layQt = new FlowLayout;
	QStringList formatsQt;
	formatsQt << "Bitmap" << "*.bmp, *.bitmap"
		  << "Direct Draw Surface" << "*.dds"
		  << "Graphics Interchange Format" << "*.gif"
		  << "Microsoft Icon" << "*.ico, *.icns"
		  << "Joint Photographic Experts Group" << "*.jpg, *.jpeg"
		  << "JPEG-2000" << "*.jpeg2000, *.jp2, *.jpc, *.j2k, *.jpf, *.jpx, *.jpm, *.mj2"
		  << "Multiple-image Network Graphics" << "*.mng"
		  << "Personal Icon" << "*.picon"
		  << "Portable Network Graphics" << "*.png"
		  << "Portable bitmap" << "*.pbm"
		  << "Portable graymap" << "*.pgm"
		  << "Portable pixmap" << "*.ppm"
		  << "Scalable Vector Graphics" << "*.svg, *.svgz"
		  << "Tagged Image File Format" << "*.tif, *.tiff"
		  << "Wireless bitmap" << "*.wbmp, *.webp"
		  << "X Windows system bitmap" << "*.xbm"
		  << "X Windows system pixmap" << "*.xpm";

	for(int i = 0; i < formatsQt.length()/2; ++i) {

		SettingsTabOtherFileTypesTiles *check = new SettingsTabOtherFileTypesTiles(formatsQt.at(2*i+1));
		check->setToolTip(formatsQt.at(2*i) + " (" + formatsQt.at(2*i+1) + ")");
		allCheckQt.insert(formatsQt.at(2*i+1),check);
		layQt->addWidget(check);

	}

	QHBoxLayout *layQtBut = new QHBoxLayout;
	CustomLabel *extraQt = new CustomLabel(tr("Extra File Types:"));
	extraQt->setWordWrap(false);
	extraQtEdit = new CustomLineEdit;
	CustomPushButton *qtMarkAll = new CustomPushButton(tr("Mark All"));
	CustomPushButton *qtMarkNone = new CustomPushButton(tr("Mark None"));
	layQtBut->addWidget(extraQt);
	layQtBut->addWidget(extraQtEdit);
	layQtBut->addStretch();
	layQtBut->addWidget(qtMarkAll);
	layQtBut->addWidget(qtMarkNone);

	layFile->addWidget(titleQt);
	layFile->addSpacing(10);
	layFile->addLayout(layQt);
	layFile->addSpacing(5);
	layFile->addLayout(layQtBut);
	layFile->addSpacing(35);

	QSignalMapper *mapQtMark = new QSignalMapper;
	mapQtMark->setMapping(qtMarkAll,"qtMark");
	connect(qtMarkAll, SIGNAL(clicked()), mapQtMark, SLOT(map()));
	connect(mapQtMark, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));

	QSignalMapper *mapQtNone = new QSignalMapper;
	mapQtNone->setMapping(qtMarkNone,"qtNone");
	connect(qtMarkNone, SIGNAL(clicked()), mapQtNone, SLOT(map()));
	connect(mapQtNone, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));

#ifndef GM
	CustomLabel *gmDisabled = new CustomLabel("<b><i>" + tr("Use of GraphicsMagick has been disabled as PhotoQt was compiled/installed!") + "</i></b>");
	gmDisabled->setWordWrap(true);
#endif


	/****************
	 *  GM WORKING  *
	 ****************/

	CustomLabel *titleGmWorking = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - GraphicsMagick") + "</span></b><br><br>" + tr("PhotoQt makes use of GraphicsMagick for support of many different image formats. The list below are all those formats, that were successfully displayed using test images. If you prefer not to have one or the other enabled in PhotoQt, you can simply disable individual formats below.") + "<br>" + tr("There are a few formats, that were not tested in PhotoQt (due to lack of a test image). You can find those in the 'Untested' category below.") + "</b>");
	titleGmWorking->setWordWrap(true);

	FlowLayout *layGm = new FlowLayout;
	QStringList formatsGm;
	formatsGm << "AVS X image" << "*.avs, *.x"
		  << "Continuous Acquisition and Life-cycle Support Type 1" << "*.cals, *.cal, *.dcl, *.ras"
		  << "Kodak Cineon" << "*.cin"
		  << "Dr Halo" << "*.cut"
		  << "Digital Imaging and Communications in Medicine (DICOM) image" << "*.acr, *.dcm, *.dicom, *.dic"
		  << "ZSoft IBM PC multi-page Paintbrush image" << "*.dcx"
		  << "Microsoft Windows Device Independent Bitmap" << "*.dib"
		  << "Digital Moving Picture Exchange" << "*.dpx"
		  << "Encapsulated PDF" << "*.epdf"
		  << "Group 3 FAX" << "*.fax"
		  << "Flexible Image Transport System" << "*.fits, *.fts, *.fit"
		  << "FlashPix Format" << "*.fpx"
		  << "JPEG Network Graphics" << "*.jng"
		  << "MATLAB image format" << "*.mat"
		  << "Magick image file format" << "*.miff"
		  << "Bi-level bitmap in least-significant-byte first order" << "*.mono"
		  << "MTV Raytracing image format" << "*.mtv"
		  << "On-the-air Bitmap" << "*.otb"
		  << "Xv's Visual Schnauzer thumbnail format" << "*.p7"
		  << "Palm pixmap" << "*.palm"
		  << "Portable Arbitrary Map" << "*.pam"
		  << "Photo CD" << "*.pcd, *.pcds"
		  << "ZSoft IBM PC Paintbrush file" << "*.pcx"
		  << "Palm Database ImageViewer Format" << "*.pdb"
		  << "Apple Macintosh QuickDraw/PICT file" << "*.pict, *.pct, *.pic"
		  << "Alias/Wavefront RLE image format" << "*.pix, *.pal"
		  << "Portable anymap" << "*.pnm"
		  << "Adobe Photoshop bitmap file" << "*.psd"
		  << "Pyramid encoded TIFF" << "*.ptif, *.ptiff"
		  << "Seattle File Works image" << "*.sfw"
		  << "Irix RGB image" << "*.sgi"
		  << "SUN Rasterfile" << "*.sun"
		  << "Truevision Targa image" << "*.tga"
		  << "Text files" << "*.txt"
		  << "VICAR rasterfile format" << "*.vicar"
		  << "Khoros Visualization Image File Format" << "*.viff"
		  << "Word Perfect Graphics File" << "*.wpg"
		  << "X Windows system window dump" << "*.xwd";

	for(int i = 0; i < formatsGm.length()/2; ++i) {

		SettingsTabOtherFileTypesTiles *check = new SettingsTabOtherFileTypesTiles(formatsGm.at(2*i+1));
		allCheckGm.insert(formatsGm.at(2*i+1),check);
		check->setToolTip(formatsGm.at(2*i) + " (" + formatsGm.at(2*i+1) + ")");
		layGm->addWidget(check);
#ifndef GM
		check->setEnabled(false);
#endif

	}

	QHBoxLayout *layGmBut = new QHBoxLayout;
	CustomPushButton *gmMarkAll = new CustomPushButton(tr("Mark All"));
	CustomPushButton *gmMarkNone = new CustomPushButton(tr("Mark None"));
	layGmBut->addStretch();
	layGmBut->addWidget(gmMarkAll);
	layGmBut->addWidget(gmMarkNone);

#ifndef GM
	titleGmWorking->setEnabled(false);
	gmMarkAll->setEnabled(false);
	gmMarkNone->setEnabled(false);

	layFile->addWidget(gmDisabled);
	layFile->addSpacing(10);
#endif
	layFile->addWidget(titleGmWorking);
	layFile->addSpacing(10);
	layFile->addLayout(layGm);
	layFile->addSpacing(5);
	layFile->addLayout(layGmBut);
	layFile->addSpacing(35);


	QSignalMapper *mapGmMark = new QSignalMapper;
	mapGmMark->setMapping(gmMarkAll,"gmMark");
	connect(gmMarkAll, SIGNAL(clicked()), mapGmMark, SLOT(map()));
	connect(mapGmMark, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));

	QSignalMapper *mapGmNone = new QSignalMapper;
	mapGmNone->setMapping(gmMarkNone,"gmNone");
	connect(gmMarkNone, SIGNAL(clicked()), mapGmNone, SLOT(map()));
	connect(mapGmNone, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));



	/******************************
	 *  GM WORKING GHOSTSCRIPT  *
	 ******************************/

	CustomLabel *titleGmWorkingGhostscript = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - GraphicsMagick (requires Ghostscript)") + "</span></b><br><br>" + tr("The following file types are supported by GraphicsMagick, and they have been tested and work. However, they require Ghostscript to be installed on the system.") + "</b>");
	titleGmWorkingGhostscript->setWordWrap(true);

	FlowLayout *layGmGhostscript = new FlowLayout;
	QStringList formatsGmGhostscript;
	formatsGmGhostscript << "Encapsulated PostScript" << "*.eps, *.epsf"
			     << "Encapsulated PostScript Interchange" << "*.epi, *.epsi, *.ept"
			     << "Level II Encapsulated PostScript" << "*.eps2"
			     << "Level III Encapsulated PostScript" << "*.eps3"
			     << "Portable Document Format" << "*.pdf"
			     << "Adobe PostScript" << "*.ps"
			     << "Adobe Level II PostScript" << "*.ps2"
			     << "Adobe Level III PostScript" << "*.ps3";

	for(int i = 0; i < formatsGmGhostscript.length()/2; ++i) {

		SettingsTabOtherFileTypesTiles *check = new SettingsTabOtherFileTypesTiles(formatsGmGhostscript.at(2*i+1));
		allCheckGmGhostscript.insert(formatsGmGhostscript.at(2*i+1),check);
		check->setToolTip(formatsGmGhostscript.at(2*i) + " (" + formatsGmGhostscript.at(2*i+1) + ")");
		layGmGhostscript->addWidget(check);
#ifndef GM
		check->setEnabled(false);
#endif

	}

	QHBoxLayout *layGmGhostscriptBut = new QHBoxLayout;
	CustomPushButton *gmGhostscriptMarkAll = new CustomPushButton(tr("Mark All"));
	CustomPushButton *gmGhostscriptMarkNone = new CustomPushButton(tr("Mark None"));
	layGmGhostscriptBut->addStretch();
	layGmGhostscriptBut->addWidget(gmGhostscriptMarkAll);
	layGmGhostscriptBut->addWidget(gmGhostscriptMarkNone);

#ifndef GM
	titleGmWorkingGhostscript->setEnabled(false);
	gmGhostscriptMarkAll->setEnabled(false);
	gmGhostscriptMarkNone->setEnabled(false);
#endif
	layFile->addWidget(titleGmWorkingGhostscript);
	layFile->addSpacing(10);
	layFile->addLayout(layGmGhostscript);
	layFile->addSpacing(5);
	layFile->addLayout(layGmGhostscriptBut);
	layFile->addSpacing(35);


	QSignalMapper *mapGmGhostscriptMark = new QSignalMapper;
	mapGmGhostscriptMark->setMapping(gmGhostscriptMarkAll,"gmGhostscriptMark");
	connect(gmGhostscriptMarkAll, SIGNAL(clicked()), mapGmGhostscriptMark, SLOT(map()));
	connect(mapGmGhostscriptMark, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));

	QSignalMapper *mapGmGhostscriptNone = new QSignalMapper;
	mapGmGhostscriptNone->setMapping(gmGhostscriptMarkNone,"gmGhostscriptNone");
	connect(gmGhostscriptMarkNone, SIGNAL(clicked()), mapGmGhostscriptNone, SLOT(map()));
	connect(mapGmGhostscriptNone, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));



	/*****************************
	 *  EXTERNAL TOOLS REQUIRED  *
	 *****************************/

	QFormLayout *formlay = new QFormLayout;

	CustomLabel *titleExtra = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - Other tools required") + "</span></b><br><br>" + tr("The following filetypes are supported by means of other third party tools. You first need to install them before you can use them.") + "<br><br><b>" + tr("Note") + "</b>: " + tr("If an image format is also provided by GraphicsMagick/Qt, then PhotoQt first chooses the external tool (if enabled).") + "</b>");

	// XCFTOOLS (XCF support)

	SettingsTabOtherFileTypesTiles *checkXcf = new SettingsTabOtherFileTypesTiles("*.xcf");
	checkXcf->setToolTip("Gimp XCF");
	allCheckExtra.insert("**.xcf",checkXcf);
	CustomLabel *xcfDesc = new CustomLabel(tr("Gimp's XCF file format.") + "<br><br>" + tr("Makes use of:") + " xcftools - https://github.com/j-jorge/xcftools");
	xcfDesc->setToolTip(tr("Click to open website"));
	xcfDesc->setWordWrap(false);
	CustomLabel *xcfWarning = new CustomLabel(tr("Warning: '%1' not found!").arg("xcftools"));
	xcfWarning->setFontColor("orange","orange");
	xcfWarning->hide();
	QVBoxLayout *xcfLabels = new QVBoxLayout;
	xcfLabels->addStretch();
	xcfLabels->addWidget(xcfDesc);
	xcfLabels->addWidget(xcfWarning);
	xcfLabels->addStretch();
	allExtraToolNotFound.insert("xcf2png",xcfWarning);

	QSignalMapper *mapXcf = new QSignalMapper;
	mapXcf->setMapping(xcfDesc, "https://github.com/j-jorge/xcftools");
	connect(mapXcf, SIGNAL(mapped(QString)), this, SLOT(openWebsite(QString)));
	connect(xcfDesc, SIGNAL(clicked()), mapXcf, SLOT(map()));

	formlay->addRow(checkXcf,xcfLabels);

	// LIBQPSD (PSD support)

	SettingsTabOtherFileTypesTiles *checkPsd = new SettingsTabOtherFileTypesTiles("*.psb, *.psd");
	checkPsd->setToolTip("Adobe PSD/PSB");
	allCheckExtra.insert("**.psb, **.psd",checkPsd);
	CustomLabel *psdDesc = new CustomLabel(tr("Adobe Photoshop PSD and PSB.") + "<br><br>" + tr("Makes use of:") + " libqpsd - https://github.com/Code-ReaQtor/libqpsd");
	psdDesc->setToolTip(tr("Click to open website"));
	psdDesc->setWordWrap(false);
	CustomLabel *psdWarning = new CustomLabel(tr("Warning: Qt5 plugin for PSD/PSB (%1) not found!").arg("libqpsd"));
	psdWarning->setFontColor("orange","orange");
	psdWarning->hide();
	QVBoxLayout *psdLabels = new QVBoxLayout;
	psdLabels->addStretch();
	psdLabels->addWidget(psdDesc);
	psdLabels->addWidget(psdWarning);
	psdLabels->addStretch();
	allExtraToolNotFound.insert("libqpsd",psdWarning);

	QSignalMapper *mapPsd = new QSignalMapper;
	mapPsd->setMapping(psdDesc, "https://github.com/Code-ReaQtor/libqpsd");
	connect(mapPsd, SIGNAL(mapped(QString)), this, SLOT(openWebsite(QString)));
	connect(psdDesc, SIGNAL(clicked()), mapPsd, SLOT(map()));

	formlay->addRow(checkPsd,psdLabels);


	QHBoxLayout *extrasLay = new QHBoxLayout;
	extrasLay->addStretch();
	extrasLay->addLayout(formlay);
	extrasLay->addStretch();

	layFile->addWidget(titleExtra);
	layFile->addSpacing(10);
	layFile->addLayout(extrasLay);
	layFile->addSpacing(10);



	/*****************
	 *  GM UNTESTED  *
	 *****************/

	CustomLabel *titleGmUntested = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - GraphicsMagick (Untested)") + "</span></b><br><br>"
						       + tr("The following file types are generally supported by GraphicsMagick, but I wasn't able to test them in PhotoQt (due to lack of test images). They might very well be working, but I simply can't say. If you decide to enable some of the, the worst that could happen ist, that you see an error image instead of the actual image.") + "<br><br>"
						       + "<i>" + tr("If you happen to have an image in one of those formats and don't mind sending it to me, that'd be really cool...") + "</i>");
	titleGmUntested->setWordWrap(true);

	FlowLayout *layGmUntested = new FlowLayout;
	QStringList formatsGmUntested;
	formatsGmUntested << "HP-GL plotter language" << "*.hp, *.hpgl"
			  << "Joint Bi-level Image experts Group file interchange format" << "*.jbig, *.jbg"
			  << "Seattle File Works multi-image file" << "*.pwp"
			  << "Sun Raster Image" << "*.rast"
			  << "Alias/Wavefront image" << "*.rla"
			  << "Utah Run length encoded image" << "*.rle"
			  << "Scitex Continuous Tone Picture" << "*.sct"
			  << "PSX TIM file" << "*.tim";

	for(int i = 0; i < formatsGmUntested.length()/2; ++i) {

		SettingsTabOtherFileTypesTiles *check = new SettingsTabOtherFileTypesTiles(formatsGmUntested.at(2*i+1));
		check->setToolTip(formatsGmUntested.at(2*i) + "(" + formatsGmUntested.at(2*i+1) + ")");
		allCheckGmUntested.insert(formatsGmUntested.at(2*i+1),check);
		layGmUntested->addWidget(check);
#ifndef GM
		check->setEnabled(false);
#endif

	}

	QHBoxLayout *layGmUntestedBut = new QHBoxLayout;
	CustomPushButton *gmUntestedMarkAll = new CustomPushButton(tr("Mark All"));
	CustomPushButton *gmUntestedMarkNone = new CustomPushButton(tr("Mark None"));
	layGmUntestedBut->addStretch();
	layGmUntestedBut->addWidget(gmUntestedMarkAll);
	layGmUntestedBut->addWidget(gmUntestedMarkNone);

	QSignalMapper *mapGmUntestedMark = new QSignalMapper;
	mapGmUntestedMark->setMapping(gmUntestedMarkAll,"gmUntestedMark");
	connect(gmUntestedMarkAll, SIGNAL(clicked()), mapGmUntestedMark, SLOT(map()));
	connect(mapGmUntestedMark, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));

	QSignalMapper *mapGmUntestedNone = new QSignalMapper;
	mapGmUntestedNone->setMapping(gmUntestedMarkNone,"gmUntestedNone");
	connect(gmUntestedMarkNone, SIGNAL(clicked()), mapGmUntestedNone, SLOT(map()));
	connect(mapGmUntestedNone, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));

	layFile->addWidget(titleGmUntested);
	layFile->addSpacing(10);
	layFile->addLayout(layGmUntested);
	layFile->addSpacing(5);
	layFile->addLayout(layGmUntestedBut);
	layFile->addSpacing(35);



	layOther->addStretch();
	layFile->addStretch();

}

// Load the settings
void SettingsTabOther::loadSettings() {

	if(verbose) std::clog << "Load Settings (Other)" << std::endl;

	context->loadContext();

	defaults.clear();

	// Get translation set. If none set, take system locale
	QString code1 = globSet.value("Language").toString();
	QString code2 = code1;
	if(code1.trimmed() == "") {
		code1 = QLocale::system().name();
		code2 = QLocale::system().name().split("_").at(0);
	}
	// Check appropriate language tile
	for(int i = 0; i < allLangTiles.length(); ++i) {
		if(code1 == allLangTiles.at(i)->langCode || code2 == allLangTiles.at(i)->langCode) {
			allLangTiles.at(i)->button->setChecked(true);
			defaults.insert("Language",globSet.value("Language").toString());
			break;
		}
	}

	quickSet->setChecked(globSet.value("QuickSettings").toBool());
	defaultValue = quickSet->isChecked();

	// Qt file formats
	QStringList formatsSetQt = globSet.value("KnownFileTypesQt").toString().split(",");
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterQt(allCheckQt);
	while (iterQt.hasNext()) {
		iterQt.next();
		QStringList types_part = QString(iterQt.key()).split(", ",QString::SkipEmptyParts);
		iterQt.value()->setChecked(false);
		for(int i = 0; i < types_part.length(); ++i) {
			if(formatsSetQt.contains(types_part.at(i))) {
				iterQt.value()->setChecked(true);
				break;
			}

		}
	}

	// Extras file formats
	QStringList formatsSetExtra = globSet.value("KnownFileTypesExtras").toString().split(",");
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterExtra(allCheckExtra);
	while (iterExtra.hasNext()) {
		iterExtra.next();
		QStringList types_part = QString(iterExtra.key()).split(", ",QString::SkipEmptyParts);
		iterExtra.value()->setChecked(false);
		for(int i = 0; i < types_part.length(); ++i) {
			if(formatsSetExtra.contains(types_part.at(i))) {
				iterExtra.value()->setChecked(true);
				break;
			}

		}
	}

#ifdef GM
	QStringList formatsSetGm = globSet.value("KnownFileTypesGm").toString().split(",");
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGm(allCheckGm);
	while (iterGm.hasNext()) {
		iterGm.next();

		QStringList types_part = QString(iterGm.key()).split(", ",QString::SkipEmptyParts);
		iterGm.value()->setChecked(false);
		for(int i = 0; i < types_part.length(); ++i) {
			if(formatsSetGm.contains(types_part.at(i))) {
				iterGm.value()->setChecked(true);
				break;
			}

		}
	}
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmGhostscript(allCheckGmGhostscript);
	while (iterGmGhostscript.hasNext()) {
		iterGmGhostscript.next();

		QStringList types_part = QString(iterGmGhostscript.key()).split(", ",QString::SkipEmptyParts);
		iterGmGhostscript.value()->setChecked(false);
		for(int i = 0; i < types_part.length(); ++i) {
			if(formatsSetGm.contains(types_part.at(i))) {
				iterGmGhostscript.value()->setChecked(true);
				break;
			}

		}
	}
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmUntested(allCheckGmUntested);
	while (iterGmUntested.hasNext()) {
		iterGmUntested.next();

		QStringList types_part = QString(iterGmUntested.key()).split(", ",QString::SkipEmptyParts);
		iterGmUntested.value()->setChecked(false);
		for(int i = 0; i < types_part.length(); ++i) {
			if(formatsSetGm.contains(types_part.at(i))) {
				iterGmUntested.value()->setChecked(true);
				break;
			}

		}
	}
#endif

	extraQtEdit->setText(globSet.value("KnownFileTypesQtExtras").toString());

	// Also update visibility of warning labels for Extra filetypes
	QMapIterator<QString, CustomLabel*> iterExtraWarning(allExtraToolNotFound);
	while(iterExtraWarning.hasNext()) {
		iterExtraWarning.next();
		if(iterExtraWarning.key() == "libqpsd") {
			qDebug() << QImageReader::supportedImageFormats();
			iterExtraWarning.value()->setVisible(!QImageReader::supportedImageFormats().contains("psd"));
		} else {
			QProcess which;
			which.setStandardOutputFile(QProcess::nullDevice());
			which.start(QString("which %1").arg(iterExtraWarning.key()));
			which.waitForFinished();
			// If it isn't -> display error
			iterExtraWarning.value()->setVisible(which.exitCode());
		}
	}


}

// Save the settings
void SettingsTabOther::saveSettings() {

	if(verbose) std::clog << "Save Settings (Other)" << std::endl;

	updatedSet.clear();

	for(int i = 0; i < allLangTiles.length(); ++i) {
		if(allLangTiles.at(i)->button->isChecked()) {
			if(defaults.value("Language").toString() != allLangTiles.at(i)->langCode) {
				updatedSet.insert("Language",allLangTiles.at(i)->langCode);
				defaults.remove("Language");
				defaults.insert("Language",allLangTiles.at(i)->langCode);
			}
			break;
		}
	}


	if(defaultValue != quickSet->isChecked())
		updatedSet.insert("QuickSettings",quickSet->isChecked());


	QStringList formatsSetQt;
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterQt(allCheckQt);
	while (iterQt.hasNext()) {
		iterQt.next();
		if(iterQt.value()->isChecked()) {
			formatsSetQt.append(QString(iterQt.key()).split(", ",QString::SkipEmptyParts));
		}
	}
	updatedSet.insert("KnownFileTypesQt",formatsSetQt.join(","));

	QStringList formatsSetExtras;
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterExtra(allCheckExtra);
	while (iterExtra.hasNext()) {
		iterExtra.next();
		if(iterExtra.value()->isChecked()) {
			formatsSetExtras.append(QString(iterExtra.key()).split(", ",QString::SkipEmptyParts));
		}
	}
	updatedSet.insert("KnownFileTypesExtras",formatsSetExtras.join(","));

	QStringList formatsSetGm;
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGm(allCheckGm);
	while (iterGm.hasNext()) {
		iterGm.next();
		if(iterGm.value()->isChecked()) formatsSetGm.append(QString(iterGm.key()).split(", ",QString::SkipEmptyParts));
	}
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmGhostscript(allCheckGmGhostscript);
	while (iterGmGhostscript.hasNext()) {
		iterGmGhostscript.next();
		if(iterGmGhostscript.value()->isChecked()) formatsSetGm.append(QString(iterGmGhostscript.key()).split(", ",QString::SkipEmptyParts));
	}
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmUntested(allCheckGmUntested);
	while (iterGmUntested.hasNext()) {
		iterGmUntested.next();
		if(iterGmUntested.value()->isChecked()) formatsSetGm.append(QString(iterGmUntested.key()).split(", ",QString::SkipEmptyParts));
	}
	updatedSet.insert("KnownFileTypesGm",formatsSetGm.join(","));

	updatedSet.insert("KnownFileTypesQtExtras",extraQtEdit->text());

	context->saveContext();

}


void SettingsTabOther::markAllNone(QString cat) {

	if(cat.startsWith("qt")) {

		QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterQt(allCheckQt);
		while (iterQt.hasNext()) {
			iterQt.next();
			iterQt.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	} else if(cat.startsWith("gmUntested")) {

		QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmUntested(allCheckGmUntested);
		while (iterGmUntested.hasNext()) {
			iterGmUntested.next();
			iterGmUntested.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	} else if(cat.startsWith("gmGhostscript")) {

		QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmGhostscriptUntested(allCheckGmGhostscript);
		while (iterGmGhostscriptUntested.hasNext()) {
			iterGmGhostscriptUntested.next();
			iterGmGhostscriptUntested.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	} else if(cat.startsWith("gm")) {

		QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGm(allCheckGm);
		while (iterGm.hasNext()) {
			iterGm.next();
			iterGm.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	}

}

SettingsTabOther::~SettingsTabOther() { }
