#ifndef SETTINGS_H
#define SETTINGS_H

#include "shortcuts.h"
#include "settingstabexif.h"
#include "settingstabother.h"
#include "settingstablookandfeel.h"
#include "settingstabthumbnail.h"
#include "settingstabshortcuts.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QPropertyAnimation>
#include <QtDebug>
#include <QVBoxLayout>
#include <QShortcut>
#include <QApplication>

class Settings : public QWidget {

	Q_OBJECT

public:
	Settings(QWidget *parent = 0, QMap<QString,QVariant> globalSet = QMap<QString,QVariant>(), bool v = false);
	~Settings();

	bool verbose;

	// The global Settings (passed on when initialising this class)
	QMap<QString,QVariant> globSet;

	// All shortcuts
	Shortcuts *sh;

	// The tab widget
	QTabWidget *tabs;

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

	void makeShow();
	void makeHide();
	bool isVisible() { return isShown; }
	void setRect(QRect rect);

private:
	void setupTabs();

	// Confirm restoration of default settings
	CustomConfirm *restoreDefaultConfirm;

	// The geometries of the widget
	QRect rectShown;
	QRect rectHidden;
	QRect aniStart;

	// Two display booleans
	bool isShown;

	// The property animation
	QPropertyAnimation *ani;

	CustomCheckBox *switchExtended;

private slots:
	// Animation functions
	void animate();
	void aniFinished();

	// Called when tab changed
	void tabChanged();

	void toggleExtended();

public slots:

	// Load and Save functions
	void loadSettings();
	void saveSettings();

	// Restore default settings & shortcuts
	void restoreDefaultSettings();
	void restoreDefaultShortcuts();

protected:
	void paintEvent(QPaintEvent *);

signals:
	// The widget is opened/closed
	void blockFunc(bool);

	// Update the settings
	void updateSettings(QMap<QString,QVariant>);

	// Ask mainwindow.cpp to restore default settings
	void restoreDefault();



};

#endif // SETTINGS_H
