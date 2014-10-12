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

#ifndef MAINMENUITEM_H
#define MAINMENUITEM_H

#include <QLabel>
#include <QEvent>

// A custom MenuItem used for the Menu
class MainMenuItem : public QLabel {

	Q_OBJECT
public:
	MainMenuItem(QString txt = "", QString icon = "", QString setData = "", int closeMe = 1, bool enabled = true);
	~MainMenuItem();

	// Setting an icon at the beginning of the item
	void setIcon(QString);

	// The look of the label when hovered/normal
	QString ssNormal;
	QString ssHover;

	// The data connected with this label
	QString data;

	// Closing setting for this label
	int close;

protected:
	bool event(QEvent *e);

signals:
	void clicked();

};

#endif // MAINMENUITEM_H
