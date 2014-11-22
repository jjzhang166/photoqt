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

#ifndef SETTINGSTABSHORTCUTSKEYDETECT_H
#define SETTINGSTABSHORTCUTSKEYDETECT_H

#include "../widgets/mywidget.h"

#include "../customelements/custompushbutton.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customcombobox.h"
#include "../customelements/customlabel.h"

#include <QKeyEvent>
#include <QComboBox>

// A widget for detecting a new key shortcut/mouse action
class ShortcutKeyDetect : public MyWidget {

	Q_OBJECT

public:
	ShortcutKeyDetect(QWidget *parent = 0);
	~ShortcutKeyDetect();

	// The function for which the shortcut is for
	CustomLabel *function;

	// The category and the id of associated tile
	QString cat;
	QString id;

	// All the shortcuts already in use
	QStringList alreadySetKeys;

	// The radio buttons to choose between key shortcut and mouse action
	CustomRadioButton *keyShortcut;
	CustomRadioButton *mouseShortcut;

	// The labels for key combinations
	CustomLabel *keyCombo;
	CustomLabel *keyExistsError;

	// The comboboxes for mouse actions
	CustomComboBox *mouseMod;
	CustomComboBox *mouseBut;
	CustomPushButton *okAndSet;

	// The new shortcut
	QString newkey;

	// Analyse the keyevent (called by mainwindow.cpp)
	void analyseKeyEvent(QKeyEvent *event);

public slots:
	// Set the current function name
	void setFunctionname(QString name);

	// Set key combination (when changing shortcut)
	void setKeyCombo(QString combo);

	// Set a mouse action as shortcut
	void setMouseCombo();

	// Enable/Disable the other type of shortcut
	void setRightTypeDisEnabled();

signals:
	// Got a new key combo
	void gotKeys(QString category, QString identification, QString newKey);

	// The shortcut detection was cancelled (if the tile was newly added, it is removed again)
	void shortcutCancelled();

};

#endif // SETTINGSTABSHORTCUTSKEYDETECT_H
