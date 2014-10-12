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

#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QTimer>
#include <QImageReader>
#include <QTimeLine>
#include <QPainter>
#include <QGraphicsScene>
#include <QBitmap>

// This item is used for displaying the big image
class GraphicsItem : public QObject, public QGraphicsPixmapItem {

	Q_OBJECT

public:
	GraphicsItem(QGraphicsItem *parent = 0);
	~GraphicsItem();

	// This movie is used for animated images
	QMovie *mov;

	// Setting a new Pixmap/Movie
	void setMovie(QString m, int width, int height);
	void setPixmap(const QPixmap &pixmap, bool dontStopMov = false, bool smoothTransition = true, bool movieUpdate = false, bool transitionUpdate = false);

	// This handles the transition when switching between images (calls function repeatedly)
	QTimeLine *trans;

	// The previous and current (new) pixmap, needed for transitions
	QPixmap prevItem;
	QPixmap curItem;

	// What is the current setting of the transitions?
	int transitionSetting;

	// Is either the previous or the current image rotated (i.e. no rotation)?
	bool curRotated;
	bool prevRotated;

	// Has the item been zoomed?
	bool itemZoomed;

	void setTransBarHeight(int height);
	int getTransBarHeight() { return addTransBar; }

private:
	int addTransBar;

public slots:
	// Set next movie image as pixmap
	void movieNextImg();

	// Catch movie errors
	void movieError(QImageReader::ImageReaderError);

	// Compose the pixmaps (i.e. add transparency, etc.)
	void composePixmap(int);

	// A change in the transition setting
	void transitionSetChange(int);

signals:
	// Update the main image scene rect
	void updateSceneBigRect();

};

#endif // GRAPHICSITEM_H
