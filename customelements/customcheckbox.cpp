#include "customcheckbox.h"

CustomCheckBox::CustomCheckBox(const QString &text, QWidget *parent) : QCheckBox(text, parent) {

	this->setCursor(Qt::PointingHandCursor);

	// Default indicator size
	indicSize = 15;

	// A fontsize of 0 leaves the default
	fontsize = 0;

	// The stylesheet
	QString css = "";

	css += "QCheckBox::indicator:unchecked {";
		css += "image: url(:/img/checkbox_unchecked.png);";
	css += "}";
	css += "QCheckBox::indicator:checked {";
		css += "image: url(:/img/checkbox_checked.png);";
	css += "}";
	css += "QCheckBox {";
		css += "background:transparent;";
		css += "color: white;";
	css += "}";
	css += "QCheckBox::indicator {";
		css += QString("width: %1px;").arg(indicSize);
		css += QString("height: %1px;").arg(indicSize);
	css += "}";

	// Standard indicator image
	imgChkd = ":/img/checkbox_checked.png";
	imgUnchkd = ":/img/checkbox_unchecked.png";

	// Standard font color
	fontCol = "white";

	this->setStyleSheet(css);

}

// The tick indicator can be resized
void CustomCheckBox::setIndicatorSize(int indicatorsize) {

	QString css = "QCheckBox::indicator:unchecked {";
		css += QString("image: url(%1);").arg(imgUnchkd);
	css += "}";
	css += "QCheckBox::indicator:checked {";
		css += QString("image: url(%1);").arg(imgChkd);
	css += "}";
	css += "QCheckBox {";
		css += "background:transparent;";
		if(fontsize != 0)
			css += QString("font-size: %1pt;").arg(fontsize);
		css += "color: " + fontCol + ";";
	css += "}";
	css += "QCheckBox::indicator {";
		css += QString("width: %1px;").arg(indicatorsize);
		css += QString("height: %1px;").arg(indicatorsize);
	css += "}";

	indicSize = indicatorsize;

	this->setStyleSheet(css);

}

// And the font color can be changed
void CustomCheckBox::setFontColor(QString col) {

	QString css = "QCheckBox::indicator:unchecked {";
		css += QString("image: url(%1);").arg(imgUnchkd);
	css += "}";
	css += "QCheckBox::indicator:checked {";
		css += QString("image: url(%1);").arg(imgChkd);
	css += "}";
	css += "QCheckBox {";
		css += "background:transparent;";
		if(fontsize != 0)
			css += QString("font-size: %1pt;").arg(fontsize);
		css += "color: " + col + ";";
	css += "}";
	css += "QCheckBox::indicator {";
		css += QString("width: %1px;").arg(indicSize);
		css += QString("height: %1px;").arg(indicSize);
	css += "}";

	fontCol = col;

	this->setStyleSheet(css);

}

// Adjust font size
void CustomCheckBox::setFontSize(int size) {

	fontsize = size;

	setIndicatorSize(indicSize);

}

// change indicator image
void CustomCheckBox::setIndicatorImage(QString chkd, QString unchkd) {

	imgChkd = chkd;
	imgUnchkd = unchkd;

	setIndicatorSize(indicSize);

}


CustomCheckBox::~CustomCheckBox() { }
