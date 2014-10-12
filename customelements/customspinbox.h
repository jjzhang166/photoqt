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

#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include <QSpinBox>
#include <QGraphicsOpacityEffect>

// A custom (styled) spinbox
class CustomSpinBox : public QSpinBox {

	Q_OBJECT

public:
	explicit CustomSpinBox(QWidget *parent = 0);
	~CustomSpinBox();

private:
	QString backgroundCol;
	QString selectionCol;
	QString borderCol;
	int borderWidth;
	QString fontcolor;

public slots:
	void setEnabled(bool);
	void setBackground(QString col, QString selectionCol) { backgroundCol = col; this->selectionCol = selectionCol; setCSS(); }
	void setBorder(QString col, int w) { borderCol = col; borderWidth = w; setCSS(); }
	void setFontColor(QString col) { fontcolor = col; setCSS(); }

private:
	void setCSS();

};

#endif // CUSTOMSPINBOX_H
