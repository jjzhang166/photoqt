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

#ifndef QUICKSETTINGS_H
#define QUICKSETTINGS_H

#include "../customelements/customlabel.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customline.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QSignalMapper>

class QuickSettings : public QWidget {

	Q_OBJECT

public:
	QuickSettings(QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false, QWidget *parent = 0);

	bool verbose;

	// Global Settings
	QMap<QString,QVariant> globSet;

	bool isVisible() { return isShown; }
	void makeHide();
	void makeShow();

	void setRect(QRect fullscreen);

	void loadSettings();

private:
	// This boolean stores if the widget is shown or hidden
	bool isShown;

	// The blocking boolean
	bool blockAll;

	// The two rects for the two states (hidden/shown)
	QRect rectShown;
	QRect rectHidden;

	// The main layout of the widget
	QVBoxLayout *central;

	// The animation class
	QPropertyAnimation *ani;

	// This boolean stores if mouse triggering is en-/disabled
	bool mouseTrickerEnable;


	CustomRadioButton *composite;
	CustomRadioButton *fakedtrans;
	CustomRadioButton *imagebg;
	CustomRadioButton *coloured;
	CustomCheckBox *trayicon;
	CustomCheckBox *loopthroughfolder;
	CustomCheckBox *windowmode;
	CustomCheckBox *windowdeco;
	CustomCheckBox *clickonempty;
	CustomCheckBox *thumbnailskeepvisible;
	CustomCheckBox *thumbnailsdynamic;
	CustomCheckBox *quickSettings;

	QMap<QString,bool> defaults;

private slots:
	// Animate open/close the widget
	void animate();

	void settingChanged();

signals:
	// Tell mainwindow.cpp to update the settings
	void updateSettings(QMap<QString,QVariant> newset);
	void emulateShortcut(QString);

protected:
	void paintEvent(QPaintEvent *);

};


#endif // QUICKSETTINGS_H
