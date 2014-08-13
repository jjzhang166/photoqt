#ifndef CROPWIDGET_H
#define CROPWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QMouseEvent>
#include <QPainter>
#include <iostream>
#include <QtDebug>
#include <QRubberBand>

class CropWidget : public QWidget {

	Q_OBJECT

public:
	explicit CropWidget(QWidget *parent = 0);
	QRubberBand *rubberBand;
	QPoint origin;

protected:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	bool selectionStarted;
	QRect selectionRect;

};


#endif // CROPWIDGET_H
