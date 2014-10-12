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

#ifndef VIEWBIGLAY_H
#define VIEWBIGLAY_H

#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QSignalMapper>
#include <QMouseEvent>
#include <QFileInfo>

#include "../customelements/customlabel.h"

// A custom label (the "x" top right corner) for closing PhotoQt
class QuickInfoLabel : public QLabel {
	Q_OBJECT

public:
	QuickInfoLabel(QWidget *parent = 0, QString objName = "", bool v = false);

	bool verbose;

	bool hideFilepathShowFilename;

	QMenu *c;
	QAction *dohideFilepath;
	QAction *dohide;
	QString version;

	QMap<QString,QVariant> globSet;

protected:
	void mouseReleaseEvent(QMouseEvent *e);
	void contextMenuEvent(QContextMenuEvent *ev);
signals:
	void clicked();
};


// This item is used for displaying the big image
class ViewBigLay : public QVBoxLayout {

	Q_OBJECT

public:
	ViewBigLay(QMap<QString,QVariant> set, bool v);
	~ViewBigLay();

	bool verbose;

	void setPosition(QString pos);
// rotation indicator
/*	void updateInfo(QString currentfile, int countpos, int counttot, bool tosave);*/
	void updateInfo(QString currentfile, int countpos, int counttot);

	bool slideshowRunning;
	bool slideshowHide;

	void setSettings(QMap<QString,QVariant>);

public slots:
	void hideItem();

private:
	QMap<QString,QVariant> globSet;

// rotation indicator
/*	CustomLabel *quickInfoSaveBOT;*/

	QuickInfoLabel *quickInfoCounterBOT;
	CustomLabel *quickInfoSepBOT;
	QuickInfoLabel *quickInfoFilenameBOT;

// rotation indicator
/*	CustomLabel *quickInfoSaveTOP;*/

	QuickInfoLabel *quickInfoCounterTOP;
	QuickInfoLabel *quickInfoFilenameTOP;
	QuickInfoLabel *closeWindowX;

	CustomLabel *quickInfoSepTOP;

signals:
	void clickOnX(QString);
	void updateSettings(QMap<QString,QVariant>);

};

#endif // VIEWBIGLAY_H
