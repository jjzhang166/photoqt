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
#include <QMap>
#include <QVariant>
#include <QDesktopServices>
#include <QUrl>

#ifdef EXIV2
#include "exiv2/image.hpp"
#include "exiv2/exif.hpp"
#endif

// This class provides a widget for displaying meta (Exif) information
class Details : public QWidget {

	Q_OBJECT

public:
	Details(QWidget *parent = 0, QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false);
	~Details();

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

#ifdef EXIV2
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
	QMap<QString,CustomLabel*> items;
	void setupLabels();

	// A label displaying an "Nothing loaded" message
	CustomLabel *empty;

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

#ifdef EXIV2

	// Functions to manipulate data a little
	QString exifExposureTime(QString value);
	QString exifFNumberFLength(QString value);
	QString exifPhotoTaken(QString value);
	QStringList exifGps(QString gpsLonRef, QString gpsLon, QString gpsLatRef, QString gpsLat);
#endif

	// Rotation/Flipping values
	int rotationDeg;
	bool flipHor;

public slots:
	// Update the label data
	void updateData(QString currentfile, QSize origSize, bool exiv2Supported = true);

private slots:

	// Animate open/close the widget
	void animate();

	// Adjusting the widget height
	void adjustHeight();

	// A click on the GPS coordinates
	void gpsClick();

	// Yes/No clicked in confirmation widget
	void rotConfYes();
	void rotConfNo();

signals:
	// Update orientation of big image
	void setOrientation(int rotation, bool flipHor);

	// Tell mainwindow.cpp to update the settings
	void updateSettings(QMap<QString,QVariant> newset);

protected:
	void paintEvent(QPaintEvent *);


};


#endif // EXIFWIDGET_H
