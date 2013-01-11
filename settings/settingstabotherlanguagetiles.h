#ifndef SETTINGSTABOTHERLANGUAGETILES_H
#define SETTINGSTABOTHERLANGUAGETILES_H

#include "../customelements/customradiobutton.h"
#include <QWidget>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>

class SettingsTabOtherLanguageTiles : public QWidget {

	Q_OBJECT

public:
	SettingsTabOtherLanguageTiles(QString lang, QString code, QWidget *parent = 0);
	~SettingsTabOtherLanguageTiles();

	// The css
	QString css;
	QString cssBackgroundNorm;
	QString cssBackgroundHov;

	// The back label (this is the widget being styled
	QLabel *back;

	// The language code (usually two or three letters)
	QString langCode;

	// A button to select it
	CustomRadioButton *button;

public slots:
	// Toggle button
	void buttonToggled(bool tgld);

protected:
	void mouseMoveEvent(QMouseEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);

};

#endif // SETTINGSTABOTHERLANGUAGETILES_H
