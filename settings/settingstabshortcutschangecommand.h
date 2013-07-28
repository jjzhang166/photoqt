#ifndef SETTINGSTABSHORTCUTSCHANGECOMMAND_H
#define SETTINGSTABSHORTCUTSCHANGECOMMAND_H

#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"

#include <QWidget>
#include <QStyleOption>
#include <QTimeLine>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QPainter>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>

class ShortcutChangeCommand : public QWidget {

	Q_OBJECT

public:
	ShortcutChangeCommand(QWidget *parent = 0);
	~ShortcutChangeCommand();

	// These are the dimensions
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// The animation instance
	QPropertyAnimation *ani;
	bool isShown;

	// The central widget containing all the content
	QWidget *center;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	// The original executeable (needed when change cancelled)
	QString command;
	// Edit executeable
	CustomLineEdit *cmd;
	// Current tile id
	QString id;
	// Current tile category
	QString cat;

public slots:
	// The animation/fading functions
	void animate();
	void fadeStep();
	void aniFinished();

	// Save/Abort changed
	void saveChanges();
	void abortChanges();

	// This function effectively calles animate(), but adjusts a couple parameters beforehand
	void animateCmd(QString cmd, QString id);

	// Browse for an executeable
	void selectCmd();

protected:
	void paintEvent(QPaintEvent *);

signals:
	void commandChanged(QString identify, QString command);
	void commandCanceled(QString identify);

};

#endif // SETTINGSTABSHORTCUTSCHANGECOMMAND_H
