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

#ifndef SCALE_H
#define SCALE_H

#include <QFileDialog>
#include <QSignalMapper>
#include <QGridLayout>
#include <QImageReader>

#include "../customelements/customlabel.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customslider.h"
#include "../widgets/customconfirm.h"
#include "../widgets/mywidget.h"

class Scale : public MyWidget {

	Q_OBJECT

public:
	Scale(bool verbose, QWidget *parent = 0);
	~Scale();

	void scale(QString filename, QSize s);

	CustomPushButton *enterInPlace;
	CustomConfirm *confirmNotSupported;
	CustomConfirm *confirmInPlace;
	CustomConfirm *confirmNew;

	// First we need to enable the spinboxes and then we show the widget
	void makeShow() { widthSpin->setEnabled(true); heightSpin->setEnabled(true); MyWidget::makeShow(); }

private:
	bool verbose;

	CustomLabel *curSize;
	QString currentfile;
	double widthByHeightRatio;

	CustomSpinBox *widthSpin;
	CustomSpinBox *heightSpin;
	CustomCheckBox *keepratio;

	CustomSlider *quality;

	bool ignoreSizeChange;
	QString lastEdit;

public slots:
	void disableAllSpinBoxAndClose();

private slots:
	void sizeChanged();
	void keepRatioClicked();

	void enterClicked();

	void doScale(QString filename);

};




#endif // SCALE_H
