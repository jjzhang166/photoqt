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

#ifndef SETTINGSTABSHORTCUTS_H
#define SETTINGSTABSHORTCUTS_H

#include "../customelements/customscrollbar.h"
#include "settingstabshortcutstiles.h"
#include "settingstabshortcutskeydetect.h"
#include "settingstabshortcutschangecommand.h"
#include "../flowlayout/flowlayout.h"
#include "../widgets/customconfirm.h"

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QDateTime>

// Adjusting the shortcuts
class SettingsTabShortcuts : public QWidget {

	Q_OBJECT

public:
	SettingsTabShortcuts(QWidget *parent = 0, bool verbose = false);
	~SettingsTabShortcuts();

	bool verbose;

	// The scrollbar
	CustomScrollbar *scrollbar;

	// The internal functions
	QMap<QString,QMap<QString,QString> > internFunctions;
	QMap<QString,QList<QString> > internFunctionsOrder;
	// All the flowlayouts for exisiting and available shortcuts
	QMap<QString, FlowLayout*> allLayoutsSET;
	QMap<QString, FlowLayout*> allLayoutsAVAIL;

	// A QMap for all the tiles
	QMap<QString, QMap<QString, ShortcutsTiles*> > allTiles;

	// All the set key shortcuts
	QMap<QString,QList<QVariant> > allKeyShortcuts;
	QMap<QString,QList<QVariant> > allMouseShortcuts;

	// Load the user set and the available shortcuts
	void loadAvailShortcuts();

	// A widget for detecting a shortcut or set a mouse action
	ShortcutKeyDetect *detect;

	// Change the command
	ShortcutChangeCommand *changeCommand;

	// Load shortcuts
	void loadUserSetShortcuts();

	// Save shortcut
	void saveShortcuts();
	QMap<QString,QList<QVariant> > allKeyShortcutsNEW;
	QMap<QString,QList<QVariant> > allMouseShortcutsNEW;

	// Any change to the shortcuts? Need to update then
	bool shortcutsChanged;

	// Ask for confirmation when setting default set of shortcuts
	CustomConfirm *setDefaultConfirm;

public slots:
	// Remove a tile
	void removeTile(QString key, QString cat);
	// Add a new tile
	void addNewTile(QString exe, QString cat);
	// Set a new combination to a tile
	void analyseKeyCombo(QString category, QString identification, QString newKey);
	// Identify a new key combination
	void getNewKeyCombo(QString cat, QString id, QString exe);
	// The shortcut detection was cancelled
	void shortcutDetectCancelled();
	// Change the external command
	void analyseChangedCommand(QString id, QString cmd);
	// When it got cancelled and the tile was just added, then we need to remove the tile
	void analyseChangedCancelled(QString id);
	// There has been some change to the shortcuts, so this sets up the boolean above
	void markForChange();

signals:
	// Request settings.cpp to set default set of shortcuts
	void setDefaultShortcuts();

};

#endif // SETTINGSTABSHORTCUTS_H
