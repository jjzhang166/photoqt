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

#include "aboutwidget.h"
#include <iostream>

About::About(QWidget *parent) : MyWidget(parent) {

	this->setBorderArea(100,50);

	QVBoxLayout *lay = new QVBoxLayout;

	// The string right at the top, giving the license and author information
	license = new CustomLabel;
	license->setWordWrap(true);
	lay->addWidget(license);

	// The logo (Thanks to Archie Arevalo)
	QLabel *logo = new QLabel;
	logo->setPixmap(QPixmap(":/img/logo.png"));
	QHBoxLayout *logoLay = new QHBoxLayout;
	logo->setStyleSheet("background: transparent");
	logoLay->addStretch();
	logoLay->addWidget(logo);
	logoLay->addStretch();
	lay->addSpacing(20);
	lay->addLayout(logoLay);
	lay->addSpacing(30);

	// The main text block
	QString txt = tr("PhotoQt is a simple image viewer, designed to be good looking, highly configurable, yet easy to use and fast.") +  "<br><br>";
	txt += tr("Up until version 0.9.1, PhotoQt used to be called simply 'Photo'. But Photo isn't quite a unique and creative name (born simply out of the need to give it some name). Searching the web for it was close to impossible. Whether PhotoQt is a much more creative name than Photo is debatable, but it certainly is a little more unique. Simply searching the web for 'photoqt' results in some (more or less) relevant websites amongst the top results. On top of it, quite often the name of this application was thought to already be PhotoQt (or Photo-Qt), a confusion probably mainly caused by its website being photoqt.org.") + "<br>";
	txt += tr("Well, long story short, starting version 1.0, its name is PhotoQt.") + "<br><br>";
	txt += tr("I'm not a trained programmer. I'm a simple Maths student that loves doing stuff like this. All I know about programming I taught myself over the years. So PhotoQt might not be done in the best of ways, several things could probably be improved (from a programming point of view). But it's as good as I can do it, and I'm constantly learning and improving things.") + "<br><br>";
	txt += tr("I heard a number of times people saying, that PhotoQt is a \"copy\" of Picasa's image viewer. Well, it's not. In fact, I myself have never used Picasa. I have seen it in use though by others, and I can't deny that it influenced the basic design idea a little. But I'm not trying to do something \"like Picasa\". I try to do my own thing, and to do it as good as I can. If some consider it to be a replacement for Picasa, well, I feel a little honoured (since Picasa is created by \"proper\" programmers).");
	QString txt2 = tr("Don't forget to check out the website:") + " http://PhotoQt.org";
	QString txt3 = tr("If there's one thing I want to achieve with PhotoQt, then it is, that you find it useful. If I accomplished that, then I'm glad. There's no other reason I'm doing that. I don't want money or anything, but just be helpful.") + "<br><br><br><br>";
	txt3 += tr("If you find a bug or if you have a question or suggestion, tell me. I'm open to any feedback I get :)") + "<br><br>";
	txt3 += "<center><h2>" + tr("Thanks to everybody who<br>contributed to PhotoQt<br>and/or translated PhotoQt to another language!<br><br>You guys rock!! :-)") + "</h2></center><br><br>";
	txt3 += tr("You want to join the team and do something, e.g. translating PhotoQt to another language? Drop me and email (Lukas@photoqt.org), and for translations, check the project page on Transifex: http://transifex.com/p/photo.") + "<br><br><br>";

	CustomLabel *text = new CustomLabel(txt);
	CustomLabel *text2 = new CustomLabel(txt2);
	CustomLabel *text3 = new CustomLabel(txt3);
	text->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text->setFontSize(11);
	text->setWordWrap(true);
	text2->setToolTip("http://photoqt.org");
	text2->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text2->setCursor(Qt::PointingHandCursor);
	text2->setFontSize(11);
	text2->setWordWrap(true);
	text3->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text3->setFontSize(11);
	text3->setWordWrap(true);
	lay->addWidget(text);
	lay->addSpacing(10);
	lay->addWidget(text2);
	lay->addSpacing(10);
	lay->addWidget(text3);
	lay->addStretch();
	connect(text2, SIGNAL(clicked()), this, SLOT(openWebsite()));

	this->setWidgetLayout(lay);

	// A close button at the bottom
	//: This string is written onto the "close" button of the about widget
	close = new CustomPushButton(tr("Okay, I got enough of that..."));
	QHBoxLayout *closeLay = new QHBoxLayout;
	closeLay->addStretch();
	closeLay->addWidget(close);
	closeLay->addStretch();
	closeLay->setMargin(10);
	CustomLine *line = new CustomLine;
	QVBoxLayout *botLay = new QVBoxLayout;
	botLay->addWidget(line);
	botLay->addLayout(closeLay);
	this->addButtonLayout(botLay);
	connect(close, SIGNAL(clicked()), this, SLOT(animate()));

}

// Set the license with version (called from mainwindow.cpp)
void About::setLicense(QString version) {

	license->setText("<div style=\"font-size: 8pt\">PhotoQt " + version + ", Lukas Spies, 2014 (Lukas@photoqt.org)" + " &ndash; " + tr("website:") + " photoqt.org &ndash; " + tr("Licensed under GPLv2 or later, without any guarantee") + "</div>");

}

void About::openWebsite() {
	if(!QDesktopServices::openUrl(QUrl("http://photoqt.org")))
		std::cerr << "ERROR: Couldn't open website..." << std::endl;
}
