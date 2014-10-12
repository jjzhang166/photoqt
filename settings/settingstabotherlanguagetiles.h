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
#ifndef SETTINGSTABOTHERLANGUAGETILES_H
#define SETTINGSTABOTHERLANGUAGETILES_H

#include "../customelements/customradiobutton.h"
#include "../customelements/customlabel.h"
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>

class SettingsTabOtherLanguageTiles : public QWidget {

	Q_OBJECT

public:
	SettingsTabOtherLanguageTiles(QString lang, QString code, QWidget *parent = 0);
	~SettingsTabOtherLanguageTiles();

	// The css
	QString css;
	QString cssBackgroundNorm;
	QString cssBackgroundHov;

	// The back label (this is the widget being styled
	CustomLabel *back;

	// The language code (usually two or three letters)
	QString langCode;

	// A button to select it
	CustomRadioButton *button;

public slots:
	// Toggle button
	void buttonToggled(bool tgld);

protected:
	void mouseMoveEvent(QMouseEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);

};

#endif // SETTINGSTABOTHERLANGUAGETILES_H
