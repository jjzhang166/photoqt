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

#ifndef SLIDESHOWSETTINGS_H
#define SLIDESHOWSETTINGS_H

#include "../widgets/mywidget.h"

#include "../customelements/customscrollbar.h"
#include "../customelements/customslider.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customline.h"

#include <QDir>
#include <QFileDialog>

class SlideShow : public MyWidget {

	Q_OBJECT

public:
	SlideShow(QMap<QString,QVariant> set, bool verbose = false, QWidget *parent = 0);
	~SlideShow();

	bool verbose;

	QMap<QString,QVariant> globSet;

	// Adjust time
	CustomSlider *timeSlider;
	// Adjust transition
	CustomSlider *trans;

	// Adjust music
	CustomCheckBox *musicEnable;
	CustomLineEdit *musicPath;

	// Adjust quickinfo labels
	CustomCheckBox *hideQuickInfo;

public slots:
	// Start slideshow
	void andStart();

private slots:
	// Browse filesystem for music file
	void browseForMusic();

signals:
	// Start the slideshow
	void startSlideShow();

};

#endif // SLIDESHOWSETTINGS_H
