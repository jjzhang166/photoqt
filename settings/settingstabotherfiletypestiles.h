#ifndef SETTINGSTABOTHERFILETYPESTILES_H
#define SETTINGSTABOTHERFILETYPESTILES_H

#include "../customelements/customcheckbox.h"

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>

class SettingsTabOtherFileTypesTiles : public QWidget {

	Q_OBJECT

public:
	SettingsTabOtherFileTypesTiles(QString ftype, QWidget *parent = 0);
	~SettingsTabOtherFileTypesTiles();

	QString filetype;

	// The label that's being styled
	QLabel *back;

	// And the checkbox to enable or disable this tile
	CustomCheckBox *enabled;

	// Set this checkbox checked
	void setChecked(bool chkd);
	bool isChecked() { return enabled->isChecked(); }
	void setEnabled(bool);

private:
	// Different css styles
	QString css;
	QString cssOff;
	QString cssBackgroundNorm;
	QString cssBackgroundOffNorm;
	QString cssBackgroundHov;
	QString cssBackgroundOffHov;

private slots:
	// A click on a checkbox
	void checkboxClicked();

protected:
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void paintEvent(QPaintEvent *);

};

#endif // SETTINGSTABOTHERFILETYPESTILES_H
