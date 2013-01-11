#include "thumbnailview.h"

ThumbnailView::ThumbnailView() : QGraphicsView() {

	this->setMouseTracking(true);

}


// Manipulate the wheel event
void ThumbnailView::wheelEvent(QWheelEvent *event) {

	int delta = event->delta();
	if(delta < 0)
		delta = -50;
	else if(delta > 0)
		delta = 50;

	// A vertical scrolling is translated to a horizontal scrolling
	QWheelEvent *w = new QWheelEvent(event->pos(),delta,event->buttons(),event->modifiers(),Qt::Horizontal);
	QGraphicsView::wheelEvent(w);


}


ThumbnailView::~ThumbnailView() { }
