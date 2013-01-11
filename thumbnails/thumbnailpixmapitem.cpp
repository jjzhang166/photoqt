#include "thumbnailpixmapitem.h"

ThumbnailPixmapItem::ThumbnailPixmapItem(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsPixmapItem(parent,scene) {

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

	if(presented)
		QGraphicsPixmapItem::setPixmap(hov);
	else
		QGraphicsPixmapItem::setPixmap(norm);


}

// Implementing hover effect
bool ThumbnailPixmapItem::sceneEvent(QEvent *event) {

	if(event->type() == QEvent::GraphicsSceneHoverLeave && presented == false) {
		QGraphicsPixmapItem::setPixmap(norm);
	} else if(event->type() == QEvent::GraphicsSceneHoverEnter || event->type() == QEvent::GraphicsSceneMouseMove) {
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
