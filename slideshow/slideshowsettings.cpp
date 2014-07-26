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

#include "slideshowsettings.h"
#include <iostream>

SlideShow::SlideShow(QMap<QString, QVariant> set, bool v, QWidget *parent) : MyWidget(parent) {

	this->setBorderArea(150,100);

	// Setting some variables
	globSet = set;
	verbose = v;

	// The main layout
	QVBoxLayout *lay = new QVBoxLayout;

	// The title of the widget
	CustomLabel *title = new CustomLabel("<center><h1>" + tr("Start a Slideshow") + "</h1></center>");
	title->setStyleSheet("color: white; background: none");
	title->setWordWrap(true);
	lay->addWidget(title);
	lay->addSpacing(20);

	// A short description
	CustomLabel *desc = new CustomLabel(tr("There are several settings that can be adjusted for a slideshow, like the time between the image, if and how long the transition between the images should be, and also a music file can be specified that is played in the background.") + "<br><br>" + tr("Once you have set the desired options, you can also start a slideshow the next time via 'Quickstart', i.e. skipping this settings window."));
	desc->setWordWrap(true);
	lay->addWidget(desc);
	lay->addSpacing(10);

	// Adjust time
	timeSlider = new CustomSlider;
	timeSlider->setMinimum(1);
	timeSlider->setMaximum(300);
	timeSlider->setSingleStep(1);
	timeSlider->setPageStep(1);
	timeSlider->setValue(globSet.value("SlideShowTime").toInt());
	CustomSpinBox *timeSpin = new CustomSpinBox;
	timeSpin->setMinimum(1);
	timeSpin->setMaximum(300);
	timeSpin->setSingleStep(1);
	timeSpin->setSuffix("s");
	timeSpin->setValue(globSet.value("SlideShowTime").toInt());
	connect(timeSlider, SIGNAL(valueChanged(int)), timeSpin, SLOT(setValue(int)));
	connect(timeSpin, SIGNAL(valueChanged(int)), timeSlider, SLOT(setValue(int)));
	QHBoxLayout *timeLay = new QHBoxLayout;
	timeLay->addStretch();
	timeLay->addWidget(timeSlider);
	timeLay->addWidget(timeSpin);
	timeLay->addStretch();
	CustomLabel *timeLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Time in between") + "</b></span><br><br>" + tr("Adjust the time between the images. The time specified here is the amount of time the image will be completely visible, i.e. the transitioning (if set) is not part of this time."));
	timeLabel->setWordWrap(true);
	lay->addWidget(timeLabel);
	lay->addSpacing(5);
	lay->addLayout(timeLay);
	lay->addSpacing(10);

	// Adjust transition
	trans = new CustomSlider(Qt::Horizontal);
	trans->setMinimum(0);
	trans->setMaximum(10);
	trans->setOrientation(Qt::Horizontal);
	trans->setTickPosition(QSlider::TicksBelow);
	trans->setPageStep(1);
	trans->setSingleStep(1);
	trans->setValue(globSet.value("SlideShowTransition").toInt());
	CustomLabel *transLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Smooth Transition") + "</span></b> " + "<br><br>" + tr("Here you can set, if you want the images to fade into each other, and how fast they shall do that."));
	transLabel->setWordWrap(true);
	CustomLabel *noTrans = new CustomLabel(tr("No Transition"));
	CustomLabel *longTrans = new CustomLabel(tr("Long Transition"));
	QHBoxLayout *transLay = new QHBoxLayout;
	transLay->addStretch();
	transLay->addWidget(noTrans);
	transLay->addWidget(trans);
	transLay->addWidget(longTrans);
	transLay->addStretch();
	lay->addWidget(transLabel);
	lay->addSpacing(5);
	lay->addLayout(transLay);
	lay->addSpacing(10);

	// Adjust quickinfo labels
	hideQuickInfo = new CustomCheckBox(tr("Hide Quickinfos"));
	hideQuickInfo->setChecked(globSet.value("SlideShowHideQuickinfo").toBool());
	CustomLabel *hideQuickLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Hide Quickinfo") + "</span></b> " + "<br><br>" + tr("Depending on your setup, PhotoQt displays some information at the top edge, like position in current directory or file path/name. Here you can disable them temporarily for the slideshow."));
	hideQuickLabel->setWordWrap(true);
	QHBoxLayout *hideQuickLay = new QHBoxLayout;
	hideQuickLay->addStretch();
	hideQuickLay->addWidget(hideQuickInfo);
	hideQuickLay->addStretch();
	lay->addWidget(hideQuickLabel);
	lay->addSpacing(5);
	lay->addLayout(hideQuickLay);
	lay->addSpacing(10);

	// Adjust music
	musicEnable = new CustomCheckBox(tr("Enable Music"));
	musicEnable->setChecked(globSet.value("SlideShowMusicFile").toString() != "");
	QHBoxLayout *musicEnableLay = new QHBoxLayout;
	musicEnableLay->addStretch();
	musicEnableLay->addWidget(musicEnable);
	musicEnableLay->addStretch();
	musicPath = new CustomLineEdit;
	musicPath->setEnabled(globSet.value("SlideShowMusicFile").toString() != "");
	musicPath->setReadOnly(true);
	musicPath->setCursor(Qt::PointingHandCursor);
	musicPath->setToolTip(tr("Click to select/change music file"));
	musicPath->setText(globSet.value("SlideShowMusicFile").toString());
	QHBoxLayout *musicPathLay = new QHBoxLayout;
	musicPathLay->addStretch();
	musicPathLay->addWidget(musicPath);
	musicPathLay->addStretch();
	CustomLabel *musicLabel = new CustomLabel("<b><span style=\"font-size:12pt\">" + tr("Background Music") + "</span></b> " + "<br><br>" + tr("Some might like to listen to some music while the slideshow is running. Here you can select a music file you want to be played in the background."));
	musicLabel->setWordWrap(true);
	lay->addWidget(musicLabel);
	lay->addSpacing(5);
	lay->addLayout(musicEnableLay);
	lay->addLayout(musicPathLay);
	connect(musicEnable, SIGNAL(toggled(bool)), musicPath, SLOT(setEnabled(bool)));
	connect(musicPath, SIGNAL(clicked()), this, SLOT(browseForMusic()));

	lay->addStretch();
	this->setWidgetLayout(lay);

	CustomLine *line = new CustomLine;
	this->addWidgetAtBottom(line);

	// Start or don't start slideshow
	CustomPushButton *ok = new CustomPushButton(tr("Okay, lets start"));
	CustomPushButton *cancel = new CustomPushButton(tr("Maybe later"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(ok);
	butLay->addWidget(cancel);
	butLay->addStretch();
	this->addButtonLayout(butLay);

	connect(ok, SIGNAL(clicked()), this, SLOT(andStart()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));


}

// browse for a music file
void SlideShow::browseForMusic() {

	if(verbose) std::clog << "slb: Browse for music file" << std::endl;

	QString oldPath = QDir::homePath();
	if(musicPath->text() != "" && QFileInfo(musicPath->text()).exists())
		oldPath = musicPath->text();

	QString newFile = QFileDialog::getOpenFileName(0,tr("Select Music File"),oldPath);

	if(newFile != "")
		musicPath->setText(newFile);

}


// Start slideshow
void SlideShow::andStart() {

	makeHide();
	emit startSlideShow();

}

SlideShow::~SlideShow() { }
