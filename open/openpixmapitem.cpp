#include "openpixmapitem.h"

OpenPixmap::OpenPixmap(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsPixmapItem(parent,scene) {

	// Needed for hover effects
	this->setAcceptHoverEvents(true);

	this->setCursor(Qt::PointingHandCursor);

	// The item type and the path (needs to be set manually)
	itemType = "";
	path = "";

}

// Set the pixmaps
void OpenPixmap::setPixmap(const QPixmap &pixmapNormal, const QPixmap &pixmapHover, int position) {

	normal = pixmapNormal;
	hover = pixmapHover;

	QGraphicsPixmapItem::setPixmap(normal);

}

// The normal pixmap
void OpenPixmap::setNormal() {

	QGraphicsPixmapItem::setPixmap(normal);

}

// The hover pixmap
void OpenPixmap::setHover() {


	QGraphicsPixmapItem::setPixmap(hover);

}

// Hover effect
bool OpenPixmap::sceneEvent(QEvent *e) {

	if(e->type() == QEvent::GraphicsSceneHoverMove || e->type() == QEvent::GraphicsSceneHoverEnter) {
		emit newHover(path);
		setHover();
	} else if(e->type() == QEvent::GraphicsSceneHoverLeave) {
		emit newHover("");
		setNormal();
	}

	QGraphicsItem::sceneEvent(e);

	return e->isAccepted();

}

// Click event
void OpenPixmap::mousePressEvent(QGraphicsSceneMouseEvent *) {
	this->setCursor(Qt::WaitCursor);
	emit clicked(itemType,path);

}

OpenPixmap::~OpenPixmap() { }
