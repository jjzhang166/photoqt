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

#include "dropdownmenuitem.h"

// A custom QLabel used for the menu
MainMenuItem::MainMenuItem(QString txt, QString icon, QString setData, int closeMe, bool clickable) : QLabel(txt) {

	this->setStyleSheet("background: black");

	// Enable mouse tracking for hover effect
	this->setMouseTracking(true);

	ssNormal = "QLabel { background: transparent; border: none; color: grey; } QLabel:disabled { color: rgb(80,80,80); font-weight: bold }";
	ssHover = "QLabel { background: transparent; border: none; color: white; } QLabel:disabled { color: rgb(80,80,80); font-weight: bold; }";

	// Set the default stylesheet
	MainMenuItem::setStyleSheet(ssNormal);

	// Set the transmitted data
	data = setData.trimmed();
	close = closeMe;
	this->setEnabled(clickable);
	setIcon(icon);

	this->setCursor(Qt::PointingHandCursor);

}

// Get all the needed events and connect them to the slots
bool MainMenuItem::event(QEvent *e) {

	if(e->type() == QEvent::MouseMove)
		MainMenuItem::setStyleSheet(ssHover);
	else if(e->type() == QEvent::Leave)
		MainMenuItem::setStyleSheet(ssNormal);

	if(e->type() == QEvent::MouseButtonPress)
		emit clicked();

	QList<QEvent::Type> types;
	types << QEvent::MouseButtonDblClick;
	types << QEvent::MouseButtonPress;
	types << QEvent::MouseButtonRelease;

	if(!types.contains(e->type()))
		QLabel::event(e);

	return e->isAccepted();

}

// Add an icon to the front of the string
void MainMenuItem::setIcon(QString icon) {

	if(icon != "")
		this->setText("<img height=\"13\" src=\"" + icon + "\">&nbsp;" + this->text());

}

MainMenuItem::~MainMenuItem() { }
