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

	this->setWordWrap(true);

	fontsize = "9pt";
	fontcolor = "white";
	tooltipColor = "white";
	bold = false;

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
	css += "background: transparent;";
	css += "font-size: " + fontsize + ";";
	if(bold) css += "font-weight: bold;";
	css += "}";
	css += "QToolTip { padding: 1px; opacity: 250; color: " + tooltipColor +"; }";

	this->setStyleSheet(css);

}

CustomLabel::~CustomLabel() { }
