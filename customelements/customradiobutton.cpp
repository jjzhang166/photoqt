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

#include "customradiobutton.h"

CustomRadioButton::CustomRadioButton(const QString &text, QWidget *parent) : QRadioButton(text, parent) {

	bgColour = "transparent";
	colour = "white";
	padding = 0;
	borderRadius = 0;
	hoverColour = "";
	hovering = false;
	hoveringEnabled = false;

	imgUnchkd = ":/img/radiobutton_unchecked.png";
	imgUnchkdDis = ":/img/radiobutton_unchecked_disabled.png";
	imgChkd = ":/img/radiobutton_checked.png";
	imgChkdDis = ":/img/radiobutton_checked_disabled.png";

	setBackgroundColor("");

	this->setCursor(Qt::PointingHandCursor);

	connect(this, SIGNAL(toggled(bool)), this, SLOT(checkToggle()));

	setCSS();
	this->setToolTip(text);

}

void CustomRadioButton::checkToggle() {

	if(!this->geometry().contains(QCursor::pos()) && !this->isChecked()) {
		hovering = false;
		setCSS();
	} else if(this->isChecked()) {
		hovering = true;
		setCSS();
	}

}

void CustomRadioButton::setHoverEffect(QString hovCol) {

	hoveringEnabled = true;

	hoverColour = hovCol;

}

void CustomRadioButton::setBackgroundColor(QString col) {

	bgColour = col;
	if(bgColour == "")
		bgColour = "transparent";

	if(hoverColour == "")
		hoverColour = bgColour;

	setCSS();

}

void CustomRadioButton::setColor(QString col) {

	colour = col;
	if(colour == "")
		colour = "white";

	setCSS();

}

void CustomRadioButton::setPadding(int pad) {

	padding = pad;
	setCSS();

}

void CustomRadioButton::setBorderRadius(int rad) {

	borderRadius = rad;
	setCSS();

}

void CustomRadioButton::setEnabled(bool b) {

	QRadioButton::setEnabled(b);

	setCSS();

}

void CustomRadioButton::setCSS() {


	QString css = "QRadioButton {";
		css += QString("color: %1;").arg(colour);
		css += QString("background-color: %1;").arg((hovering && hoveringEnabled) ? hoverColour : bgColour);
		css += QString("padding: %1px;").arg(padding);
		css += QString("border-radius: %1px;").arg(borderRadius);
	css += "}";
	css += "QRadioButton::indicator:unchecked {";
		css += QString("image: url(%1);").arg(this->isEnabled() ? imgUnchkd : imgUnchkdDis);
	css += "}";
	css += "QRadioButton::indicator:checked {";
		css += QString("image: url(%1);").arg(this->isEnabled() ? imgChkd : imgChkdDis);
	css += "}";
	css += "QRadioButton::indicator {";
		css += "width: 15px;";
		css += "height: 15px;";
	css += "}";
	css += "QRadioButton:disabled {";
		css += "color: grey;";
	css += "}";
	css += "QToolTip {font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt; background: rgba(255,255,255,200); }";

	this->setStyleSheet(css);

}

// change indicator image
void CustomRadioButton::setIndicatorImage(QString chkd, QString chkdDis, QString unchkd, QString unchkDis) {

	imgChkd = chkd;
	imgChkdDis = chkdDis;
	imgUnchkd = unchkd;
	imgUnchkdDis = unchkDis;

	setCSS();

}

void CustomRadioButton::enterEvent(QEvent *e) {

	hovering = true;
	setCSS();

	QRadioButton::enterEvent(e);

}

void CustomRadioButton::mouseMoveEvent(QMouseEvent *e) {

	hovering = true;
	setCSS();

	QRadioButton::mouseMoveEvent(e);

}

void CustomRadioButton::leaveEvent(QEvent *e) {

	if(!this->isChecked())
		hovering = false;
	setCSS();

	QRadioButton::leaveEvent(e);

}

CustomRadioButton::~CustomRadioButton() { }
