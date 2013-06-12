#ifndef DROPDOWNMENU_H
#define DROPDOWNMENU_H

#include "dropdownmenuitem.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>

class DropDownMenu : public QWidget {

	Q_OBJECT

private:
	// Set specialised function
	void setSlideshow();

	// Set standard function
	void setStandard(QString txt, QString icon, QString data, int active);

	void setSep();
	void setHeading(QString txt);


public:
	DropDownMenu(QWidget *parent = 0);
	~DropDownMenu();

	void setRect(QRect rect);

	void setSensitivity(int xSens, int ySens);

	void makeShow();
	void makeHide();
	bool isVisible() { return isShown; }


private:
	// We use Qt's property animation feature
	QPropertyAnimation *ani;

	// This booleand stores, if the widget is hidden/shown
	bool isShown;

	// The dimensions of the widget
	QRect rectHidden;
	QRect rectShown;

	// The size of the sensitivity area
	int ySensitivity;
	int xSensitivity;

	// Stores if menu position is top or bottom
	bool posAtBottom;

	// The layout holding all the labels
	QVBoxLayout *mLay;

public slots:
	// Animate the menu in/out
	void animate();

	// A click on a menu item
	void menuClicked();

protected:
	// Making styling of widget possible again
	void paintEvent(QPaintEvent *);

signals:
	void itemClicked(QString,int);

};

#endif // DROPDOWNMENU_H
