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

#ifndef CUSTOMCONFIRM_H
#define CUSTOMCONFIRM_H

#include "../customelements/custompushbutton.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlabel.h"
#include "../widgets/mywidget.h"

#include <QTextEdit>

class CustomConfirm : public MyWidget {

	Q_OBJECT

public:
	// This class is used a couple times to have a quick and easy way of displaying a confirmation widget
	CustomConfirm(QString title, QString text, QString yesText = "", QString noText = "", QSize size = QSize(600,400), QString borderColor = "rgb(130,130,130)", QString backgroundColor = "rgba(0,0,0,200)", QWidget *parent = 0);
	~CustomConfirm();

	// A yes and a no is possible (with custom text of course)
	CustomPushButton *yes;
	CustomPushButton *no;

	// Possible to show checkbox
	CustomCheckBox *dontShowAgain;
	void setDontShowAgain();

signals:
	// Click on the buttons
	void confirmed();
	void rejected();

};

#endif // CUSTOMCONFIRM_H
