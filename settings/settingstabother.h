#ifndef SETTINGSTABGENERAL_H
#define SETTINGSTABGENERAL_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customlineedit.h"
#include "../customelements/customlabel.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customtabwidget.h"
#include "settingstabotherlanguagetiles.h"
#include "settingstabothercontext.h"
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

	TabWidget *tabs;
	QWidget *tabOther;
	QWidget *tabFile;

	QMap<QString,CustomCheckBox*> allCheckQt;
	QMap<QString,CustomCheckBox*> allCheckGm;
	QMap<QString,CustomCheckBox*> allCheckGmUnstable;
	CustomLineEdit *extraQtEdit;
	CustomLineEdit *extraGmEdit;

	// The language selection
	QStringList langDesc;
	QStringList langShort;
	QList<SettingsTabOtherLanguageTiles*> allLangTiles;

	// Context menu adjusting area
	Context *context;

	// All the known file foormats
	CustomLineEdit *knownFile;

	void paintEvent(QPaintEvent *);

private slots:
	void markAllNone(QString cat);

};

#endif // SETTINGSTABGENERAL_H
