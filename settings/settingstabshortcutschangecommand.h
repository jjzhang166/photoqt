#ifndef SETTINGSTABSHORTCUTSCHANGECOMMAND_H
#define SETTINGSTABSHORTCUTSCHANGECOMMAND_H

#include "../widgets/mywidget.h"
#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"

#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>

class ShortcutChangeCommand : public MyWidget {

	Q_OBJECT

public:
	ShortcutChangeCommand(QWidget *parent = 0);
	~ShortcutChangeCommand();

	// The original executeable (needed when change cancelled)
	QString command;
	// Edit executeable
	CustomLineEdit *cmd;
	// Current tile id
	QString id;
	// Current tile category
	QString cat;

public slots:
	// Save/Abort changed
	void saveChanges();
	void abortChanges();

	// This function effectively calles animate(), but adjusts a couple parameters beforehand
	void animateCmd(QString cmd, QString id);

	// Browse for an executeable
	void selectCmd();

signals:
	void commandChanged(QString identify, QString command);
	void commandCanceled(QString identify);

};

#endif // SETTINGSTABSHORTCUTSCHANGECOMMAND_H
