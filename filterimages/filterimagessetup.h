#ifndef FILTERIMAGESSETUP_H
#define FILTERIMAGESSETUP_H

#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>


class FilterImagesSetup : public QWidget {

	Q_OBJECT

public:
	// This class is used a couple times to have a quick and easy way of displaying a confirmation widget
	FilterImagesSetup(QWidget *parent = 0);
	~FilterImagesSetup();

	void makeHide();
	void makeShow();
	bool isVisible() { return isShown; }
	void setRect(QRect);

	CustomPushButton *enter;
	CustomPushButton *cancel;

private:
	// These are the dimensions
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	bool isShown;

	// The animation instance
	QPropertyAnimation *ani;

	// The central widget containing all the content
	QWidget *center;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	CustomLineEdit *edit;

private slots:
	// The animation/fading functions
	void animate();
	void fadeStep();
	void aniFinished();

	void okayClicked();

	void editTextChanged();

signals:
	// Widget blocks the rest of photo as long as it's open
	void blockFunc(bool block);

	void setFilter(QStringList);

protected:
	void paintEvent(QPaintEvent *);
	void mouseReleaseEvent(QMouseEvent *);

};


#endif // FILTERIMAGESSETUP_H
