/***********************************************
 *
 * THIS CLASS IS CURRENTLY NOT USED!
 * THE CODE IS LEFT HERE, IN CASE THAT FEATURE
 * MIGHT BE IMPLEMENTED IN THE FUTURE...
 *
 * *********************************************/

#include "cropwidget.h"

CropWidget::CropWidget(QWidget *parent) : QWidget(parent) {

	this->setCursor(Qt::CrossCursor);

	rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

}


void CropWidget::mousePressEvent(QMouseEvent *e) {

	rubberBand->hide();

	origin = e->pos();
	rubberBand->setGeometry(QRect(origin, QSize()));
	rubberBand->show();

}

void CropWidget::mouseMoveEvent(QMouseEvent *e) {

	rubberBand->setGeometry(QRect(origin, e->pos()).normalized());

	repaint();

}

void CropWidget::paintEvent(QPaintEvent *e) {

	QWidget::paintEvent(e);

	QRect rubberRect = rubberBand->geometry();

	QRect useRect;
	if(rubberRect.width() < 0 && rubberRect.height() < 0) {
		useRect.setTopLeft(rubberRect.bottomRight());
		useRect.setBottomRight(rubberRect.topLeft());
	} else if(rubberRect.width() < 0) {
		useRect.setTopRight(rubberRect.topLeft());
		useRect.setBottomLeft(rubberRect.bottomRight());
	} else if(rubberRect.height() < 0) {
		useRect.setTopRight(rubberRect.bottomRight());
		useRect.setBottomLeft(rubberRect.topLeft());
	} else
		useRect = rubberRect;

	QRect rect1(0,0,useRect.x(),this->height());
	QRect rect2(useRect.x()+useRect.width(),0,this->width()-useRect.x()-useRect.width(),this->height());
	QRect rect3(useRect.x(),0,useRect.width(), useRect.y());
	QRect rect4(useRect.x(),useRect.y()+useRect.height(), useRect.width(), this->height()-useRect.y()-useRect.height());

	QPainter painter(this);
	painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::NoPen));
	painter.setBrush(QBrush(QColor(0,0,0,220)));

	painter.drawRect(rect1);
	painter.drawRect(rect2);
	painter.drawRect(rect3);
	painter.drawRect(rect4);

	painter.end();

}

void CropWidget::mouseReleaseEvent(QMouseEvent *e) {

	if(rubberBand->width() < 5 && rubberBand->height() < 5) {
		rubberBand->setGeometry(QRect());
		repaint();
	}

}

int CropWidget::show() {

	QWidget::show();
	emit blockFunc(true);

}

int CropWidget::hide() {

	QWidget::hide();
	emit blockFunc(false);

}
