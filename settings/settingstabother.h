#ifndef SETTINGSTABGENERAL_H
#define SETTINGSTABGENERAL_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"
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

class SettingsTabOther: public QWidget {

	Q_OBJECT

public:
	SettingsTabOther(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~SettingsTabOther();

	void toggleExtended(bool extended);

	bool verbose;

	CustomScrollbar *scrollbar;

	// The language selection
	QStringList langDesc;
	QStringList langShort;
	QList<SettingsTabOtherLanguageTiles*> allLangTiles;

	// Context menu adjusting area
	Context *context;

	QWidget *widgetWindow;
	CustomCheckBox *windowMode;

	QWidget *widgetKnown;
	// All the known file foormats
	CustomLineEdit *knownFile;

	// Global Settings
	QMap<QString, QVariant> globSet;
	QMap<QString, QVariant> updatedSet;
	QMap<QString, QVariant> defaults;

	// Load/Save Settings
	void loadSettings();
	void saveSettings();

private:
	void paintEvent(QPaintEvent *);
};

#endif // SETTINGSTABGENERAL_H
