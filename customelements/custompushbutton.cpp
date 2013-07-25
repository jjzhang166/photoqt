#include "custompushbutton.h"
#include <QtDebug>

CustomPushButton::CustomPushButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {

	noRoundedCorners = false;
	hover = false;
	enabled = true;
	padding = 0;

	this->setCursor(Qt::PointingHandCursor);

	this->setMouseTracking(true);

	setCSS();

}

// Per default the button has rounded corners - can be disabled
void CustomPushButton::removeRoundedCorners() {
	noRoundedCorners = true;
	setCSS();
}

// When disabled, we need to adjust some colors to make it visible
void CustomPushButton::setEnabled(bool e) {

	enabled = e;
	setCSS();
	QPushButton::setEnabled(enabled);

}

void CustomPushButton::setCSS() {

	QString css = "";
	if(rgba == "")
		css += hover ? "background: rgba(80,80,80,100);" : "background: rgba(20,20,20,150);";
	else
		css += "background: " + rgba + ";";
	enabled ? css += "color: white;" : css += "color: grey;";

	(padding != 0) ? css += QString("padding: %1px;").arg(padding) : css += "padding: 6px 10px;";
	enabled ? css += "border: 1px solid rgba(100,100,100,100);" : css += "border: 1px solid rgba(50,50,50,100);";
	(!noRoundedCorners) ? css += "border-radius: 8px;" : css += "border-radius: 0;";

	this->setStyleSheet(css);

}

void CustomPushButton::enterEvent(QEvent *) {

	hover = true;

	setCSS();

}

void CustomPushButton::leaveEvent(QEvent *) {

	hover = false;

	setCSS();

}

CustomPushButton::~CustomPushButton() { }
