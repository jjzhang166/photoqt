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

#ifndef THUMBNAILS_H
#define THUMBNAILS_H

#include "../customelements/customscrollbar.h"
#include "thumbnailpixmapitem.h"
#include "thumbnailview.h"
#include "threadforthumbnails.h"

#include <QGraphicsView>
#include <QVariant>
#include <QMap>
#include <QFileInfo>
#include <QDir>
#include <QStyleOption>
#include <QPropertyAnimation>
#include <QTextDocument>
#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QApplication>

// The thumbnail view
class Thumbnails : public QWidget {

	Q_OBJECT

public:
	Thumbnails(QWidget *parent = 0, bool verbose = false, QMap<QString,QVariant> set = QMap<QString,QVariant>());
	~Thumbnails();

	bool verbose;

	// The global settings
	QMap<QString,QVariant> globSet;

	// The Graphicsview
	ThumbnailView *view;

	// Load the current directory
	void loadDir(bool amReloadingDir = false);

	// The current filepath
	void setCurrentfile(QString file) { currentfile = QFileInfo(file).fileName(); currentdir = QFileInfo(file).absolutePath(); }
	QString getCurrentfile() { return currentdir+currentfile; }

	// The total amount of images
	int counttot;
	// the current position in folder
	int countpos;

	// Disable thumbnails
	bool noThumbs;

	// Some lists containing info about items in directory
	QFileInfoList allImgsInfo;
	QString getImageFilePathAt(int pos) { return allImgsInfo.at(pos).absoluteFilePath(); }
	int getImageFilePathIndexOf(QString s) { return allImgsInfo.indexOf(QFileInfo(s)); }

	QList<ThumbnailPixmapItem *> allPixmaps;

	bool thumbLoadedThroughClick;

	// Called from mainwindow.cpp drawImg(), after ensuring visibility of thumbnail
	void startThread();
	bool newlyLoadedDir;

	// Update which item in the thumbnail view is hovered
	void updateThbViewHoverNormPix(QString oldpath, QString newpath);

	void setRect(QRect rect);

	bool isVisible() { return isShown; }

	static bool compareNamesFileInfo(const QFileInfo &s1fileinfo, const QFileInfo &s2fileinfo);

	QRect areaShown() { return rectShown; }

	void setFilter(QStringList filter) { imageFilter = filter; }
	void removeFilter() { imageFilter.clear(); }

private:
	QString currentfile;
	QString currentdir;

	bool isShown;

	// The animation instances
	QPropertyAnimation *ani;

	// The geometries of the view
	QRect rectHidden;
	QRect rectShown;

	// The thumbnail thread and a list for all the pixmaps
	ThumbThread *thumbThread;

	bool animateInAndOut;

	QStringList imageFilter;

public slots:
	void makeShow();
	void makeHide();

	// Stop the creation of thumbnails
	void stopThbCreation();

	// Jump to beginning/end of list
	void gotoFirstLast(QString side);

	void scrolledView();

private slots:
	// Animate the widget
	void animate();
	void aniFinished();

	// got a click on an item
	void gotClick(QString path);

	// Update a thumbnail
	void updateThumb(QImage img, QString path, int origwidth, int origheight, int pos);


protected:
	void paintEvent(QPaintEvent *);

signals:
	// Load a new image
	void loadNewImg(QString path);

};

#endif // THUMBNAILS_H
