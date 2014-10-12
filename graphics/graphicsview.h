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

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "../customelements/customlabel.h"
#include "../customelements/customscrollbar.h"

#include <QGraphicsView>
#include <QWheelEvent>
#include <QTimer>
#include <QDesktopWidget>
#include <QMenu>
#include <QDir>
#include <QTimeLine>
#include <QDateTime>

#include <QWidgetAction>
#include <QHBoxLayout>

// the graphicsview for the main image
class GraphicsView : public QGraphicsView {

	Q_OBJECT

public:
	GraphicsView(QMap<QString,QVariant> set, QWidget *parent = 0);
	~GraphicsView();

	// All the global settings
	QMap<QString,QVariant> globSet;

	// The absolute current scale factor
	int absoluteScaleFactor;

	bool imgLoaded;

	void showContext() { setupContextMenu(); menu->popup(QCursor::pos()); }

private:

	QGraphicsScene sceneBig;

	// The timer checks at startup in short intervals if the graphicsscene is correctly set up, and whenever that's the case it's calling the drawImg() function (see mainwindow.cpp)
	QTimer *t;

	// These two maps hold all the mouse shortcuts set
	QMap<QString,QString> mouseSh;
	QMap<QString,bool> mouseClose;

	// Currently we don't allow click/drag combination as shortcuts. Probably in next version we will
	bool mousedragged;
	bool buttonpressed;

	// The context menu
	QMenu *menu;

	// Has the context menu been set up?
	bool menuSetup;
	qint64 menuLastChange;

	void setupContextMenu();



	bool cropImage;
	bool cropSelectionStarted;
	QRect cropSelectionRect;

public slots:
	// Set a mouse shortcut
	void setShortcut(QString key, bool close, QString id);

private slots:
	// A click on a context menu item
	void contextMenuClicked();
	void contextMenuClickedWidgetAction();

protected:
	// This events are used for zooming and menu opening/closing
	void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);


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
