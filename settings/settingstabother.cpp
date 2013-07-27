#include "settingstabother.h"

SettingsTabOther::SettingsTabOther(QWidget *parent, QMap<QString, QVariant> set, bool v) : QWidget(parent) {

	// The global settings
	globSet = set;

	verbose = v;

	this->setStyleSheet("background: transparent; color: white");




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
	QLabel *titleOther = new QLabel("<center><h1>" + tr("Other Settings") + "</h1></center>");
	layOther->addWidget(titleOther);
	layOther->addSpacing(20);
	QLabel *titleFile = new QLabel("<center><h1>" + tr("Known File Types") + "</h1></center>");
	layFile->addWidget(titleFile);
	layFile->addSpacing(20);



	// CHOOSE A LANGUAGE
	QLabel *langLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Choose Language") + "</span></b><br><br>" + tr("There are a good few different languages available. Thanks to everybody who took the time to translate PhotoQt!"));
	langLabel->setWordWrap(true);
	layOther->addWidget(langLabel);
	layOther->addSpacing(15);


	// All the languages available. They are sorted according to their language code (except English)

	langDesc << "English";
	langShort << "en";

	// Arabic
	langDesc << QString::fromUtf8("العربية (Amar Tahaif)");
	langShort << "ar";

	// Czech
	langDesc << QString::fromUtf8("Čeština (Tomáš Bartl)");
	langShort << "cs";

	// German
	langDesc << "Deutsch (Lukas Spies)";
	langShort << "de";

	// Greek
	langDesc << QString::fromUtf8("Ελληνικά (Dimitrios Glentadakis)");
	langShort << "el";

	// Spanish
	langDesc << QString::fromUtf8("Español (Manuel E. Gutierrez)");
	langShort << "es_ES";

	// French
	langDesc << QString::fromUtf8("Français (Mathieu Marache & Olivier Devineau)");
	langShort << "fr";

	// Hungarian
	langDesc << QString::fromUtf8("Magyar (Zoltan Hoppár)");
	langShort << "hu";

	// Italian
	langDesc << "Italiano (Vincenzo Cerminara)";
	langShort << "it";

	// Norwegian Bokmal
	langDesc << QString::fromUtf8("Bokmål (Ola Haugen Havrevoll)");
	langShort << "nb_NO";

	// Norwegian Nynorsk
	langDesc << "Nynorsk (Ola Haugen Havrevoll)";
	langShort << "nn_NO";

	// Polish
	langDesc << "Polski (Daniel Korbel)";
	langShort << "pl";

	// Russian
	langDesc << QString::fromUtf8("Pусский (Yuriy Tkach)");
	langShort << "ru_RU";

	//Slovakian
	langDesc << QString::fromUtf8("Slovenčina (Lukáš Dobránsky)");
	langShort << "sk";

	// Serbian
	langDesc << QString::fromUtf8("српски екавски (Mladen Pejaković)");
	langShort << "sr_RS";

	// Turkish
	langDesc << QString::fromUtf8("Türkçe (Onuralp SEZER)");
	langShort << "tr";

	// Ukrainian
	langDesc << QString::fromUtf8("Українська (Yuriy Tkach)");
	langShort << "uk_UA";

	// Viatnemese
	langDesc << QString::fromUtf8("Tiếng Việt (Nguyễn Hữu Tài)");
	langShort << "vi";

	// The Chinese translation hasn't been updated in a long time, and only 3% are currently translated. It's removed for now from the PhotoQt app, but remains at Transifex, and if somebody starts updating it again, it'll get included again.
	// Chinese
//	langDesc << "Chinese (Dianjin Wang)";
//	langShort << "zh_CN";

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


	// Adjust context menu
	QLabel *contextMenuLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Adjust Context Menu") + "</span></b><br><br>" + tr("Here you can adjust the context menu. You can simply drag and drop the entries, edit them, add a new one and remove an existing one."));
	contextMenuLabel->setWordWrap(true);
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
//	QLabel *knownLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Known File Types") + "</span></b><br><br>" + tr("Here you can adjust the list of known image types. Some image types (especially more exotic ones) aren't supported by every Qt installation (depending on which image plugins you've got installed, etc.). If there's any problem with an image type, you can take it out of the list here. If you want to add an image type, simply add it to the list.") + "<br><b>" + tr("Only change the list if you know what you're doing!") + "</b>");
//	knownLabel->setWordWrap(true);

	QLabel *titleQt = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Qt File Types") + "</span></b><br><br>" + tr("Qt File Types") + "</b>");
	titleQt->setWordWrap(true);

	QGridLayout *layQt = new QGridLayout;
	QStringList formatsQt;
	formatsQt << ".bmp" << ".gif" << ".tif" << ".tiff" << ".jpeg2000" << ".jpeg" << ".jpg" << ".png" << ".pbm" << ".pgm" << ".ppm" << ".xbm" << ".xpm";
	formatsQt.sort();
	for(int i = 0; i < formatsQt.length(); ++i) {

		CustomCheckBox *check = new CustomCheckBox(formatsQt.at(i));
		allCheckQt.insert(formatsQt.at(i),check);
		layQt->addWidget(check,i/10,i%10);

	}

	QHBoxLayout *layQtBut = new QHBoxLayout;
	CustomLabel *extraQt = new CustomLabel(tr("Extra File Types:"));
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



	QLabel *titleGmWorking = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Gm File Types") + "</span></b><br><br>" + tr("Known to be working Gm File Types") + "</b>");

	QGridLayout *layGm = new QGridLayout;
	QStringList formatsGm;
	formatsGm << ".art" << ".avs" << ".x" << ".cals" << ".cgm" << ".cur" << ".cut" << ".acr" << ".dcm" << ".dicom" << ".dic" << ".dcx" << ".dib" << ".dpx" << ".emf" << ".epdf" << ".epi" << ".eps" << ".eps2" << ".eps3" << ".epsf" << ".epsi" << ".ept" << ".fax" << ".fig" << ".fits" << ".fts" << ".fit" << ".fpx" << ".gplt" << ".ico" << ".jbg" << ".jbig" << ".jng" << ".jp2" << ".j2k" << ".jpf" << ".jpx" << ".jpm" << ".mj2" << ".jpc" << ".mat" << ".miff" << ".mng" << ".mpc" << ".mtv" << ".otb" << ".p7" << ".palm" << ".pam" << ".pcd" << ".pcds" << ".pcx" << ".pdb" << ".pdf" << ".picon" << ".pict" << ".pct" << ".pic" << ".pix" << ".pnm" << ".ps" << ".ps2" << ".ps3" << ".psd" << ".ptif" << ".ras" << ".rast" << ".rad" << ".sgi" << ".sun" << ".svg" << ".tga" << ".vicar" << ".viff" << ".wbmp" << ".wbm" << ".xcf" << ".xwd";
	formatsGm.sort();
	for(int i = 0; i < formatsGm.length(); ++i) {

		CustomCheckBox *check = new CustomCheckBox(formatsGm.at(i));
		allCheckGm.insert(formatsGm.at(i),check);
		layGm->addWidget(check,i/10,i%10);

	}

	QHBoxLayout *layGmBut = new QHBoxLayout;
	CustomLabel *extraGm = new CustomLabel(tr("Extra File Types:"));
	extraGmEdit = new CustomLineEdit;
	CustomPushButton *gmMarkAll = new CustomPushButton(tr("Mark All"));
	CustomPushButton *gmMarkNone = new CustomPushButton(tr("Mark None"));
	layGmBut->addWidget(extraGm);
	layGmBut->addWidget(extraGmEdit);
	layGmBut->addStretch();
	layGmBut->addWidget(gmMarkAll);
	layGmBut->addWidget(gmMarkNone);

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





	QLabel *titleGmUnstable = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Unstable Gm File Types") + "</span></b><br><br>" + tr("Unstable Gm File Types") + "</b>");
	titleGmUnstable->setWordWrap(true);

	QGridLayout *layGmUnstable = new QGridLayout;
	QStringList formatsGmUnstable;
	formatsGmUnstable << ".gray" << ".hpgl" << ".mono" << ".msl" << ".mvg" << ".pcl" << ".pfa" << ".pfb" << ".pwp" << ".rgb" << ".rgba" << ".rla" << ".rle" << ".sct" << ".sfw" << ".tim" << ".uil" << ".uyvy" << ".wmf" << ".wpg" << ".yuv";
	formatsGmUnstable.sort();
	for(int i = 0; i < formatsGmUnstable.length(); ++i) {

		CustomCheckBox *check = new CustomCheckBox(formatsGmUnstable.at(i));
		allCheckGmUnstable.insert(formatsGmUnstable.at(i),check);
		layGmUnstable->addWidget(check,i/10,i%10);

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

	if(verbose) qDebug() << "Load Settings (Other)";

	context->loadContext();

	defaults.clear();

	for(int i = 0; i < allLangTiles.length(); ++i) {
		if(globSet.value("Language").toString() == allLangTiles.at(i)->langCode) {
			allLangTiles.at(i)->button->setChecked(true);
			defaults.insert("Language",globSet.value("Language").toString());
			break;
		}
	}

	QStringList formatsSetQt = globSet.value("KnownFileTypesQt").toString().replace("*","").split(",");
	QMapIterator<QString, CustomCheckBox*> iterQt(allCheckQt);
	while (iterQt.hasNext()) {
		iterQt.next();
		iterQt.value()->setChecked(formatsSetQt.contains(iterQt.key()) ? true : false);
	}

	QStringList formatsSetGm = globSet.value("KnownFileTypesGm").toString().replace("*","").split(",");
	QMapIterator<QString, CustomCheckBox*> iterGm(allCheckGm);
	while (iterGm.hasNext()) {
		iterGm.next();
		iterGm.value()->setChecked(formatsSetGm.contains(iterGm.key()) ? true : false);
	}
	QMapIterator<QString, CustomCheckBox*> iterGmUnstable(allCheckGmUnstable);
	while (iterGmUnstable.hasNext()) {
		iterGmUnstable.next();
		iterGmUnstable.value()->setChecked(formatsSetGm.contains(iterGmUnstable.key()) ? true : false);
	}

	extraQtEdit->setText(globSet.value("KnownFileTypesQtExtras").toString());
	extraGmEdit->setText(globSet.value("KnownFileTypesGmExtras").toString());


}

// Save the settings
void SettingsTabOther::saveSettings() {

	if(verbose) qDebug() << "Save Settings (Other)";

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

	QStringList formatsSetQt;
	QMapIterator<QString, CustomCheckBox*> iterQt(allCheckQt);
	while (iterQt.hasNext()) {
		iterQt.next();
		if(iterQt.value()->isChecked()) formatsSetQt.append("*" + iterQt.key());
	}
	updatedSet.insert("KnownFileTypesQt",formatsSetQt.join(","));

	QStringList formatsSetGm;
	QMapIterator<QString, CustomCheckBox*> iterGm(allCheckGm);
	while (iterGm.hasNext()) {
		iterGm.next();
		if(iterGm.value()->isChecked()) formatsSetGm.append("*" + iterGm.key());
	}
	QMapIterator<QString, CustomCheckBox*> iterGmUnstable(allCheckGmUnstable);
	while (iterGmUnstable.hasNext()) {
		iterGmUnstable.next();
		if(iterGmUnstable.value()->isChecked()) formatsSetGm.append("*" + iterGmUnstable.key());
	}
	updatedSet.insert("KnownFileTypesGm",formatsSetGm.join(","));

	updatedSet.insert("KnownFileTypesQtExtras",extraQtEdit->text());
	updatedSet.insert("KnownFileTypesGmExtras",extraGmEdit->text());

	context->saveContext();

	qDebug() << "OTHER SETTINGS:" << updatedSet;

}


void SettingsTabOther::markAllNone(QString cat) {

	if(cat.startsWith("qt")) {

		QMapIterator<QString, CustomCheckBox*> iterQt(allCheckQt);
		while (iterQt.hasNext()) {
			iterQt.next();
			iterQt.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	} else if(cat.startsWith("gmunst")) {

		QMapIterator<QString, CustomCheckBox*> iterGmUnstable(allCheckGmUnstable);
		while (iterGmUnstable.hasNext()) {
			iterGmUnstable.next();
			iterGmUnstable.value()->setChecked(cat.endsWith("Mark") ? true : false);
		}

	} else if(cat.startsWith("gm")) {

		QMapIterator<QString, CustomCheckBox*> iterGm(allCheckGm);
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
