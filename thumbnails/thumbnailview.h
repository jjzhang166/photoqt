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
