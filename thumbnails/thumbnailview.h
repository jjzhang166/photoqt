#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QtDebug>

// We translate a vertical scrolling into a horizontal one
class ThumbnailView : public QGraphicsView {

	Q_OBJECT

public:
	ThumbnailView();
	~ThumbnailView();

protected:
	void wheelEvent(QWheelEvent *event);

};

#endif // THUMBNAILVIEW_H
