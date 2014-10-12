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

#ifndef FILTERIMAGESDISPLAY_H
#define FILTERIMAGESDISPLAY_H

#include "../customelements/customlabel.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>

// Label in thumbnail view to display filter (if set)
class FilterImagesDisplay : public QWidget {

	Q_OBJECT

public:
	FilterImagesDisplay(QWidget *parent = 0);
	~FilterImagesDisplay();

	void showFilter(QStringList filter);

private:
	bool isShown;
	CustomLabel *display;

private slots:
	void removeClicked();

signals:
	void removeImageFilter();
	void changeFilter();

protected:
	void paintEvent(QPaintEvent *);

};



#endif // FILTERIMAGESDISPLAY_H
