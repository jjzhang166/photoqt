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

#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>

// A custom QPushButton, restyled
class CustomPushButton : public QPushButton {

	Q_OBJECT

public:
	explicit CustomPushButton(const QString &text = "", QWidget *parent = 0);
	~CustomPushButton();

	// Per default the button has rounded corners - can be disabled
	void setPadding(int pad) { padding = pad; setCSS(); }
	void setRGBA(int r, int g, int b, int a) { rgba = QString("rgba(%1,%2,%3,%4)").arg(r).arg(g).arg(b).arg(a); setCSS(); }
	void setFontSize(QString fs) { fontsize = fs; setCSS(); }
	void setBold(bool b) { bold = b; setCSS(); }

private:
	bool hover;
	bool enabled;
	int padding;
	QString fontsize;
	QString rgba;
	bool bold;

	void setCSS();

public slots:
	// When disabled, we need to adjust some colors to make it visible
	void setEnabled(bool);

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

};

#endif // CUSTOMPUSHBUTTON_H
