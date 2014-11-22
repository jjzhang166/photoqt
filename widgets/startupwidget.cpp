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

#include "startupwidget.h"

StartUpWidget::StartUpWidget(QWidget *parent) : MyWidget(parent) {

	this->setFullscreen(true);

	// A close button to close widget
	CustomPushButton *close = new CustomPushButton(tr("Okay, I got enough now. Lets start!"));
	close->setFontSize("12pt");
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->setMargin(8);
	butLay->addStretch();
	butLay->addWidget(close);
	butLay->addStretch();
	connect(close, SIGNAL(clicked()), this, SLOT(animate()));

	// Line above buttons
	CustomLine *line = new CustomLine;

	// Create and set the main layout
	QVBoxLayout *lay = new QVBoxLayout;
	this->setWidgetLayout(lay);
	this->addWidgetAtBottom(line);
	this->addButtonLayout(butLay);

	// The title is set depending on update or fresh install
	title = new CustomLabel;
	title->setFontSize(20);
	title->setBold(true);
	title->setAlignment(Qt::AlignCenter);
	title->setTextInteractionFlags(Qt::TextSelectableByMouse);

	// The logo
	CustomLabel *logo = new CustomLabel;
	QPixmap splash(":/img/logo.png");
	logo->setPixmap(splash);
	logo->setAlignment(Qt::AlignCenter);

	// The introduction is set depending on update or fresh install
	customIntro = new CustomLabel;
	customIntro->setFontSize(12);
	customIntro->setWordWrap(true);
	customIntro->setTextInteractionFlags(Qt::TextSelectableByMouse);

	// the description is the same in both cases
	CustomLabel *desc = new CustomLabel;
	desc->setFontSize(12);
	desc->setWordWrap(true);
	desc->setTextInteractionFlags(Qt::TextSelectableByMouse);

	QString descTxt = "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Many File Formats") + "</span><br>" + tr("PhotoQt used to be able to only work with those file formats natively supported by Qt (which is already pretty good). But it now can also make use of GraphicsMagick, an image library. Currently, there are more than 80 different file formats supported! You can find a list of it in the settings (Tab \"Other\"). There you can en-/disable different ones and also add custom file endings.") + "<br><br>";
	descTxt += "<div align=\"center\"><img src=\":/img/startupfileformats.png\"></div><bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Many Settings Possible") + "</span><br>" + tr("PhotoQt has an extensive settings area. By default you can call it with the shortcut \"e\" or through the dropdown menu at the top edge towards the top right corner. You can adjust (almost) everything in PhotoQt, and it's certainly worth having a look there. Each setting usually comes with a little explanation text.") + "<br><bR>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Thumbnails") + "</span><br>" + tr("What would be an image viewer without thumbnails support? It would only be half as good. Whenever you load an image, PhotoQt loads the other images in the directory in the background (per default it only loads the ones whose thumbnails are currently visible). It lines them up in a row at the bottom edge (move your mouse there to see them). There are many settings just for the thumbnails, like size, liftup, en-/disabled, type, filename, permanently shown/hidden, etc. PhotoQt's quite flexible with that.") + "<br><br>";
	descTxt += "<div align=\"center\"><img src=\":/img/startupthumbs.png\"></div><bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Shortcuts") + "</span><br>" + tr("One of the many strengths of PhotoQt is the ability to easily set a shortcut for almost anything. Even mouse shortcuts are possible!") + "<br><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Image Information") + "</span><br>" + tr("Most images store some information about the image in the file. PhotoQt can read and display a good bit of that. You can find this information in the slide-in window at the left edge of PhotoQt.") + "<bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Basic File Actions") + "</span><br>" + tr("Basic file actions like renaming a file, or moving/copying it or deleting it can be done from inside of PhotoQt. However, you can only operate on a single file at a time. So for bigger operations, it'd be a better solution to use your choice of file manager.") + "<br><br>";
	descTxt += "<div align=\"center\"><img src=\":/img/startupfileaction.png\"></div><bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Wallpaper") + "</span><br>" + tr("For different desktop environments (like e.g. XFCE4, Openbox, Gnome, ...) you can set an image your viewing directly as wallpaper. Depending on the DE there are also different options available. Unfortunately, KDE is currently not supported.") + "<br><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Slideshow") + "</span><br>" + tr("PhotoQt also brings a slideshow feature. When you start a slideshow, it starts at the currently displayed image. There are a couple simply settings like transition and speed, and you can also set a music file that is played in the background. When the slideshow takes longer than the music file, then PhotoQt starts the music file all over from the beginning. At anytime during the slideshow, you can move the mouse cursor to the top edge of the screen to get a little bar, where you can pause/exit the slideshow and adjust the music volume.") + "<br><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Localisation") + "</span><br>" + tr("PhotoQt comes with a number of translations. Many have taken some of their time to create/update one of them. Not all of them are (yet) complete, so why don't you help?") + "<br><br><br>";
	descTxt += tr("There are many many more features. Best is, you just give it a go. Don't forget to check the settings to make PhotoQt \"your own\".") + "<br><br><br>" + tr("Enjoy :-)") + "<bR><br><br>";

	desc->setText(descTxt);

	lay->addSpacing(40);
	lay->addWidget(logo);
	lay->addSpacing(30);
	lay->addWidget(title);
	lay->addSpacing(20);
	lay->addWidget(customIntro);
	lay->addSpacing(10);
	lay->addWidget(desc);
	lay->addStretch();

}


void StartUpWidget::setUpdateMsg() {

	title->setText(tr("PhotoQt was successfully updated"));

	QString introTxt = "<b>" + tr("Welcome back to PhotoQt.") + "<br><br>";
	introTxt += tr("It has been more than a year since the last stable release of PhotoQt was released. For a while development has stalled as I was busy finishing my Bachelor degree, but I came back and once again there are some really cool (I believe) changes in this release. I gave my best to ensure that everything is working as smoothly as possible. But please forgive me if a bug sneaked in here or there again, they are never intended, I promise!") + "<br><br>";
	introTxt += tr("Below you find a short overview over the features of PhotoQt. But feel free to skip it and just get started.") + "</b><br>";

	customIntro->setText(introTxt);

}

void StartUpWidget::setInstallMsg() {

	title->setText(tr("PhotoQt was successfully installed"));

	QString introTxt = "<b>" + tr("Welcome to PhotoQt. PhotoQt is an image viewer, aimed at being fast, highly customisable and good looking.") + "<br><br>";
	introTxt += tr("This app started out almost three years ago, and it has developed quite a bit since then. Although I gave my best to ensure that everything is working as smoothly as possible, please forgive me if a bug sneaked in here or there again, they are never intended, I promise!") + "<br><br><br>";
	introTxt += tr("Here below you find a short overview of what PhotoQt has to offer, but feel free to skip it and just get started.");


	customIntro->setText(introTxt);

}

StartUpWidget::~StartUpWidget() { }
