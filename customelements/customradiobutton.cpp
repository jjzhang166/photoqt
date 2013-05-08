#include "customradiobutton.h"

CustomRadioButton::CustomRadioButton(const QString &text, QWidget *parent) : QRadioButton(text, parent) {

	bgColour = "transparent";
	colour = "white";
	padding = 0;
	borderRadius = 0;
	hoverColour = "";
	hovering = false;
	hoveringEnabled = false;

	imgUnchkd = ":/img/radiobutton_unchecked.png";
	imgChkd = ":/img/radiobutton_checked.png";

	setBackgroundColor("");

	this->setCursor(Qt::PointingHandCursor);

	connect(this, SIGNAL(toggled(bool)), this, SLOT(checkToggle()));

}

void CustomRadioButton::checkToggle() {

	if(!this->geometry().contains(QCursor::pos()) && !this->isChecked()) {
		hovering = false;
		setCSS();
	} else if(this->isChecked()) {
		hovering = true;
		setCSS();
	}

}

void CustomRadioButton::setHoverEffect(QString hovCol) {

	hoveringEnabled = true;

	hoverColour = hovCol;

}

void CustomRadioButton::setBackgroundColor(QString col) {

	bgColour = col;
	if(bgColour == "")
		bgColour = "transparent";

	if(hoverColour == "")
		hoverColour = bgColour;

	setCSS();

}

void CustomRadioButton::setColor(QString col) {

	colour = col;
	if(colour == "")
		colour = "white";

	setCSS();

}

void CustomRadioButton::setPadding(int pad) {

	padding = pad;
	setCSS();

}

void CustomRadioButton::setBorderRadius(int rad) {

	borderRadius = rad;
	setCSS();

}

void CustomRadioButton::setCSS() {


	QString css = "QRadioButton {";
		css += QString("color: %1;").arg(colour);
		css += QString("background-color: %1;").arg((hovering && hoveringEnabled) ? hoverColour: bgColour);
		css += QString("padding: %1px;").arg(padding);
		css += QString("border-radius: %1px;").arg(borderRadius);
	css += "}";
	css += "QRadioButton::indicator:unchecked {";
		css += QString("image: url(%1);").arg(imgUnchkd);
	css += "}";
	css += "QRadioButton::indicator:checked {";
		css += QString("image: url(%1);").arg(imgChkd);
	css += "}";
	css += "QRadioButton::indicator {";
		css += "width: 15px;";
		css += "height: 15px;";
	css += "}";

	this->setStyleSheet(css);

}

// change indicator image
void CustomRadioButton::setIndicatorImage(QString chkd, QString unchkd) {

	imgChkd = chkd;
	imgUnchkd = unchkd;

	setCSS();

}

void CustomRadioButton::enterEvent(QEvent *e) {

	hovering = true;
	setCSS();

	QRadioButton::enterEvent(e);

}

void CustomRadioButton::mouseMoveEvent(QMouseEvent *e) {

	hovering = true;
	setCSS();

	QRadioButton::mouseMoveEvent(e);

}

void CustomRadioButton::leaveEvent(QEvent *e) {

	if(!this->isChecked())
		hovering = false;
	setCSS();

	QRadioButton::leaveEvent(e);

}

CustomRadioButton::~CustomRadioButton() { }
