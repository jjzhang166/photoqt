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

#include "customtabwidget.h"
#include <QLayout>

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {

	bold = false;
	borderTopColor = "";
	borderTopWidth = 0;
	borderBotColor = "";
	borderBotWidth = 0;
	expandTabs = true;

	tabBar()->setCursor(Qt::PointingHandCursor);

	QString css = "QTabWidget::pane {";
	css += "border-bottom: 4px double black;";
	css += "padding: 10px;";
	css += "padding-top: 20px;";
	css += "}";
	css += "QTabWidget::tab-bar {";
	css += "alignment: center;";
	css += "}";
	css += "QTabWidget::pane {";
	css += "position: absolute;";
	css += "top: -0.5em;";
	css += "}";
	css += "QToolTip {font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt; background: rgba(255,255,255,200); }";

	this->setStyleSheet(css);

}


void TabWidget::resizeEvent(QResizeEvent *) {

	int tabCount = tabBar()->count();

	if(tabCount != 0) {

		int width = this->width()-4;

		tabWidth = width/tabCount - 14;

		setCSS();

	}

}

void TabWidget::setCSS() {


		QString css = "QTabBar::tab {";
		css += "background: rgba(30,30,30,150);";
		css += "color: rgb(150,150,150);";
		bold ? css += "font-weight: bold;" : css += "";
		css += QString("width: %1px;").arg(expandTabs ? tabWidth : 200);
		css += "padding: 5px;";
		borderTopWidth != 0 ? css += QString("border-top: %1px solid %2;").arg(borderTopWidth).arg(borderTopColor) : css += "";
		borderBotWidth != 0 ? css += QString("border-bottom: %1px solid %2;").arg(borderBotWidth).arg(borderBotColor) : css += "";
		css += "}";
		css += "QTabBar::tab:selected, QTabBar::tab:hover {";
		css += "background: rgba(103,103,103,150);";
		css += "color: white;";
		css += "}";

		tabBar()->setStyleSheet(css);

}


TabWidget::~TabWidget() { }
