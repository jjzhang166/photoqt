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

#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

// A custom QTabWidget
class TabWidget : public QTabWidget {

	Q_OBJECT

public:
	explicit TabWidget(QWidget *parent = 0);
	~TabWidget();

	void makeBold(bool b) { bold = b; }
	void setBorderTop(QString col, int wid) { borderTopColor = col; borderTopWidth = wid; setCSS(); }
	void setBorderBot(QString col, int wid) { borderBotColor = col; borderBotWidth = wid; setCSS(); }
	void expand(bool exp) { expandTabs = exp; }

private:
	void setCSS();
	int tabWidth;
	bool bold;
	QString borderTopColor;
	int borderTopWidth;
	QString borderBotColor;
	int borderBotWidth;
	bool expandTabs;

protected:
	void resizeEvent(QResizeEvent *);

};

#endif // CUSTOMTABWIDGET_H
