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
