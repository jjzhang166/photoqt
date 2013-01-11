#include "settingstabshortcuts2.h"

SettingsTabShortcuts2::SettingsTabShortcuts2(QWidget *parent) : QWidget(parent) {

	this->setStyleSheet("background: transparent; color: white;");


	CustomScrollbar *scrollbar = new CustomScrollbar;
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


	keyDetect = new ShortcutKeyDetect(this->parentWidget());
	connect(keyDetect, SIGNAL(gotKeys(QString,QString,int)), this, SLOT(detectCombo(QString,QString,int)));


	setup = false;



	QLabel *title = new QLabel("<center><h1>Shortcuts</h1></center>");

	QLabel *desc = new QLabel("Here you can adjust the shortcuts, add new ones, remove existing ones or change a key combination. The shortcuts are grouped into 4 different categories for internal commands plus a category for external commands. The boxes on the right side contain all the existing shortcuts. To add a shortcut for one of the available function you can either double click on the tiles or click the \"+\" button. This automatically opens another widget where you can set a key combination.");
	desc->setWordWrap(true);
	desc->setStyleSheet("background: transparent;");

	lay->addWidget(title);
	lay->addWidget(desc);



	navigationIntern << "__gotoFirstThb";
	navigationInternRead << tr("Go to first Image");

	navigationIntern << "__gotoLastThb";
	navigationInternRead << tr("Go to last Image");

	navigationIntern << "__hide";
	navigationInternRead << tr("Hide to System Tray");

	navigationIntern << "__next";
	navigationInternRead << tr("Next Image");

	navigationIntern << "__open";
	navigationInternRead << tr("Open new file");

	navigationIntern << "__prev";
	navigationInternRead << tr("Previous Image");

	navigationIntern << "__close";
	navigationInternRead << tr("Quit photo");



	imageIntern << "__zoomIn";
	imageInternRead << tr("Zoom In");

	imageIntern << "__zoomOut";
	imageInternRead << tr("Zoom Out");

	imageIntern << "__zoomReset";
	imageInternRead << tr("Reset Zoom");

	imageIntern << "__zoomActual";
	imageInternRead << tr("Zoom to actual size");

	imageIntern << "__rotateR";
	imageInternRead << tr("Rotate Right");

	imageIntern << "__rotateL";
	imageInternRead << tr("Rotate Left");

	imageIntern << "__rotate0";
	imageInternRead << tr("Reset Rotation");

	imageIntern << "__flipH";
	imageInternRead << tr("Flip Horizontally");

	imageIntern << "__flipV";
	imageInternRead << tr("Flip Vertically");



	fileIntern << "__rename";
	fileInternRead << tr("Rename File");

	fileIntern << "__delete";
	fileInternRead << tr("Delete File");

	fileIntern << "__copy";
	fileInternRead << tr("Copy file to a new location");

	fileIntern << "__move";
	fileInternRead << tr("Move file to a new location");



	otherIntern << "__stopThb";
	otherInternRead << tr("Interrupt Thumbnail creation");

	otherIntern << "__reloadThb";
	otherInternRead << tr("Reload Thumbnails");

	otherIntern << "__hideMeta";
	otherInternRead << tr("Hide/Show Exif Info");

	otherIntern << "__settings";
	otherInternRead << tr("Show settings");

	otherIntern << "__about";
	otherInternRead << tr("About photo");

	otherIntern << "__print";
	otherInternRead << tr("Print Image");



	/*********************
	 * GENERAL * GENERAL *
	 * GENERAL * GENERAL *
	 * GENERAL * GENERAL *
	 *********************/

	QLabel *navigationTitle = new QLabel("<center>Category: Navigation</center>");
	navigationTitle->setStyleSheet("font-weight: bold;");

	scrollbarNavigationSET = new CustomScrollbar;
	scrollbarNavigationSET->keepAlwaysVisible(true);
	scrollNavigationSET = new QScrollArea;
	scrollNavigationSET->setObjectName("scrollnavigationset");
	scrollNavigationSET->setStyleSheet("QScrollArea#scrollnavigationset { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollNavigationSET->setFixedHeight(210);
	gridNavigationSET = new QGridLayout(scrollNavigationSET);
	gridNavigationSET->setMargin(0);
	gridNavigationSET->setHorizontalSpacing(5);
	gridNavigationSET->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgNavigationSET = new QWidget(scrollNavigationSET);
	scrollWidgNavigationSET->setLayout(gridNavigationSET);
	scrollNavigationSET->setWidget(scrollWidgNavigationSET);
	scrollNavigationSET->setWidgetResizable(true);
	scrollNavigationSET->setVerticalScrollBar(scrollbarNavigationSET);


	scrollbarNavigationAVAIL = new CustomScrollbar;
	scrollbarNavigationAVAIL->keepAlwaysVisible(true);
	scrollNavigationAVAIL = new QScrollArea;
	scrollNavigationAVAIL->setObjectName("scrollnavigationavail");
	scrollNavigationAVAIL->setStyleSheet("QScrollArea#scrollnavigationavail { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollNavigationAVAIL->setFixedHeight(210);
	gridNavigationAVAIL = new QGridLayout(scrollNavigationAVAIL);
	gridNavigationAVAIL->setMargin(0);
	gridNavigationAVAIL->setHorizontalSpacing(5);
	gridNavigationAVAIL->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgNavigationAVAIL = new QWidget(scrollNavigationAVAIL);
	scrollWidgNavigationAVAIL->setLayout(gridNavigationAVAIL);
	scrollNavigationAVAIL->setWidget(scrollWidgNavigationAVAIL);
	scrollNavigationAVAIL->setWidgetResizable(true);
	scrollNavigationAVAIL->setVerticalScrollBar(scrollbarNavigationAVAIL);

	navigationSetSHrow = 0;
	navigationSetSHcol = 0;

	QHBoxLayout *layNavi = new QHBoxLayout;
	layNavi->addWidget(scrollNavigationSET);
	layNavi->addWidget(scrollNavigationAVAIL);



	/*************************
	 * IMAGE * IMAGE * IMAGE *
	 * IMAGE * IMAGE * IMAGE *
	 * IMAGE * IMAGE * IMAGE *
	 *************************/

	QLabel *imageTitle = new QLabel("<center>Category: Image</center>");
	imageTitle->setStyleSheet("font-weight: bold;");

	scrollbarImageSET = new CustomScrollbar;
	scrollbarImageSET->keepAlwaysVisible(true);
	scrollImageSET = new QScrollArea;
	scrollImageSET->setObjectName("scrollimageset");
	scrollImageSET->setStyleSheet("QScrollArea#scrollimageset { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollImageSET->setFixedHeight(210);
	gridImageSET = new QGridLayout(scrollImageSET);
	gridImageSET->setMargin(0);
	gridImageSET->setHorizontalSpacing(5);
	gridImageSET->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgImageSET = new QWidget(scrollImageSET);
	scrollWidgImageSET->setLayout(gridImageSET);
	scrollImageSET->setWidget(scrollWidgImageSET);
	scrollImageSET->setWidgetResizable(true);
	scrollImageSET->setVerticalScrollBar(scrollbarImageSET);


	scrollbarImageAVAIL = new CustomScrollbar;
	scrollbarImageAVAIL->keepAlwaysVisible(true);
	scrollImageAVAIL = new QScrollArea;
	scrollImageAVAIL->setObjectName("scrollimageavail");
	scrollImageAVAIL->setStyleSheet("QScrollArea#scrollimageavail { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollImageAVAIL->setFixedHeight(210);
	gridImageAVAIL = new QGridLayout(scrollImageAVAIL);
	gridImageAVAIL->setMargin(0);
	gridImageAVAIL->setHorizontalSpacing(5);
	gridImageAVAIL->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgImageAVAIL = new QWidget(scrollImageAVAIL);
	scrollWidgImageAVAIL->setLayout(gridImageAVAIL);
	scrollImageAVAIL->setWidget(scrollWidgImageAVAIL);
	scrollImageAVAIL->setWidgetResizable(true);
	scrollImageAVAIL->setVerticalScrollBar(scrollbarImageAVAIL);

	imageSetSHrow = 0;
	imageSetSHcol = 0;

	QHBoxLayout *layImg = new QHBoxLayout;
	layImg->addWidget(scrollImageSET);
	layImg->addWidget(scrollImageAVAIL);



	/**********************
	 * FILE * FILE * FILE *
	 * FILE * FILE * FILE *
	 * FILE * FILE * FILE *
	 **********************/

	QLabel *fileTitle = new QLabel("<center>Category: File</center>");
	fileTitle->setStyleSheet("font-weight: bold;");

	scrollbarFileSET = new CustomScrollbar;
	scrollbarFileSET->keepAlwaysVisible(true);
	scrollFileSET = new QScrollArea;
	scrollFileSET->setObjectName("scrollfileset");
	scrollFileSET->setStyleSheet("QScrollArea#scrollfileset { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollFileSET->setFixedHeight(210);
	gridFileSET = new QGridLayout(scrollFileSET);
	gridFileSET->setMargin(0);
	gridFileSET->setHorizontalSpacing(5);
	gridFileSET->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgFileSET = new QWidget(scrollFileSET);
	scrollWidgFileSET->setLayout(gridFileSET);
	scrollFileSET->setWidget(scrollWidgFileSET);
	scrollFileSET->setWidgetResizable(true);
	scrollFileSET->setVerticalScrollBar(scrollbarFileSET);


	scrollbarFileAVAIL = new CustomScrollbar;
	scrollbarFileAVAIL->keepAlwaysVisible(true);
	scrollFileAVAIL = new QScrollArea;
	scrollFileAVAIL->setObjectName("scrollfileavail");
	scrollFileAVAIL->setStyleSheet("QScrollArea#scrollfileavail { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollFileAVAIL->setFixedHeight(210);
	gridFileAVAIL = new QGridLayout(scrollFileAVAIL);
	gridFileAVAIL->setMargin(0);
	gridFileAVAIL->setHorizontalSpacing(5);
	gridFileAVAIL->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgFileAVAIL = new QWidget(scrollFileAVAIL);
	scrollWidgFileAVAIL->setLayout(gridFileAVAIL);
	scrollFileAVAIL->setWidget(scrollWidgFileAVAIL);
	scrollFileAVAIL->setWidgetResizable(true);
	scrollFileAVAIL->setVerticalScrollBar(scrollbarFileAVAIL);

	fileSetSHrow = 0;
	fileSetSHcol = 0;

	QHBoxLayout *layFile = new QHBoxLayout;
	layFile->addWidget(scrollFileSET);
	layFile->addWidget(scrollFileAVAIL);


	/*************************
	 * OTHER * OTHER * OTHER *
	 * OTHER * OTHER * OTHER *
	 * OTHER * OTHER * OTHER *
	 *************************/

	QLabel *otherTitle = new QLabel("<center>Category: Other</center>");
	otherTitle->setStyleSheet("font-weight: bold;");

	scrollbarOtherSET = new CustomScrollbar;
	scrollbarOtherSET->keepAlwaysVisible(true);
	scrollOtherSET = new QScrollArea;
	scrollOtherSET->setObjectName("scrollotherset");
	scrollOtherSET->setStyleSheet("QScrollArea#scrollotherset { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollOtherSET->setFixedHeight(210);
	gridOtherSET = new QGridLayout(scrollOtherSET);
	gridOtherSET->setMargin(0);
	gridOtherSET->setHorizontalSpacing(5);
	gridOtherSET->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgOtherSET = new QWidget(scrollOtherSET);
	scrollWidgOtherSET->setLayout(gridOtherSET);
	scrollOtherSET->setWidget(scrollWidgOtherSET);
	scrollOtherSET->setWidgetResizable(true);
	scrollOtherSET->setVerticalScrollBar(scrollbarOtherSET);


	scrollbarOtherAVAIL = new CustomScrollbar;
	scrollbarOtherAVAIL->keepAlwaysVisible(true);
	scrollOtherAVAIL = new QScrollArea;
	scrollOtherAVAIL->setObjectName("scrollotheravail");
	scrollOtherAVAIL->setStyleSheet("QScrollArea#scrollotheravail { background: rgba(255,255,255,20); border-radius: 5px; }");
	scrollOtherAVAIL->setFixedHeight(210);
	gridOtherAVAIL = new QGridLayout(scrollOtherAVAIL);
	gridOtherAVAIL->setMargin(0);
	gridOtherAVAIL->setHorizontalSpacing(5);
	gridOtherAVAIL->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scrollWidgOtherAVAIL = new QWidget(scrollOtherAVAIL);
	scrollWidgOtherAVAIL->setLayout(gridOtherAVAIL);
	scrollOtherAVAIL->setWidget(scrollWidgOtherAVAIL);
	scrollOtherAVAIL->setWidgetResizable(true);
	scrollOtherAVAIL->setVerticalScrollBar(scrollbarOtherAVAIL);

	otherSetSHrow = 0;
	otherSetSHcol = 0;

	QHBoxLayout *layOther = new QHBoxLayout;
	layOther->addWidget(scrollOtherSET);
	layOther->addWidget(scrollOtherAVAIL);



//	QVBoxLayout *lay = new QVBoxLayout;

	lay->addSpacing(5);
	lay->addWidget(navigationTitle);
	lay->addLayout(layNavi);
	lay->addSpacing(5);
	lay->addWidget(imageTitle);
	lay->addLayout(layImg);
	lay->addSpacing(5);
	lay->addWidget(fileTitle);
	lay->addLayout(layFile);
	lay->addSpacing(5);
	lay->addWidget(otherTitle);
	lay->addLayout(layOther);

//	this->setLayout(lay);

//	setupAvailShortcuts();

}

void SettingsTabShortcuts2::setupAvailShortcuts() {

	int cGen = scrollNavigationAVAIL->width()/100;

	qDebug() << "c:" << scrollNavigationAVAIL->width();
	int rowGen = 0;
	int colGen = 0;


	QMap<QString,int> tmpNav;

	for(int i = 0; i < navigationInternRead.length(); ++i)
		tmpNav.insert(navigationInternRead.at(i),i);

	QStringList tmpKeysNav = tmpNav.keys();
	tmpKeysNav.sort();

	for(int i = 0; i < tmpKeysNav.length(); ++i) {

		int pos = tmpNav.value(tmpKeysNav.at(i));

//	for(int i = 0; i < navigationIntern.size(); ++i) {

		ShortcutsTiles *l = new ShortcutsTiles(navigationIntern.at(pos), navigationInternRead.at(pos), "navigation", "avail");
		gridNavigationAVAIL->addWidget(l,rowGen,colGen);
		++colGen;
		if(colGen == cGen) {
			rowGen += 1;
			colGen = 0;
		}

		connect(l, SIGNAL(addShortcut(QString,QString)), this, SLOT(addSH(QString,QString)));

	}



	int cImg = scrollImageAVAIL->width()/100;
	int rowImg = 0;
	int colImg = 0;

	QMap<QString,int> tmpImg;

	for(int i = 0; i < imageInternRead.length(); ++i)
		tmpImg.insert(imageInternRead.at(i),i);

	QStringList tmpKeysImg = tmpImg.keys();
	tmpKeysImg.sort();

	for(int i = 0; i < tmpKeysImg.length(); ++i) {

		int pos = tmpImg.value(tmpKeysImg.at(i));

		ShortcutsTiles *l = new ShortcutsTiles(imageIntern.at(pos), imageInternRead.at(pos), "image", "avail");
		gridImageAVAIL->addWidget(l,rowImg,colImg);
		++colImg;
		if(colImg == cImg) {
			rowImg += 1;
			colImg = 0;
		}

		connect(l, SIGNAL(addShortcut(QString,QString)), this, SLOT(addSH(QString,QString)));

	}


	int cFile = scrollFileAVAIL->width()/100;
	int rowFile = 0;
	int colFile = 0;

	QMap<QString,int> tmpFile;

	for(int i = 0; i < fileInternRead.length(); ++i)
		tmpFile.insert(fileInternRead.at(i),i);

	QStringList tmpKeysFile = tmpFile.keys();
	tmpKeysFile.sort();

	for(int i = 0; i < tmpKeysFile.length(); ++i) {

		int pos = tmpFile.value(tmpKeysFile.at(i));

		ShortcutsTiles *l = new ShortcutsTiles(fileIntern.at(pos), fileInternRead.at(pos), "file", "avail");
		gridFileAVAIL->addWidget(l,rowFile,colFile);
		++colFile;
		if(colFile == cFile) {
			rowFile += 1;
			colFile = 0;
		}

		connect(l, SIGNAL(addShortcut(QString,QString)), this, SLOT(addSH(QString,QString)));

	}


	int cOther = scrollOtherAVAIL->width()/100;
	int rowOther = 0;
	int colOther = 0;

	QMap<QString,int> tmpOther;

	for(int i = 0; i < otherInternRead.length(); ++i)
		tmpOther.insert(otherInternRead.at(i),i);

	QStringList tmpKeysOther = tmpOther.keys();
	tmpKeysOther.sort();

	for(int i = 0; i < tmpKeysOther.length(); ++i) {

		int pos = tmpOther.value(tmpKeysOther.at(i));

		ShortcutsTiles *l = new ShortcutsTiles(otherIntern.at(pos), otherInternRead.at(pos), "other", "avail");
		gridOtherAVAIL->addWidget(l,rowOther,colOther);
		++colOther;
		if(colOther == cOther) {
			rowOther += 1;
			colOther = 0;
		}

		connect(l, SIGNAL(addShortcut(QString,QString)), this, SLOT(addSH(QString,QString)));

	}

}

void SettingsTabShortcuts2::loadAllShortcuts() {

	allNavigationExist.clear();
	allImageExist.clear();
	allFileExist.clear();
	allOtherExist.clear();

	QMapIterator<QString, int> i2(keysFORkeySH);
	while (i2.hasNext()) {
		i2.next();

//		qDebug() << "sh" << keySH.value(i2.value()).at(2).toString();

		QString cat = "";
		QString key = keySH.value(i2.value()).at(1).toString();

		if(navigationIntern.contains(keySH.value(i2.value()).at(2).toString()))
			cat = "navigation";
		if(imageIntern.contains(keySH.value(i2.value()).at(2).toString()))
			cat = "image";
		if(fileIntern.contains(keySH.value(i2.value()).at(2).toString()))
			cat = "file";
		if(otherIntern.contains(keySH.value(i2.value()).at(2).toString()))
			cat = "other";

		addSH(keySH.value(i2.value()).at(2).toString(),cat,key);

	}

	setShortcuts("navigation");
	setShortcuts("image");
	setShortcuts("file");
	setShortcuts("other");

}

void SettingsTabShortcuts2::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


void SettingsTabShortcuts2::addSH(QString exe, QString cat, QString keycombo) {

	qDebug() << "exe:" << exe << "- cat:" << cat << "- keycombo:" << keycombo;

	if(cat == "navigation") {

		int c = scrollNavigationSET->width()/100;
		int key = 0;
		key = navigationIntern.indexOf(exe);

		ShortcutsTiles *newitem = new ShortcutsTiles(exe,navigationInternRead.at(key),cat,"exist");
//		newitem->setFixedHeight(100);
//		newitem->setFixedWidth(100);

//		newitem->row = navigationSetSHrow;
//		newitem->col = navigationSetSHcol;

		if(keycombo == "") {
			gridNavigationSET->addWidget(newitem,navigationSetSHrow,navigationSetSHcol);
			navigationSetSHcol += 1;
			if(navigationSetSHcol == c) {
				navigationSetSHrow += 1;
				navigationSetSHcol = 0;
			}
		}
		connect(newitem, SIGNAL(removeItem(int,int,QString,int)), this, SLOT(removeSH(int,int,QString,int)));
		connect(newitem, SIGNAL(renewShortcut(QString,int)), this, SLOT(renewShortcut(QString,int)));

		newitem->posInList = allNavigationExist.length();

		allNavigationExist.append(newitem);

		if(keycombo == "") {
			keyDetect->cat = cat;
//			keyDetect->posInList = allNavigationExist.length()-1;
			keyDetect->setFunctionname(navigationInternRead.at(key));
			keyDetect->animate();
		} else
			detectCombo(keycombo,cat,allNavigationExist.length()-1);

	} else if(cat == "image") {

		int c = scrollImageSET->width()/100;
		int key = 0;
		key = imageIntern.indexOf(exe);

		ShortcutsTiles *newitem = new ShortcutsTiles(exe,imageInternRead.at(key),cat,"exist");
//		newitem->setFixedHeight(100);
//		newitem->setFixedWidth(100);

		newitem->row = imageSetSHrow;
		newitem->col = imageSetSHcol;

		if(keycombo == "") {
			gridImageSET->addWidget(newitem,imageSetSHrow,imageSetSHcol);
			imageSetSHcol += 1;
			if(imageSetSHcol == c) {
				imageSetSHrow += 1;
				imageSetSHcol = 0;
			}
		}
		connect(newitem, SIGNAL(removeItem(int,int,QString,int)), this, SLOT(removeSH(int,int,QString,int)));
		connect(newitem, SIGNAL(renewShortcut(QString,int)), this, SLOT(renewShortcut(QString,int)));

		newitem->posInList = allImageExist.length();

		allImageExist.append(newitem);

		if(keycombo == "") {
			keyDetect->cat = cat;
//			keyDetect->posInList = allImageExist.length()-1;
			keyDetect->setFunctionname(imageInternRead.at(key));
			keyDetect->animate();
		} else
			detectCombo(keycombo,cat,allImageExist.length()-1);

	} else if(cat == "file") {

		int c = scrollFileSET->width()/100;
		int key = 0;
		key = fileIntern.indexOf(exe);

		ShortcutsTiles *newitem = new ShortcutsTiles(exe,fileInternRead.at(key),cat,"exist");
//		newitem->setFixedHeight(100);
//		newitem->setFixedWidth(100);

		newitem->row = fileSetSHrow;
		newitem->col = fileSetSHcol;

		if(keycombo == "") {
			gridFileSET->addWidget(newitem,fileSetSHrow,fileSetSHcol);
			fileSetSHcol += 1;
			if(fileSetSHcol == c) {
				fileSetSHrow += 1;
				fileSetSHcol = 0;
			}
		}
		connect(newitem, SIGNAL(removeItem(int,int,QString,int)), this, SLOT(removeSH(int,int,QString,int)));
		connect(newitem, SIGNAL(renewShortcut(QString,int)), this, SLOT(renewShortcut(QString,int)));

		newitem->posInList = allFileExist.length();

		allFileExist.append(newitem);

		if(keycombo == "") {
			keyDetect->cat = cat;
//			keyDetect->posInList = allFileExist.length()-1;
			keyDetect->setFunctionname(fileInternRead.at(key));
			keyDetect->animate();
		} else
			detectCombo(keycombo,cat,allFileExist.length()-1);

	} else if(cat == "other") {

		int c = scrollOtherSET->width()/100;
		int key = 0;
		key = otherIntern.indexOf(exe);

		ShortcutsTiles *newitem = new ShortcutsTiles(exe,otherInternRead.at(key),cat,"exist");
//		newitem->setFixedHeight(100);
//		newitem->setFixedWidth(100);

		newitem->row = otherSetSHrow;
		newitem->col = otherSetSHcol;
		if(keycombo == "") {
			gridOtherSET->addWidget(newitem,otherSetSHrow,otherSetSHcol);
			otherSetSHcol += 1;
			if(otherSetSHcol == c) {
				otherSetSHrow += 1;
				otherSetSHcol = 0;
			}
		}
		connect(newitem, SIGNAL(removeItem(int,int,QString,int)), this, SLOT(removeSH(int,int,QString,int)));
		connect(newitem, SIGNAL(renewShortcut(QString,int)), this, SLOT(renewShortcut(QString,int)));

		newitem->posInList = allOtherExist.length();

		allOtherExist.append(newitem);

		if(keycombo == "") {
			keyDetect->keyCombo->setText("<center>&lt;Press Keys&gt;</center>");
			keyDetect->cat = cat;
//			keyDetect->posInList = allOtherExist.length()-1;
			keyDetect->setFunctionname(otherInternRead.at(key));
			keyDetect->animate();
		} else
			detectCombo(keycombo,cat,allOtherExist.length()-1);

	}

}

void SettingsTabShortcuts2::renewShortcut(QString cat, int posInList) {

	keyDetect->cat = cat;
//	keyDetect->posInList = posInList;

	qDebug() << allNavigationExist;

	if(cat == "navigation") {
		keyDetect->setFunctionname(navigationInternRead.at(navigationIntern.indexOf(allNavigationExist.at(posInList)->exe)));
		keyDetect->keyCombo->setText("<center>&lt;" + allNavigationExist.at(posInList)->shortcut->text().remove("<").remove(">") + "&gt;</center>");
	} else if(cat == "image") {
		keyDetect->setFunctionname(imageInternRead.at(posInList));
		keyDetect->keyCombo->setText("<center>&lt;" + allImageExist.at(posInList)->shortcut->text().remove("<").remove(">") + "&gt;</center>");
	} else if(cat == "file") {
		keyDetect->setFunctionname(fileInternRead.at(posInList));
		keyDetect->keyCombo->setText("<center>&lt;" + allFileExist.at(posInList)->shortcut->text().remove("<").remove(">") + "&gt;</center>");
	} else if(cat == "other") {
		keyDetect->setFunctionname(otherInternRead.at(posInList));
		keyDetect->keyCombo->setText("<center>&lt;" + allOtherExist.at(posInList)->shortcut->text().remove("<").remove(">") + "&gt;</center>");
	}
	keyDetect->animate();

}

void SettingsTabShortcuts2::removeSH(int row, int col, QString cat, int posInList) {

	if(cat == "navigation") {

		for(int i = 0; i < allNavigationExist.length(); ++i) {
			allNavigationExist.at(i)->hide();
		}

		delete allNavigationExist.at(posInList);
		allNavigationExist.removeAt(posInList);

		setShortcuts("navigation");

	} else if(cat == "image") {

		for(int i = 0; i < allImageExist.length(); ++i) {
			allImageExist.at(i)->hide();
		}

		delete allImageExist.at(posInList);
		allImageExist.removeAt(posInList);

		setShortcuts("image");

	} else if(cat == "file") {

		for(int i = 0; i < allFileExist.length(); ++i) {
			allFileExist.at(i)->hide();
		}

		delete allFileExist.at(posInList);
		allFileExist.removeAt(posInList);

		setShortcuts("file");

	} else if(cat == "other") {

		for(int i = 0; i < allOtherExist.length(); ++i) {
			allOtherExist.at(i)->hide();
		}

		delete allOtherExist.at(posInList);
		allOtherExist.removeAt(posInList);

		setShortcuts("other");

	}

}

void SettingsTabShortcuts2::setShortcuts(QString category) {

	if(category == "navigation") {

		navigationSetSHrow = 0;
		navigationSetSHcol = 0;

		int c = scrollNavigationSET->width()/100;

		QMap<QString,int> tmp;

		for(int i = 0; i < allNavigationExist.length(); ++i)
			tmp.insert(allNavigationExist.at(i)->back->text(),i);

		QStringList tmpKeys = tmp.keys();
		tmpKeys.sort();

		for(int i = 0; i < tmpKeys.length(); ++i) {

			int pos = tmp.value(tmpKeys.at(i));

			gridNavigationSET->addWidget(allNavigationExist.at(pos),navigationSetSHrow,navigationSetSHcol);
			navigationSetSHcol += 1;
			if(navigationSetSHcol == c) {
				navigationSetSHrow += 1;
				navigationSetSHcol = 0;
			}
			allNavigationExist.at(pos)->posInList = i;
			allNavigationExist.at(pos)->show();

		}

	} else if(category == "image") {

		imageSetSHrow = 0;
		imageSetSHcol = 0;

		int c = scrollImageSET->width()/100;

		QMap<QString,int> tmp;

		for(int i = 0; i < allImageExist.length(); ++i)
			tmp.insert(allImageExist.at(i)->back->text(),i);

		QStringList tmpKeys = tmp.keys();
		tmpKeys.sort();

		for(int i = 0; i < tmpKeys.length(); ++i) {

			int pos = tmp.value(tmpKeys.at(i));

			gridImageSET->addWidget(allImageExist.at(pos),imageSetSHrow,imageSetSHcol);
			imageSetSHcol += 1;
			if(imageSetSHcol == c) {
				imageSetSHrow += 1;
				imageSetSHcol = 0;
			}
			allImageExist.at(pos)->posInList = i;
			allImageExist.at(pos)->show();

		}

	} else if(category == "file") {

		fileSetSHrow = 0;
		fileSetSHcol = 0;

		int c = scrollFileSET->width()/100;

		QMap<QString,int> tmp;

		for(int i = 0; i < allFileExist.length(); ++i)
			tmp.insert(allFileExist.at(i)->back->text(),i);

		QStringList tmpKeys = tmp.keys();
		tmpKeys.sort();

		for(int i = 0; i < tmpKeys.length(); ++i) {

			int pos = tmp.value(tmpKeys.at(i));

			gridFileSET->addWidget(allFileExist.at(pos),fileSetSHrow,fileSetSHcol);
			fileSetSHcol += 1;
			if(fileSetSHcol == c) {
				fileSetSHrow += 1;
				fileSetSHcol = 0;
			}
//			allFileExist.at(pos)->posInList = i;
			allFileExist.at(pos)->show();
		}

	} else if(category == "other") {

		otherSetSHrow = 0;
		otherSetSHcol = 0;

		int c = scrollOtherSET->width()/100;

		QMap<QString,int> tmp;

		for(int i = 0; i < allOtherExist.length(); ++i)
			tmp.insert(allOtherExist.at(i)->back->text(),i);

		QStringList tmpKeys = tmp.keys();
		tmpKeys.sort();

		for(int i = 0; i < tmpKeys.length(); ++i) {

			int pos = tmp.value(tmpKeys.at(i));

			gridOtherSET->addWidget(allOtherExist.at(pos),otherSetSHrow,otherSetSHcol);
			otherSetSHcol += 1;
			if(otherSetSHcol == c) {
				otherSetSHrow += 1;
				otherSetSHcol = 0;
			}
//			allOtherExist.at(pos)->posInList = i;
			allOtherExist.at(pos)->show();
		}

	}

}

void SettingsTabShortcuts2::detectCombo(QString keycombo, QString cat, int posInList) {

//	posInList -= 1;

	if(cat == "navigation")
		allNavigationExist.at(posInList)->shortcut->setText("<" + keycombo + ">");
	else if(cat == "image")
		allImageExist.at(posInList)->shortcut->setText("<" + keycombo + ">");
	else if(cat == "file")
		allFileExist.at(posInList)->shortcut->setText("<" + keycombo + ">");
	else if(cat == "other")
		allOtherExist.at(posInList)->shortcut->setText("<" + keycombo + ">");


}

void SettingsTabShortcuts2::saveShortcuts() {

	keysFORkeySHnew.clear();
	keySHnew.clear();
	int totalpos = 0;

	for(int i = 0; i < allNavigationExist.length(); ++i) {

		QString key = allNavigationExist.at(i)->shortcut->text().remove(">").remove("<");

		QList<QVariant> list;
		list << false << key << allNavigationExist.at(i)->exe;

		qDebug() << "EXE:" << allNavigationExist.at(i)->exe;

		keysFORkeySHnew.insert(key,totalpos+1);
		keySHnew.insert(totalpos+1,list);

		++totalpos;

	}

	for(int i = 0; i < allImageExist.length(); ++i) {

		QString key = allImageExist.at(i)->shortcut->text().remove(">").remove("<");

		QList<QVariant> list;
		list << false << key << allImageExist.at(i)->exe;

		keysFORkeySHnew.insert(key,totalpos);
		keySHnew.insert(totalpos+1,list);

		++totalpos;

	}

	for(int i = 0; i < allFileExist.length(); ++i) {

		QString key = allFileExist.at(i)->shortcut->text().remove(">").remove("<");

		QList<QVariant> list;
		list << false << key << allFileExist.at(i)->exe;

		keysFORkeySHnew.insert(key,totalpos);
		keySHnew.insert(totalpos+1,list);

		++totalpos;

	}

	for(int i = 0; i < allOtherExist.length(); ++i) {

		QString key = allOtherExist.at(i)->shortcut->text().remove(">").remove("<");

		QList<QVariant> list;
		list << false << key << allOtherExist.at(i)->exe;

		keysFORkeySHnew.insert(key,totalpos);
		keySHnew.insert(totalpos+1,list);

		++totalpos;

	}

	qDebug() << "NEW SH:" << keySHnew;

}
