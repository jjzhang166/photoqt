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

#ifndef SETTINGSTABLOOKANDFEEL_H
#define SETTINGSTABLOOKANDFEEL_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customslider.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customtabwidget.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>

#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>

#include <QColorDialog>
#include <QPushButton>
#include <QSpinBox>

#include <QScrollArea>
#include <QScrollBar>
#include <QSignalMapper>
#include <QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QToolTip>

#include <QApplication>
#include <QDesktopWidget>


#include <QGraphicsEffect>


// Settings to customise the look and feel
class SettingsTabLookAndFeel : public QWidget {

	Q_OBJECT

public:
	SettingsTabLookAndFeel(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~SettingsTabLookAndFeel();

	TabWidget *tabs;
	QWidget *tabLook;
	QWidget *tabFeel;

	bool verbose;

	// The global settings
	QMap<QString,QVariant> globSet;

	// The scrollbar
	CustomScrollbar *scrollbarLook;
	CustomScrollbar *scrollbarFeel;

	// This holds the updated settings when saved
	QMap<QString,QVariant> updatedSet;
	QMap<QString,QVariant> defaults;

private:

	// Adjust background
	CustomRadioButton *compositeBackground;
	CustomRadioButton *backgroundImageUseScreenshot;
	CustomRadioButton *backgroundImageUseCustom;
	CustomRadioButton *noBackgroundImage;
	CustomLabel *backgroundImage;
	CustomCheckBox *backgroundImgStretchToFit;
	CustomCheckBox *backgroundImgScaleToFit;
	CustomCheckBox *backgroundImgCenter;

	// Adjust background overlay color
	QColorDialog *background;
	CustomPushButton *selectCol;

	// Adjust tray icon usage
	CustomCheckBox *trayIcon;

	// Adjust looping through the folder
	CustomCheckBox *loopThroughFolder;

	// Transitioning between images (DISABLED AT THE MOMENT)
	CustomSlider *transition;

	// Border around main image
	CustomSlider *borderAroundImgSlider;
	CustomSpinBox *borderAroundImgSpinBox;

	// Menu sensitivity
	CustomSlider *menu;

	// Window Mode
	CustomCheckBox *windowMode;
	CustomCheckBox *windowDeco;

	// Close on click on grey
	CustomCheckBox *grey;

	// Hide the quickinfo
	CustomCheckBox *hideCounter;
	CustomCheckBox *hideFilePATH;
	CustomCheckBox *hideFilename;
	CustomCheckBox *hideX;


public slots:
	// Load and save settings
	void loadSettings();
	void saveSettings();

private slots:
	// background color changed
	void newBgColorSelected(QColor col);

	// Change background image (i.e. browse for new image)
	void changeBackgroundImage();

	// Stretch/Scale/Center image
	void backgroundDispType();

	// Set a background image
	void setBackgroundImage(QString path, bool empty = false);

private:
	void paintEvent(QPaintEvent *);

};

#endif // SETTINGSTABLOOKANDFEEL_H
