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

#include "custompushbutton.h"
#include <QtDebug>

CustomPushButton::CustomPushButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {

	hover = false;
	enabled = true;
	padding = 0;
	bold = false;
	fontsize = "";

	this->setCursor(Qt::PointingHandCursor);

	this->setMouseTracking(true);

	setCSS();
	this->setToolTip(text);

}

// When disabled, we need to adjust some colors to make it visible
void CustomPushButton::setEnabled(bool e) {

	enabled = e;
	setCSS();
	QPushButton::setEnabled(enabled);

}

void CustomPushButton::setCSS() {

	QString css = "QPushButton {";
	css += "background:" + (rgba == "" ?
				 (hover ? (enabled ?  "rgba(80,80,80,100);" : "rgba(40,40,40,100);") : "rgba(20,20,20,150);") :
				  rgba + ";");
	css += "color:" + QString(enabled ? "white;" : "grey;");
	css += "font-weight:" + QString(bold ? "bold;" : "normal;");

	css += "padding:" + (padding != 0 ? QString("%1px;").arg(padding) : "6px 10px;");
	css += "border: 1px solid " + QString(enabled ? "rgba(100,100,100,100);" : "rgba(50,50,50,100);");
	css += "border-radius: 0;";
	if(fontsize != "") css += QString("font-size: %1;").arg(fontsize);

	css += "}";

	css += "QToolTip {font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt; background: rgba(255,255,255,200); }";

	this->setStyleSheet(css);

}

void CustomPushButton::enterEvent(QEvent *) {

	hover = true;

	setCSS();

}

void CustomPushButton::leaveEvent(QEvent *) {

	hover = false;

	setCSS();

}

CustomPushButton::~CustomPushButton() { }
