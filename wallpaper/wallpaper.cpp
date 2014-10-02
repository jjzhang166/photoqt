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

#include "wallpaper.h"
#include <iostream>

Wallpaper::Wallpaper(QMap<QString, QVariant> set, bool v, QWidget *parent) : MyWidget(parent) {

	this->setBorderArea(150,100);

	verbose = v;
	globSet = set;


	// The buttons to proceed or abort are always visible (i.e. not scrolled)
	ok = new CustomPushButton(tr("Okay, do it!"));
	cancel = new CustomPushButton(tr("Nooo, don't!"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(ok);
	butLay->addWidget(cancel);
	butLay->addStretch();

	CustomLine *line = new CustomLine;
	QVBoxLayout *botLay = new QVBoxLayout;
	botLay->addWidget(line);
	botLay->addLayout(butLay);

	this->addButtonLayout(botLay);

	connect(ok, SIGNAL(clicked()), this, SLOT(goAheadAndSetWallpaper()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));

	QVBoxLayout *lay = new QVBoxLayout;

	// the widget title
	QLabel *title = new QLabel("<center><span style=\"font-size: 30pt; font-weight: bold\">" + tr("Set as Wallpaper") + "</span></center>");
	title->setStyleSheet("color: white");
	lay->addWidget(title);
	lay->addSpacing(10);


	// The filename is always set to this label
	filenameLabel = new CustomLabel("<center>---</center>");
	filenameLabel->setFontColor("lightgrey");
	filenameLabel->setItalic(true);
	lay->addWidget(filenameLabel);
	lay->addSpacing(20);


	// Label explaining the wm detection
	CustomLabel *wmDetectedLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Window Manager") + "</span></b> " + "<br><br>" + tr("PhotoQt tries to detect you window manager according to the environment variables set by your system. If it still got it wrong, you can change the window manager here manually."));
	wmDetectedLabel->setWordWrap(true);
	lay->addWidget(wmDetectedLabel);
	lay->addSpacing(10);

	// Combobox to change WM
	wm = new CustomComboBox;
	wm->setFontSize(15);
	wm->setBorder(1,"white");
	wm->addItem("KDE4","kde");
	wm->addItem("Gnome/Unity","gnome");
	wm->addItem("XFCE4","xfce");
	wm->addItem("Razor-Qt", "razor");
	wm->addItem(tr("Other"),"other");
	QHBoxLayout *wmLay = new QHBoxLayout;
	wmLay->addStretch();
	wmLay->addWidget(wm);
	wmLay->addStretch();
	lay->addLayout(wmLay);
	lay->addSpacing(10);


	// This message is used to show an (error or other) message depending on the window manager
	wmMessage = new CustomLabel;
	wmMessage->setAlignment(Qt::AlignCenter);
	wmMessage->setWordWrap(true);
	wmMessage->setVisible(false);
	lay->addSpacing(10);
	lay->addWidget(wmMessage);
	lay->addSpacing(20);

	connect(wm, SIGNAL(currentIndexChanged(int)), this, SLOT(wmSelected()));



	//////// GNOME SETTINGS /////////////

	// Some image options can be set (the label is globsl, because it is hidden/shown depending on wm choice)
	gnomePicOpsLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Picture Options") + "</span></b> " + "<br><br>" + tr("There are several picture options that can be set for the wallpaper image."));
	gnomePicOpsLabel->setMargin(5);
	gnomePicOpsLabel->setWordWrap(true);

	// A layout holding the image options
	QVBoxLayout *gnomePicOpsLayCenter = new QVBoxLayout;
	gnomeButGrp = new QButtonGroup;
	gnomePictureOptions.clear();
	// These options are possible
	QStringList picOpts;
	picOpts << "wallpaper";
	picOpts << "centered";
	picOpts << "scaled";
	picOpts << "zoom";
	picOpts << "spanned";
	// Setup radiobuttons, add to layout, to button group and store in QMap
	for(int i = 0; i < picOpts.size(); ++i) {
		CustomRadioButton *chk = new CustomRadioButton(picOpts.at(i));
		if(i == 3) chk->setChecked(true);
		gnomePicOpsLayCenter->addWidget(chk);
		gnomeButGrp->addButton(chk);
		gnomePictureOptions.insert(picOpts.at(i),chk);
	}
	// And center in a horizontal layout
	QHBoxLayout *gnomePicOpsLay = new QHBoxLayout;
	gnomePicOpsLay->addStretch();
	gnomePicOpsLay->addLayout(gnomePicOpsLayCenter);
	gnomePicOpsLay->addStretch();

	// Add label and button layout to central widget layout
	lay->addWidget(gnomePicOpsLabel);
	lay->addLayout(gnomePicOpsLay);



	////// THE FOLLOWING SCREEN SELECT IS USED BY DIFFERENT WMs //////

	QVBoxLayout *wmMonitorLayCenter = new QVBoxLayout;
	wmMonitorSelect.clear();
	QDesktopWidget desk;
	for(int i = 0; i < desk.screenCount(); ++i) {
		CustomCheckBox *mon = new CustomCheckBox(tr("Screen") + QString(" #%1").arg(i));
		mon->setChecked(true);
		wmMonitorLayCenter->addWidget(mon);
		wmMonitorSelect.insert(i,mon);
	}

	QHBoxLayout *wmMonitorLay = new QHBoxLayout;
	wmMonitorLay->addStretch();
	wmMonitorLay->addLayout(wmMonitorLayCenter);
	wmMonitorLay->addStretch();

	wmMonitorLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Select Monitors") + "</span></b> " + "<br><br>" + tr("The wallpaper can be set to either of the available monitors (or any combination)."));
	wmMonitorLabel->setWordWrap(true);
	wmMonitorLabel->setMargin(5);

	lay->addWidget(wmMonitorLabel);
	lay->addLayout(wmMonitorLay);



	/////// XFCE SETTINGS /////////////

	QVBoxLayout *xfcePicOpsLayCenter = new QVBoxLayout;

	xfceButGrp = new QButtonGroup;
	xfcePictureOptions.clear();
	QStringList xfcePicOpts;
	xfcePicOpts << "automatic";
	xfcePicOpts << "centered";
	xfcePicOpts << "tiled";
	xfcePicOpts << "spanned";
	xfcePicOpts << "scaled";
	xfcePicOpts << "magnified";
	for(int i = 0; i < xfcePicOpts.size(); ++i) {
		CustomRadioButton *chk = new CustomRadioButton(xfcePicOpts.at(i));
		if(i == 0) chk->setChecked(true);
		xfcePicOpsLayCenter->addWidget(chk);
		xfceButGrp->addButton(chk);
		xfcePictureOptions.insert(xfcePicOpts.at(i),chk);
	}

	QHBoxLayout *xfcePicOpsLay = new QHBoxLayout;
	xfcePicOpsLay->addStretch();
	xfcePicOpsLay->addLayout(xfcePicOpsLayCenter);
	xfcePicOpsLay->addStretch();

	xfcePicOpsLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Picture Options") + "</span></b> " + "<br><br>" + tr("There are several picture options that can be set for the wallpaper image."));
	xfcePicOpsLabel->setMargin(5);
	xfcePicOpsLabel->setWordWrap(true);

	lay->addWidget(xfcePicOpsLabel);
	lay->addLayout(xfcePicOpsLay);



	///////// OTHER SETTINGS ///////////

	otherFeh = new CustomRadioButton(tr("Use 'feh'"));
	otherFeh->setIndicatorImage(":/img/checkbox_checked.png",":/img/checkbox_checked.png",":/img/checkbox_unchecked.png",":/img/checkbox_unchecked.png");
	otherFeh->setChecked(true);
	otherNitrogen = new CustomRadioButton(tr("Use 'nitrogen'"));
	otherNitrogen->setIndicatorImage(":/img/checkbox_checked.png",":/img/checkbox_checked.png",":/img/checkbox_unchecked.png",":/img/checkbox_unchecked.png");

	QButtonGroup *otherGrp = new QButtonGroup;
	otherGrp->addButton(otherFeh);
	otherGrp->addButton(otherNitrogen);

	QHBoxLayout *externLay = new QHBoxLayout;
	externLay->addStretch();
	externLay->addWidget(otherFeh);
	externLay->addWidget(otherNitrogen);
	externLay->addStretch();
	connect(otherFeh, SIGNAL(clicked()), this, SLOT(swapFehNitrogen()));
	connect(otherNitrogen, SIGNAL(clicked()), this, SLOT(swapFehNitrogen()));

	otherNitrogenGrp = new QButtonGroup;
	otherNitrogenOptions.clear();
	QVBoxLayout *otherNitrogenPicOptLay = new QVBoxLayout;
	otherNitrogenPicOptLay->setMargin(5);
	QStringList listNitrogen;
	listNitrogen << "auto";
	listNitrogen << "centered";
	listNitrogen << "scaled";
	listNitrogen << "tiled";
	listNitrogen << "zoom";
	listNitrogen << "zoom-fill";
	QString eleN;
	foreach(eleN, listNitrogen) {
		CustomRadioButton *r = new CustomRadioButton("--set-" + eleN);
		r->setObjectName(eleN);
		if(eleN == "auto") r->setChecked(true);
		otherNitrogenPicOptLay->addWidget(r);
		otherNitrogenOptions.insert(eleN,r);
		otherNitrogenGrp->addButton(r);
	}

	otherFehGrp = new QButtonGroup;
	otherFehOptions.clear();
	QVBoxLayout *otherFehPicOptLay = new QVBoxLayout;
	otherFehPicOptLay->setMargin(5);
	QStringList listFeh;
	listFeh << "center";
	listFeh << "fill";
	listFeh << "max";
	listFeh << "scale";
	listFeh << "tile";
	QString eleF;
	foreach(eleF, listFeh) {
		CustomRadioButton *r = new CustomRadioButton("--bg-" + eleF);
		r->setObjectName(eleF);
		if(eleF == "fill") r->setChecked(true);
		otherFehPicOptLay->addWidget(r);
		otherFehOptions.insert(eleF,r);
		otherFehGrp->addButton(r);
	}


	QHBoxLayout *otherVLay = new QHBoxLayout;
	otherVLay->setMargin(5);
	otherVLay->addStretch();
	otherVLay->addLayout(otherNitrogenPicOptLay);
	otherVLay->addLayout(otherFehPicOptLay);
	otherVLay->addStretch();



	lay->addLayout(externLay);
	lay->addLayout(otherVLay);
	lay->addSpacing(10);
	lay->addStretch();

	this->setWidgetLayout(lay);

	detectWM();
	wmSelected();

}

// Show settings for feh/nitrogen
void Wallpaper::swapFehNitrogen() {

	if(otherFeh->isChecked()) {

		QMapIterator<QString, CustomRadioButton*> iN(otherNitrogenOptions);
		while (iN.hasNext()) {
			iN.next();
			iN.value()->hide();
		}


		QMapIterator<QString, CustomRadioButton*> iF(otherFehOptions);
		while (iF.hasNext()) {
			iF.next();
			if(wm->currentIndex() == wm->count()-1)
				iF.value()->show();
			else
				iF.value()->hide();
		}

	} else {

		QMapIterator<QString, CustomRadioButton*> iN(otherNitrogenOptions);
		while (iN.hasNext()) {
			iN.next();
			if(wm->currentIndex() == wm->count()-1)
				iN.value()->show();
			else
				iN.value()->hide();
		}

		QMapIterator<QString, CustomRadioButton*> iF(otherFehOptions);
		while (iF.hasNext()) {
			iF.next();
			iF.value()->hide();
		}

	}

}

// API function, called from mainwindow.cpp
void Wallpaper::setWallpaper(QString file) {

	if(file != "") {

		if(verbose) std::clog << "Wallpaper request: " << file.toStdString() << std::endl;

		// Set filename to label
		filename = file;
		filenameLabel->setText("<center>'" + QFileInfo(file).fileName() + "'</file>");

		// And show widget
		if(!isVisible()) makeShow();

	}

}

// Abort wallpaper settings
void Wallpaper::dontSetWallpaper() {

	if(isVisible()) makeHide();

}

// Detect the currently running wm (according to environment variables
void Wallpaper::detectWM() {

	if(QString(getenv("KDE_FULL_SESSION")).toLower() == "true")
		wm->setCurrentIndex(0);
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "gnome" || QString(getenv("XDG_CURRENT_DESKTOP")).toLower() == "unity" || QString(getenv("DESKTOP_SESSION")).toLower() == "ubuntu")
		wm->setCurrentIndex(1);
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "xfce4")
		wm->setCurrentIndex(2);
	else if(QString(getenv("XDG_CURRENT_DESKTOP")) == "Razor")
		wm->setCurrentIndex(3);
	else
		wm->setCurrentIndex(wm->count()-1);

}

// When the wm choice has changed
void Wallpaper::wmSelected() {

	// These two hold whether the gnome/xfce specific options have to be visible or not
	bool hideKDE = true;
	bool hideGNOME = true;
	bool hideXFCE = true;
	bool hideRazor = true;
	bool hideOther = true;

	// The current wm index
	int current = wm->currentIndex();

	// KDE
	if(current == 0) {

		wmMessage->setText(tr("Sorry, KDE doesn't (yet) offer the feature to change the wallpaper except from their own system settings. Hopefully this will change in the future, but until then there's nothing that can be done here."));
		wmMessage->setStyleSheet("color: red; font-weight: bold");

		ok->setEnabled(false);
		wmMessage->setVisible(true);

		hideKDE = false;

	// GNOME
	} else if(current == 1) {

		ok->setEnabled(true);
		wmMessage->setVisible(false);

		hideGNOME = false;

	// XFCE4
	} else if(current == 2) {

		ok->setEnabled(true);
		wmMessage->setVisible(false);

		hideXFCE = false;

	// RAZOR
	} else if(current == 3) {

		wmMessage->setText(tr("Sorry, Razor-Qt doesn't yet support this feature... hopefully that'll change soon!"));
		wmMessage->setStyleSheet("color: red; font-weight: bold");

		ok->setEnabled(false);
		wmMessage->setVisible(true);

		hideRazor = false;


	// OTHER
	} else if(current == wm->count()-1) {
		wmMessage->setText(tr("PhotoQt uses feh or nitrogen to change the background of the desktop background. This is meant particularly for window managers that don't natively support this (like e.g. Openbox)."));
		wmMessage->setStyleSheet("color: white");
		ok->setEnabled(true);
		wmMessage->setVisible(true);

		hideOther = false;

	}

	if(hideGNOME) {
		gnomePicOpsLabel->hide();
		QMapIterator<QString, CustomRadioButton*> i(gnomePictureOptions);
		while (i.hasNext()) {
			i.next();
			i.value()->hide();
		}
	} else {
		gnomePicOpsLabel->show();
		QMapIterator<QString, CustomRadioButton*> i(gnomePictureOptions);
		while (i.hasNext()) {
			i.next();
			i.value()->show();
		}
	}

	if(hideXFCE) {

		if(hideRazor) {
			wmMonitorLabel->hide();
			QMapIterator<int, CustomCheckBox*> i2(wmMonitorSelect);
			while (i2.hasNext()) {
				i2.next();
				i2.value()->hide();
			}
		}

		xfcePicOpsLabel->hide();
		QMapIterator<QString, CustomRadioButton*> i3(xfcePictureOptions);
		while (i3.hasNext()) {
			i3.next();
			i3.value()->hide();
		}
	} else {
		// The monitor selection is only visible if there's more than one monitor connected
		QDesktopWidget desk;
		if(desk.screenCount() > 1) {
			wmMonitorLabel->show();
			QMapIterator<int, CustomCheckBox*> i2(wmMonitorSelect);
			while (i2.hasNext()) {
				i2.next();
				i2.value()->show();
			}
		} else {
			wmMonitorLabel->hide();
			QMapIterator<int, CustomCheckBox*> i2(wmMonitorSelect);
			while (i2.hasNext()) {
				i2.next();
				i2.value()->hide();
			}
		}

		xfcePicOpsLabel->show();
		QMapIterator<QString, CustomRadioButton*> i3(xfcePictureOptions);
		while (i3.hasNext()) {
			i3.next();
			i3.value()->show();
		}
	}

	if(hideRazor) {

		if(hideXFCE) {
			wmMonitorLabel->hide();
			QMapIterator<int, CustomCheckBox*> i2(wmMonitorSelect);
			while (i2.hasNext()) {
				i2.next();
				i2.value()->hide();
			}
		}

	} else {

		// UNCOMMENT WHEN FEATURE GETS IMPLEMENTED
/*		wmMonitorLabel->show();
		QMapIterator<int, CustomCheckBox*> i2(wmMonitorSelect);
		while (i2.hasNext()) {
			i2.next();
			i2.value()->show();
		}*/

		// REMOVE WHEN FEATURE GETS IMPLEMENTED
		if(hideXFCE) {
			wmMonitorLabel->hide();
			QMapIterator<int, CustomCheckBox*> i2(wmMonitorSelect);
			while (i2.hasNext()) {
				i2.next();
				i2.value()->hide();
			}
		}

	}

	if(hideOther) {

		otherFeh->hide();
		otherNitrogen->hide();

		QMapIterator<QString, CustomRadioButton*> i(otherFehOptions);
		while (i.hasNext()) {
			i.next();
			i.value()->hide();
		}

		swapFehNitrogen();

	} else {

		otherFeh->show();
		otherNitrogen->show();

		swapFehNitrogen();

	}

}



// Actually go ahead and set wallpaper
void Wallpaper::goAheadAndSetWallpaper() {

	// Hide widget
	makeHide();

	// Call right function
	if(wm->currentIndex() == 0)
		setKDE();
	else if(wm->currentIndex() == 1)
		setGNOME();
	else if(wm->currentIndex() == 2)
		setXFCE();
	else if(wm->currentIndex() == 3)
		setRazor();
	else if(wm->currentIndex() == wm->count()-1)
		setOTHER();


}



// The program should never reach this function (since a selection of "KDE" will disable the OK button, but it is included as a placeholder for a future implementation
void Wallpaper::setKDE() {

	std::cerr << "KDE -- not yet supported";

}

void Wallpaper::setGNOME() {

	if(verbose) std::clog << "Set Gnome wallpaper." << std::endl;

	QProcess proc;
	proc.execute(QString("gsettings set org.gnome.desktop.background picture-options %1").arg(gnomeButGrp->checkedButton()->text()));
	proc.execute(QString("gsettings set org.gnome.desktop.background picture-uri file://%1").arg(filename));

}

void Wallpaper::setXFCE() {

	if(verbose) std::clog << "Set XFCE wallpaper." << std::endl;

	QStringList xfcePicOpts;
	xfcePicOpts << "automatic";
	xfcePicOpts << "centered";
	xfcePicOpts << "tiled";
	xfcePicOpts << "spanned";
	xfcePicOpts << "scaled";
	xfcePicOpts << "magnified";

	QMapIterator<int, CustomCheckBox*> i3(wmMonitorSelect);
	int i = 0;
	while (i3.hasNext()) {
		i3.next();


		if(i3.value()->isChecked()) {

			QProcess proc;

			// Trying to set image style
			if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-style -s %2").arg(i).arg(xfcePicOpts.indexOf(xfceButGrp->checkedButton()->text())))) {

				// If we don't succeed, then the property most likely doesn't exist
				if(verbose) std::clog << "image-style property not found, trying to create it" << std::endl;


				// Create Property
				if(proc.execute(QString("xfconf-query -c xfce4-desktop -n -p /backdrop/screen0/monitor%1/image-style -t int -s 0").arg(i)) == 1)
					std::cerr << QString("ERROR: Unable to create property '/backdrop/screen0/monitor%1/image-style'!").arg(i).toStdString() << std::endl;
				else {

					// Trying to set image style again
					if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-style -s %2").arg(i).arg(i3.key())) == 1)
						std::cerr << "ERROR: Couldn't set image-style!" << std::endl;
					else
						if(verbose) std::clog << "Image style set..." << std::endl;

				}

			} else
				if(verbose) std::clog << "image-style set" << std::endl;




			if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-path -s \"%2\"").arg(i).arg(filename)) == 1) {

				// If we don't succeed, then the property most likely doesn't exist
				if(verbose) std::clog << "image-path property not found, trying to create it" << std::endl;

				// Create Property
				if(proc.execute(QString("xfconf-query -c xfce4-desktop -n -p /backdrop/screen0/monitor%1/image-path -t string -s ''").arg(i)) == 1)
					std::cerr << QString("ERROR: Unable to create property '/backdrop/screen0/monitor%1/image-path'!").arg(i).toStdString() << std::endl;
				else {

					// And try again setting wallpaper
					if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor%1/image-path -s \"%2\"").arg(i).arg(filename)) == 1)
						std::cerr << QString("ERROR: Unable to create property '/backdrop/screen0/monitor%1image-path' needed to set background!").arg(i).toStdString() << std::endl;
					else
						if(verbose) std::clog << "Image set..." << std::endl;
				}
			}
			if(verbose) std::clog << "Property found, image set..." << std::endl;

		} else
			if(verbose) std::clog << "Monitor #" << i << " not checked..." << std::endl;

		++i;

	}

}

// Razor doesn't provide functionality yet
void Wallpaper::setRazor() {

	std::cerr << "RAZOR not yet supported" << std::endl;

}

void Wallpaper::setOTHER() {

	if(verbose) std::clog << "Set OTHER wallpaper." << std::endl;

	if(otherFeh->isChecked()) {

		QString option = "--bg-" + otherFehGrp->checkedButton()->objectName();

		QProcess::execute(QString("feh %1 %2").arg(option).arg(filename));

	} else {

		QString option = "--set-" + otherNitrogenGrp->checkedButton()->objectName();

		QProcess::execute(QString("nitrogen %1 %2").arg(option).arg(filename));

	}

}

void Wallpaper::accept() {

	if(ok->isEnabled())
		ok->animateClick();

}

Wallpaper::~Wallpaper() { }
