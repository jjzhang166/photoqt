#include "quicksettings.h"

QuickSettings::QuickSettings(QMap<QString, QVariant> set, bool v, QWidget *parent) : QWidget(parent) {

	verbose = v;

	// The global settings
	globSet = set;

	// style widget
	this->setStyleSheet("QWidget { border-radius: 8px; border-bottom-left-radius: 0px; border-top-left-radius: 0px; background-color: rgba(0, 0, 0, 200); }");
	this->setContentsMargins(10,10,10,10);

	// Initiate and set layout
	central = new QVBoxLayout;
	this->setLayout(central);

	// Setup animation
	ani = new QPropertyAnimation(this,"geometry");

	// A boolean storing if widget is shown or hidden
	isShown = false;

	// block widget in certain circumstances
	blockAll = false;

	// Store if widget shall be triggered my mouse movement
	mouseTrickerEnable = true;

	// The dimensions of the widget (shown and hidden)
	rectShown = QRect(0,50,300,600);
	rectHidden = QRect(-10,-10,10,10);

	// Initially the widget it hidden
	this->setGeometry(rectHidden);

}

void QuickSettings::setRect(QRect fullscreen) {

	rectShown = QRect(fullscreen.width()-300, (fullscreen.height()-600)/3, 300, 600);
	rectHidden = QRect(fullscreen.width()+rectShown.width(),rectShown.y(),rectShown.width(),rectShown.height());

}

void QuickSettings::makeShow() {
	if(!isShown) animate();
}

void QuickSettings::makeHide() {
	if(isShown) animate();
}

// Animate open/close the widget
void QuickSettings::animate() {

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

	}

}

// Make the widget styleable
void QuickSettings::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
