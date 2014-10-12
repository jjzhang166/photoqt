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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "shortcuts.h"
#include "settingstabexif.h"
#include "settingstabother.h"
#include "settingstablookandfeel.h"
#include "settingstabthumbnail.h"
#include "settingstabshortcuts.h"

#include "../customelements/customtabwidget.h"
#include "../widgets/mywidget.h"

#include <QtDebug>
#include <QShortcut>
#include <QApplication>

class Settings : public MyWidget {

	Q_OBJECT

public:
	Settings(QMap<QString,QVariant> globalSet = QMap<QString,QVariant>(), bool v = false, QWidget *parent = 0);
	~Settings();

	bool verbose;

	// The global Settings (passed on when initialising this class)
	QMap<QString,QVariant> globSet;

	// All shortcuts
	Shortcuts *sh;

	// The tab widget
	TabWidget *tabs;

	// All the tabs
	SettingsTabOther *tabOther;
	SettingsTabLookAndFeel *tabLookFeel;
	SettingsTabThumbnail *tabThumb;
	SettingsTabExif *tabExif;
	SettingsTabShortcuts *tabShortcuts;

	bool tabsSetup;

	// Switch Tabs back/for
	void nextTab();
	void prevTab();

	// We need to overwrite it for setting some additional stuff
	void makeShow();

private:
	void setupTabs();

	// Confirm restoration of default settings
	CustomConfirm *restoreDefaultConfirm;

private slots:
	// Called when tab changed
	void tabChanged();

public slots:

	// Load and Save functions
	void loadSettings();
	void saveSettings();

	// Restore default settings & shortcuts
	void restoreDefaultSettings();
	void restoreDefaultShortcuts();

signals:
	// Update the settings
	void updateSettings(QMap<QString,QVariant>);

	// Ask mainwindow.cpp to restore default settings
	void restoreDefault();

};

#endif // SETTINGS_H
