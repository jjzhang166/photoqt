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

	// The new logo
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
	txt += tr("With PhotoQt I try to be different than other image viewers (after all, there are plenty of good image viewers already out there). Its interface is kept very simple, yet there is an abundance of settings to customize the look and feel to make PhotoQt YOUR image viewer.") + "<br><br>";
	txt += tr("I'm not a trained programmer. I'm a simple Maths student that loves doing stuff like this. Most of my programming knowledge I taught myself over the past 10-ish years, and it has been developing a lot since I started PhotoQt. During my studies in university I learned a lot about the basics of programming that I was missing. And simply working on PhotoQt gave me a lot of invaluable experience. So the code of PhotoQt might in places not quite be done in the best of ways, but I think it's getting better and better with each release.") + "<br><br>";
	txt += tr("I heard a number of times people saying, that PhotoQt is a 'copy' of Picasa's image viewer. Well, it's not. In fact, I myself have never used Picasa. I have seen it in use though by others, and I can't deny that it influenced the basic design idea a little. But I'm not trying to do something 'like Picasa'. I try to do my own thing, and to do it as good as I can.");
	QString txt2 = tr("Don't forget to check out the website:") + " http://PhotoQt.org";
	QString txt3 = tr("If you find a bug or if you have a question or suggestion, tell me. I'm open to any feedback I get :)");
	QString txt4 = "<center><h2>" + tr("Thanks to everybody who contributed to PhotoQt and/or translated PhotoQt to another language! You guys rock!") + "</h2></center>";
	QString txt5 = tr("You want to join the team and do something, e.g. translating PhotoQt to another language? Drop me and email (Lukas@photoqt.org), and for translations, check the project page on Transifex: http://transifex.com/p/photo.");

	CustomLabel *text = new CustomLabel(txt);
	CustomLabel *text2 = new CustomLabel(txt2);
	CustomLabel *text3 = new CustomLabel(txt3);
	CustomLabel *text4 = new CustomLabel(txt4);
	CustomLabel *text5 = new CustomLabel(txt5);
	text->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text->setFontSize(11);
	text->setWordWrap(true);
	text2->setToolTip(tr("Click to go to:") + " http://photoqt.org");
	text2->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text2->setCursor(Qt::PointingHandCursor);
	text2->setFontSize(11);
	text2->setWordWrap(true);
	text3->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text3->setFontSize(11);
	text3->setWordWrap(true);
	text4->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text4->setFontSize(11);
	text4->setWordWrap(true);
	text4->setFixedWidth(600);
	QHBoxLayout *text4Lay = new QHBoxLayout;
	text4Lay->addStretch();
	text4Lay->addWidget(text4);
	text4Lay->addStretch();
	text5->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text5->setFontSize(11);
	text5->setWordWrap(true);
	lay->addWidget(text);
	lay->addSpacing(10);
	lay->addWidget(text2);
	lay->addSpacing(10);
	lay->addWidget(text3);
	lay->addSpacing(20);
	lay->addLayout(text4Lay);
	lay->addSpacing(20);
	lay->addWidget(text5);
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
