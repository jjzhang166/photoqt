#ifndef STARTUPWIDGET_H
#define STARTUPWIDGET_H

#include "../customelements/customscrollbar.h"
#include "../customelements/custompushbutton.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollArea>

class StartUpWidget : public QWidget {

	Q_OBJECT

public:
	StartUpWidget(QWidget *parent = 0);
	~StartUpWidget();

	// There's a little difference between an update message and a fresh install message
	void setUpdateMsg();
	void setInstallMsg();

	void makeShow();
	void makeHide();
	bool isVisible() { return isShown; }
	void setRect(QRect rect);


private:

	// The geometries of the widget
	QRect rectShown;
	QRect rectHidden;
	QRect aniStart;

	// Two display booleans
	bool isShown;

	// The property animation
	QPropertyAnimation *ani;

	// the custom scrollbar on the right
	CustomScrollbar *scrollbar;

	// The title and intro can be customised by above functions
	QLabel *title;
	QLabel *customIntro;


private slots:
	// The animation functions
	void animate();
	void aniFinished();

signals:
	// Block all function in mainwindow and activate system keys
	void finished();

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

};

#endif // STARTUPWIDGET_H
