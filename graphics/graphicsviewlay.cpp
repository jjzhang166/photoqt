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

#include "graphicsviewlay.h"
#include <iostream>

ViewBigLay::ViewBigLay(QMap<QString, QVariant> set, bool v) : QVBoxLayout() {

	verbose = v;

	globSet = set;


	slideshowRunning = false;
	slideshowHide = false;


	QHBoxLayout *quickInfoTOP = new QHBoxLayout;

	// We have labels for the top and for the bottom. So there's no need to restart PhotoQt when switching thumbnails from top to bottom or vice versa

// rotation indicator
/*	quickInfoSaveTOP = new CustomLabel("*");*/
/*	quickInfoSaveTOP->setBold(true);*/
/*	quickInfoSaveTOP->hide();*/

	quickInfoCounterTOP = new QuickInfoLabel(0,"quickinfoCounterTOP",verbose);
	quickInfoCounterTOP->setStyleSheet("color: white");
	quickInfoCounterTOP->hide();
	quickInfoSepTOP = new CustomLabel("--");
	quickInfoSepTOP->hide();
	quickInfoFilenameTOP = new QuickInfoLabel(0,"quickinfoFilenameTOP",verbose);
	quickInfoFilenameTOP->setText(tr("Open File to Begin."));
	quickInfoFilenameTOP->setStyleSheet("color: white");
	quickInfoFilenameTOP->hide();
	quickInfoFilenameTOP->globSet = globSet;
	closeWindowX = new QuickInfoLabel(0,"closewindowXTOP",verbose);
	closeWindowX->setText("x");
	closeWindowX->setStyleSheet("color: white; padding: 5px");
	closeWindowX->setCursor(Qt::PointingHandCursor);
	closeWindowX->setVisible(!globSet.value("HideX").toBool());
	QSignalMapper *mapperXTOP = new QSignalMapper;
	mapperXTOP->setMapping(closeWindowX,"0:::::__hide");
	connect(closeWindowX, SIGNAL(clicked()), mapperXTOP, SLOT(map()));
	connect(mapperXTOP, SIGNAL(mapped(QString)), this, SIGNAL(clickOnX(QString)));
	connect(quickInfoCounterTOP->dohide, SIGNAL(triggered()), this, SLOT(hideItem()));
	connect(quickInfoFilenameTOP->dohide, SIGNAL(triggered()), this, SLOT(hideItem()));
	connect(quickInfoFilenameTOP->dohideFilepath, SIGNAL(triggered()), this, SLOT(hideItem()));
	connect(closeWindowX->dohide, SIGNAL(triggered()), this, SLOT(hideItem()));

// rotation indicator
/*	quickInfoTOP->addWidget(quickInfoSaveTOP);*/

	quickInfoTOP->addWidget(quickInfoCounterTOP);
	quickInfoTOP->addWidget(quickInfoSepTOP);
	quickInfoTOP->addWidget(quickInfoFilenameTOP);
	quickInfoTOP->addStretch();
	quickInfoTOP->addWidget(closeWindowX);

	this->addLayout(quickInfoTOP);

	QHBoxLayout *quickInfoBOT = new QHBoxLayout;

// rotation indicator
/*	quickInfoSaveBOT = new CustomLabel("**");*/
/*	quickInfoSaveBOT->setBold(true);*/
/*	quickInfoSaveBOT->hide();*/

	quickInfoCounterBOT = new QuickInfoLabel(0,"quickinfoCounterBOT",verbose);
	quickInfoCounterBOT->setStyleSheet("color: white");
	quickInfoCounterBOT->hide();
	quickInfoSepBOT = new CustomLabel("--");
	quickInfoSepBOT->hide();
	quickInfoFilenameBOT = new QuickInfoLabel(0,"quickinfoFilenameBOT",verbose);
	quickInfoFilenameBOT->setText(tr("Open File to Begin."));
	quickInfoFilenameBOT->setStyleSheet("color: white");
	quickInfoFilenameBOT->hide();
	quickInfoFilenameBOT->globSet = globSet;
	connect(quickInfoCounterBOT->dohide, SIGNAL(triggered()), this, SLOT(hideItem()));
	connect(quickInfoFilenameBOT->dohide, SIGNAL(triggered()), this, SLOT(hideItem()));
	connect(quickInfoFilenameBOT->dohideFilepath, SIGNAL(triggered()), this, SLOT(hideItem()));

// rotation indicator
/*	quickInfoBOT->addWidget(quickInfoSaveBOT);*/

	quickInfoBOT->addWidget(quickInfoCounterBOT);
	quickInfoBOT->addWidget(quickInfoSepBOT);
	quickInfoBOT->addWidget(quickInfoFilenameBOT);
	quickInfoBOT->addStretch();

	this->addStretch();
	this->addLayout(quickInfoBOT);


}

void ViewBigLay::setSettings(QMap<QString, QVariant> set) {

	globSet = set;

	quickInfoCounterTOP->hideFilepathShowFilename = set.value("HideFilepathShowFilename").toBool();
	quickInfoFilenameTOP->hideFilepathShowFilename = set.value("HideFilepathShowFilename").toBool();
	closeWindowX->hideFilepathShowFilename = set.value("HideFilepathShowFilename").toBool();
	quickInfoCounterBOT->hideFilepathShowFilename = set.value("HideFilepathShowFilename").toBool();
	quickInfoFilenameBOT->hideFilepathShowFilename = set.value("HideFilepathShowFilename").toBool();


}

void ViewBigLay::setPosition(QString pos) {

	if(pos == "Bottom") {
		quickInfoCounterTOP->hide();
		quickInfoFilenameTOP->hide();
		quickInfoSepTOP->hide();
		closeWindowX->hide();
	} else if(pos == "Top") {
		quickInfoCounterBOT->hide();
		quickInfoFilenameBOT->hide();
		quickInfoSepBOT->hide();
		closeWindowX->hide();
	}

}


// rotation indicator
/*void ViewBigLay::updateInfo(QString currentfile, int countpos, int counttot, bool tosave) {*/
void ViewBigLay::updateInfo(QString currentfile, int countpos, int counttot) {

	if(verbose) std::clog << "Update Quickinfo labels (show/hide)" << std::endl;

	// If a slideshow is running and the user disabled all the quickinfos for that
	if(slideshowRunning && slideshowHide) {

// rotation indicator
/*		quickInfoSaveTOP->hide();*/
/*		quickInfoSaveBOT->hide();*/

		quickInfoCounterTOP->hide();
		quickInfoFilenameTOP->hide();
		quickInfoSepTOP->hide();

		quickInfoCounterBOT->hide();
		quickInfoSepBOT->hide();
		quickInfoFilenameBOT->hide();

		closeWindowX->hide();

	} else if(globSet.value("ThumbnailPosition").toString() == "Bottom") {

		if(currentfile != "") {

// rotation indicator
/*			quickInfoSaveTOP->setVisible(tosave);*/

			quickInfoCounterTOP->setText(QString("%1/%2").arg(countpos+1).arg(counttot));

			if(globSet.value("HideFilepathShowFilename").toBool())
				quickInfoFilenameTOP->setText(QFileInfo(currentfile).fileName());
			else
				quickInfoFilenameTOP->setText(currentfile);

			quickInfoCounterTOP->setVisible(!globSet.value("HideCounter").toBool());
			quickInfoFilenameTOP->setVisible(!globSet.value("HideFilename").toBool());
			closeWindowX->setVisible(!globSet.value("HideX").toBool());
			quickInfoSepTOP->setVisible((globSet.value("HideFilename").toBool() == globSet.value("HideCounter").toBool()) && !globSet.value("HideCounter").toBool());
		} else {

// rotation indicator
/*			quickInfoSaveTOP->setVisible(false);*/

			quickInfoFilenameTOP->setText((countpos == -1 && counttot == -1) ? tr("No images match current filter") : tr("Open File to Begin."));
			quickInfoCounterTOP->hide();
			quickInfoSepTOP->hide();
			quickInfoFilenameTOP->show();
		}

// rotation indicator
/*		quickInfoSaveBOT->setVisible(false);*/

		quickInfoFilenameBOT->hide();
		quickInfoCounterBOT->hide();
		quickInfoSepBOT->hide();
		quickInfoFilenameBOT->hide();

	} else if(globSet.value("ThumbnailPosition").toString() == "Top") {

		if(currentfile != "") {

// rotation indicator
/*			quickInfoSaveBOT->setVisible(tosave);*/

			quickInfoCounterBOT->setText(QString("%1/%2").arg(countpos+1).arg(counttot));

			if(globSet.value("HideFilepathShowFilename").toBool())
				quickInfoFilenameBOT->setText(QFileInfo(currentfile).fileName());
			else
				quickInfoFilenameBOT->setText(currentfile);

			quickInfoCounterBOT->setVisible(!globSet.value("HideCounter").toBool());
			quickInfoFilenameBOT->setVisible(!globSet.value("HideFilename").toBool());
			closeWindowX->setVisible(!globSet.value("HideX").toBool());
			quickInfoSepBOT->setVisible(!globSet.value("HideFilename").toBool() && !globSet.value("HideCounter").toBool());
		} else {

// rotation indicator
/*			quickInfoSaveBOT->setVisible(false);*/

			quickInfoFilenameBOT->setText((countpos == -1 && counttot == -1) ? tr("No images match current filter") : tr("Open File to Begin."));
			quickInfoCounterBOT->hide();
			quickInfoSepBOT->hide();
			quickInfoFilenameBOT->show();
		}

// rotation indicator
/*		quickInfoSaveTOP->setVisible(false);*/

		quickInfoFilenameTOP->hide();
		quickInfoCounterTOP->hide();
		quickInfoSepTOP->hide();
		quickInfoFilenameTOP->hide();

	}

}

void ViewBigLay::hideItem() {

	QMap<QString,QVariant> updateSet;

	QString objName = ((QAction *) sender())->objectName();
	if(verbose) std::clog << "Hide quickinfo: " << objName.toStdString() << std::endl;

	if(objName.startsWith("quickinfoCounter")) {
		quickInfoCounterTOP->hide();
		quickInfoCounterBOT->hide();
		quickInfoSepTOP->hide();
		quickInfoSepBOT->hide();
		updateSet.insert("HideCounter",true);
	}
	if(objName.startsWith("quickinfoFilepath"))
		updateSet.insert("HideFilepathShowFilename",true);
	if(objName.startsWith("quickinfoFilename")) {
		quickInfoSepTOP->hide();
		quickInfoSepBOT->hide();
		quickInfoFilenameTOP->hide();
		quickInfoFilenameBOT->hide();
		updateSet.insert("HideFilename",true);
	}
	if(objName.startsWith("closewindowX")) {
		closeWindowX->hide();
		updateSet.insert("HideX",true);
	}


	emit updateSettings(updateSet);

}


ViewBigLay::~ViewBigLay() { }




/*####################################################################################
  ####################################################################################
  ####################################################################################
  ####################################################################################
  ####################################################################################*/


// A custom label for enabling a right click on them, and adding a clicked() event
QuickInfoLabel::QuickInfoLabel(QWidget *parent, QString objName, bool v) : QLabel(parent) {

	verbose = v;
	hideFilepathShowFilename = false;

	this->setObjectName(objName);

	this->setStyleSheet("QLabel { color: white; }");

	c = new QMenu;
	if(objName.startsWith("quickinfoFilename")) {
		dohideFilepath = new QAction(tr("Hide Filepath, leave Filename"),c);
		dohideFilepath->setObjectName("quickinfoFilepath");
		c->addAction(dohideFilepath);
	}
	dohide = new QAction(tr("Hide this item"),c);
	dohide->setObjectName(objName);
	c->setStyleSheet("QMenu { background-color: black; margin: 2px; } QMenu::item { color: grey; } QMenu::item:selected { color: white; } QMenu::item::disabled { color: black; }");
	c->addAction(dohide);

}

// The label on the top right (the little "x") for closing the app
void QuickInfoLabel::mouseReleaseEvent(QMouseEvent *e) {

	if(e->button() == Qt::LeftButton)
		emit clicked();

}

// Showing the context menu
void QuickInfoLabel::contextMenuEvent(QContextMenuEvent*) {
	if(this->objectName() == "quickinfoFilename")
		dohideFilepath->setVisible(!hideFilepathShowFilename);
	c->popup(QCursor::pos());
}
