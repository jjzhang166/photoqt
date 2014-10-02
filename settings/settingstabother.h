#ifndef SETTINGSTABGENERAL_H
#define SETTINGSTABGENERAL_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customlineedit.h"
#include "../customelements/customlabel.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customtabwidget.h"
#include "settingstabotherlanguagetiles.h"
#include "settingstabothercontext.h"
#include "settingstabotherfiletypestiles.h"
#include "../flowlayout/flowlayout.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QButtonGroup>
#include <QLineEdit>
#include <QTextEdit>
#include <QSignalMapper>

class SettingsTabOther: public QWidget {

	Q_OBJECT

public:
	SettingsTabOther(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~SettingsTabOther();

	bool verbose;

	TabWidget *tabs;

	// The scrollbars
	CustomScrollbar *scrollbarOther;
	CustomScrollbar *scrollbarFile;

	// Global Settings
	QMap<QString, QVariant> globSet;
	QMap<QString, QVariant> updatedSet;
	QMap<QString, QVariant> defaults;

	// Load/Save Settings
	void loadSettings();
	void saveSettings();

private:
	QWidget *tabOther;
	QWidget *tabFile;

	QMap<QString,SettingsTabOtherFileTypesTiles*> allCheckQt;
	QMap<QString,SettingsTabOtherFileTypesTiles*> allCheckGm;
	QMap<QString,SettingsTabOtherFileTypesTiles*> allCheckGmUnstable;
	CustomLineEdit *extraQtEdit;
	CustomLineEdit *extraGmEdit;

	// The language selection
	QStringList langDesc;
	QStringList langShort;
	QList<SettingsTabOtherLanguageTiles*> allLangTiles;

	// Quick Settings
	CustomCheckBox *quickSet;
	bool defaultValue;

	// Context menu adjusting area
	Context *context;

	// All the known file foormats
	CustomLineEdit *knownFile;

	void paintEvent(QPaintEvent *);

private slots:
	void markAllNone(QString cat);

};

#endif // SETTINGSTABGENERAL_H
