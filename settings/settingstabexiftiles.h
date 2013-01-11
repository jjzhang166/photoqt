#ifndef SETTINGSTABEXIFTILES_H
#define SETTINGSTABEXIFTILES_H

#include "../customelements/customcheckbox.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>

class SettingsTabExifTiles : public QWidget {

	Q_OBJECT

public:
	SettingsTabExifTiles(QString exifTxt, QString exifIntern, QWidget *parent = 0);
	~SettingsTabExifTiles();

	// Different css styles
	QString css;
	QString cssOff;
	QString cssBackgroundNorm;
	QString cssBackgroundOffNorm;
	QString cssBackgroundHov;
	QString cssBackgroundOffHov;

	// This string holds the exif key value (the itemsShort list in settingstabexif.cpp)
	QString intern;

	// The label that's being styled
	QLabel *back;

	// And the checkbox to enable or disable this tile
	CustomCheckBox *enabled;

	// Set this checkbox checked
	void setChecked(bool chkd);

public slots:
	// A click on a checkbox
	void checkboxClicked();

protected:
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

private:
	void paintEvent(QPaintEvent *);

};

#endif // SETTINGSTABEXIFTILES_H
