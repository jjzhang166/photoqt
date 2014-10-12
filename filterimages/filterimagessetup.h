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

#ifndef FILTERIMAGESSETUP_H
#define FILTERIMAGESSETUP_H

#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customradiobutton.h"
#include "../widgets/mywidget.h"

class FilterImagesSetup : public MyWidget {

	Q_OBJECT

public:
	// Widget to set an image filter (by extension)
	FilterImagesSetup(QWidget *parent = 0);
	~FilterImagesSetup();

	CustomPushButton *enter;
	CustomPushButton *cancel;

	void makeShow(QString dir) { curDir = dir; edit->setEnabled(true); edit->setFocus(); edit->selectAll(); MyWidget::makeShow(); }

private:
	// The central widget containing all the content
	QWidget *center;

	QString curDir;

	CustomLineEdit *edit;

private slots:
	void disableAndClose() { edit->setEnabled(false); makeHide(); }

	void okayClicked();

	void editTextChanged();

signals:
	// Set/Remove filter
	void setFilter(QString dir, QStringList searchwords);
	void removeFilter();

};


#endif // FILTERIMAGESSETUP_H
