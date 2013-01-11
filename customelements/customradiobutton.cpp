#include "customradiobutton.h"

CustomRadioButton::CustomRadioButton(const QString &text, QWidget *parent) : QRadioButton(text, parent) {

	QString css = "QRadioButton {";
		css += "color: white;";
	css += "}";
	css += "QRadioButton::indicator:unchecked {";
		css += "image: url(:/img/radiobutton_unchecked.png);";
	css += "}";
	css += "QRadioButton::indicator:checked {";
		css += "image: url(:/img/radiobutton_checked.png);";
	css += "}";
	css += "QRadioButton {";
		css += "background:transparent;";
	css += "}";

	this->setStyleSheet(css);

	this->setCursor(Qt::PointingHandCursor);

}

CustomRadioButton::~CustomRadioButton() { }
