#ifndef SETTINGSTABEXIF_H
#define SETTINGSTABEXIF_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customslider.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customlabel.h"
#include "settingstabexiftiles.h"
#include "../flowlayout/flowlayout.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QButtonGroup>

class SettingsTabExif : public QWidget {

	Q_OBJECT

public:
	SettingsTabExif(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~SettingsTabExif();

	bool verbose;

	CustomScrollbar *scrollbar;

	// Global settings
	QMap<QString,QVariant> globSet;

	// The updated settings
	QMap<QString,QVariant> updatedSet;

	// Load and save settings
	void loadSettings();
	void saveSettings();

private:

	// The flowlayout wraps around the corners dynamically = AWESOME!
	FlowLayout *flow;

	// The items PhotoQt understands. allItemsShort contains the actual Exif key (without the "Exif"), the first one is the one that's being displayed
	QStringList allItems;
	QStringList allItemsShort;

	// We store all the tiles for later access
	QList<SettingsTabExifTiles *> allTiles;

	// En-/Disable triggering on mouse movement
	CustomCheckBox *triggerOnMouse;

	// Exif Rotation
	CustomRadioButton *exifRotNev;
	CustomRadioButton *exifRotAlw;
	CustomRadioButton *exifRotAsk;

	// Online map for GPS location
	CustomRadioButton *radioGoogle;
	CustomRadioButton *radioBing;
	CustomRadioButton *radioOSM;

	// The default settings (for detecting user changed)
	QMap<QString, QVariant> defaults;

	// Adjustable font size of slider
	CustomSlider *setFontSizeSlider;


public slots:
	// Quick way to diable or enable all tiles
	void disEnableAll();

protected:
	void paintEvent(QPaintEvent *);
};

#endif // SETTINGSTABEXIF_H
