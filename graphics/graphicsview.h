#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QtDebug>
#include <QTimer>
#include <QDesktopWidget>
#include <QMenu>
#include <QDir>
#include <QTimeLine>
#include <QDateTime>

// the graphicsview for the main image
class GraphicsView : public QGraphicsView {

	Q_OBJECT

public:
	GraphicsView(QMap<QString,QVariant> set, QWidget *parent = 0);
	~GraphicsView();

	QGraphicsScene sceneBig;

	// The timer checks at startup in short intervals if the graphicsscene is correctly set up, and whenever that's the case it's calling the drawImg() function (see mainwindow.cpp)
	QTimer *t;

	// All the global settings
	QMap<QString,QVariant> globSet;

	// These two maps hold all the mouse shortcuts set
	QMap<QString,QString> mouseSh;
	QMap<QString,bool> mouseClose;

	// Currently we don't allow click/drag combination as shortcuts. Probably in next version we will
	bool mousedragged;
	bool buttonpressed;

	// The absolute current scale factor
	int absoluteScaleFactor;

	// The context menu
	QMenu *menu;

	// Has the context menu been set up?
	bool menuSetup;
	qint64 menuLastChange;

public slots:
	// Set a mouse shortcut
	void setShortcut(QString key, bool close, QString id);

	// A click on a context menu item
	void contextMenuClicked();

protected:
	// This events are used for zooming and menu opening/closing
	void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	// Mouse moved
	void mousePos(int,int);

	// Mouse click
	void clicked(QPoint);

	// Mouse shortcut triggered
	void shMouseDo(QString,bool mouseAction);

	// Context menu item selected
	void loadContextMenuAction(QString);

};

#endif // GRAPHICSVIEW_H
