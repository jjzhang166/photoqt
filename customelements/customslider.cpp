#include "customslider.h"

CustomSlider::CustomSlider(Qt::Orientation orientation, QWidget *parent) : QSlider(orientation,parent) {

	QString css = "QSlider::groove:horizontal {";
		css += "border: 1px solid #000000;";
		css += "height: 6px;";
		css += "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ededed, stop:1 #ffffff);";
		css += "margin: 2px 0;";
	css += "}";

	css += "QSlider::handle:horizontal {";
		css += "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #000000, stop:1 #282828);";
		css += "border: 1px solid grey;";
		css += "width: 18px;";
		css += "margin: -2px 0;";
		css += "border-radius: 3px;";
	css += "}";

	this->setStyleSheet(css);
	this->setCursor(Qt::OpenHandCursor);

	this->setTickPosition(QSlider::TicksBelow);

}

void CustomSlider::mousePressEvent(QMouseEvent *ev) {
	this->setCursor(Qt::ClosedHandCursor);
	QSlider::mousePressEvent(ev);
}

void CustomSlider::mouseReleaseEvent(QMouseEvent *ev) {
	this->setCursor(Qt::OpenHandCursor);
	QSlider::mouseReleaseEvent(ev);
}

CustomSlider::~CustomSlider() { }
