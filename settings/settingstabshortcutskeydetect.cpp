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

#include "settingstabshortcutskeydetect.h"

ShortcutKeyDetect::ShortcutKeyDetect(QWidget *parent) : QWidget(parent) {

	this->setStyleSheet("background: rgba(0,0,0,0)");

	// The current category
	cat = "";

	// The geometries for the widget
	rectShown = QRect();
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect();

	// The central widget containing all the contents
	center = new QWidget(this);
	center->setObjectName("center");
	center->setStyleSheet("QWidget#center { background: rgba(0,0,0,200); border-radius: 20px; }");

	// The animation instances
	ani = new QPropertyAnimation(center, "geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));
	isShown = false;
	this->setGeometry(rectHidden);

	// The fading instances
	fadeBack = new QTimeLine;
	fadeBack->setLoopCount(5);
	backAlphaShow = 130;
	backAlphaCur = 0;
	fadeBackIN = true;
	connect(fadeBack, SIGNAL(valueChanged(qreal)), this, SLOT(fadeStep()));

	// the title label
	QLabel *t = new QLabel("<center>" + tr("Detecting Shortcut") + "</center>");
	t->setWordWrap(true);
	t->setStyleSheet("font-size: 15pt; color: white; font-weight: bold; background: transparent");

	// the function label
	function = new QLabel("<center>function</center>");
	function->setStyleSheet("color: white; background: transparent; font-style: italic; font-size: 10pt;");

	// The two radiobuttons to switch between the two shortcut types
	keyShortcut = new CustomRadioButton(tr("Key Shortcut"));
	QHBoxLayout *keyShortcutLay = new QHBoxLayout;
	keyShortcutLay->addSpacing(30);
	keyShortcutLay->addWidget(keyShortcut);
	keyShortcutLay->addStretch();
	mouseShortcut = new CustomRadioButton(tr("Mouse Shortcut"));
	QHBoxLayout *mouseShortcutLay = new QHBoxLayout;
	mouseShortcutLay->addSpacing(30);
	mouseShortcutLay->addWidget(mouseShortcut);
	mouseShortcutLay->addStretch();


	// The key combination label
	keyCombo = new QLabel;
	keyCombo->setStyleSheet("color: white; font-size: 12pt; background: transparent");

	// This label shows an error message when the shortcut is already set
	keyExistsError = new QLabel(" ");
	keyExistsError->setStyleSheet("color: red; font-weight: bold; font-size: 10pt; background: transparent");
	keyExistsError->setWordWrap(true);

	// A combobox for the modifiers for a mouse action
	mouseMod = new CustomComboBox;
	mouseMod->addItem("---","---");
	mouseMod->addItem(tr("Ctrl"),"Ctrl");
	mouseMod->addItem(tr("Alt"),"Alt");
	mouseMod->addItem(tr("Shift"),"Shift");
	mouseMod->addItem(tr("Ctrl+Alt"),"Ctrl+Alt");
	mouseMod->addItem(tr("Ctrl+Alt"),"Ctrl+Alt");
	mouseMod->addItem(tr("Alt+Shift"),"Alt+Shift");
	mouseMod->addItem(tr("Ctrl+Alt+Shift"),"Ctrl+Alt+Shift");

	// A combobox for the mouse buttons
	mouseBut = new CustomComboBox;
	//: The following string relates to the mouse button
	mouseBut->addItem(tr("Left Button"),"Left Button");
	//: The following string relates to the mouse button
	mouseBut->addItem(tr("Right Button"),"Right Button");
	//: The following string relates to the mouse button
	mouseBut->addItem(tr("Middle Button"),"Middle Button");
	//: The following string relates to the mouse wheel
	mouseBut->addItem(tr("Wheel Up"),"Wheel Up");
	//: The following string relates to the mouse wheel
	mouseBut->addItem(tr("Wheel Down"),"Wheel Down");

	// This button accepts the new mouse action and sets it
	okAndSet = new CustomPushButton(tr("Okay"));
	connect(okAndSet, SIGNAL(clicked()), this, SLOT(setMouseCombo()));

	// The mouse layout
	QHBoxLayout *mouseLay = new QHBoxLayout;
	mouseLay->addStretch();
	mouseLay->addWidget(mouseMod);
	mouseLay->addWidget(mouseBut);
	mouseLay->addWidget(okAndSet);
	mouseLay->addStretch();


	// The close and cancel button at the bottom
	CustomPushButton *close = new CustomPushButton(tr("Cancel and don't set a shortcut"));
	connect(close, SIGNAL(clicked()), this, SLOT(animate()));
	connect(close, SIGNAL(clicked()), this, SIGNAL(shortcutCancelled()));
	QHBoxLayout *closeLay = new QHBoxLayout;
	closeLay->addStretch();
	closeLay->addWidget(close);
	closeLay->addStretch();


	// The main layout
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addStretch();
	lay->addWidget(t);
	lay->addSpacing(15);
	lay->addWidget(function);
	lay->addSpacing(30);
	lay->addLayout(keyShortcutLay);
	lay->addSpacing(5);
	lay->addWidget(keyCombo);
	lay->addSpacing(5);
	lay->addWidget(keyExistsError);
	lay->addSpacing(5);
	lay->addLayout(mouseShortcutLay);
	lay->addSpacing(5);
	lay->addLayout(mouseLay);
	lay->addSpacing(30);
	lay->addLayout(closeLay);
	lay->addStretch();
	// Setting the main layout
	center->setLayout(lay);

	// Connect the radiobuttons to a function disabling each other
	connect(mouseShortcut, SIGNAL(clicked()), this, SLOT(setRightTypeDisEnabled()));
	connect(keyShortcut, SIGNAL(clicked()), this, SLOT(setRightTypeDisEnabled()));

	// Per default a key shortcut is looked for
	keyShortcut->setChecked(true);
	setRightTypeDisEnabled();


}

// The animation function
void ShortcutKeyDetect::animate() {

	QRect shown = QRect((rectShown.width()-500)/2,(rectShown.height()-400)/2,500,400);

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		// The background is initially transparent but the geometry is full
		this->setStyleSheet(QString("background: rgba(0,0,0,0);"));
		this->setGeometry(rectShown);

		// Widget is shown
		isShown = true;

		// Set the default combo text
		keyCombo->setText("<center>&lt;" + tr("Press a key combination") + "&gt;</center>");

		// Animate widget
		ani->setDuration(200);
		ani->setStartValue(rectAni);
		ani->setEndValue(shown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		// Fade background in
		fadeBack->setDuration(200);
		fadeBack->setLoopCount(5);
		fadeBackIN = true;
		fadeBack->start();

		// Make sure this widget is on top
		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		// Fade background out
		fadeBack->setDuration(100);
		fadeBack->setLoopCount(5);
		fadeBackIN = false;
		fadeBack->start();

		// Widget is hidden again
		isShown = false;

		// Start animation out
		ani->setDuration(300);
		ani->setStartValue(shown);
		ani->setEndValue(rectAni);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

	}
}

// After animation finished
void ShortcutKeyDetect::aniFinished() {

	// We need to set rectHidden, otherwise the widget will be invisible (100% opacity), but still there, i.e. it blocks the whole gui
	if(!isShown)
		this->setGeometry(rectHidden);

}

// Each step the fading timer calls this function
void ShortcutKeyDetect::fadeStep() {

	// Fade in
	if(fadeBackIN) {
		backAlphaCur += backAlphaShow/5;
		if(backAlphaCur > backAlphaShow)
			backAlphaCur = backAlphaShow;
	// Fade out
	} else {
		backAlphaCur -= backAlphaShow/5;
		if(backAlphaCur < 0)
			backAlphaCur = 0;
	}

	// Update stylesheet
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaCur));

}

// Set the function name
void ShortcutKeyDetect::setFunctionname(QString name) {

	function->setText("<center>'" + name + "'</center>");

}

// Dis/Enable the key/mouse elements depending on which radiobutton is checked
void ShortcutKeyDetect::setRightTypeDisEnabled() {

	if(mouseShortcut->isChecked()) {
		keyCombo->setEnabled(false);
		keyCombo->setStyleSheet("color: grey; font-size: 12pt; background: transparent");
		mouseMod->setEnabled(true);
		mouseBut->setEnabled(true);
		okAndSet->setEnabled(true);
		keyExistsError->setText(" ");
		keyCombo->setText("<center>&lt;" + tr("Press a key combination") + "&gt;</center>");
	} else {
		keyCombo->setEnabled(true);
		keyCombo->setStyleSheet("color: white; font-size: 12pt; background: transparent");
		mouseMod->setEnabled(false);
		mouseBut->setEnabled(false);
		okAndSet->setEnabled(false);
	}

}

// Analyse the QKeyEvent (called by mainwindow.cpp
void ShortcutKeyDetect::analyseKeyEvent(QKeyEvent *e) {

	// This string holds the new key
	newkey = "";

	// Check for modifiers
	if(e->modifiers().testFlag(Qt::ControlModifier))
		newkey += "Ctrl+";
	if(e->modifiers().testFlag(Qt::AltModifier))
		newkey += "Alt+";
	if(e->modifiers().testFlag(Qt::ShiftModifier))
		newkey += "Shift+";

	// Get the second key pressed
	if(e->key() == Qt::Key_Escape)
		newkey += "Escape";
	else if(e->key() == Qt::Key_Right)
		newkey += "Right";
	else if(e->key() == Qt::Key_Left)
		newkey += "Left";
	else if(e->key() == Qt::Key_Up)
		newkey += "Up";
	else if(e->key() == Qt::Key_Down)
		newkey += "Down";
	else if(e->key() == Qt::Key_Space)
		newkey += "Space";
	else if(e->key() == Qt::Key_Delete)
		newkey += "Delete";
	else if(e->key() == Qt::Key_Home)
		newkey += "Home";
	else if(e->key() == Qt::Key_End)
		newkey += "End";
	else if(e->key() == Qt::Key_PageUp)
		newkey += "PgUp";
	else if(e->key() == Qt::Key_PageDown)
		newkey += "PgDown";
	else if(e->key() == Qt::Key_Insert)
		newkey += "Insert";
	else if(e->key() == Qt::Key_Tab)
		newkey += "Tab";
	else if(e->key() != 16777251 && e->key() != 16777248 && e->key() != 16777249)
		newkey += QKeySequence(e->key()).toString();

	// If for some reason an empty string is passed on, show a quotation mark (should never happen)
	if(newkey == "")
		newkey = tr("Press a key combination");

	// Set key combo
	keyCombo->setText("<center>&lt;" + newkey + "&gt;</center>");

	// If just a normal key or a modifier+normal key is pressed: emit key combo and exit
	if(e->key() != 16777251 && e->key() != 16777248 && e->key() != 16777249) {

		// Emit Key Combo
		if(alreadySetKeys.contains(newkey)) {
			keyExistsError->setText("<center>" + tr("Key combination exists already. Please choose another one!") + "</center>");
		} else {
			keyExistsError->setText(" ");
			emit gotKeys(cat,id,newkey);
			this->animate();
		}

	}

}

// Set a new mouse combination
void ShortcutKeyDetect::setMouseCombo() {

	newkey = "[M]";
	newkey += mouseMod->itemData(mouseMod->currentIndex()).toString();
	newkey += "+";
	newkey += mouseBut->itemData(mouseBut->currentIndex()).toString();

	emit gotKeys(cat,id,newkey);

	this->animate();

}


void ShortcutKeyDetect::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


ShortcutKeyDetect::~ShortcutKeyDetect() { }
