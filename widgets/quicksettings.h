#ifndef QUICKSETTINGS_H
#define QUICKSETTINGS_H

//#include "../widgets/mywidget.h"
#include "../customelements/customlabel.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>

class QuickSettings : public QWidget {

	Q_OBJECT

public:
	QuickSettings(QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false, QWidget *parent = 0);

	bool verbose;

	// Global Settings
	QMap<QString,QVariant> globSet;

	bool isVisible() { return isShown; }
	void makeHide();
	void makeShow();

	void setRect(QRect fullscreen);

private:
	// This boolean stores if the widget is shown or hidden
	bool isShown;

	// The blocking boolean
	bool blockAll;

	// The two rects for the two states (hidden/shown)
	QRect rectShown;
	QRect rectHidden;

	// The main layout of the widget
	QVBoxLayout *central;

	// The animation class
	QPropertyAnimation *ani;

	// This boolean stores if mouse triggering is en-/disabled
	bool mouseTrickerEnable;

private slots:

	// Animate open/close the widget
	void animate();

signals:
	// Tell mainwindow.cpp to update the settings
	void updateSettings(QMap<QString,QVariant> newset);

protected:
	void paintEvent(QPaintEvent *);

};


#endif // QUICKSETTINGS_H
