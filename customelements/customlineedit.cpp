#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QString txt, QWidget *parent) : QLineEdit(txt, parent) {

	// A value of 0 leaves the default
	width = 0;

	QString css = "QLineEdit {";
		css += "border: 2px solid gray;";
		css += "border-radius: 10px;";
		css += "padding: 4px 8px;";
		css += "color: white;";
		css += "background: rgba(0,0,0,100);";
		css += "selection-background-color: white;";
		css += "selection-color: black;";
	css += "}";

	this->setStyleSheet(css);

}

// Set minimum width of line edit
void CustomLineEdit::setMinWidth(int w) {

	width = w;

	QString css = "QLineEdit {";
		css += "border: 2px solid gray;";
		css += "border-radius: 10px;";
		css += "padding: 4px 8px;";
		css += "color: white;";
		css += QString("min-width: %1px;").arg(width);
		css += "background: rgba(0,0,0,100);";
		css += "selection-background-color: white;";
		css += "selection-color: black;";
	css += "}";


	this->setStyleSheet(css);

}


void CustomLineEdit::mouseReleaseEvent(QMouseEvent *) {
	emit clicked();
}


CustomLineEdit::~CustomLineEdit() { }
