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

#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QComboBox>
#include <QGraphicsOpacityEffect>

// A custom styled combobox
class CustomComboBox : public QComboBox {
	Q_OBJECT
public:
	explicit CustomComboBox(QWidget *parent = 0);
	~CustomComboBox();

	void setFontSize(int fsize);
	void setBorder(int width, QString colour);

private:
	int fontsize;
	bool enabled;

	int borderWidth;
	QString borderColour;

	void setCSS();

private slots:
	void updateToolTip(QString);

public slots:
	// When disabled, we need to adjust some colors to make it visible
	void setEnabled(bool);
	
};

#endif // CUSTOMCOMBOBOX_H
