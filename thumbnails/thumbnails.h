#ifndef THUMBNAILS_H
#define THUMBNAILS_H

#include "../customelements/customscrollbar.h"
#include "thumbnailpixmapitem.h"
#include "thumbnailview.h"
#include "../threadforthumbnails.h"

#include <QGraphicsView>
#include <QVariant>
#include <QMap>
#include <QFileInfo>
#include <QDir>
#include <QtDebug>
#include <QStyleOption>
#include <QPropertyAnimation>
#include <QTextDocument>
#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QScrollBar>

// The thumbnail view
class Thumbnails : public QWidget {

	Q_OBJECT

public:
	Thumbnails(QWidget *parent = 0, bool verbose = false);
	~Thumbnails();

	bool verbose;

	// The global settings
	QMap<QString,QVariant> globSet;

	// The Graphicsview
	ThumbnailView *view;

	// The current filepath
	QString currentfile;

	// The total amount of images
	int counttot;

	// the current position in folder
	int countpos;

	// Some lists containing info about items in directory
	QFileInfoList allImgsInfo;
	QStringList allImgsPath;

	// The animation instances
	bool isShown;
	QPropertyAnimation *ani;

	// The geometries of the view
	QRect rectHidden;
	QRect rectShown;

	// The thumbnail thread and a list for all the pixmaps
	ThumbThread *thumbThread;
	QList<ThumbnailPixmapItem *> allPixmaps;

	// Update which item in the thumbnail view is hovered
	void updateThbViewHoverNormPix(QString oldpath, QString newpath);

	// Load the current directory
	void loadDir();

	// Disable thumbnails
	bool noThumbs;

	bool thumbLoadedThroughClick;

	// Called from mainwindow.cpp drawImg(), after ensuring visibility of thumbnail
	void startThread();
	bool newlyLoadedDir;

public slots:
	// Animate the widget
	void animate();

	// Stop the creation of thumbnails
	void stopThbCreation();

	// got a click on an item
	void gotClick(QString path);

	// Update a thumbnail
	void updateThumb(QImage img,QString path, int pos);

	// Jump to beginning/end of list
	void gotoFirstLast(QString side);

	void scrolledView();

private:
	void paintEvent(QPaintEvent *);

signals:
	// Load a new image
	void loadNewImg(QString path);

};

#endif // THUMBNAILS_H
