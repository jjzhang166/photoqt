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

#include "settingstabothercontext.h"
#include <iostream>

Context::Context(QWidget *parent, bool v) : QScrollArea(parent) {

	verbose = v;

	// No tiles at startup of course
	allTiles.clear();

	this->setMouseTracking(true);
	this->setAcceptDrops(true);

	this->setStyleSheet("background: rgba(255,255,255,50); border-radius: 5px;");

	lay = new QVBoxLayout;

	// The header widget
	QWidget *header = new QWidget(this);
	header->setStyleSheet("background: white; color: black; font-weight: bold; font-style: italic");
	QLabel *exeHeader = new QLabel(tr("Executeable"));
	exeHeader->setAlignment(Qt::AlignCenter);
	QLabel *descHeader = new QLabel(tr("Menu Text"));
	descHeader->setAlignment(Qt::AlignCenter);
	QHBoxLayout *headerLay = new QHBoxLayout;
	headerLay->addSpacing(130);
	headerLay->addWidget(exeHeader);
	headerLay->addWidget(descHeader);
	headerLay->addSpacing(20);
	header->setLayout(headerLay);

	lay->addWidget(header);
	lay->addStretch();

	QWidget *widg = new QWidget;
	widg->setLayout(lay);

	this->setWidget(widg);
	this->setWidgetResizable(true);

	this->setFixedHeight(250);
	this->setFixedWidth(550);

	CustomScrollbar *scr = new CustomScrollbar;
	scr->keepAlwaysVisible(true);
	this->setVerticalScrollBar(scr);

}

void Context::mousePressEvent(QMouseEvent *e) {

	// Start dragging
	if(e->button() == Qt::LeftButton) {

		if(verbose) std::clog << "setOC: Start dragging" << std::endl;

		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData;

		bool gotWidget = false;

		mimeData->setData("photo/menuitem","someaction");
		drag->setMimeData(mimeData);

		for(int i = 0; i < allTiles.length(); ++i) {

			ContextTile *tile = allTiles.at(i);

			// Take the tile
			if(tile->geometry().contains(e->pos()) && e->pos().x() < tile->x()+tile->dragLabel->x()+tile->dragLabel->width()) {
				drag->setPixmap(QPixmap::grabWidget(allTiles.at(i)));
				oldIndex = lay->indexOf(allTiles.at(i));
				gotWidget = true;
				break;
			}

		}
		if(gotWidget)
			drag->exec();

	}

}

void Context::dragEnterEvent(QDragEnterEvent *e) {

	// Accept this mime type
	if(e->mimeData()->hasFormat("photo/menuitem"))
		e->acceptProposedAction();

}

void Context::dropEvent(QDropEvent *e) {

	// The new index is determined
	int newIndex = lay->indexOf(this->childAt(e->pos()));
	if(newIndex == -1)
		newIndex = lay->indexOf(this->childAt(e->pos())->parentWidget());

	if(verbose) std::clog << "setOC: Item dropped: " << oldIndex << " - " << newIndex << std::endl;

	// If a valid tile was dragged onto another valid tile, move it
	if(newIndex != -1 && newIndex != 0 && oldIndex != -1 && oldIndex != 0 && this->childAt(e->pos()) != 0) {

		ContextTile *c = (ContextTile *)lay->takeAt(oldIndex)->widget();
		c->index = newIndex;
		lay->insertWidget(newIndex,c);
	}


}

// Load the context menu entries
void Context::loadContext() {

	if(verbose) std::clog << "setOC: Load context menu from file" << std::endl;

	// First delete all existing tiles
	for(int i = 0; i < allTiles.length(); ++i) {
		QWidget *t = lay->takeAt(lay->indexOf(allTiles.at(i)))->widget();
		t->hide();
		delete t;
	}
	allTiles.clear();

	// Load the file
	QFile file(QDir::homePath() + "/.photoqt/contextmenu");
	if(file.open(QIODevice::ReadOnly)) {

		QTextStream in(&file);

		QStringList all = in.readAll().split("\n\n");

		// And set up all the tiles
		for(int i = 0; i < all.length(); ++i) {

			QString entry = all.at(i);

			ContextTile *tile = new ContextTile(entry.split("\n").at(0),entry.split("\n").at(1),this);
			tile->index = lay->count()-1;
			allTiles.append(tile);
			lay->insertWidget(lay->count()-1,tile);

			connect(tile, SIGNAL(deleteMe(int)), this, SLOT(deleteTile(int)));

		}

	}

}

// Add a new entry at the end
void Context::addNewEntry() {

	if(verbose) std::clog << "setOC: Add new entry" << std::endl;

	ContextTile *tile = new ContextTile("exe","text",this);
	tile->index = lay->count()-1;
	allTiles.append(tile);
	lay->insertWidget(lay->count()-1,tile);

	connect(tile, SIGNAL(deleteMe(int)), this, SLOT(deleteTile(int)));

	tile->cmd->setFocus();
	tile->cmd->selectAll();

}

// Save context menu
void Context::saveContext() {

	if(verbose) std::clog << "setOC: Save context menu" << std::endl;

	// This map is sorted according to the tiles order in the widget (possibly changed by user)
	QMap<int,QStringList> items;
	for(int i = 0; i < allTiles.length(); ++i) {
		ContextTile *t = allTiles.at(i);
		QStringList l;
		l << t->cmd->text() << t->desc->text();
		items.insert(lay->indexOf(t),l);
	}

	// Compose file content
	QString cont = "";
	for(int i = 1; i <= allTiles.length(); ++i) {

		if(i != 1)
			cont += "\n\n";

		QStringList l = items.value(i);

		cont += l.at(0);
		cont += "\n";
		cont += l.at(1);

	}

	// And save the file
	QFile file(QDir::homePath() + "/.photoqt/contextmenu");
	if(file.remove()) {
		if(file.open(QIODevice::WriteOnly)) {

			QTextStream out(&file);
			out << cont;

			file.close();

		} else
			std::cerr << "ERROR: Couldn't write contextmenu file." << std::endl;

	} else
		std::cerr << "ERROR: Couldn't remove old contextmenu file" << std::endl;

}

// Delete an existing entry
void Context::deleteTile(int index) {

	if(verbose) std::clog << "setOC: Delete tile: " << index << std::endl;

	int newIndex = 0;

	for(int i = 0; i < allTiles.length(); ++i) {

		ContextTile *t = allTiles.at(i);

		if(t->index == index) {
			t->hide();
			delete lay->takeAt(lay->indexOf(t));
			allTiles.removeAt(i);
		} else {
			t->index = newIndex;
			++newIndex;
		}

	}

}

Context::~Context() { }
