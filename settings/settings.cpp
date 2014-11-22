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

#include "settings.h"
#include <iostream>

Settings::Settings(QMap<QString, QVariant> glob, bool v, QWidget *parent) : MyWidget(0,parent) {

	// This is a fullscreen widget
	this->setFullscreen(true);

	// Some variables
	verbose = v;
	tabsSetup = false;

	// the settings and shortcuts
	globSet = glob;
	sh = new Shortcuts;
	sh->verbose = verbose;
	sh->loadSH();

	// The tab widget
	tabs = new TabWidget;
	tabs->makeBold(true);

	// Main layout
	QVBoxLayout *lay = new QVBoxLayout;
	this->setWidgetLayout(lay);
	lay->addWidget(tabs);

	// Some general buttons
	CustomPushButton *setDefault = new CustomPushButton(tr("Restore Default Settings"));
	CustomPushButton *saveExit = new CustomPushButton(tr("Save Changes and Exit"));
	CustomPushButton *cancel = new CustomPushButton(tr("Exit and Discard Changes"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addWidget(setDefault);
	butLay->addStretch();
	butLay->addWidget(saveExit);
	butLay->addWidget(cancel);
	butLay->addSpacing(30);
	lay->addLayout(butLay);

	// Ask for confirmation of restoring default settings
	restoreDefaultConfirm = new CustomConfirm(tr("Restore Default Settings?"),tr("Do you really want to get rid of your custom settings and set the default ones? This only resets all settings. A default set of shortcuts can be set in the shortcuts tab.") + "<br><br>" + tr("This step cannot be reversed!"),tr("Yep, I want new stuff"),tr("Um, no, not really"),QSize(550,300),"default","default",this);

	// Some signals/slots
	connect(saveExit, SIGNAL(clicked()), this, SLOT(animate()));
	connect(saveExit, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(loadSettings()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));

	connect(setDefault, SIGNAL(clicked()), restoreDefaultConfirm, SLOT(animate()));
	connect(restoreDefaultConfirm, SIGNAL(confirmed()), this, SLOT(restoreDefaultSettings()));

}

// Set up the tabs (only called once)
void Settings::setupTabs() {

	if(!tabsSetup) {

		qApp->setOverrideCursor(Qt::WaitCursor);

		tabsSetup = true;

		// Initiate the tabs
		tabOther = new SettingsTabOther(this,globSet,verbose);
		tabLookFeel = new SettingsTabLookAndFeel(this,globSet,verbose);
		tabThumb = new SettingsTabThumbnail(this,globSet,verbose);
		tabExif = new SettingsTabExif(this,globSet,verbose);
		tabShortcuts = new SettingsTabShortcuts(this,verbose);
		tabShortcuts->allKeyShortcuts = sh->allKeyShortcuts;
		tabShortcuts->allMouseShortcuts = sh->allMouseShortcuts;
		tabShortcuts->loadUserSetShortcuts();

		// Add the tabs
		tabs->addTab(tabLookFeel, QIcon(), tr("Look and Behaviour"));
		tabs->addTab(tabThumb, QIcon(), tr("Thumbnails"));
		tabs->addTab(tabExif, QIcon(), tr("Details"));
		tabs->addTab(tabOther, QIcon(), tr("Other Settings"));
		tabs->addTab(tabShortcuts, QIcon(), tr("Shortcuts"));
		connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));

		// Pass on settings
		tabLookFeel->globSet = globSet;
		tabThumb->globSet = globSet;
		tabExif->globSet = globSet;
		tabOther->globSet = globSet;

		connect(tabShortcuts, SIGNAL(setDefaultShortcuts()), this, SLOT(restoreDefaultShortcuts()));


		// Set all the QRects
		restoreDefaultConfirm->setRects(getRectShown(), getRectHidden(), getRectAni());
		tabShortcuts->detect->setRects(getRectShown(), getRectHidden(), getRectAni());
		tabShortcuts->setDefaultConfirm->setRects(getRectShown(), getRectHidden(), getRectAni());
		tabShortcuts->changeCommand->setRects(getRectShown(), getRectHidden(), getRectAni());
		tabThumb->confirmClean->setRects(getRectShown(), getRectHidden(), getRectAni());
		tabThumb->confirmErase->setRects(getRectShown(), getRectHidden(), getRectAni());


		// And load settings
		loadSettings();

		qApp->restoreOverrideCursor();


	}

}

// We overwrite the makeShow function, since we need to set some more stuff
void Settings::makeShow() {

	if(!isVisible()) {

		if(!tabsSetup) setupTabs();

		MyWidget::makeShow();

		tabs->setCurrentIndex(0);

		tabLookFeel->scrollbarLook->setScrollbarShown();
		tabLookFeel->scrollbarFeel->setScrollbarShown();
		tabThumb->scrollbarLook->setScrollbarShown();
		tabThumb->scrollbarTune->setScrollbarShown();
		tabShortcuts->scrollbar->setScrollbarShown();

	}

}

// Load all settings
void Settings::loadSettings() {

	if(verbose) std::clog << "set: Request to Load Settings" << std::endl;

	tabLookFeel->loadSettings();
	tabThumb->loadSettings();
	tabExif->loadSettings();
	tabOther->loadSettings();

}

// Save all settings
void Settings::saveSettings() {

	if(verbose) std::clog << "set: Request to Save Settings and Shortcuts" << std::endl;

	tabShortcuts->saveShortcuts();
	sh->allKeyShortcuts = tabShortcuts->allKeyShortcutsNEW;
	sh->allMouseShortcuts = tabShortcuts->allMouseShortcutsNEW;
	sh->saveSH();

	tabLookFeel->saveSettings();
	tabThumb->saveSettings();
	tabExif->saveSettings();
	tabOther->saveSettings();

	QList<QMap<QString,QVariant> > allnewsets;
	allnewsets << tabLookFeel->updatedSet;
	allnewsets << tabThumb->updatedSet;
	allnewsets << tabExif->updatedSet;
	allnewsets << tabOther->updatedSet;

	QMap<QString,QVariant> updatedSet;

	for(int j = 0; j < allnewsets.length(); ++j) {

		QMapIterator<QString, QVariant> i(allnewsets.at(j));
		while (i.hasNext()) {
			i.next();
			updatedSet[i.key()] = i.value();
		}

	}

	emit updateSettings(updatedSet);

}

void Settings::restoreDefaultSettings() {

	if(verbose) std::clog << "set: Request to restore default settings" << std::endl;

	emit restoreDefault();

}

void Settings::restoreDefaultShortcuts() {

	if(verbose) std::clog << "set: Request to restore default shortcuts" << std::endl;

	sh->setDefault();
	tabShortcuts->allKeyShortcuts = sh->allKeyShortcuts;
	tabShortcuts->allMouseShortcuts = sh->allMouseShortcuts;
	tabShortcuts->loadUserSetShortcuts();

}

// On Tab Change, scroll to top
void Settings::tabChanged() {

	if(verbose) std::clog << "set: Current Tab Changed" << std::endl;

	if(tabs->currentIndex() == 1)
		tabThumb->setDatabaseInfo();

	tabLookFeel->scrollbarLook->setValue(0);
	tabLookFeel->scrollbarFeel->setValue(0);
	tabThumb->scrollbarLook->setValue(0);
	tabThumb->scrollbarTune->setValue(0);
	tabOther->scrollbarOther->setValue(0);
	tabOther->scrollbarFile->setValue(0);
	tabExif->scrollbar->setValue(0);
	tabShortcuts->scrollbar->setValue(0);

	tabLookFeel->tabs->setCurrentIndex(0);
	tabThumb->tabs->setCurrentIndex(0);
	tabOther->tabs->setCurrentIndex(0);

}

// Go to next tab
void Settings::nextTab() {

	if(verbose) std::clog << "set: Next Tab" << std::endl;

	int current = tabs->currentIndex();
	if(current == 4)
		current = -1;
	++current;
	tabs->setCurrentIndex(current);

}

// Go to prev tab
void Settings::prevTab() {

	if(verbose) std::clog << "set: Prev Tab" << std::endl;

	int current = tabs->currentIndex();
	if(current == 0)
		current = 5;
	--current;
	tabs->setCurrentIndex(current);

}

Settings::~Settings() { }
