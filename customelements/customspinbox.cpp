#include "customspinbox.h"

CustomSpinBox::CustomSpinBox(QWidget *parent) : QSpinBox(parent) {

	this->setCursor(Qt::PointingHandCursor);
	this->setButtonSymbols(QSpinBox::NoButtons);

	QString css = "QSpinBox {";
		css += "selection-background-color: transparent;";
		css += "color: white;";
	css += "}";


	this->setStyleSheet(css);

}

CustomSpinBox::~CustomSpinBox() { }
