#ifndef SETTINGSTABTHUMBNAIL_H
#define SETTINGSTABTHUMBNAIL_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customslider.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customspinbox.h"
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

	bool verbose;

	// The database to do some operation on it
	QSqlDatabase db;

	// the global settings
	QMap<QString,QVariant> globSet;

	// The main scroll bar
	QScrollArea *scroll;
	QWidget *scrollWidg;
	CustomScrollbar *scrollbar;

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

	// use file-name-only thumbnail
	CustomCheckBox *filenameInsteadThb;
	CustomSlider *filenameFontSizeSlider;
	CustomSpinBox *filenameFontSizeSpin;

	// Disable thumbnails
	CustomCheckBox *thumbDisable;

	// Use thumbnail cache
	CustomCheckBox *thumbCache;

	// Type of cache
	CustomRadioButton *cacheFile;
	CustomRadioButton *cacheDatabase;

	// A label displaying some info about the database
	QLabel *dbInfo;

	// Clean or rebuild database
	CustomPushButton *cleanDatabase;
	CustomPushButton *eraseDatabase;

	// In here are the updated settings stored
	QMap<QString,QVariant> updatedSet;
	QMap<QString, QVariant> defaults;

	// Set some database info
	void setDatabaseInfo();
	int noOfEntriesInDb;

	// Confirmation dialogs
	CustomConfirm *confirmClean;
	CustomConfirm *confirmErase;

public slots:

	// Load and save settings
	void loadSettings();
	void saveSettings();

	// clean and erase database
	void doCleanDatabase();
	void doEraseDatabase();

private:
	void paintEvent(QPaintEvent *);

};

#endif // SETTINGSTABTHUMBNAIL_H
