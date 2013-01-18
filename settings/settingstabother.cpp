#include "settingstabother.h"

SettingsTabOther::SettingsTabOther(QWidget *parent, QMap<QString, QVariant> set) : QWidget(parent) {

	// The global settings
	globSet = set;

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
	QLabel *langLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Choose Language") + "</span></b><br><br>" + tr("There are a good few different languages available. Thanks to everybody who took the time to translate Photo!"));
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

	// Chinese
	langDesc << "Chinese (Dianjin Wang)";
	langShort << "zh_CN";

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


	// Adjust known file formats
	QLabel *knownLabel = new QLabel("<b><span style=\"font-size:12pt\">" + tr("Known File Types") + "</span></b><br><br>" + tr("Here you can adjust the list of known image types. Some image types (especially more exotic ones) aren't supported by every Qt installation (depending on which image plugins you've got installed, etc.). If there's any problem with an image type, you can take it out of the list here. If you want to add an image type, simply add it to the list.") + "<br><b>" + tr("Only change the list if you know what you're doing!") + "</b>");
	knownLabel->setWordWrap(true);
	knownFile = new CustomLineEdit("Known File Types");
	knownFile->setMinWidth(600);
	QHBoxLayout *knownLay = new QHBoxLayout;
	knownLay->addStretch();
	knownLay->addWidget(knownFile);
	knownLay->addStretch();
	lay->addWidget(knownLabel);
	lay->addSpacing(5);
	lay->addLayout(knownLay);
	lay->addSpacing(20);


	lay->addStretch();

}

// Load the settings
void SettingsTabOther::loadSettings() {

	context->loadContext();

	defaults.clear();

	for(int i = 0; i < allLangTiles.length(); ++i) {
		if(globSet.value("Language").toString() == allLangTiles.at(i)->langCode) {
			allLangTiles.at(i)->button->setChecked(true);
			defaults.insert("Language",globSet.value("Language").toString());
			break;
		}
	}

	knownFile->setText(globSet.value("KnownFileTypes").toString());
	defaults.insert("KnownFileTypes",globSet.value("KnownFileTypes").toString());

}

// Save the settings
void SettingsTabOther::saveSettings() {

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

	if(defaults.value("KnownFileTypes").toString() != knownFile->text()) {
		updatedSet.insert("KnownFileTypes",knownFile->text());
		defaults.remove("KnownFileTypes");
		defaults.insert("KnownFileTypes",knownFile->text());
	}

	context->saveContext();

}

void SettingsTabOther::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabOther::~SettingsTabOther() { }
