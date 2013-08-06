#ifndef SETTINGSTABOTHERCONTEXTTILES_H
#define SETTINGSTABOTHERCONTEXTTILES_H

#include "../customelements/customlabel.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>

class ContextTile : public QWidget {

	Q_OBJECT

public:
	ContextTile(QString cmdTxt, QString descTxt, QWidget *parent = 0);
	~ContextTile();

	// The command and the menu text
	QLineEdit *cmd;
	QLineEdit *desc;

	// The index in the layout
	int index;

	// SDome css
	QString css;
	QString cssBackgroundNorm;
	QString cssBackgroundHov;

	// A label for dragging
	CustomLabel *dragLabel;

public slots:
	// When user requested deletion
	void pleaseDeleteMe();

signals:
	// Tell parent to delete this index
	void deleteMe(int ind);

protected:
	void paintEvent(QPaintEvent *);

};


#endif // SETTINGSOTHERCONTEXTTILE_H
