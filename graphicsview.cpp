#include "graphicsview.h"

GraphicsView::GraphicsView(QMap<QString, QVariant> set, QWidget *parent) : QGraphicsView(parent) {

	globSet = set;

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
	this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

	// The context menu
	menu = new QMenu(this);
	menu->setStyleSheet("QMenu { background-color: rgba(0,0,0,150); margin: 2px; } QMenu::item { background: transparent; color: grey; } QMenu::item:selected { background: transparent; color: white; } QMenu::item::disabled { color: #444444; }");

	// Menu hasn't yet been setup
	menuSetup = false;
	menuLastChange = 0;

}

// A click on a context menu item
void GraphicsView::contextMenuClicked() {

	QAction *action = (QAction *)sender();
	QString cmd = action->data().toString();

	emit loadContextMenuAction(QString("1:::::%1").arg(cmd));

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

// Show (and possibly set up) the context menu
void GraphicsView::contextMenuEvent(QContextMenuEvent *event) {

	// Get the last modification date
	qint64 secs = QFileInfo(QDir::homePath() + "/.photo/contextmenu").lastModified().toMSecsSinceEpoch();

	// If the menu has been modified since last time or hasn't been setup yet, we set it up
	if(secs != menuLastChange || menuSetup) {

		menuSetup = true;
		menuLastChange = secs;

		// Make sure it's empty (after a change, we would re-setup the menu and not calling clear() would lead to double entries)
		menu->clear();

		// A disabled option
		QAction *t = new QAction("Image Context Options",menu);
		t->setDisabled(true);
		menu->addAction(t);

		// Now actually read in the entries from file
		QFile file(QDir::homePath() + "/.photo/contextmenu");

		if(file.open(QIODevice::ReadOnly)) {

			QTextStream in(&file);

			QStringList all = in.readAll().split("\n\n");

			// And set up the menu
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

	// Show the menu
	menu->popup(event->globalPos());

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
void GraphicsView::mousePressEvent(QMouseEvent *event) {

	// A left button can possibly include dragging -> don't take for shortcut
	if(event->button() == Qt::LeftButton)
		buttonpressed = true;

	QGraphicsView::mousePressEvent(event);

}

// Detect mouse button release events
void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {

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

	QGraphicsView::mouseReleaseEvent(event);

}

// Get the current mouse position
void GraphicsView::mouseMoveEvent(QMouseEvent *event) {

	emit mousePos(event->pos().x(),event->pos().y());

	// Detect click + dragging
	if(buttonpressed)
		mousedragged = true;

	QGraphicsView::mouseMoveEvent(event);


}

GraphicsView::~GraphicsView() { }
