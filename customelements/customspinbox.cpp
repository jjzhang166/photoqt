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

#include "customspinbox.h"

CustomSpinBox::CustomSpinBox(QWidget *parent) : QSpinBox(parent) {

	this->setCursor(Qt::PointingHandCursor);
	this->setButtonSymbols(QSpinBox::NoButtons);

	backgroundCol = "";
	selectionCol = "";
	fontcolor = "white";

	setCSS();

}

void CustomSpinBox::setCSS() {

	QString css = "QSpinBox {";
	if(backgroundCol != "" || selectionCol != "") {
		css += "background: " + backgroundCol + ";";
		css += "selection-background-color: " + selectionCol + ";";
	} else
		css += "selection-background-color: transparent;";
	if(borderCol != "" && borderWidth > 0)
		css += QString("border: %1px solid %2;").arg(borderWidth).arg(borderCol);
	css += QString("color: %1;").arg(fontcolor);
	css += "}";
	css += "QToolTip {font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt; background: rgba(255,255,255,200); }";


	this->setStyleSheet(css);

}

void CustomSpinBox::setEnabled(bool b) {

	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect;
	eff->setOpacity(b ? 1 : 0.5);
	this->setGraphicsEffect(eff);

	QSpinBox::setEnabled(b);

}

CustomSpinBox::~CustomSpinBox() { }
