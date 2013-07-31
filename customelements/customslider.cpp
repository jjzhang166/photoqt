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

#include "customslider.h"

CustomSlider::CustomSlider(Qt::Orientation orientation, QWidget *parent) : QSlider(orientation,parent) {

	QString css = "QSlider::groove:horizontal {";
		css += "border: 1px solid #000000;";
		css += "height: 6px;";
		css += "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ededed, stop:1 #ffffff);";
		css += "margin: 2px 0;";
	css += "}";

	css += "QSlider::handle:horizontal {";
		css += "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #000000, stop:1 #282828);";
		css += "border: 1px solid grey;";
		css += "width: 18px;";
		css += "margin: -2px 0;";
		css += "border-radius: 3px;";
	css += "}";

	this->setStyleSheet(css);
	this->setCursor(Qt::OpenHandCursor);

	this->setTickPosition(QSlider::TicksBelow);

}

void CustomSlider::mousePressEvent(QMouseEvent *ev) {
	this->setCursor(Qt::ClosedHandCursor);
	QSlider::mousePressEvent(ev);
}

void CustomSlider::mouseReleaseEvent(QMouseEvent *ev) {
	this->setCursor(Qt::OpenHandCursor);
	QSlider::mouseReleaseEvent(ev);
}

CustomSlider::~CustomSlider() { }
