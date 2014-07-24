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

// Label in thumbnail view to display filter (if set)
class FilterImagesDisplay : public QWidget {

	Q_OBJECT

public:
	FilterImagesDisplay(QWidget *parent = 0);
	~FilterImagesDisplay();

	void showFilter(QStringList filter);

private:
	bool isShown;
	CustomLabel *display;

private slots:
	void removeClicked();

signals:
	void removeImageFilter();
	void changeFilter();

protected:
	void paintEvent(QPaintEvent *);

};



#endif // FILTERIMAGESDISPLAY_H
