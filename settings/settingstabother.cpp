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
	allCheckGmUnstable.clear();


	// Adjust known file formats
	CustomLabel *titleQt = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - Qt") + "</span></b><br><br>" + tr("These are the standard file types supported by Qt. Depending on your system, this list can vary a little.") + "<br>" + tr("If you want to add a file type not in the list, you can add them in the text box below. You have to enter the formats like '*.ending', all seperated by commas.") + "</b>");
	titleQt->setWordWrap(true);

	FlowLayout *layQt = new FlowLayout;
	QStringList formatsQt;
	formatsQt << ".bmp" << ".gif" << ".tif" << ".tiff" << ".jpeg2000" << ".jpeg" << ".jpg" << ".png" << ".pbm" << ".pgm" << ".ppm" << ".xbm" << ".xpm";
	formatsQt.sort();
	for(int i = 0; i < formatsQt.length(); ++i) {

		SettingsTabOtherFileTypesTiles *check = new SettingsTabOtherFileTypesTiles(formatsQt.at(i));
		check->setToolTip(formatsQt.at(i));
		allCheckQt.insert(formatsQt.at(i),check);
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

	CustomLabel *titleGmWorking = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - GraphicsMagick") + "</span></b><br><br>" + tr("PhotoQt makes use of GraphicsMagick for support of many different file types. Not all of the formats supported by GraphicsMagick make sense in an image viewer. There are some that aren't quite working at the moment, you can find them in the 'Unstable' category below.") + "<br>" + tr("If you want to add a file type not in the list, you can add them in the text box below. You have to enter the formats like '*.ending', all seperated by commas.") + "</b>");
	titleGmWorking->setWordWrap(true);

	FlowLayout *layGm = new FlowLayout;
	QStringList formatsGm;
	formatsGm << ".art" << ".avs" << ".x" << ".cals" << ".cgm" << ".cur" << ".cut" << ".acr" << ".dcm" << ".dicom" << ".dic" << ".dcx" << ".dib" << ".dpx" << ".emf" << ".epdf" << ".epi" << ".eps" << ".eps2" << ".eps3" << ".epsf" << ".epsi" << ".ept" << ".fax" << ".fig" << ".fits" << ".fts" << ".fit" << ".fpx" << ".gplt" << ".ico" << ".jbg" << ".jbig" << ".jng" << ".jp2" << ".j2k" << ".jpf" << ".jpx" << ".jpm" << ".mj2" << ".jpc" << ".mat" << ".miff" << ".mng" << ".mpc" << ".mtv" << ".otb" << ".p7" << ".palm" << ".pam" << ".pcd" << ".pcds" << ".pcx" << ".pdb" << ".pdf" << ".picon" << ".pict" << ".pct" << ".pic" << ".pix" << ".pnm" << ".ps" << ".ps2" << ".ps3" << ".psd" << ".ptif" << ".ras" << ".rast" << ".rad" << ".sgi" << ".sun" << ".svg" << ".tga" << ".vicar" << ".viff" << ".wbmp" << ".wbm" << ".xcf" << ".xwd";
	formatsGm.sort();
	for(int i = 0; i < formatsGm.length(); ++i) {

		SettingsTabOtherFileTypesTiles *check = new SettingsTabOtherFileTypesTiles(formatsGm.at(i));
		allCheckGm.insert(formatsGm.at(i),check);
		check->setToolTip(formatsGm.at(i));
		layGm->addWidget(check);
#ifndef GM
		check->setEnabled(false);
#endif

	}

	QHBoxLayout *layGmBut = new QHBoxLayout;
	CustomLabel *extraGm = new CustomLabel(tr("Extra File Types:"));
	extraGm->setWordWrap(false);
	extraGmEdit = new CustomLineEdit;
	CustomPushButton *gmMarkAll = new CustomPushButton(tr("Mark All"));
	CustomPushButton *gmMarkNone = new CustomPushButton(tr("Mark None"));
	layGmBut->addWidget(extraGm);
	layGmBut->addWidget(extraGmEdit);
	layGmBut->addStretch();
	layGmBut->addWidget(gmMarkAll);
	layGmBut->addWidget(gmMarkNone);

#ifndef GM
	titleGmWorking->setEnabled(false);
	gmMarkAll->setEnabled(false);
	gmMarkNone->setEnabled(false);
	extraGm->setEnabled(false);
	extraGmEdit->setEnabled(false);

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





	CustomLabel *titleGmUnstable = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("File Types - GraphicsMagick (Unstable)") + "</span></b><br><br>" + tr("The following file types are supported by GraphicsMagick, but aren't quite working in PhotoQt just yet. If you want to experiment around a little, feel free to enable some of them. They shouldn't cause PhotoQt to crash, but you might see an error image instead of the actual image.") + "</b>");
	titleGmUnstable->setWordWrap(true);

	FlowLayout *layGmUnstable = new FlowLayout;
	QStringList formatsGmUnstable;
	formatsGmUnstable << ".gray" << ".hpgl" << ".mono" << ".msl" << ".mvg" << ".pcl" << ".pfa" << ".pfb" << ".pwp" << ".rgb" << ".rgba" << ".rla" << ".rle" << ".sct" << ".sfw" << ".tim" << ".uil" << ".uyvy" << ".wmf" << ".wpg" << ".yuv";
	formatsGmUnstable.sort();
	for(int i = 0; i < formatsGmUnstable.length(); ++i) {

		SettingsTabOtherFileTypesTiles *check = new SettingsTabOtherFileTypesTiles(formatsGmUnstable.at(i));
		check->setToolTip(formatsGmUnstable.at(i));
		allCheckGmUnstable.insert(formatsGmUnstable.at(i),check);
		layGmUnstable->addWidget(check);
#ifndef GM
		check->setEnabled(false);
#endif

	}

	QHBoxLayout *layGmButUnstable = new QHBoxLayout;
	CustomPushButton *gmMarkAllUnstable = new CustomPushButton(tr("Mark All"));
	CustomPushButton *gmMarkNoneUnstable = new CustomPushButton(tr("Mark None"));
	layGmButUnstable->addStretch();
	layGmButUnstable->addWidget(gmMarkAllUnstable);
	layGmButUnstable->addWidget(gmMarkNoneUnstable);

	layFile->addWidget(titleGmUnstable);
	layFile->addSpacing(10);
	layFile->addLayout(layGmUnstable);
	layFile->addSpacing(5);
	layFile->addLayout(layGmButUnstable);
	layFile->addSpacing(35);

#ifndef GM
	titleGmUnstable->setEnabled(false);
	gmMarkAllUnstable->setEnabled(false);
	gmMarkNoneUnstable->setEnabled(false);
#endif

	QSignalMapper *mapGmMarkUnst = new QSignalMapper;
	mapGmMarkUnst->setMapping(gmMarkAllUnstable,"gmunstMark");
	connect(gmMarkAllUnstable, SIGNAL(clicked()), mapGmMarkUnst, SLOT(map()));
	connect(mapGmMarkUnst, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));

	QSignalMapper *mapGmNoneUnst = new QSignalMapper;
	mapGmNoneUnst->setMapping(gmMarkNoneUnstable,"gmunstNone");
	connect(gmMarkNoneUnstable, SIGNAL(clicked()), mapGmNoneUnst, SLOT(map()));
	connect(mapGmNoneUnst, SIGNAL(mapped(QString)), this, SLOT(markAllNone(QString)));



	layOther->addStretch();
	layFile->addStretch();

}

// Load the settings
void SettingsTabOther::loadSettings() {

	if(verbose) std::clog << "Load Settings (Other)" << std::endl;

	context->loadContext();

	defaults.clear();

	for(int i = 0; i < allLangTiles.length(); ++i) {
		if(globSet.value("Language").toString() == allLangTiles.at(i)->langCode) {
			allLangTiles.at(i)->button->setChecked(true);
			defaults.insert("Language",globSet.value("Language").toString());
			break;
		}
	}

	quickSet->setChecked(globSet.value("QuickSettings").toBool());
	defaultValue = quickSet->isChecked();

	QStringList formatsSetQt = globSet.value("KnownFileTypesQt").toString().replace("*","").split(",");
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterQt(allCheckQt);
	while (iterQt.hasNext()) {
		iterQt.next();
		iterQt.value()->setChecked(formatsSetQt.contains(iterQt.key()) ? true : false);
	}

#ifdef GM
	QStringList formatsSetGm = globSet.value("KnownFileTypesGm").toString().replace("*","").split(",");
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGm(allCheckGm);
	while (iterGm.hasNext()) {
		iterGm.next();
		iterGm.value()->setChecked(formatsSetGm.contains(iterGm.key()) ? true : false);
	}
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmUnstable(allCheckGmUnstable);
	while (iterGmUnstable.hasNext()) {
		iterGmUnstable.next();
		iterGmUnstable.value()->setChecked(formatsSetGm.contains(iterGmUnstable.key()) ? true : false);
	}
#endif

	extraQtEdit->setText(globSet.value("KnownFileTypesQtExtras").toString());
	extraGmEdit->setText(globSet.value("KnownFileTypesGmExtras").toString());


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
		if(iterQt.value()->isChecked()) formatsSetQt.append("*" + iterQt.key());
	}
	updatedSet.insert("KnownFileTypesQt",formatsSetQt.join(","));

	QStringList formatsSetGm;
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGm(allCheckGm);
	while (iterGm.hasNext()) {
		iterGm.next();
		if(iterGm.value()->isChecked()) formatsSetGm.append("*" + iterGm.key());
	}
	QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmUnstable(allCheckGmUnstable);
	while (iterGmUnstable.hasNext()) {
		iterGmUnstable.next();
		if(iterGmUnstable.value()->isChecked()) formatsSetGm.append("*" + iterGmUnstable.key());
	}
	updatedSet.insert("KnownFileTypesGm",formatsSetGm.join(","));

	updatedSet.insert("KnownFileTypesQtExtras",extraQtEdit->text());
	updatedSet.insert("KnownFileTypesGmExtras",extraGmEdit->text());

	context->saveContext();

}


void SettingsTabOther::markAllNone(QString cat) {

	if(cat.startsWith("qt")) {

		QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterQt(allCheckQt);
		while (iterQt.hasNext()) {
			iterQt.next();
			iterQt.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	} else if(cat.startsWith("gmunst")) {

		QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGmUnstable(allCheckGmUnstable);
		while (iterGmUnstable.hasNext()) {
			iterGmUnstable.next();
			iterGmUnstable.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	} else if(cat.startsWith("gm")) {

		QMapIterator<QString, SettingsTabOtherFileTypesTiles*> iterGm(allCheckGm);
		while (iterGm.hasNext()) {
			iterGm.next();
			iterGm.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	}

}



void SettingsTabOther::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabOther::~SettingsTabOther() { }
