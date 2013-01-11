#ifndef THUMBNAILPIXMAPITEM_H
#define THUMBNAILPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

// The pixmap item for the thumbnail view
class ThumbnailPixmapItem : public QObject, public QGraphicsPixmapItem {

	Q_OBJECT

public:
	ThumbnailPixmapItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
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
