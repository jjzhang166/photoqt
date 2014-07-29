#include "manipulatelabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>

ManipulateLabel::ManipulateLabel(QWidget *parent) : QLabel(parent) {

	selectionStarted=false;

	QAction *saveAction=contextMenu.addAction("Save");

	connect(saveAction,SIGNAL(triggered()),this,SLOT(saveSlot()));

}

ManipulateLabel::~ManipulateLabel() { }

void ManipulateLabel::paintEvent(QPaintEvent *e) {

	QLabel::paintEvent(e);

	if(selectionStarted) {

		QRect rect1(0,0,selectionRect.x(),this->height());
		QRect rect2(selectionRect.x()+selectionRect.width(),0,this->width()-selectionRect.x()-selectionRect.width(),this->height());
		QRect rect3(selectionRect.x(),0,selectionRect.width(), selectionRect.y());
		QRect rect4(selectionRect.x(),selectionRect.y()+selectionRect.height(), selectionRect.width(), this->height()-selectionRect.y()-selectionRect.height());

		QPainter painter(this);
		painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::NoPen));
		painter.setBrush(QBrush(QColor(0,0,0,200)));

		painter.drawRect(rect1);
		painter.drawRect(rect2);
		painter.drawRect(rect3);
		painter.drawRect(rect4);

	}

}

void ManipulateLabel::mousePressEvent(QMouseEvent *e) {

	if (e->button()==Qt::RightButton) {
		if (selectionRect.contains(e->pos()))
			contextMenu.exec(this->mapToGlobal(e->pos()));
	} else {
		selectionStarted=true;
		selectionRect.setTopLeft(e->pos());
		selectionRect.setBottomRight(e->pos());
	}

}

void ManipulateLabel::mouseMoveEvent(QMouseEvent *e) {

	if (selectionStarted) {
		selectionRect.setBottomRight(e->pos());
		repaint();
	}

}

void ManipulateLabel::mouseReleaseEvent(QMouseEvent *e) {

	selectionStarted=false;

}

void ManipulateLabel::saveSlot() {

    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save File"),  "/home", QObject::tr("Images (*.jpg)"));
    this->pixmap()->copy(selectionRect).save(fileName);

}
