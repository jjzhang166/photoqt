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

#ifndef SETTINGSTABTHUMBNAIL_H
#define SETTINGSTABTHUMBNAIL_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customslider.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customtabwidget.h"
#include "../customelements/customlabel.h"
#include "../widgets/customconfirm.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QSpinBox>
#include <QRadioButton>
#include <QPushButton>
#include <QDir>
#include <QButtonGroup>
#include <QtSql>

// The tab for setup of thumbnails
class SettingsTabThumbnail : public QWidget {

	Q_OBJECT

public:
	SettingsTabThumbnail(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~SettingsTabThumbnail();

	TabWidget *tabs;
	QWidget *tabLook;
	QWidget *tabTune;

	CustomScrollbar *scrollbarLook;
	CustomScrollbar *scrollbarTune;

	bool verbose;

	// the global settings
	QMap<QString,QVariant> globSet;

	// Confirmation dialogs
	CustomConfirm *confirmClean;
	CustomConfirm *confirmErase;

	// In here are the updated settings stored
	QMap<QString,QVariant> updatedSet;

	void setDatabaseInfo();

private:

	// The database to do some operation on it
	QSqlDatabase db;

	// The size of the thumbnails
	CustomSlider *thumbSizeSlider;
	CustomSpinBox *thumbSizeSpin;

	// The border around the thumbnails
	CustomSlider *borderAroundSlider;
	CustomSpinBox *borderAroundSpin;

	// The amount of liftup of the thumbnails
	CustomSlider *thbLiftUpSlider;
	CustomSpinBox *thbLiftUpSpin;

	// Position of thumbnails
	CustomRadioButton *thbPosTop;
	CustomRadioButton *thbPosBot;

	// Keep thumbnails always visible or fade out
	CustomCheckBox *keepVisible;

	// Enable dynamic thumbnails (i.e. only create the ones you need)
	CustomCheckBox *dynamicThumbnails;

	// use file-name-only thumbnail
	CustomCheckBox *filenameInsteadThb;
	CustomSlider *filenameFontSizeSlider;
	CustomSpinBox *filenameFontSizeSpin;

	// Disable thumbnails
	CustomCheckBox *thumbDisable;

	// Use thumbnail cache
	CustomCheckBox *thumbCache;

	CustomCheckBox *writeFilename;
	CustomCheckBox *writeDimensions;

	// Type of cache
	CustomRadioButton *cacheFile;
	CustomRadioButton *cacheDatabase;

	// A label displaying some info about the database
	CustomLabel *dbInfo;

	// Clean or rebuild database
	CustomPushButton *cleanDatabase;
	CustomPushButton *eraseDatabase;

	QMap<QString, QVariant> defaults;

	// Set some database info
	int noOfEntriesInDb;

public slots:

	// Load and save settings
	void loadSettings();
	void saveSettings();

	void updateCacheStuff();

private slots:

	// clean and erase database
	void doCleanDatabase();
	void doEraseDatabase();

private:
	void paintEvent(QPaintEvent *);

};

#endif // SETTINGSTABTHUMBNAIL_H
