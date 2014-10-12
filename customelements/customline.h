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

#ifndef CUSTOMLINE_H
#define CUSTOMLINE_H

#include <QFrame>

class CustomLine : public QFrame {
public:
	explicit CustomLine(QString color = "white", Qt::Orientation o = Qt::Horizontal) {
		if(o == Qt::Vertical) {
			this->setFixedWidth(1);
			this->setFrameShape(QFrame::VLine);
		} else {
			this->setFixedHeight(1);
			this->setFrameShape(QFrame::HLine);
		}
		this->setFrameShadow(QFrame::Sunken);
		this->setStyleSheet("background-color: " + color);
	}

};

#endif // CUSTOMLINE_H
