#include "custompushbutton.h"

CustomPushButton::CustomPushButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {

	noRoundedCorners = false;

	this->setStyleSheet("color: white; background: rgba(0,0,0,150); padding: 4px 8px; border-radius: 8px; border: 1px solid white;");

	this->setCursor(Qt::PointingHandCursor);

}

// Per default the button has rounded corners - can be disabled
void CustomPushButton::removeRoundedCorners() {
	this->setStyleSheet("color: white; background: rgba(0,0,0,150); padding: 4px 8px; border: 1px solid white; border-radius: 0;");
	noRoundedCorners = true;
}

// When disabled, we need to adjust some colors to make it visible
void CustomPushButton::setEnabled(bool enabled) {

	QString css = "background: rgba(0,0,0,150);";
	if(enabled)
		css += "color: white;";
	else
		css += "color: grey;";
	css += "padding: 4px 8px;";
	if(enabled)
		css += "border: 1px solid white;";
	else
		css += "border: 1px solid grey;";
	if(!noRoundedCorners)
		css += "border-radius: 8px;";
	else
		css += "border-radius: 0;";
	this->setStyleSheet(css);

	QPushButton::setEnabled(enabled);

}

CustomPushButton::~CustomPushButton() { }
