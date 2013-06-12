#ifndef EXIFWIDGET_H
#define EXIFWIDGET_H

#include "../customelements/customcheckbox.h"
#include "../customelements/customlabel.h"
#include "customconfirm.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QImageReader>
#include <QFileInfo>
#include <QDateTime>
#include <QPropertyAnimation>
#include <QtDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QMap>
#include <QVariant>
#include <QDesktopServices>
#include <QUrl>

#ifdef WITH_EXIV2
#include "exiv2/image.hpp"
#include "exiv2/exif.hpp"
#endif

// This class provides a widget for displaying meta (Exif) information
class Exif : public QWidget {

	Q_OBJECT

public:
	Exif(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~Exif();

	bool verbose;

	// Global Settings
	QMap<QString,QVariant> globSet;

	// The "stay open" state and button
	CustomCheckBox *stay;

	bool isVisible() { return isShown; }
	void makeHide();
	void makeShow();

	void setRect(QRect rect);
	void updateFontsize();

#ifdef WITH_EXIV2
	CustomConfirm *rotConf;
#endif



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

	// All the labels for displaying exif info
	QMap<QString,QLabel*> items;
	void setupLabels();

	// A label displaying an "Nothing loaded" message
	QLabel *empty;

	// The animation class
	QPropertyAnimation *ani;

	// All the label data for displaying and detecting data
	QMap<QString,QString> keyVal;
	QStringList labels;
	QStringList labelsId;
	QMap<QString,QString> units;

	// This QMap stores QMaps for "translatin" exif data into a human readable format
	QMap<QString, QMap<QString,QString> > mapAll;

	// This boolean stores if mouse triggering is en-/disabled
	bool mouseTrickerEnable;

#ifdef WITH_EXIV2

	// The online map service opened when clicked on GPS coordinates in the exif window
	QString onlineservice;

	// Functions to manipulate data a little
	QString exifExposureTime(QString value);
	QString exifFNumberFLength(QString value);
	QString exifPhotoTaken(QString value);
	QString exifGps(QString gpsLonRef, QString gpsLon, QString gpsLatRef, QString gpsLat);
#endif

	// Rotation/Flipping values
	int rotationDeg;
	bool flipHor;

	// Fontsize can be adjusted
	QString labelCSS;
	QString labelCSSfontsize;

public slots:
	// Update the label data
	void updateData(QString currentfile, QSize origSize, bool exiv2Supported = true);

private slots:

	// Animate open/close the widget
	void animate();

	// Adjusting the widget height
	void adjustHeight();

#ifdef WITH_EXIV2
	// A click on the GPS coordinates
	void gpsClick();

	// Yes/No clicked in confirmation widget
	void rotConfYes();
	void rotConfNo();
#endif

signals:
	// Update orientation of big image
	void setOrientation(int rotation, bool flipHor);

	// Tell mainwindow.cpp to update the settings
	void updateSettings(QMap<QString,QVariant> newset);

protected:
	void paintEvent(QPaintEvent *);


};


#endif // EXIFWIDGET_H
