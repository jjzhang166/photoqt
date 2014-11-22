/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "../customelements/customscrollbar.h"
#include "../customelements/custompushbutton.h"

#include <iostream>

#include <QWidget>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QScrollArea>

// A generic widget to provide all the animation functionality
class MyWidget : public QWidget {

	Q_OBJECT

public:
	// Different constructors
	MyWidget(QString borderColor = "", QString backgroundColor = "", QWidget *parent = 0);
	MyWidget(int layoutMargin = 10, QWidget *parent = 0);
	MyWidget(QWidget *parent = 0);
	~MyWidget();

	// Show/Hide
	void makeHide();
	void makeShow();
	bool isVisible() { return isShown; }

	// If PhotoQt is resized we set the new QRects to everywhere
	void setRect(QRect rect);
	void setRects(QRect shown, QRect hidden, QRect ani) { rectShown = shown; rectHidden = hidden; rectAni = ani; }

	// Get current QRects
	QRect getRectShown() { return rectShown; }
	QRect getRectHidden() { return rectHidden; }
	QRect getRectAni() { return rectAni; }

	// Set layout to the central widget
	void setWidgetLayout(QHBoxLayout *layout) { centralWidget->setLayout(layout); }
	void setWidgetLayout(QVBoxLayout *layout) { centralWidget->setLayout(layout); }
	void setWidgetLayout(QGridLayout *layout) { centralWidget->setLayout(layout); }

	// Add a permanent widget at the bottom (outside of scrollwidget)
	void addWidgetAtBottom(QWidget *w) { mainlayout->addWidget(w); }

	// Add button layout at the bottom (outside of scrollwidget)
	void addButtonLayout(QHBoxLayout *layout) { mainlayout->addLayout(layout); }
	void addButtonLayout(QVBoxLayout *layout) { mainlayout->addLayout(layout); }

	// Some styling
	void setVisibleArea(QSize s) { visibleArea = s; borderLeftRight = -1; borderTopDown = -1; }
	void setBorderArea(int leftRight, int topDown) { visibleArea = QSize(); borderLeftRight = leftRight; borderTopDown = topDown; }
	void setFullscreen(bool fullscreen) { this->fullscreen = fullscreen; }

	// Reset the animation target
	void resetAnimationTarget(QObject *w) { ani->stop(); ani->setTargetObject(w); }

private:
	// Setup MyWidget (called from all three constructors)
	void setup(int layoutMargin, QString borderColor, QString backgroundColor);

	// The central widget
	QWidget *center;
	CustomScrollbar *scrollbar;

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

	// Some styling
	QSize visibleArea;
	QString borderColor;
	QString backgroundColor;
	int borderLeftRight;
	int borderTopDown;
	bool fullscreen;

	// Central widget and layout
	QVBoxLayout *mainlayout;
	QWidget *centralWidget;

private slots:
	// The animation functions
	void animate();
	void aniFinished();

	// Each fade step calls this function
	void fadeStep();

signals:
	// Block all function in mainwindow and activate system keys
	void blockFunc(bool block);
	void widgetClosed();

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

	// A click on the slightly darkened background (outside of central widget) closes it
	void mouseReleaseEvent(QMouseEvent *);

};



#endif // MYWIDGET_H
