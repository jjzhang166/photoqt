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

ShortcutKeyDetect::ShortcutKeyDetect(QWidget *parent) : MyWidget(parent) {

	this->setVisibleArea(QSize(500,400));

	// The current category
	cat = "";

	// the title label
	CustomLabel *t = new CustomLabel("<center>" + tr("Detecting Shortcut") + "</center>");
	t->setWordWrap(true);
	t->setFontSize(15);
	t->setBold(true);

	// the function label
	function = new CustomLabel("<center>function</center>");
	function->setFontSize(10);
	function->setItalic(true);

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
	keyCombo = new CustomLabel;
	keyCombo->setFontSize(12);

	// This label shows an error message when the shortcut is already set
	keyExistsError = new CustomLabel(" ");
	keyExistsError->setFontColor("red");
	keyExistsError->setBold(true);
	keyExistsError->setFontSize(10);
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
	setWidgetLayout(lay);

	// Connect the radiobuttons to a function disabling each other
	connect(mouseShortcut, SIGNAL(clicked()), this, SLOT(setRightTypeDisEnabled()));
	connect(keyShortcut, SIGNAL(clicked()), this, SLOT(setRightTypeDisEnabled()));

	// Per default a key shortcut is looked for
	keyShortcut->setChecked(true);
	setRightTypeDisEnabled();


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
		if(newkey == "") keyCombo->setText("<center>&lt;" + tr("Press a key combination") + "&gt;</center>");
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
			makeHide();
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

	makeHide();

}

void ShortcutKeyDetect::setKeyCombo(QString combo) {

	keyCombo->setText("<center>&lt;" + combo + "&gt;</center>");

}

ShortcutKeyDetect::~ShortcutKeyDetect() { }
