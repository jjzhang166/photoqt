#ifndef OPENFILE_H
#define OPENFILE_H

#include "customelements/customscrollbar.h"
#include "customelements/custompushbutton.h"
#include "customelements/customcheckbox.h"

#include "open/openpixmapitem.h"
#include "open/opencrumbs.h"
#include "threadforthumbnails.h"

#include <QWidget>
#include <QPainter>
#include <QTreeView>
#include <QFileSystemModel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTextDocument>
#include <QDesktopWidget>
#include <QApplication>
#include <QCheckBox>

class Open : public QWidget {

	Q_OBJECT

public:
	Open(QWidget *parent = 0);
	~Open();

	QMap<QString,QVariant> globSet;

	// The animation used
	QPropertyAnimation *ani;
	bool isShown;

	// The currently loaded dir
	QString dir;

	// The QRects for the different shapes
	QRect rectHidden;
	QRect aniStart;
	QRect rectShown;

	// We use a couple of threads and alternate between them, so that each thread has enough time to terminate before being used again. This holds the currently used thread
	int threadNumber;
	// This holds the total amount of threads in use
	int openThreadNum;
	// And the threads themselves
	QList<ThumbThread*> openThreadList;

	// All the pixmaps for the folders and the images in the current directory
	QMap<QString,OpenPixmap*> allPixFold;
	QMap<QString,OpenPixmap*> allPixImg;

	// The filesystem tree on the left
	CustomScrollbar *fileSysTreeScrollbar;
	QTreeView *fileSysTree;
	QFileSystemModel *fileSysModel;

	// Show hidden folders/files?
	CustomCheckBox *showHiddenFolders;
	// Automatically load thumbnails for images on dir load?
	CustomCheckBox *autoLoadThumbs;

	// The graphics elements
	QGraphicsView *view;
	QGraphicsScene scene;

	// The crumbs
	Crumbs *crumbs;

	// Which item is hovered? We need to know this for enabling keyboard navigation
	QString pathHovered;


public slots:

	// Animation of the widget
	void animate();
	void aniFinished();

	// A crumb has been clicked
	void crumbsClicked(QString path);

	// A click on a filesystem row
	void fileSysTreeClicked(QModelIndex clkd);

	// Got a click on an item
	void gotClick(QString itemType, QString path);

	// Enter/Return loads the currently hovered item
	void gotEnter();

	// Load the directory
	void loadDir();

	// Move the selection (by keyboard)
	void moveSel(QString direction);

	// A new item is hovered
	void newHover(QString path);

	// Adjust the checkbox after the settings have been loaded/changed
	void setChkBoxToggled();

	// Select and open the right directory in the filesystemtree
	void setDirFileSysTree(QString d);

	// Stop the thumbnail creation
	void stopThb();

	// Update a thumbnail (coming from thread)
	void updateThumb(QImage p, QString f, int pos);

private:
	void paintEvent(QPaintEvent *);

signals:
	// A click on an image
	void loadNewImg(QString path);
	// Stop the thumbnail creation of the main thumbnail view
	void stopThbCreation();
	// Signalise that widget is opened/closed
	void blockFunc(bool block);
	// A checkbox was clicked, i.e. a setting changed
	void settingChanged(QMap<QString,QVariant> changedSet);

};

#endif // OPENFILE_H
