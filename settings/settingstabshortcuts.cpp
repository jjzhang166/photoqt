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

#include "settingstabshortcuts.h"
#include <iostream>

SettingsTabShortcuts::SettingsTabShortcuts(QWidget *parent, bool v) : QWidget(parent) {

	verbose = v;

	this->setStyleSheet("background: transparent; color: white;");

	// The main scroll widget
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

	// The widget for detecting a new shortcut/mouse action
	detect = new ShortcutKeyDetect(this->parentWidget());
	detect->show();
	connect(detect, SIGNAL(gotKeys(QString,QString,QString)), this, SLOT(analyseKeyCombo(QString,QString,QString)));
	connect(detect, SIGNAL(shortcutCancelled()), this, SLOT(shortcutDetectCancelled()));

	// A widget to change the external command
	changeCommand = new ShortcutChangeCommand(this->parentWidget());
	changeCommand->show();
	connect(changeCommand, SIGNAL(commandChanged(QString,QString)), this, SLOT(analyseChangedCommand(QString,QString)));
	connect(changeCommand, SIGNAL(commandCanceled(QString)), this, SLOT(analyseChangedCancelled(QString)));


	// The title
	QLabel *title = new QLabel("<center><h1>" + tr("Shortcuts") + "</h1></center>");

	// A description
	QLabel *desc = new QLabel(tr("Here you can adjust the shortcuts, add new or remove existing ones, or change a key combination. The shortcuts are grouped into 4 different categories for internal commands plus a category for external commands. The boxes on the right side contain all the possible commands. To add a shortcut for one of the available function you can either double click on the tile or click the \"+\" button. This automatically opens another widget where you can set a key combination."));
	desc->setWordWrap(true);
	desc->setStyleSheet("background: transparent;");

	lay->addWidget(title);
	lay->addWidget(desc);
	lay->addSpacing(10);

	// Set the default shortcuts
	CustomPushButton *setDefault = new CustomPushButton(tr("Set default shortcuts"));
	QHBoxLayout *setDefLay = new QHBoxLayout;
	setDefLay->addStretch();
	setDefLay->addWidget(setDefault);
	setDefLay->addStretch();
	lay->addLayout(setDefLay);
	lay->addSpacing(10);
	setDefaultConfirm = new CustomConfirm(tr("Set default shortcuts?"), tr("This reverses all changes to the shortcuts and sets the set of default ones.") + "<br><br>" + tr("This step cannot be reversed!"), tr("Give me defaults"), tr("Hm, maybe not now"),QSize(500,250),"default","default",this->parentWidget());
	setDefaultConfirm->show();
	connect(setDefault, SIGNAL(clicked()), setDefaultConfirm, SLOT(animate()));
	connect(setDefaultConfirm, SIGNAL(confirmed()), this, SIGNAL(setDefaultShortcuts()));


	// a QMap for all the tiles
	allTiles.insert("navigation",QMap<QString,ShortcutsTiles*>());
	allTiles.insert("image",QMap<QString,ShortcutsTiles*>());
	allTiles.insert("file",QMap<QString,ShortcutsTiles*>());
	allTiles.insert("other",QMap<QString,ShortcutsTiles*>());


	// A map for all internal functions in the category "navigation"
	QMap<QString,QString> navigation;
	navigation.insert("__gotoFirstThb",tr("Go to first Image"));
	navigation.insert("__gotoLastThb",tr("Go to last Image"));
	navigation.insert("__hide",tr("Hide to System Tray"));
	navigation.insert("__next",tr("Next Image"));
	navigation.insert("__open",tr("Open New File"));
	navigation.insert("__filterImages",tr("Filter Images in Folder"));
	navigation.insert("__prev",tr("Previous Image"));
	navigation.insert("__close",tr("Quit PhotoQt"));
	QList<QString> navigationOrder;
	navigationOrder << "__open"
			<< "__filterImages"
			<< "__next"
			<< "__prev"
			<< "__gotoFirstThb"
			<< "__gotoLastThb"
			<< "__hide"
			<< "__close";

	// A map for all internal functions in the category "image"
	QMap<QString, QString> image;
	image.insert("__zoomIn",tr("Zoom In"));
	image.insert("__zoomOut",tr("Zoom Out"));
	image.insert("__zoomReset",tr("Reset Zoom"));
	image.insert("__zoomActual",tr("Zoom to Actual Size"));
	image.insert("__rotateR",tr("Rotate Right"));
	image.insert("__rotateL",tr("Rotate Left"));
	image.insert("__rotate0",tr("Reset Rotation"));
	image.insert("__flipH",tr("Flip Horizontally"));
	image.insert("__flipV",tr("Flip Vertically"));
	image.insert("__scale", tr("Scale Image"));
	QList<QString> imageOrder;
	imageOrder << "__zoomIn"
			<< "__zoomOut"
			<< "__zoomActual"
			<< "__zoomReset"
			<< "__rotateR"
			<< "__rotateL"
			<< "__rotate0"
			<< "__flipH"
			<< "__flipV"
			<< "__scale";

	// A map for all internal functions in the category "file"
	QMap<QString,QString> file;
	file.insert("__rename",tr("Rename File"));
	file.insert("__delete",tr("Delete File"));
	file.insert("__copy",tr("Copy File to a New Location"));
	file.insert("__move",tr("Move File to a New Location"));
	QList<QString> fileOrder;
	fileOrder << "__rename"
			<< "__delete"
			<< "__copy"
			<< "__move";

	// A map for all internal functions in the category "other"
	QMap<QString,QString> other;
	other.insert("__stopThb",tr("Interrupt Thumbnail Creation"));
	other.insert("__reloadThb",tr("Reload Thumbnails"));
	other.insert("__hideMeta",tr("Hide/Show Exif Info"));
	other.insert("__showContext", tr("Show Context Menu"));
	other.insert("__settings",tr("Show Settings"));
	other.insert("__slideshow",tr("Start Slideshow"));
	other.insert("__slideshowQuick",tr("Start Slideshow (Quickstart)"));
	other.insert("__about",tr("About PhotoQt"));
	other.insert("__wallpaper",tr("Set as Wallpaper"));
	QList<QString> otherOrder;
	otherOrder << "__stopThb"
			<< "__reloadThb"
			<< "__hideMeta"
			<< "__showContext"
			<< "__settings"
			<< "__slideshow"
			<< "__slideshowQuick"
			<< "__about"
			<< "__wallpaper";

	// Add them to the main QMap
	internFunctions.insert("navigation",navigation);
	internFunctions.insert("image",image);
	internFunctions.insert("file",file);
	internFunctions.insert("other",other);
	internFunctionsOrder.insert("navigation",navigationOrder);
	internFunctionsOrder.insert("image",imageOrder);
	internFunctionsOrder.insert("file",fileOrder);
	internFunctionsOrder.insert("other",otherOrder);


	// All the flowlayouts holding the tiles - USER SET
	FlowLayout *naviLaySET = new FlowLayout;
	FlowLayout *imageLaySET = new FlowLayout;
	FlowLayout *fileLaySET = new FlowLayout;
	FlowLayout *otherLaySET = new FlowLayout;
	FlowLayout *externLaySET = new FlowLayout;
	allLayoutsSET.insert("navigation",naviLaySET);
	allLayoutsSET.insert("image",imageLaySET);
	allLayoutsSET.insert("file",fileLaySET);
	allLayoutsSET.insert("other",otherLaySET);
	allLayoutsSET.insert("extern",externLaySET);

	// All the flowlayouts holding the tiles - AVAILABLE
	FlowLayout *naviLayAVAIL = new FlowLayout;
	FlowLayout *imageLayAVAIL = new FlowLayout;
	FlowLayout *fileLayAVAIL = new FlowLayout;
	FlowLayout *otherLayAVAIL = new FlowLayout;
	FlowLayout *externLayAVAIL = new FlowLayout;
	allLayoutsAVAIL.insert("navigation",naviLayAVAIL);
	allLayoutsAVAIL.insert("image",imageLayAVAIL);
	allLayoutsAVAIL.insert("file",fileLayAVAIL);
	allLayoutsAVAIL.insert("other",otherLayAVAIL);
	allLayoutsAVAIL.insert("extern",externLayAVAIL);


	// This is the order in which the categories are to be displayed
	QStringList tmp;
	tmp << "Navigation" << "Image" << "File" << "Other" << "Extern";
	QStringList tmpTranslated;
	tmpTranslated << tr("Navigation") << tr("Image") << tr("File") << tr("Other") << tr("Extern");

	for(int i = 0; i < tmp.length(); ++i) {

		// Category title
		QLabel *catTitle = new QLabel;
		catTitle->setText(QString("<center>" + tr("Category:") + " %1</center>").arg(tmpTranslated.at(i)));
		catTitle->setStyleSheet("font-weight: bold;");

		// The scrollwidget for the user set shortcuts
		CustomScrollbar *scrollbarLeft = new CustomScrollbar;
		scrollbarLeft->keepAlwaysVisible(true);
		QScrollArea *scrollLeft = new QScrollArea;
		scrollLeft->setStyleSheet("background: rgba(255,255,255,10); border-radius: 5px;");
		scrollLeft->setFixedHeight(210);
		QWidget *scrollWidgLeft = new QWidget(scrollLeft);
		scrollWidgLeft->setLayout(allLayoutsSET.value(tmp.at(i).toLower()));
		scrollLeft->setWidget(scrollWidgLeft);
		scrollLeft->setWidgetResizable(true);
		scrollLeft->setVerticalScrollBar(scrollbarLeft);

		// The scrollwidget for the available shortcuts
		CustomScrollbar *scrollbarRight = new CustomScrollbar;
		scrollbarRight->keepAlwaysVisible(true);
		QScrollArea *scrollRight = new QScrollArea;
		scrollRight->setStyleSheet("background: rgba(255,255,255,10); border-radius: 5px;");
		scrollRight->setFixedHeight(210);
		QWidget *scrollWidgRight = new QWidget(scrollRight);
		scrollWidgRight->setLayout(allLayoutsAVAIL.value(tmp.at(i).toLower()));
		scrollRight->setWidget(scrollWidgRight);
		scrollRight->setWidgetResizable(true);
		scrollRight->setVerticalScrollBar(scrollbarRight);

		// The two widgets are lines up horizontally
		QHBoxLayout *layCat = new QHBoxLayout;
		layCat->addWidget(scrollLeft);
		layCat->addWidget(scrollRight);

		// And add everything to the main layout
		lay->addSpacing(5);
		lay->addWidget(catTitle);
		lay->addLayout(layCat);

	}

	lay->addStretch();

	// And finally load all available shortcuts
	loadAvailShortcuts();

}

// Load the user set shortcuts - called from settings.cpp after shortcut QMap is set
void SettingsTabShortcuts::loadUserSetShortcuts() {

	if(verbose) std::clog << "setSH: Load user set shortcuts" << std::endl;

	if(!allTiles.isEmpty()) {

		QMapIterator<QString, QMap<QString, ShortcutsTiles*> > i(allTiles);
		while (i.hasNext()) {
			i.next();
			QMapIterator<QString, ShortcutsTiles*> j(i.value());
			while(j.hasNext()) {
				j.next();
				removeTile(j.key(), i.key());
			}
		}

	}

	// Load key shortcuts
	QMapIterator<QString,QList<QVariant> > i(allKeyShortcuts);
	while(i.hasNext()) {

		i.next();

		int close = i.value().at(0).toInt();
		QString cmd = i.value().at(1).toString();

		bool found = false;

		// We first have to find the right category
		QMapIterator<QString, QMap<QString,QString> > j(internFunctions);
		while(j.hasNext()) {

			j.next();

			// If we found the right category
			if(j.value().keys().contains(cmd)) {

				ShortcutsTiles *tile = new ShortcutsTiles(cmd, j.value().value(cmd),j.key(),"exist",this);
				tile->shortcut->setText(i.key());
				tile->shortcutText = i.key();
				// The id never changes during runtime. That way, we can always uniquely access this tile
				tile->id = i.key();
				allLayoutsSET.value(j.key())->addWidget(tile);
				allTiles[j.key()].insert(QString(i.key()),tile);
				connect(tile, SIGNAL(removeItem(QString,QString)), this, SLOT(removeTile(QString,QString)));
				connect(tile, SIGNAL(renewShortcut(QString,QString,QString)), this, SLOT(getNewKeyCombo(QString,QString,QString)));
				found = true;
				break;

			}

		}

		// If shortcut is not for an internal function, then it's an external shortcut
		if(!found) {

			ShortcutsTiles *tile = new ShortcutsTiles(cmd, cmd,"extern","exist",this);
			tile->shortcut->setText(i.key());
			tile->shortcutText = i.key();
			// The id never changes during runtime. That way, we can always uniquely access this tile
			tile->id = i.key();
			if(close == 1)
				tile->quit->setChecked(true);
			else
				tile->quit->setChecked(false);
			allLayoutsSET.value("extern")->addWidget(tile);
			allTiles["extern"].insert(QString(i.key()),tile);
			connect(tile, SIGNAL(removeItem(QString,QString)), this, SLOT(removeTile(QString,QString)));
			connect(tile, SIGNAL(renewShortcut(QString,QString,QString)), this, SLOT(getNewKeyCombo(QString,QString,QString)));
			connect(tile, SIGNAL(changeCmd(QString,QString)), changeCommand, SLOT(animateCmd(QString,QString)));
			connect(tile->quit, SIGNAL(clicked()), this, SLOT(markForChange()));

		}



	}

	// Load all mouse shortcuts
	QMapIterator<QString,QList<QVariant> > k(allMouseShortcuts);
	while(k.hasNext()) {

		k.next();

		bool found = false;

		int close = k.value().at(0).toInt();
		QString cmd = k.value().at(1).toString();

		// We first have to find the right category
		QMapIterator<QString, QMap<QString,QString> > l(internFunctions);
		while(l.hasNext()) {

			l.next();

			// If we found the right category
			if(l.value().keys().contains(cmd)) {

				ShortcutsTiles *tile = new ShortcutsTiles(cmd, l.value().value(cmd),l.key(),"exist",this);
				tile->shortcut->setText(QString(k.key()).remove("[M]").trimmed());
				tile->makeMouseShortcut(true);
				tile->shortcutText = QString(k.key()).remove("[M]").trimmed();
				// The id never changes during runtime. That way, we can always uniquely access this tile
				tile->id = k.key();
				allLayoutsSET.value(l.key())->addWidget(tile);
				allTiles[l.key()].insert(QString(k.key()),tile);
				connect(tile, SIGNAL(removeItem(QString,QString)), this, SLOT(removeTile(QString,QString)));
				connect(tile, SIGNAL(renewShortcut(QString,QString,QString)), this, SLOT(getNewKeyCombo(QString,QString,QString)));
				found = true;
				break;

			}

		}

		// If it's not an internal function, then it's an external one
		if(!found) {


			ShortcutsTiles *tile = new ShortcutsTiles(cmd, cmd,"extern","exist",this);
			tile->shortcut->setText(QString(k.key()).remove("[M]").trimmed());
			tile->makeMouseShortcut(true);
			tile->shortcutText = QString(k.key()).remove("[M]").trimmed();
			// The id never changes during runtime. That way, we can always uniquely access this tile
			tile->id = i.key();
			if(close == 1)
				tile->quit->setChecked(true);
			else
				tile->quit->setChecked(false);
			allLayoutsSET.value("extern")->addWidget(tile);
			allTiles["extern"].insert(QString(i.key()),tile);
			connect(tile, SIGNAL(removeItem(QString,QString)), this, SLOT(removeTile(QString,QString)));
			connect(tile, SIGNAL(renewShortcut(QString,QString,QString)), this, SLOT(getNewKeyCombo(QString,QString,QString)));
			connect(tile, SIGNAL(changeCmd(QString,QString)), changeCommand, SLOT(animateCmd(QString,QString)));
			connect(tile->quit, SIGNAL(clicked()), this, SLOT(markForChange()));

		}



	}

}

// Load all the available shortcuts
void SettingsTabShortcuts::loadAvailShortcuts() {

	if(verbose) std::clog << "setSH: Load possible shortcuts" << std::endl;

	QMapIterator<QString, QList<QString> > i(internFunctionsOrder);
	while(i.hasNext()) {

		i.next();

		for(int j = 0; j < i.value().length(); ++j) {

			QString cmd = i.value().at(j);
			QString desc = internFunctions[i.key()].value(i.value().at(j));
			ShortcutsTiles *tile = new ShortcutsTiles(cmd,desc,i.key(),"avail",this);
			allLayoutsAVAIL.value(i.key())->addWidget(tile);
			connect(tile, SIGNAL(addShortcut(QString,QString)), this, SLOT(addNewTile(QString,QString)));
		}

	}

	ShortcutsTiles *tile = new ShortcutsTiles("extern","EXTERN","extern","avail",this);
	allLayoutsAVAIL.value("extern")->addWidget(tile);
	connect(tile, SIGNAL(addShortcut(QString,QString)), this, SLOT(addNewTile(QString,QString)));

}

// Remove a tile
void SettingsTabShortcuts::removeTile(QString key, QString cat) {

	if(verbose) std::clog << "setSH: Remove tile: " << cat.toStdString() << " - " << key.toStdString() << std::endl;

	markForChange();

	allTiles[cat][key]->hide();
	delete allTiles[cat][key];
	allTiles[cat].remove(key);

}

// Add a new tile
void SettingsTabShortcuts::addNewTile(QString exe, QString cat) {

	if(verbose) std::clog << "setSH: Add new tile: " << cat.toStdString() << " - " << exe.toStdString() << std::endl;

	markForChange();

	// This is used for the id, and never changes during runtime, allowing us to always uniquely identify this tile
	QString key = QString("key%1").arg(QDateTime::currentMSecsSinceEpoch());

	ShortcutsTiles *tile = new ShortcutsTiles(exe, internFunctions[cat][exe],cat,"exist",this);
	tile->shortcut->setText("<key>");
	tile->shortcutText = key;
	tile->id = key;
	allLayoutsSET.value(cat)->addWidget(tile);
	allTiles[cat].insert(key,tile);
	connect(tile, SIGNAL(removeItem(QString,QString)), this, SLOT(removeTile(QString,QString)));
	connect(tile, SIGNAL(renewShortcut(QString,QString,QString)), this, SLOT(getNewKeyCombo(QString,QString,QString)));
	if(cat == "extern")
		connect(tile, SIGNAL(changeCmd(QString,QString)), changeCommand, SLOT(animateCmd(QString,QString)));

	// Get a new key combination/mouse shortcut
	getNewKeyCombo(cat,key,exe);

}

// Set a new shortcut that we got from the detect widget
void SettingsTabShortcuts::analyseKeyCombo(QString category, QString identification, QString newKey) {

	if(verbose) std::clog << "setSH: Analyse Key Combo: " << category.toStdString() << " - " << identification.toStdString() << " - " << newKey.toStdString() << std::endl;

	markForChange();

	if(newKey.startsWith("[M]")) {
		allTiles[category][identification]->makeMouseShortcut(true);
		newKey = newKey.remove(0,3).trimmed();
		if(newKey.startsWith("---+"))
			newKey = newKey.remove(0,4).trimmed();
	} else
		allTiles[category][identification]->makeMouseShortcut(false);

	allTiles[category][identification]->shortcutText = newKey;
	allTiles[category][identification]->shortcut->setText(newKey);

	if(category == "extern")
		changeCommand->animateCmd("<" + tr("select") + ">",identification);


}

// Open the detect widget to get a (new) key combination/mouse action
void SettingsTabShortcuts::getNewKeyCombo(QString cat, QString id, QString exe) {

	if(verbose) std::clog << "setSH: get new key combo: " << cat.toStdString() << " - " << id.toStdString() << " - " << exe.toStdString() << std::endl;

	bool mouse = id.startsWith("[M]");
	QString intern = ((ShortcutsTiles *)sender())->shortcutText;

	// First we have to get all the already in use shortcuts. They can't be assigned anymore
	QStringList alreadySetKeys;
	QMapIterator<QString,QMap<QString, ShortcutsTiles*> > i(allTiles);
	while(i.hasNext()) {
		i.next();
		QMapIterator<QString,ShortcutsTiles*> j(i.value());
		while(j.hasNext()) {
			j.next();
			// Of course, pressing the same key combination that was in use before is no problem
			if(allTiles[cat][id]->shortcutText != j.value()->shortcutText)
				alreadySetKeys << j.value()->shortcutText;
		}
	}

	// Open detect widget
	detect->cat = cat;
	detect->id = id;
	detect->alreadySetKeys = alreadySetKeys;
	if(mouse) {
		detect->mouseShortcut->animateClick();
		int i = 0;
		bool found = false;
		while(i < detect->mouseMod->count()) {
			int j = 0;
			while(j < detect->mouseBut->count()) {
				QString mod = detect->mouseMod->itemText(i) + "+";
				if(mod == "---+")
					mod = "";
				if(intern == mod + detect->mouseBut->itemText(j)) {
					detect->mouseMod->setCurrentIndex(i);
					detect->mouseBut->setCurrentIndex(j);
					found = true;
				}
				++j;
				if(found)
					break;
			}
			++i;
			if(found)
				break;
		}
	} else {
		detect->keyShortcut->animateClick();
		if(intern != "") detect->setKeyCombo(intern);
	}


	if(cat != "extern")
		detect->setFunctionname(internFunctions[cat][exe]);
	else
		detect->setFunctionname(tr("some executeable"));
	detect->makeShow();

}

// The shortcut/mouse action detection for a new shortcut was cancelled
void SettingsTabShortcuts::shortcutDetectCancelled() {

	if(allTiles[detect->cat][detect->id]->shortcutText.startsWith("key"))
		removeTile(detect->id, detect->cat);

}

void SettingsTabShortcuts::analyseChangedCommand(QString id, QString cmd) {

	if(verbose) std::clog << "setSH: Analyse changed command: " << id.toStdString() << " - " << cmd.toStdString() << std::endl;

	markForChange();

	allTiles["extern"][id]->back->setText(cmd);
	allTiles["extern"][id]->setToolTip(cmd);

}

void SettingsTabShortcuts::analyseChangedCancelled(QString id) {

	if(allTiles["extern"][id]->back->text() == "")
		removeTile(id, "extern");

}

void SettingsTabShortcuts::saveShortcuts() {

	if(verbose) std::clog << "setSH: Save Shortcuts" << std::endl;

	allKeyShortcutsNEW.clear();
	allMouseShortcutsNEW.clear();

	QMapIterator<QString, QMap<QString, ShortcutsTiles*> > i(allTiles);

	while(i.hasNext()) {

		i.next();

		QMapIterator<QString, ShortcutsTiles*> j(i.value());

		while(j.hasNext()) {

			j.next();

			QString newKey = "";

			if(j.value()->isMouseShortcut)
				newKey = "[M] ";

			newKey += j.value()->shortcutText;

			QString exe = j.value()->exe;
			if(i.key() == "extern")
				exe = j.value()->back->text();

			QList<QVariant> list;
			list << (j.value()->quit->isVisible() && j.value()->quit->isChecked()) << exe;

			if(j.value()->mouse->isVisible())
				allMouseShortcutsNEW.insert(newKey,list);
			else
				allKeyShortcutsNEW.insert(newKey,list);

		}

	}

}

void SettingsTabShortcuts::markForChange() {
	shortcutsChanged = true;
}

SettingsTabShortcuts::~SettingsTabShortcuts() { }
