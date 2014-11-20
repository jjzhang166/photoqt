#ifndef THUMBNAILS_H
#define THUMBNAILS_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QStyleOption>
#include <QPainter>
#include <QPropertyAnimation>
#include <QDir>
#include <QtDebug>
#include <QDateTime>
#include <QHBoxLayout>
#include <QTextDocument>

#include "threadforthumbnails.h"
#include "thumbnailpixmapitem.h"
#include "thumbnailview.h"

// The thumbnail view
class Thumbnails : public QWidget {

	Q_OBJECT

public:
	Thumbnails(QMap<QString,QVariant> set = QMap<QString,QVariant>(), bool verbose = false, QWidget *parent = 0);
	~Thumbnails();

	void setGlobSet(QMap<QString,QVariant> set);

	void setRect(QRect rect);

	bool isVisible() { return isShown; }

	void loadDir(QString filepath);

	void setCurrentfile(QString file) { currentfile = file; }
	QString getCurrentfile() { return currentfile; }

	QString getImageFilePathAt(int pos) { return allImgsInfo.at(pos).absoluteFilePath(); }
	int getImageFilePathIndexOf(QString s) { return allImgsInfo.indexOf(QFileInfo(s)); }

	// Update which item in the thumbnail view is hovered
	void updateThbViewHoverNormPix(QString oldpath, QString newpath);

	ThumbnailPixmapItem* getPixmap(QString path) { return allPixmaps.value(path); }
	bool pixmapLoaded(QString path) { return allPixmaps.keys().contains(path); }

	QRect areaShown() { return rectShown; }

	void clearScene() { view->scene.clear(); }

	int counttot;
	int countpos;
	QFileInfoList allImgsInfo;

	void setFilter(QStringList filter) { imageFilter = filter; }
	void removeFilter() { imageFilter.clear(); }

private:
	bool verbose;
	QMap<QString,QVariant> globSet;

	// Is it visible?
	bool isShown;
	// The animation instances
	QPropertyAnimation *ani;
	// The geometries of the view
	QRect rectHidden;
	QRect rectShown;
	bool animateInAndOut;

	// The Graphicsview
	ThumbnailView *view;

	QString currentfile;
	QStringList imageFilter;


	QMap<QString, ThumbnailPixmapItem *> allPixmaps;
	QMap<int,QString> allPixmapsPath;

	ThumbThread *thread;
	void startThread();


	static bool sort_name(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);
	static bool sort_name_desc(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);
	static bool sort_naturalname(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);
	static bool sort_naturalname_desc(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);
	static bool sort_date(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);
	static bool sort_date_desc(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);
	static bool sort_size(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);
	static bool sort_size_desc(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);

public slots:
	void makeShow();
	void makeHide();

	void centerOnCurrent(bool firstLoad = false);

	// Jump to beginning/end of list
	void gotoFirstLast(QString side);

	void abortThread() { thread->abort(); }

private slots:
	// Animate the widget
	void animate();
	void aniFinished();

	// Update a thumbnail
	void updateThumb(QImage img, QString path, int origwidth, int origheight, int pos, bool preload);

	// got a click on an item
	void gotClick(QString path);

	void scrolledView();

protected:
	void paintEvent(QPaintEvent *);

signals:
	// Load a new image
	void loadNewImg(QString path);


};


#endif // THUMBNAILS_H
