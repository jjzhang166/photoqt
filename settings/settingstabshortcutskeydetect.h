#ifndef SETTINGSTABSHORTCUTSKEYDETECT_H
#define SETTINGSTABSHORTCUTSKEYDETECT_H

#include "../customelements/custompushbutton.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customcombobox.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QLabel>
#include <QVBoxLayout>
#include <QtDebug>
#include <QKeyEvent>
#include <QComboBox>

// A widget for detecting a new key shortcut/mouse action
class ShortcutKeyDetect : public QWidget {

	Q_OBJECT

public:
	ShortcutKeyDetect(QWidget *parent = 0);
	~ShortcutKeyDetect();

	// These are the dimensions
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// The animation instance
	QPropertyAnimation *ani;
	bool isShown;

	// The central widget containing all the content
	QWidget *center;

	// The function for which the shortcut is for
	QLabel *function;

	// The category and the id of associated tile
	QString cat;
	QString id;

	// All the shortcuts already in use
	QStringList alreadySetKeys;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	// The radio buttons to choose between key shortcut and mouse action
	CustomRadioButton *keyShortcut;
	CustomRadioButton *mouseShortcut;

	// The labels for key combinations
	QLabel *keyCombo;
	QLabel *keyExistsError;

	// The comboboxes for mouse actions
	CustomComboBox *mouseMod;
	CustomComboBox *mouseBut;
	CustomPushButton *okAndSet;

	// The new shortcut
	QString newkey;

	// Analyse the keyevent (called by mainwindow.cpp)
	void analyseKeyEvent(QKeyEvent *event);

public slots:
	// The animation/fading functions
	void animate();
	void fadeStep();
	void aniFinished();

	// Set the current function name
	void setFunctionname(QString name);

	// Set a mouse action as shortcut
	void setMouseCombo();

	// Enable/Disable the other type of shortcut
	void setRightTypeDisEnabled();

protected:
	void paintEvent(QPaintEvent *);

signals:
	// Got a new key combo
	void gotKeys(QString category, QString identification, QString newKey);

	// The shortcut detection was cancelled (if the tile was newly added, it is removed again)
	void shortcutCancelled();

};

#endif // SETTINGSTABSHORTCUTSKEYDETECT_H
