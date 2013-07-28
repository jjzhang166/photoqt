#include "thumbnailview.h"

ThumbnailView::ThumbnailView(QMap<QString, QVariant> set) : QGraphicsView() {

	globSet = set;

	this->setMouseTracking(true);

	this->setAlignment(Qt::AlignCenter);

	// The scene
	this->setScene(&scene);
	scene.clear();

	// The standard cursor is the arrow cursor
	this->setCursor(Qt::ArrowCursor);

	// Last number of thumbs to the left
	lastToLeft = -1;

	thbWidth = globSet.value("ThumbnailSize").toInt();

	// Scrollbar
	scrollbar = new CustomScrollbar;
	this->setHorizontalScrollBar(scrollbar);
	this->verticalScrollBar()->hide();
	// Changed scrollbar value
	connect(scrollbar, SIGNAL(valueChanged(int)), this, SLOT(scrollbarValueChanged(int)));

	// Timer to identify end of scrolling
	updateScroll = new QTimer;
	updateScroll->setSingleShot(true);
	updateScroll->setInterval(200);
	connect(updateScroll, SIGNAL(timeout()), this, SLOT(scrollUpdateTimeout()));

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

// The value of the thumbnail has changed
void ThumbnailView::scrollbarValueChanged(int) {

	updateScroll->stop();
	updateScroll->start();

}

// The timer has timed out, i.e. the user stopped scrolling
void ThumbnailView::scrollUpdateTimeout() {

	emit movedScroll();

}


ThumbnailView::~ThumbnailView() { }
