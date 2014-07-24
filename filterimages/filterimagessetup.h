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
	// Widget to set an image filter (by extension)
	FilterImagesSetup(QWidget *parent = 0);
	~FilterImagesSetup();

	bool isVisible() { return isShown; }
	void setRect(QRect);

	CustomPushButton *enter;
	CustomPushButton *cancel;

public slots:
	void makeHide();
	void makeShow();

private:
	// These are the dimensions
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// Boolean if visible
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

	// Set/Remove filter
	void setFilter(QStringList);
	void removeFilter();

protected:
	void paintEvent(QPaintEvent *);
	void mouseReleaseEvent(QMouseEvent *);

};


#endif // FILTERIMAGESSETUP_H
