/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "thumbnailview.h"
#include <iostream>

ThumbnailView::ThumbnailView(bool v, QMap<QString, QVariant> set) : QGraphicsView() {

	globSet = set;

	verbose = v;

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

	if(verbose) std::clog << "Scrollbar value changed - update thumbnail view" << std::endl;

	emit movedScroll();

}


ThumbnailView::~ThumbnailView() { }
