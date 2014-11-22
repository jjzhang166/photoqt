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

#ifndef STARTUPWIDGET_H
#define STARTUPWIDGET_H

#include "../widgets/mywidget.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customline.h"

class StartUpWidget : public MyWidget {

	Q_OBJECT

public:
	StartUpWidget(QWidget *parent = 0);
	~StartUpWidget();

	// There's a little difference between an update message and a fresh install message
	void setUpdateMsg();
	void setInstallMsg();

private:
	// The title and intro can be customised by above functions
	CustomLabel *title;
	CustomLabel *customIntro;

signals:
	// Block all function in mainwindow and activate system keys
	void finished();

};

#endif // STARTUPWIDGET_H
