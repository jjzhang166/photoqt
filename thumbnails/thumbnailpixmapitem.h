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

#ifndef THUMBNAILPIXMAPITEM_H
#define THUMBNAILPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

// The pixmap item for the thumbnail view
class ThumbnailPixmapItem : public QObject, public QGraphicsPixmapItem {

	Q_OBJECT

public:
	ThumbnailPixmapItem(QGraphicsItem *parent = 0);
	~ThumbnailPixmapItem();

	// The normal and hovered pixmap
	QPixmap norm;
	QPixmap hov;

	// The path of the thumbnail
	QString path;

	// Stores if this image is loaded in big
	bool presented;

	// Set the normal/hovered image
	void setNormalImg();
	void setHoverImg();

public slots:
	// Set a new pixmap
	void setPixmap(const QPixmap &pixmapNorm, const QPixmap &pixmapHov);

protected:
	bool sceneEvent(QEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
	void clicked(QString path);

};

#endif // THUMBNAILPIXMAPITEM_H
