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

#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "../customelements/customscrollbar.h"

// We translate a vertical scrolling into a horizontal one
class ThumbnailView : public QGraphicsView {

	Q_OBJECT

public:
	ThumbnailView(bool v, QMap<QString,QVariant> set);
	~ThumbnailView();

	QMap<QString,QVariant> globSet;

	bool verbose;

	// The scene
	QGraphicsScene scene;

	// Number of thumbs to the left before scrolling
	int lastToLeft;

	// Width of thumbnails
	int thbWidth;

	// The scrollbar of the view
	CustomScrollbar *scrollbar;

	// When the user finished scrolling (500ms of no-scroll)
	QTimer *updateScroll;

public slots:
	void scrollbarValueChanged(int);
	void scrollUpdateTimeout();

protected:
	void wheelEvent(QWheelEvent *event);

signals:
	void movedScroll();

};

#endif // THUMBNAILVIEW_H
