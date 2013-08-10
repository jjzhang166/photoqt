#ifndef CUSTOMCONFIRM_H
#define CUSTOMCONFIRM_H

#include "../customelements/custompushbutton.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlabel.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

class CustomConfirm : public QWidget {

	Q_OBJECT

public:
	// This class is used a couple times to have a quick and easy way of displaying a confirmation widget
	CustomConfirm(QString title, QString text, QString yesText = "", QString noText = "", QSize size = QSize(600,400), QWidget *parent = 0);
	~CustomConfirm();

	// These are the dimensions
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	bool isShown;

	// A yes and a no is possible (with custom text of course)
	CustomPushButton *yes;
	CustomPushButton *no;

	// Possible to show checkbox
	CustomCheckBox *dontShowAgain;
	void setDontShowAgain();

	// Show a border around the widget?
	void showBorder(QString color, int width);

	void setRect(QRect);

private:
	// The size the user wants it to be
	QSize widgetSize;

	// The animation instance
	QPropertyAnimation *ani;

	// The central widget containing all the content
	QWidget *center;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

public slots:
	// The animation/fading functions
	void animate();
	void fadeStep();
	void aniFinished();

signals:
	// Click on the buttons
	void confirmed();
	void rejected();

	// Widget blocks the rest of photo as long as it's open
	void blockFunc(bool block);

protected:
	void paintEvent(QPaintEvent *);
	void mouseReleaseEvent(QMouseEvent *);

};

#endif // CUSTOMCONFIRM_H
