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

#include "customcheckbox.h"

CustomCheckBox::CustomCheckBox(const QString &text, QWidget *parent) : QCheckBox(text, parent) {

	this->setCursor(Qt::PointingHandCursor);

	// Default indicator size
	indicSizeH = 15;
	indicSizeW = 15;

	// A fontsize of 0 leaves the default
	fontsize = 0;

	// Standard indicator image
	imgChkd = ":/img/checkbox_checked.png";
	imgUnchkd = ":/img/checkbox_unchecked.png";
	imgChkdDisabled = ":/img/checkbox_checked_disabled.png";
	imgUnchkdDisabled = ":/img/checkbox_unchecked_disabled.png";

	// Standard font color
	fontCol = "white";

	setCSS();

	this->setToolTip(text);

}

// The tick indicator can be resized
void CustomCheckBox::setIndicatorSize(int indicatorsize) {

	setIndicatorSize(indicatorsize,indicatorsize);

}

void CustomCheckBox::setIndicatorSize(int w, int h) {

	indicSizeW = w;
	indicSizeH = h;

	setCSS();

}

// And the font color can be changed
void CustomCheckBox::setFontColor(QString col, QString colDisabled) {

	fontCol = col;
	fontColDisabled = colDisabled;

	setCSS();

}

// Adjust font size
void CustomCheckBox::setFontSize(int size) {

	fontsize = size;

	setCSS();

}

// change indicator image
void CustomCheckBox::setIndicatorImage(QString chkd, QString unchkd, QString chkdDisabled, QString unchkdDisabled) {

	imgChkd = chkd;
	imgUnchkd = unchkd;

	imgChkdDisabled = chkdDisabled;
	imgUnchkdDisabled = unchkdDisabled;

	setCSS();

}

void CustomCheckBox::setCSS() {

	QString css = "QCheckBox::indicator:unchecked {";
		css += QString("image: url(%1);").arg(isEnabled() ? imgUnchkd : imgUnchkdDisabled);
	css += "}";
	css += "QCheckBox::indicator:checked {";
		css += QString("image: url(%1);").arg(isEnabled() ? imgChkd : imgChkdDisabled);
	css += "}";
	css += "QCheckBox {";
		css += "background:transparent;";
		if(fontsize != 0)
			css += QString("font-size: %1pt;").arg(fontsize);
		css += "color: " + (isEnabled() ? fontCol : fontColDisabled) + ";";
	css += "}";
	css += "QCheckBox::indicator {";
		css += QString("width: %1px;").arg(indicSizeW);
		css += QString("height: %1px;").arg(indicSizeH);
	css += "}";
	css += "QToolTip {font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt; background: rgba(255,255,255,200); }";

	this->setStyleSheet(css);

}

void CustomCheckBox::setEnabled(bool e) {

	QCheckBox::setEnabled(e);

	setCSS();

}

CustomCheckBox::~CustomCheckBox() { }
