#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include "../customelements/customscrollbar.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>

class About : public QWidget {

	Q_OBJECT

public:
	About(QWidget *parent = 0);

	void setLicense(QString version);

	void makeHide();
	void makeShow();
	bool isVisible() { return isShown; }
	void setRect(QRect rect);


private:
	// The central widget
	QWidget *center;

	// A button to close the widget
	CustomPushButton *close;

	// Boolean about current geometry/position
	bool isShown;

	// The different QRects
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// The animation for the content widget
	QPropertyAnimation *ani;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	// The license at top
	QLabel *license;

private slots:
	// The animation functions
	void animate();
	void aniFinished();

	// Each fade step calls this function
	void fadeStep();

	void openWebsite();

signals:
	// Block all function in mainwindow and activate system keys
	void blockFunc(bool block);

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

	void mouseReleaseEvent(QMouseEvent *);

};

#endif // ABOUTWIDGET_H
