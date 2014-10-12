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

#ifndef CUSTOMRADIOBUTTON_H
#define CUSTOMRADIOBUTTON_H

#include <QRadioButton>

// A custom (styled) radiobutton
class CustomRadioButton : public QRadioButton {

	Q_OBJECT

public:
	explicit CustomRadioButton(const QString &text = "", QWidget *parent = 0);
	~CustomRadioButton();

	void setBackgroundColor(QString col);
	void setColor(QString col);
	void setPadding(int pad);
	void setBorderRadius(int rad);
	void setHoverEffect(QString hovCol);
	void setIndicatorImage(QString chkd, QString chkdDis, QString unchkd, QString unchkDis);

public slots:
	void setEnabled(bool);

private:
	QString css;
	QString bgColour;
	QString hoverColour;
	bool hoveringEnabled;
	bool hovering;
	QString colour;
	int padding;
	int borderRadius;

	QString imgChkd;
	QString imgChkdDis;
	QString imgUnchkd;
	QString imgUnchkdDis;

	void setCSS();

private slots:
	void checkToggle();

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mouseMoveEvent(QMouseEvent *);

};

#endif // CUSTOMRADIOBUTTON_H
