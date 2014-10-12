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

#ifndef WALLPAPER_H
#define WALLPAPER_H

#include "../widgets/mywidget.h"
#include "../customelements/customline.h"

#include <QFileInfo>
#include <QProcess>
#include <QButtonGroup>
#include <QDesktopWidget>
#include <QLabel>

#include "../customelements/custompushbutton.h"
#include "../customelements/customcombobox.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlabel.h"

class Wallpaper : public MyWidget {

	Q_OBJECT

public:
	Wallpaper(QMap<QString,QVariant> set, bool v, QWidget *parent = 0);
	~Wallpaper();

	QMap<QString,QVariant> globSet;

	bool verbose;

	void setWallpaper(QString file);
	void dontSetWallpaper();

	void accept();


private:
	CustomPushButton *ok;
	CustomPushButton *cancel;

	CustomComboBox *wm;
	CustomLabel *wmMessage;

	QString filename;
	CustomLabel *filenameLabel;

	QButtonGroup *gnomeButGrp;
	CustomLabel *gnomePicOpsLabel;
	QMap<QString,CustomRadioButton*> gnomePictureOptions;

	QButtonGroup *xfceButGrp;
	CustomLabel *xfcePicOpsLabel;
	QMap<QString,CustomRadioButton*> xfcePictureOptions;


	CustomRadioButton *otherFeh;
	CustomRadioButton *otherNitrogen;
	QMap<QString,CustomRadioButton*> otherFehOptions;
	QMap<QString,CustomRadioButton*> otherNitrogenOptions;
	QButtonGroup *otherFehGrp;
	QButtonGroup *otherNitrogenGrp;


	QMap<int,CustomCheckBox*> wmMonitorSelect;
	CustomLabel *wmMonitorLabel;

	void detectWM();

	void setKDE();
	void setGNOME();
	void setXFCE();
	void setRazor();
	void setOTHER();

private slots:
	void wmSelected();

	void goAheadAndSetWallpaper();

	void swapFehNitrogen();

};

#endif // WALLPAPER_H
