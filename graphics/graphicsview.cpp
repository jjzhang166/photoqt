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

#include "graphicsview.h"

GraphicsView::GraphicsView(QMap<QString, QVariant> set, QWidget *parent) : QGraphicsView(parent) {

	globSet = set;

	imgLoaded = false;


	this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	this->setObjectName("viewBig");
	this->setScene(&sceneBig);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->setDragMode(QGraphicsView::ScrollHandDrag);
	this->setMouseTracking(true);
	CustomScrollbar *vertScrollbar = new CustomScrollbar;
	CustomScrollbar *horScrollbar = new CustomScrollbar;
	horScrollbar->setOrientation(Qt::Horizontal);
	this->setVerticalScrollBar(vertScrollbar);
	this->setHorizontalScrollBar(horScrollbar);

	this->setAlignment(Qt::AlignTop);

	// Start a timer that calls the drawImg() function on startup whenever the scene is correctly set up
	t = new QTimer;

	this->setStyleSheet("background:transparent; border: none;");

	// These two booleans detect a mouse click that includes dragging
	mousedragged = false;
	buttonpressed = false;

	// The absolute current scale factor
	absoluteScaleFactor = 0;

	// The array storring the "mouse shortcuts"
	mouseSh.clear();

	// Set some render hints for better quality
	this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	// The context menu
	menu = new QMenu(this);
	menu->setStyleSheet("QMenu { background-color: rgba(0,0,0,150); margin: 2px; } QMenu::item { background: transparent; color: grey; } QMenu::item:selected { background: transparent; color: white; } QMenu::item::disabled { color: #444444; }");

	// Menu hasn't yet been setup
	menuSetup = false;
	menuLastChange = 0;

	this->raise();

}

// A click on a context menu item (QAction)
void GraphicsView::contextMenuClicked() {

	QAction *action = (QAction *)sender();
	QString cmd = action->data().toString();

	cmd.startsWith("__CTX__") ? loadContextMenuAction(cmd) : loadContextMenuAction(QString("1:::::%1").arg(cmd));

}

// A click on a context menu item (CustomLabel)
void GraphicsView::contextMenuClickedWidgetAction() {

	CustomLabel *action = (CustomLabel *)sender();
	QString cmd = action->objectName();

	cmd.startsWith("__CTX__") ? loadContextMenuAction(cmd) : loadContextMenuAction(QString("1:::::%1").arg(cmd));


}

// Called from mainwindow.cpp, setting up a shortcut
void GraphicsView::setShortcut(QString key, bool close, QString id) {

	// Remove the identifier
	key = key.remove("[M]").trimmed();

	// If no modifier is pressed, we remove the "---"
	if(key.startsWith("---"))
		key = key.remove("---+");

	// And we store the pair in the map
	mouseSh[key] = id;
	mouseClose[key] = close;

}

void GraphicsView::setupContextMenu() {

	// Get the last modification date
	qint64 secs = QFileInfo(QDir::homePath() + "/.photoqt/contextmenu").lastModified().toMSecsSinceEpoch();

	// If the menu has been modified since last time or hasn't been setup yet, we set it up
	if(secs != menuLastChange || !menuSetup) {

		menuSetup = true;
		menuLastChange = secs;

		// Make sure it's empty (after a change, we would re-setup the menu and not calling clear() would lead to double entries)
		menu->clear();


		// Move in directory
		QWidgetAction *moveImg = new QWidgetAction(menu);
		CustomLabel *move = new CustomLabel(tr("Move:"));
		move->setStyleSheet("QLabel {color: rgba(180,180,180,255); background: transparent; }");
		CustomLabel *moveFirst = new CustomLabel;
		moveFirst->setPixmap(QPixmap(":/img/firstSmall.png"));
		moveFirst->setObjectName("__CTX__movefirst");
		moveFirst->setCursor(Qt::PointingHandCursor);
		CustomLabel *moveLeft = new CustomLabel(tr("Previous"));
		moveLeft->setObjectName("__CTX__moveprev");
		moveLeft->setCursor(Qt::PointingHandCursor);
		CustomLabel *moveRight = new CustomLabel(tr("Next"));
		moveRight->setObjectName("__CTX__movenext");
		moveRight->setCursor(Qt::PointingHandCursor);
		CustomLabel *moveLast = new CustomLabel;
		moveLast->setPixmap(QPixmap(":/img/lastSmall.png"));
		moveLast->setObjectName("__CTX__movelast");
		moveLast->setCursor(Qt::PointingHandCursor);
		QWidget *moveWid = new QWidget;
		QHBoxLayout *moveLay = new QHBoxLayout;
		moveLay->addWidget(move);
		moveLay->addSpacing(5);
		moveLay->addWidget(moveFirst);
		moveLay->addSpacing(5);
		moveLay->addWidget(moveLeft);
		moveLay->addWidget(moveRight);
		moveLay->addSpacing(5);
		moveLay->addWidget(moveLast);
		moveLay->addStretch();
		moveWid->setLayout(moveLay);
		moveImg->setDefaultWidget(moveWid);
		menu->addAction(moveImg);
		connect(moveFirst, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(moveLeft, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(moveRight, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(moveLast, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));

		// Rotate Image
		QWidgetAction *rotateImg = new QWidgetAction(menu);
		CustomLabel *rot = new CustomLabel(tr("Rotate:"));
		rot->setStyleSheet("QLabel {color: rgba(180,180,180,255); background: transparent; }");
		CustomLabel *rotLeft = new CustomLabel("<img src=\":/img/rotateLeftSmall.png\"> " + tr("Left"));
		rotLeft->setObjectName("__CTX__rotateleft");
		rotLeft->setCursor(Qt::PointingHandCursor);
		CustomLabel *rotRight = new CustomLabel(tr("Right") + " <img src=\":/img/rotateRightSmall.png\">");
		rotRight->setObjectName("__CTX__rotateright");
		rotRight->setCursor(Qt::PointingHandCursor);
		QWidget *rotWid = new QWidget;
		QHBoxLayout *rotLay = new QHBoxLayout;
		rotLay->addWidget(rot);
		rotLay->addSpacing(5);
		rotLay->addWidget(rotLeft);
		rotLay->addSpacing(10);
		rotLay->addWidget(rotRight);
		rotLay->addStretch();
		rotWid->setLayout(rotLay);
		rotateImg->setDefaultWidget(rotWid);
		menu->addAction(rotateImg);
		connect(rotLeft, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(rotRight, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));

		// Flip Image
		QWidgetAction *flipImg = new QWidgetAction(menu);
		CustomLabel *flip = new CustomLabel(tr("Flip:"));
		flip->setStyleSheet("QLabel {color: rgba(180,180,180,255); background: transparent; }");
		CustomLabel *flipHor = new CustomLabel("<img src=\":/img/flipHSmall.png\"> " + tr("Horizontal"));
		flipHor->setObjectName("__CTX__fliph");
		flipHor->setCursor(Qt::PointingHandCursor);
		CustomLabel *flipVer = new CustomLabel("<img src=\":/img/flipVSmall.png\">" + tr("Vertical"));
		flipVer->setObjectName("__CTX__flipv");
		flipVer->setCursor(Qt::PointingHandCursor);
		QWidget *flipWid = new QWidget;
		QHBoxLayout *flipLay = new QHBoxLayout;
		flipLay->addWidget(flip);
		flipLay->addSpacing(5);
		flipLay->addWidget(flipHor);
		flipLay->addSpacing(5);
		flipLay->addWidget(flipVer);
		flipLay->addStretch();
		flipWid->setLayout(flipLay);
		flipImg->setDefaultWidget(flipWid);
		menu->addAction(flipImg);
		connect(flipHor, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(flipVer, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));

		// Zoom Image
		QWidgetAction *zoomImg = new QWidgetAction(menu);
		CustomLabel *zoom = new CustomLabel(tr("Zoom:"));
		zoom->setStyleSheet("QLabel {color: rgba(180,180,180,255); background: transparent; }");
		CustomLabel *zoomIn = new CustomLabel("(+) " + tr("In"));
		zoomIn->setObjectName("__CTX__zoomin");
		zoomIn->setCursor(Qt::PointingHandCursor);
		CustomLabel *zoomOut = new CustomLabel("(-) " + tr("Out"));
		zoomOut->setObjectName("__CTX__zoomout");
		zoomOut->setCursor(Qt::PointingHandCursor);
		CustomLabel *zoomActual = new CustomLabel("(1:1) " + tr("Actual"));
		zoomActual->setObjectName("__CTX__zoomactual");
		zoomActual->setCursor(Qt::PointingHandCursor);
		CustomLabel *zoomReset = new CustomLabel("(0) " + tr("Reset"));
		zoomReset->setObjectName("__CTX__zoomreset");
		zoomReset->setCursor(Qt::PointingHandCursor);
		QWidget *zoomWid = new QWidget;
		QHBoxLayout *zoomLay = new QHBoxLayout;
		zoomLay->addWidget(zoom);
		zoomLay->addSpacing(5);
		zoomLay->addWidget(zoomIn);
		zoomLay->addSpacing(5);
		zoomLay->addWidget(zoomOut);
		zoomLay->addSpacing(5);
		zoomLay->addWidget(zoomActual);
		zoomLay->addSpacing(5);
		zoomLay->addWidget(zoomReset);
		zoomLay->addStretch();
		zoomWid->setLayout(zoomLay);
		zoomImg->setDefaultWidget(zoomWid);
		menu->addAction(zoomImg);
		connect(zoomIn, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(zoomOut, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(zoomActual, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));
		connect(zoomReset, SIGNAL(clicked()), this, SLOT(contextMenuClickedWidgetAction()));

		menu->addSeparator();

		QAction *scale = new QAction(tr("Scale Image"), menu);
		scale->setData("__CTX__scaleimage");
		scale->setIcon(QIcon(":/img/scale.png"));
		menu->addAction(scale);
		connect(scale, SIGNAL(triggered()), this, SLOT(contextMenuClicked()));

		menu->addSeparator();

		QAction *openInFm = new QAction(tr("Open in default File Manager"), menu);
		openInFm->setData("__CTX__openinfm");
		openInFm->setIcon(QIcon(":/img/open.png"));
		menu->addAction(openInFm);
		connect(openInFm, SIGNAL(triggered()), this, SLOT(contextMenuClicked()));

		QAction *deleteFile = new QAction(tr("Delete File"), menu);
		deleteFile->setData("__CTX__delete");
		deleteFile->setIcon(QIcon(":/img/delete.png"));
		menu->addAction(deleteFile);
		connect(deleteFile, SIGNAL(triggered()), this, SLOT(contextMenuClicked()));

		QAction *renameFile = new QAction(tr("Rename File"), menu);
		renameFile->setData("__CTX__rename");
		renameFile->setIcon(QIcon(":/img/rename.png"));
		menu->addAction(renameFile);
		connect(renameFile, SIGNAL(triggered()), this, SLOT(contextMenuClicked()));

		menu->addSeparator();

		// Now read in the additional entries from file
		QFile file(QDir::homePath() + "/.photoqt/contextmenu");

		if(file.open(QIODevice::ReadOnly)) {

			QTextStream in(&file);

			QStringList all = in.readAll().split("\n\n");

			// And set up the rest of the menu
			for(int i = 0; i < all.length(); ++i) {

				QString cmd = all.at(i).split("\n").at(0);
				QString desc = all.at(i).split("\n").at(1);

				QString binary = cmd;
				binary.remove("%f").remove("%d");

				binary = binary.trimmed();

				QAction *action = new QAction(desc, menu);
				action->setIcon(QIcon::fromTheme(binary));
				action->setData(cmd);
				menu->addAction(action);

				connect(action, SIGNAL(triggered()), this, SLOT(contextMenuClicked()));

			}

		}

	}

}


// Zooming the image on ctrl+mouse wheel
void GraphicsView::wheelEvent(QWheelEvent *event) {

	// Booleans storring the modifiers state
	bool ctrlPressed = false;
	bool altPressed = false;
	bool shiftPressed = false;

	// Check for modifiers
	if(event->modifiers().testFlag(Qt::ControlModifier))
		ctrlPressed = true;
	if(event->modifiers().testFlag(Qt::AltModifier))
		altPressed = true;
	if(event->modifiers().testFlag(Qt::ShiftModifier))
		shiftPressed = true;

	// Booleans for wheel movement
	bool wheelup = false;
	bool wheeldown = false;

	// Check for wheel movement
	if(event->delta() > 0)
		wheelup = true;
	else
		wheeldown = true;

	// This will hold the key for the map
	QString key = "";

	// Setup the string, first the modifiers
	if(ctrlPressed)
		key = "Ctrl";
	if(altPressed) {
		if(key != "")
			key += "+";
		key += "Alt";
	}
	if(shiftPressed) {
		if(key != "")
			key += "+";
		key += "Shift";
	}

	// If a modifier is pressed, add a "+"
	if(key != "")
		key += "+";

	// Add the wheel movement
	if(wheelup)
		key += "Wheel Up";
	else if(wheeldown)
		key += "Wheel Down";

	// And check for exisiting shortcut
	if(mouseSh.keys().contains(key)) {

		QString toemit = QString("%1:::::%2").arg(int(mouseClose[key])).arg(mouseSh[key]);

		// execute the command
		emit shMouseDo(toemit, true);

	} else
		QGraphicsView::wheelEvent(event);

}

// Detecting all mouse button press events
void GraphicsView::mousePressEvent(QMouseEvent *e) {

	if(cropImage) {
		cropSelectionStarted=true;
		cropSelectionRect.setTopLeft(e->pos());
		cropSelectionRect.setBottomRight(e->pos());
	} else {

		// A left button can possibly include dragging -> don't take for shortcut
		if(e->button() == Qt::LeftButton)
			buttonpressed = true;

	}

	QGraphicsView::mousePressEvent(e);

}

// Detect mouse button release events
void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {

	if(cropImage) {

		if(cropSelectionRect.topLeft() == cropSelectionRect.bottomRight()) {
			cropSelectionRect = QRect();
			repaint();
		}

	} else {

		// Booleans storring the modifiers state
		bool ctrlPressed = false;
		bool altPressed = false;
		bool shiftPressed = false;

		// Check for modifiers
		if(event->modifiers().testFlag(Qt::ControlModifier))
			ctrlPressed = true;
		if(event->modifiers().testFlag(Qt::AltModifier))
			altPressed = true;
		if(event->modifiers().testFlag(Qt::ShiftModifier))
			shiftPressed = true;

		// Booleans for buttons
		bool doubleClick = false;
		bool rightButton = false;
		bool leftButton = false;
		bool middleButton = false;

		// Check for buttons
		if(event->type() == QMouseEvent::MouseButtonDblClick)
			doubleClick = true;
		else if(event->button() == Qt::LeftButton)
			leftButton = true;
		else if(event->button() == Qt::RightButton)
			rightButton = true;
		else if(event->button() == Qt::MiddleButton)
			middleButton = true;

		// Key string
		QString key = "";

		// Set up the string
		if(ctrlPressed)
			key = "Ctrl";
		if(altPressed) {
			if(key != "")
				key += "+";
			key += "Alt";
		}
		if(shiftPressed) {
			if(key != "")
				key += "+";
			key += "Shift";
		}

		if(key != "")
			key += "+";

		if(doubleClick)
			key += "Double Click";
		else if(rightButton)
			key += "Right Button";
		else if(leftButton)
			key += "Left Button";
		else if(middleButton)
			key += "Middle Button";

		// If mouse click doesn't include dragging -> take it
		if(!mousedragged) {

			// If shortcut exists, execute command
			if(mouseSh.keys().contains(key)) {
				QString toemit = QString("%1:::::%2").arg(int(mouseClose[key])).arg(mouseSh[key]);
				emit shMouseDo(toemit,true);
			} else if(key == "Left Button")
				emit clicked(event->pos());

		} else
			mousedragged = false;

		// Reset booleans
		buttonpressed = false;

	}

	QGraphicsView::mouseReleaseEvent(event);

}

// Get the current mouse position
void GraphicsView::mouseMoveEvent(QMouseEvent *e) {

	if(cropImage) {

		if (cropSelectionStarted) {
			cropSelectionRect.setBottomRight(e->pos());
			repaint();
		}

	} else {

		emit mousePos(e->pos().x(),e->pos().y());

		// Detect click + dragging
		if(buttonpressed)
			mousedragged = true;

	}

	QGraphicsView::mouseMoveEvent(e);


}

GraphicsView::~GraphicsView() { }
