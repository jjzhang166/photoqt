#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QString txt, QWidget *parent) : QLineEdit(txt, parent) {

	// A value of 0 leaves the default
	width = 0;

	borderColor = "rgba(100,100,100,100)";
	borderWidth = 1;

	setCSS();

}

// Set minimum width of line edit
void CustomLineEdit::setMinWidth(int w) {

	width = w;

	setCSS();

}

void CustomLineEdit::setBorder(QString col, int w) {
	borderColor = col;
	borderWidth = w;
	setCSS();
}

void CustomLineEdit::setCSS() {

	QString css = "QLineEdit {";
		css += QString("border: %1px solid %2;").arg(borderWidth).arg(borderColor);
		css += "border-radius: 10px;";
		css += "padding: 4px 8px;";
		css += "color: white;";
		if(width != 0) css += QString("min-width: %1px;").arg(width);
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
