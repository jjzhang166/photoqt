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

	QString descTxt = "<table>";

	descTxt += "<tr><td style=\"vertical-align: middle\">";
		descTxt += "<img src=\":/img/startupwidget/fileformats.png\">";
	descTxt += "</td><td style=\"vertical-align: middle\">";
		descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Many File Formats") + "</span><br>" + tr("PhotoQt can make use of GraphicsMagick, an image library, to display many different image formats. Currently, there are up to 72 different file formats supported (exact number depends on your system)! You can find a list of it in the settings (Tab 'Other'). There you can en-/disable different ones and also add custom file endings.");
	descTxt += "</td></tr>";

	descTxt += "</table>";
	descTxt += "<table>";

	descTxt += "<tr><td style=\"vertical-align: middle\">";
		descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Make PhotoQt your own") + "</span><br>" + tr("PhotoQt has an extensive settings area. By default you can call it with the shortcut 'e' or through the dropdown menu at the top edge towards the top right corner. You can adjust almost everything in PhotoQt, and it's certainly worth having a look there. Each setting usually comes with a little explanation text.");
	descTxt += "</td><td style=\"vertical-align: middle\">";
		descTxt += "<img src=\":/img/startupwidget/settings.png\">";
	descTxt += "</td></tr>";

	descTxt += "</table>";
	descTxt += "<table>";

	descTxt += "<tr><td style=\"vertical-align: middle\">";
		descTxt += "<img src=\":/img/startupwidget/thumbnails.png\">";
	descTxt += "</td><td style=\"vertical-align: middle\">";
		descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Thumbnails") + "</span><br>" + tr("What would be an image viewer without thumbnails support? It would only be half as good. Whenever you load an image, PhotoQt loads the other images in the directory in the background (by default, it tries to be smart about it and only loads the ones that are needed). It lines them up in a row at the bottom edge (move your mouse there to see them). There are many settings just for the thumbnails, like, e.g., size, liftup, en-/disabled, type, filename, permanently shown/hidden, etc. PhotoQt's quite flexible with that.");
	descTxt += "</td></tr>";

	descTxt += "</table>";
	descTxt += "<table>";

	descTxt += "<tr><td style=\"vertical-align: middle\">";
		descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Shortcuts") + "</span><br>" + tr("One of the many strengths of PhotoQt is the ability to easily set a shortcut for almost anything. Even mouse shortcuts are possible! You can choose from a huge number of internal functions, or you can run any custom script or command.");
	descTxt += "</td><td style=\"vertical-align: middle\">";
		descTxt += "<img src=\":/img/startupwidget/shortcuts.png\">";
	descTxt += "</td></tr>";

	descTxt += "</table>";
	descTxt += "<table>";

	descTxt += "<tr><td style=\"vertical-align: middle\">";
		descTxt += "<img src=\":/img/startupwidget/exif.png\">";
	descTxt += "</td><td style=\"vertical-align: middle\">";
		descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Image Information (Exif)") + "</span><br>" + tr("Most images store some information about the image in the file. PhotoQt can read and display a good bit of that (currently, only Exif is supported). You can find this information in the slide-in window hidden behind the left edge of PhotoQt.");
	descTxt += "</td></tr>";

	descTxt += "</table>";
	descTxt += "<table>";

	descTxt += "<tr><td style=\"vertical-align: middle\">";
		descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Slideshow") + "</span><br>" + tr("PhotoQt also brings a slideshow feature. When you start a slideshow, it starts at the currently displayed image. There are a couple settings like transition, speed, loop, and shuffle. Plus, you can set a music file that is played in the background. When the slideshow takes longer than the music file, then PhotoQt starts the music file all over from the beginning. At anytime during the slideshow, you can move the mouse cursor to the top edge of the screen to get a little bar, where you can pause/exit the slideshow and adjust the music volume.");
	descTxt += "</td><td style=\"vertical-align: middle\">";
		descTxt += "<img src=\":/img/startupwidget/slideshow.png\">";
	descTxt += "</td></tr>";

	descTxt += "</table>";
	descTxt += "<table>";

	descTxt += "<tr><td style=\"vertical-align: middle\">";
		descTxt += "<img src=\":/img/startupwidget/localisation.png\">";
	descTxt += "</td><td style=\"vertical-align: middle\">";
		descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Localisation") + "</span><br>" + tr("PhotoQt comes with a number of translations. Many have taken some of their time to create/update one of them (Thank you!). Not all of them are complete... do you want to help?");
	descTxt += "</td></tr>";

	descTxt += "</table><br><br><br>";

	descTxt += tr("There are many many more features. Best is, you just give it a go. Don't forget to check out the settings to make PhotoQt YOUR image viewer.") + "<br><br><br>" + tr("Enjoy :-)") + "<br><br>";

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

	QString introTxt = "<b>" + tr("Welcome back to PhotoQt. It hasn't been that long since the last release of PhotoQt. Yet there have been more changes than ever before. Most (if not all) of them were aimed at smoothing PhotoQt out. I tried to implement all the feedback and feature requests I received over the past year(s) that has been left without much attention until now.") + "<br><br>";
	introTxt += tr("I'm convinced, that this release is better than any one before. PhotoQt is working reliable and efficient in every aspect. It definitely can hold up to the more 'traditional' image viewers out there.") + "<br><br>";
	introTxt += tr("Below you find a short overview over the features of PhotoQt. But feel free to skip it and just get started.") + "</b><br>";

	customIntro->setText(introTxt);

}

void StartUpWidget::setInstallMsg() {

	title->setText(tr("PhotoQt was successfully installed"));

	QString introTxt = "<b>" + tr("Welcome to PhotoQt. PhotoQt is an image viewer, aimed at being fast and reliable, highly customisable and good looking.") + "<br><br>";
	introTxt += tr("This app started out more than three years ago, and it has developed quite a bit since then. It has become very efficient, reliable, and highly flexible (check out the settings). I'm convinced it can hold up to the more 'traditional' image viewers out there in every way.") + "<br><br>";
	introTxt += tr("Here below you find a short overview of what PhotoQt has to offer, but feel free to skip it and just get started.");


	customIntro->setText(introTxt);

}

StartUpWidget::~StartUpWidget() { }
