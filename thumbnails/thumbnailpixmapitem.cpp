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

#include "thumbnailpixmapitem.h"
#include <iostream>

ThumbnailPixmapItem::ThumbnailPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {

	// needed for effects
	this->setAcceptHoverEvents(true);

	// the path of this item
	path = "";

	// Stores if this item is loaded in full
	presented = false;

	this->setCursor(Qt::PointingHandCursor);

}

// Set a new pixmap
void ThumbnailPixmapItem::setPixmap(const QPixmap &pixmapNorm, const QPixmap &pixmapHov) {

	norm = pixmapNorm;
	hov = pixmapHov;

	QGraphicsPixmapItem::setPixmap(presented ? hov : norm);

}

// Implementing hover effect
bool ThumbnailPixmapItem::sceneEvent(QEvent *event) {

	if(event->type() == QEvent::GraphicsSceneHoverLeave && presented == false) {
		QGraphicsPixmapItem::setPixmap(norm);
	} else if(event->type() == QEvent::GraphicsSceneHoverEnter || event->type() == QEvent::GraphicsSceneHoverMove) {
		QGraphicsPixmapItem::setPixmap(hov);
	}

	return QGraphicsPixmapItem::sceneEvent(event);

}

// Set normal image
void ThumbnailPixmapItem::setNormalImg() {

	QGraphicsPixmapItem::setPixmap(norm);

}

// Set hover image
void ThumbnailPixmapItem::setHoverImg() {

	QGraphicsPixmapItem::setPixmap(hov);

}

// Load image in full
void ThumbnailPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *) {

	emit clicked(path);

}

ThumbnailPixmapItem::~ThumbnailPixmapItem() { }
