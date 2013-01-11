#include "customcombobox.h"

CustomComboBox::CustomComboBox(QWidget *parent) : QComboBox(parent) {

	QString css = "QComboBox {";
		css += "border: 1px solid rgba(255,255,255,40);";
		css += "border-radius: 0px;";
		css += "min-width: 130px;";
		css += "color: white;";
	css += "}";

	css += "QComboBox:on {";
		css += "padding-top: 0px;";
		css += "padding-left: 4px;";
	css += "}";

	css += "QComboBox QAbstractItemView {";
		css += "border-radius: 0px;";
		css += "color: white;";
		css += "selection-background-color: white;";
		css += "selection-color: black;";
	css += "}";


	css += "QComboBox::drop-down::down-arrow {";
		css += "image: url(:/img/empty.png);";
	css += "}";

	css += "QComboBox::drop-down::down-arrow:on {";
		css += "image: url(:/img/empty.png);";
	css += "}";

	this->setStyleSheet(css);

}

void CustomComboBox::setEnabled(bool enabled) {

	QString css = "QComboBox {";
		css += "border: 1px solid rgba(255,255,255,40);";
		css += "background-color: rgb(0,0,0);";
		css += "border-radius: 0px;";
		css += "min-width: 130px;";
		if(enabled)
			css += "color: white;";
		else
			css += "color: grey;";
	css += "}";

	css += "QComboBox:on {";
		css += "padding-top: 0px;";
		css += "padding-left: 4px;";
	css += "}";

	css += "QComboBox QAbstractItemView {";
		css += "border-radius: 0px;";
		if(enabled)
			css += "color: white;";
		else
			css += "color: grey;";
		css += "selection-background-color: white;";
		css += "selection-color: black;";
	css += "}";


	css += "QComboBox::drop-down::down-arrow {";
		css += "image: url(:/img/empty.png);";
	css += "}";

	css += "QComboBox::drop-down::down-arrow:on {";
		css += "image: url(:/img/empty.png);";
	css += "}";

	this->setStyleSheet(css);

	QComboBox::setEnabled(enabled);

}

CustomComboBox::~CustomComboBox() { }

