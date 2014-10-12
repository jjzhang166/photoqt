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

#ifndef CUSTOMCHECKBOX_H
#define CUSTOMCHECKBOX_H

#include <QCheckBox>
#include <QGraphicsOpacityEffect>

// A custom styled checkbox
class CustomCheckBox : public QCheckBox {

	Q_OBJECT

public:
	explicit CustomCheckBox(const QString &text = "", QWidget *parent = 0);
	~CustomCheckBox();

	// The tick indicator can be resized
	void setIndicatorSize(int indicatorsize);
	void setIndicatorSize(int indicatorWidth, int indicatorHeight);

	// And the font color can be changed
	void setFontColor(QString col);

	// A custom indicator image can be set
	void setIndicatorImage(QString imgChkd, QString imgUnchkd);

	// The font size can be adjusted
	void setFontSize(int size);

public slots:
	void setEnabled(bool e);
	void setDisabled(bool b) { setEnabled(!b); }


private:
	// The variables below correspond to the functions above, but they shouldn't be accessed/changed directly
	int indicSizeW;
	int indicSizeH;
	QString fontCol;
	QString imgChkd;
	QString imgUnchkd;
	int fontsize;

};

#endif // CUSTOMCHECKBOX_H
