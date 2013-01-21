#ifndef SETTINGSTABLOOKANDFEEL_H
#define SETTINGSTABLOOKANDFEEL_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customslider.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customradiobutton.h"

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


class ClickLabel : public QLabel {

	Q_OBJECT

public:
	ClickLabel(const QString &text = "", QWidget *parent = 0);
	~ClickLabel();

protected:
	void mouseReleaseEvent(QMouseEvent *e);

signals:
	void clicked();

};


// Settings to customise the look and feel
class SettingsTabLookAndFeel : public QWidget {

	Q_OBJECT

public:
	SettingsTabLookAndFeel(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~SettingsTabLookAndFeel();

	bool verbose;

	// The global settings
	QMap<QString,QVariant> globSet;

	// The scrollbar
	CustomScrollbar *scrollbar;

	// Adjust background
	CustomRadioButton *compositeBackground;
	CustomRadioButton *backgroundImageUseScreenshot;
	CustomRadioButton *backgroundImageUseCustom;
	CustomRadioButton *noBackgroundImage;
	ClickLabel *backgroundImage;
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

	// Close on click on grey
	CustomCheckBox *grey;

	// Hide the quickinfo
	CustomCheckBox *hideCounter;
	CustomCheckBox *hideFilePATH;
	CustomCheckBox *hideFilename;
	CustomCheckBox *hideX;

	// This holds the updated settings when saved
	QMap<QString,QVariant> updatedSet;
	QMap<QString,QVariant> defaults;


public slots:
	// background color changed
	void newBgColorSelected(QColor col);

	// Load and save settings
	void loadSettings();
	void saveSettings();

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
