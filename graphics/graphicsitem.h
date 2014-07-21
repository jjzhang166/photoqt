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
