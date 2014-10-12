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

#ifndef SETTINGSTABOTHERCONTEXTTILES_H
#define SETTINGSTABOTHERCONTEXTTILES_H

#include "../customelements/customlabel.h"
#include "../customelements/customlineedit.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>

class ContextTile : public QWidget {

	Q_OBJECT

public:
	ContextTile(QString cmdTxt, QString descTxt, QWidget *parent = 0);
	~ContextTile();

	// The command and the menu text
	QLineEdit *cmd;
	QLineEdit *desc;

	// The index in the layout
	int index;

	// SDome css
	QString css;
	QString cssBackgroundNorm;
	QString cssBackgroundHov;

	// A label for dragging
	CustomLabel *dragLabel;

public slots:
	// When user requested deletion
	void pleaseDeleteMe();

signals:
	// Tell parent to delete this index
	void deleteMe(int ind);

protected:
	void paintEvent(QPaintEvent *);

};


#endif // SETTINGSOTHERCONTEXTTILE_H
