#include "cropwidget.h"

CropWidget::CropWidget(QWidget *parent) : QWidget(parent) {

//	this->setCursor(Qt::CrossCursor);

	rubberBand = new QRubberBand(QRubberBand::Rectangle, this);


//	void Widget::mousePressEvent(QMouseEvent *event)
//	{
//	    origin = event->pos();
//	    if (!rubberBand)
//		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
//	    rubberBand->setGeometry(QRect(origin, QSize()));
//	    rubberBand->show();
//	}

//	void Widget::mouseMoveEvent(QMouseEvent *event)
//	{
//	    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
//	}

//	void Widget::mouseReleaseEvent(QMouseEvent *event)
//	{
//	    rubberBand->hide();
//	    // determine selection, for example using QRect::intersects()
//	    // and QRect::contains().
//	}

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



// We overwrite the paint event in order to display the selected range
//void CropWidget::paintEvent(QPaintEvent *e) {

//	QWidget::paintEvent(e);

//	if(selectionStarted || selectionRect.width() >= 5 || selectionRect.height() >= 5) {

//		QRect useRect;
//		if(selectionRect.width() < 0 && selectionRect.height() < 0) {
//			useRect.setTopLeft(selectionRect.bottomRight());
//			useRect.setBottomRight(selectionRect.topLeft());
//		} else if(selectionRect.width() < 0) {
//			useRect.setTopRight(selectionRect.topLeft());
//			useRect.setBottomLeft(selectionRect.bottomRight());
//		} else if(selectionRect.height() < 0) {
//			useRect.setTopRight(selectionRect.bottomRight());
//			useRect.setBottomLeft(selectionRect.topLeft());
//		} else
//			useRect = selectionRect;

//		QRect rect1(0,0,useRect.x(),this->height());
//		QRect rect2(useRect.x()+useRect.width(),0,this->width()-useRect.x()-useRect.width(),this->height());
//		QRect rect3(useRect.x(),0,useRect.width(), useRect.y());
//		QRect rect4(useRect.x(),useRect.y()+useRect.height(), useRect.width(), this->height()-useRect.y()-useRect.height());

//		QPainter painter(this);
//		painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::NoPen));
//		painter.setBrush(QBrush(QColor(0,0,0,220)));

//		painter.drawRect(rect1);
//		painter.drawRect(rect2);
//		painter.drawRect(rect3);
//		painter.drawRect(rect4);

//		painter.end();

//	} else if(!selectionStarted && selectionRect.width() < 5 && selectionRect.height() < 5) {

//		QRect r(0,0,this->width(), this->height());

//		QPainter painter(this);
//		painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::NoPen));
//		painter.setBrush(QBrush(QColor(0,0,0,150)));

//		painter.drawRect(r);

//		painter.end();

//	}

//}

//void CropWidget::mousePressEvent(QMouseEvent *e) {

//	if (e->button()==Qt::RightButton) {
////		if (selectionRect.contains(e->pos()))
////			contextMenu.exec(this->mapToGlobal(e->pos()));
//	} else {
//		selectionStarted=true;
//		selectionRect.setTopLeft(e->pos());
//		selectionRect.setBottomRight(e->pos());
//	}

//}

//void CropWidget::mouseMoveEvent(QMouseEvent *e) {

//	if (selectionStarted) {
//		selectionRect.setBottomRight(e->pos());
//		repaint();
//	}

//}

//void CropWidget::mouseReleaseEvent(QMouseEvent *e) {

//	selectionStarted=false;

//	repaint();

//	QStyleOption o;
//	o.initFrom(this);
//	QPainter p(this);
//	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
//}
