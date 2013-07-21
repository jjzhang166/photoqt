#include "settingstabother.h"

SettingsTabOther::SettingsTabOther(QWidget *parent, QMap<QString, QVariant> set, bool v) : QWidget(parent) {

	// The global settings
	globSet = set;

	verbose = v;

	this->setStyleSheet("background: transparent; color: white");

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

	// The tab title
	QLabel *title = new QLabel("<h1><center>" + tr("Other Settings") + "</center></h1>");
	lay->addWidget(title);
	lay->addSpacing(20);



	// CHOOSE A LANGUAGE
	QLabel *langLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Choose Language") + "</span></b><br><br>" + tr("There are a good few different languages available. Thanks to everybody who took the time to translate PhotoQt!"));
	langLabel->setWordWrap(true);
	lay->addWidget(langLabel);
	lay->addSpacing(15);


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

	lay->addLayout(langWidgLay);
	lay->addSpacing(30);


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
	lay->addWidget(contextMenuLabel);
	lay->addSpacing(10);
	lay->addLayout(contextLay);
	lay->addLayout(addNewLay);
	lay->addSpacing(20);

	// Adjust context menu
	widgetWindow = new QLabel;
	widgetWindow->setObjectName("widgetWindow");
	widgetWindow->setStyleSheet("QWidget#widgetWindow { background: rgba(255,255,255,20); border-radius: 20px; padding: 20px; }");
	QLabel *extWin = new QLabel(tr("Extended Setting"));
	extWin->setStyleSheet("font-weight: bold; font-size: 10px; font-style: italic");
	QHBoxLayout *extWinLay = new QHBoxLayout;
	extWinLay->addWidget(extWin);
	extWinLay->addStretch();
	QLabel *windowModeLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Window Mode") + "</span></b><br><br>" + tr("PhotoQt is designed with the space of a fullscreen app in mind. That's why it by default runs as fullscreen. However, some might prefer to have it as a normal window.") + "<br><b>" + tr("Note: It might be a little awkward to use e.g. the menu when PhotoQt is run in a window too small.") + "</b>");
	windowModeLabel->setWordWrap(true);
	windowMode = new CustomCheckBox(tr("Run Photo in Window Mode"));
	windowDeco = new CustomCheckBox(tr("Show Window Decoration"));
	QHBoxLayout *windowLay = new QHBoxLayout;
	windowLay->addStretch();
	windowLay->addWidget(windowMode);
	windowLay->addStretch();
	QHBoxLayout *decoLay = new QHBoxLayout;
	decoLay->addStretch();
	decoLay->addWidget(windowDeco);
	decoLay->addStretch();
	QVBoxLayout *widgetWindowLay = new QVBoxLayout;
	widgetWindowLay->addLayout(extWinLay);
	widgetWindowLay->addWidget(windowModeLabel);
	widgetWindowLay->addSpacing(10);
	widgetWindowLay->addLayout(windowLay);
	widgetWindowLay->addSpacing(5);
	widgetWindowLay->addLayout(decoLay);
	widgetWindowLay->addSpacing(20);
	widgetWindow->setLayout(widgetWindowLay);
	lay->addWidget(widgetWindow);
	widgetWindow->hide();


	// Adjust known file formats
	widgetKnown = new QLabel;
	widgetKnown->setObjectName("widgetKnown");
	widgetKnown->setStyleSheet("QWidget#widgetKnown { background: rgba(255,255,255,20); border-radius: 20px; padding: 20px; }");
	QLabel *extKnown = new QLabel(tr("Extended Setting"));
	extKnown->setStyleSheet("font-weight: bold; font-size: 10px; font-style: italic");
	QHBoxLayout *extKnownLay = new QHBoxLayout;
	extKnownLay->addWidget(extKnown);
	extKnownLay->addStretch();
	QLabel *knownLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Known File Types") + "</span></b><br><br>" + tr("Here you can adjust the list of known image types. Some image types (especially more exotic ones) aren't supported by every Qt installation (depending on which image plugins you've got installed, etc.). If there's any problem with an image type, you can take it out of the list here. If you want to add an image type, simply add it to the list.") + "<br><b>" + tr("Only change the list if you know what you're doing!") + "</b>");
	knownLabel->setWordWrap(true);
	knownFile = new CustomLineEdit("Known File Types");
	knownFile->setMinWidth(600);
	QHBoxLayout *knownLay = new QHBoxLayout;
	knownLay->addStretch();
	knownLay->addWidget(knownFile);
	knownLay->addStretch();
	QVBoxLayout *widgetKnownLay = new QVBoxLayout;
	widgetKnownLay->addLayout(extKnownLay);
	widgetKnownLay->addWidget(knownLabel);
	widgetKnownLay->addSpacing(5);
	widgetKnownLay->addLayout(knownLay);
	widgetKnownLay->addSpacing(20);
	widgetKnown->setLayout(widgetKnownLay);
	lay->addWidget(widgetKnown);
	widgetKnown->hide();



	lay->addStretch();

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

	windowMode->setChecked(globSet.value("WindowMode").toBool());
	defaults.insert("WindowMode",globSet.value("WindowMode").toBool());

	windowDeco->setChecked(globSet.value("WindowDecoration").toBool());
	defaults.insert("WindowDecoration",globSet.value("WindowDecoration").toBool());

	knownFile->setText(globSet.value("KnownFileTypes").toString());
	defaults.insert("KnownFileTypes",globSet.value("KnownFileTypes").toString());

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

	if(defaults.value("KnownFileTypes").toString() != knownFile->text()) {
		updatedSet.insert("KnownFileTypes",knownFile->text());
		defaults.remove("KnownFileTypes");
		defaults.insert("KnownFileTypes",knownFile->text());
	}

	context->saveContext();

}

void SettingsTabOther::toggleExtended(bool extended) {

	widgetKnown->setVisible(extended);
	widgetWindow->setVisible(extended);

}

void SettingsTabOther::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabOther::~SettingsTabOther() { }
