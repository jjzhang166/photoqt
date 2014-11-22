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

#include "mywidget.h"

// The normal contructor
MyWidget::MyWidget(QWidget *parent) : QWidget(parent) {

	setup(10, "", "");

}

// A constructor to change the margin (default: 10)
MyWidget::MyWidget(int layoutMargin, QWidget *parent) : QWidget(parent) {

	setup(layoutMargin, "", "");

}

// A constructor to change the color of the central widget border
MyWidget::MyWidget(QString borderColor, QString backgroundColor, QWidget *parent) : QWidget(parent) {

	if(borderColor == "default") borderColor = "rgb(130,130,130)";
	if(backgroundColor == "default") backgroundColor = "rgba(0,0,0,200)";

	setup(10, borderColor, backgroundColor);

}

// Setup widget
void MyWidget::setup(int layoutMargin, QString borderColor, QString backgroundColor) {

	// Some styling
	visibleArea = QSize(600,400);
	this->borderColor = borderColor;
	this->backgroundColor = (backgroundColor == "" ? "rgba(0,0,0,200)" : backgroundColor);
	borderLeftRight = -1;
	borderTopDown = -1;
	fullscreen = false;

	// The different QRects
	rectShown = QRect();
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect();

	// The current geometry and position
	isShown = false;
	this->setGeometry(rectHidden);

	// Some variables
	fadeBack = new QTimeLine;
	fadeBack->setLoopCount(5);
	backAlphaShow = 130;
	backAlphaCur = 0;
	fadeBackIN = true;
	connect(fadeBack, SIGNAL(valueChanged(qreal)), this, SLOT(fadeStep()));

	// The current widget look
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaShow));

	// the central widget containing all the information
	center = new QWidget(this);
	center->setObjectName("center");
	// For some reason, if the border is not defined right here at the beginning, it wont be visible...
	if(borderColor == "")
		center->setStyleSheet(QString("#center { background: %1; border-radius: 10px; font-size: 12pt; }").arg(this->backgroundColor));
	else
		center->setStyleSheet(QString("#center {background: %1; border-radius: 15px; font-size: 12pt; border: 2px solid %2; }").arg(this->backgroundColor).arg(borderColor));

	// The current animation framework
	ani = new QPropertyAnimation(center,"geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	// Create and set the scrollarea with main layout
	QVBoxLayout *central = new QVBoxLayout;
	central->setMargin(layoutMargin);
	QScrollArea *scroll = new QScrollArea(this);
	scroll->setObjectName("scrollWidget");
	scroll->setStyleSheet("QWidget#scrollWidget { background: transparent; padding-bottom: 3px; border-radius: 0px; }");
	QWidget *scrollWidget = new QWidget(scroll->widget());
	scrollWidget->setStyleSheet("background: transparent;");
	scroll->setWidgetResizable(true);
	scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	scrollWidget->setLayout(central);
	scroll->setWidget(scrollWidget);
	mainlayout = new QVBoxLayout;
	if(layoutMargin == 0) mainlayout->setMargin(0);
	center->setLayout(mainlayout);
	mainlayout->addWidget(scroll);

	// This widget gets the main layout set to
	centralWidget = new QWidget;
	central->addWidget(centralWidget);

	// And set the custom scrollbar
	scrollbar = new CustomScrollbar;
	scroll->setVerticalScrollBar(scrollbar);

}

void MyWidget::makeHide() {

	if(isShown) animate();

}

void MyWidget::makeShow() {

	if(!isShown) animate();

}

// If PhotoQt was resized when this widget is open, we resize it, too
void MyWidget::setRect(QRect rect) {

	rectShown = rect;
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect(rect.width()/2.0,-75,1,1);

	if(isShown) {

		this->setGeometry(rectShown);

		QRect shown;
		if(fullscreen)
			shown = rectShown;
		else if(borderLeftRight == -1 || borderTopDown == -1)
			shown = QRect((rectShown.width()-visibleArea.width())/2.0,(rectShown.height()-visibleArea.height())/2.0,visibleArea.width(),visibleArea.height());
		else
			shown = QRect(borderLeftRight, borderTopDown, rectShown.width()-borderLeftRight*2, rectShown.height()-borderTopDown*2);

		center->setGeometry(shown);

	} else
		this->setGeometry(rectHidden);

}


// The animation function
void MyWidget::animate() {

	QRect shown;
	if(fullscreen)
		shown = rectShown;
	else if(borderLeftRight == -1 || borderTopDown == -1)
		shown = QRect((rectShown.width()-visibleArea.width())/2.0,(rectShown.height()-visibleArea.height())/2.0,visibleArea.width(),visibleArea.height());
	else
		shown = QRect(borderLeftRight, borderTopDown, rectShown.width()-borderLeftRight*2, rectShown.height()-borderTopDown*2);

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		// The background is initially transparent but the geometry is full
		this->setStyleSheet(QString("background: rgba(0,0,0,0);"));
		this->setGeometry(rectShown);

		// Widget is shown
		isShown = true;

		// Animate widget
		ani->setDuration(400);
		ani->setStartValue(rectAni);
		ani->setEndValue(shown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		// Fade background in
		if(!fullscreen) {
			fadeBack->setDuration(200);
			fadeBack->setLoopCount(5);
			fadeBackIN = true;
			fadeBack->start();
		}

		// Block all base functions
		emit blockFunc(true);

		// Make sure this widget is on top
		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		// Fade background out
		if(!fullscreen) {
			fadeBack->setDuration(100);
			fadeBack->setLoopCount(5);
			fadeBackIN = false;
			fadeBack->start();
		}

		// Widget is hidden again
		isShown = false;

		// Start animation out
		ani->setDuration(300);
		ani->setStartValue(shown);
		ani->setEndValue(rectAni);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

		// Unblock all base functions
		emit blockFunc(false);

	}

}


// Every fade step for the background
void MyWidget::fadeStep() {

	// Fade in
	if(fadeBackIN) {
		backAlphaCur += backAlphaShow/5;
		if(backAlphaCur > backAlphaShow)
			backAlphaCur = backAlphaShow;
	// Fade out
	} else {
		backAlphaCur -= backAlphaShow/5;
		if(backAlphaCur < 0)
			backAlphaCur = 0;
	}

	// Update stylesheet
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaCur));

}

// Handle widget when animation is finished
void MyWidget::aniFinished() {

	// Move widget out of screen
	if(!isShown) {
		this->setGeometry(rectHidden);
		emit widgetClosed();
		scrollbar->setValue(0);
	} else
		scrollbar->setScrollbarShown();


}

// Click on background closes dialog
void MyWidget::mouseReleaseEvent(QMouseEvent *e) {

	if(!center->geometry().contains(e->pos()))
		animate();

}


void MyWidget::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

MyWidget::~MyWidget() { }
