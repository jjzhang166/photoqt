/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
