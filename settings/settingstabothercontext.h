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

#ifndef SETTINGSTABOTHERCONTEXT_H
#define SETTINGSTABOTHERCONTEXT_H

#include "settingstabothercontexttiles.h"
#include "../customelements/customscrollbar.h"
#include "../widgets/customconfirm.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QDir>
#include <QDrag>
#include <QMimeData>

class Context : public QScrollArea {

	Q_OBJECT

public:
	Context(QWidget *parent = 0, bool verbose = false);
	~Context();

	bool verbose;

	// The main layout
	QVBoxLayout *lay;

	// All the entries
	QList<ContextTile *> allTiles;

	// The old index (needed fro drag&drop)
	int oldIndex;

	// Load/Save context menu
	void loadContext();
	void saveContext();

public slots:
	// Add a new entry
	void addNewEntry();

	// Remove an entry
	void deleteTile(int index);

protected:
	void mousePressEvent(QMouseEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

};

#endif // SETTINGSTABOTHERCONTEXT_H
