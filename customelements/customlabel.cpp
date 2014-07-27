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

#include "customlabel.h"

CustomLabel::CustomLabel(const QString &text, QWidget *parent) : QLabel(text, parent) {

	data = "";

	this->setWordWrap(true);

	fontsize = 9;
	fontcolor = "white";
	padding = 0;
	backgroundcolor = "transparent";
	bold = false;
	italic = false;
	lineheight = "";

	borderwidth = 0;
	bordercolor = "black";

	this->setMouseTracking(true);

	setCSS();

}

void CustomLabel::setEnabled(bool e) {

	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect;
	eff->setOpacity(e ? 1 : 0.5);
	this->setGraphicsEffect(eff);

	QLabel::setEnabled(e);

	setCSS();

}

void CustomLabel::setCSS() {

	QString css = "QLabel { ";

	css += "color: " + fontcolor + ";";
	css += "background: " + backgroundcolor + ";";
	if(borderwidth != 0) css += QString("border: %1px solid %2;").arg(borderwidth).arg(bordercolor);
	css += QString("font-size: %1pt;").arg(fontsize);
	if(bold) css += "font-weight: bold;";
	if(italic) css += "font-style: italic;";
	if(padding != 0) css += QString("padding: %1px;").arg(padding);
	if(lineheight != "") css += QString("line-height: %1;").arg(lineheight);
	css += "}";
	css += "QToolTip { font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt; background: rgba(255,255,255,200); }";

	this->setStyleSheet(css);

}

void CustomLabel::setToolTipNoWrap(bool b) {

	tooltipNoWrap = b;
	setToolTip(this->toolTip());

}

void CustomLabel::setToolTip(QString t) {

	if(tooltipNoWrap) t = "<p style='white-space:pre'>" + t + "</p>";
	QLabel::setToolTip(t);

}

CustomLabel::~CustomLabel() { }
