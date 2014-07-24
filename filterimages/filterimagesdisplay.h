#ifndef FILTERIMAGESDISPLAY_H
#define FILTERIMAGESDISPLAY_H

#include "../customelements/customlabel.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>

class FilterImagesDisplay : public QWidget {

	Q_OBJECT

public:
	// This class is used a couple times to have a quick and easy way of displaying a confirmation widget
	FilterImagesDisplay(QWidget *parent = 0);
	~FilterImagesDisplay();

	void makeHide();
	void makeShow();

	void showFilter(QStringList filter);

private:
	bool isShown;

	CustomLabel *display;

private slots:
	void removeClicked();

signals:
	void removeImageFilter();


protected:
	void paintEvent(QPaintEvent *);

};



#endif // FILTERIMAGESDISPLAY_H
