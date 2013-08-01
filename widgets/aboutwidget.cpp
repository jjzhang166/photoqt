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

About::About(QWidget *parent) : QWidget(parent) {

	// The different QRects
	rectShown = QRect();
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect();

	// The current geometry and position
	isShown = false;
	this->setGeometry(rectHidden);

	// Some variables
	fadeBack = new QTimeLine;
	fadeBack->setLoopCount(5);
	backAlphaShow = 130;
	backAlphaCur = 0;
	fadeBackIN = true;
	connect(fadeBack, SIGNAL(valueChanged(qreal)), this, SLOT(fadeStep()));

	// The current widget look
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaShow));

	// the central widget containing all the information
	center = new QWidget(this);
	center->setObjectName("center");
	center->setStyleSheet("QWidget#center { background: rgba(0,0,0,200); border-radius: 10px; font-size: 12pt; }");

	// The current animation framework
	ani = new QPropertyAnimation(center,"geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	// Create and set the scrollarea with main layout
	QVBoxLayout *central = new QVBoxLayout;
	central->setMargin(10);
	QScrollArea *scroll = new QScrollArea(this);
	scroll->setObjectName("scrollWidget");
	scroll->setStyleSheet("QWidget#scrollWidget { background: transparent;border-bottom: 1px solid white; padding-bottom: 3px; border-radius: 0px; }");
	QWidget *scrollWidget = new QWidget(scroll->widget());
	scrollWidget->setStyleSheet("background: transparent;");
	scroll->setWidgetResizable(true);
	scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	scrollWidget->setLayout(central);
	scroll->setWidget(scrollWidget);
	QVBoxLayout *scCentral = new QVBoxLayout;
	center->setLayout(scCentral);
	scCentral->addWidget(scroll);

	CustomScrollbar *scrollbar = new CustomScrollbar;
	scroll->setVerticalScrollBar(scrollbar);

	// The string right at the top, giving the license and author information
	license = new QLabel;
	license->setWordWrap(true);
	license->setStyleSheet("color: white");
	central->addWidget(license);

	// The logo (Thanks to Archie Arevalo)
	QLabel *logo = new QLabel;
	logo->setPixmap(QPixmap(":/img/logo.png"));
	QHBoxLayout *logoLay = new QHBoxLayout;
	logo->setStyleSheet("background: transparent");
	logoLay->addStretch();
	logoLay->addWidget(logo);
	logoLay->addStretch();
	central->addSpacing(20);
	central->addLayout(logoLay);
	central->addSpacing(30);

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
	txt3 += tr("You want to join the team and do something, e.g. translating PhotoQt to another language? Drop me and email (Lukas@photoqt.org), and for translations, check the project page on Transifex: http://transifex.com/p/photoqt.") + "<br><br><br>";

	CustomLabel *text = new CustomLabel(txt);
	CustomLabel *text2 = new CustomLabel(txt2);
	CustomLabel *text3 = new CustomLabel(txt3);
	text->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text->setFontSize("11pt");
	text->setWordWrap(true);
	text2->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text2->setCursor(Qt::PointingHandCursor);
	text2->setFontSize("11pt");
	text2->setWordWrap(true);
	text3->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text3->setFontSize("11pt");
	text3->setWordWrap(true);
	central->addWidget(text);
	central->addSpacing(10);
	central->addWidget(text2);
	central->addSpacing(10);
	central->addWidget(text3);
	central->addStretch();
	connect(text2, SIGNAL(clicked()), this, SLOT(openWebsite()));

	// A close button at the bottom
	//: This string is written onto the "close" button of the about widget
	close = new CustomPushButton(tr("Okay, I got enough of that..."));
	QHBoxLayout *closeLay = new QHBoxLayout;
	closeLay->addStretch();
	closeLay->addWidget(close);
	closeLay->addStretch();
	closeLay->setMargin(10);
	scCentral->addLayout(closeLay);
	connect(close, SIGNAL(clicked()), this, SLOT(animate()));





}

// Set the license with version (called from mainwindow.cpp)
void About::setLicense(QString version) {

	license->setText("<div style=\"font-size: 8pt\">PhotoQt " + version + ", Lukas Spies, 2013 (Lukas@photoqt.org)" + " &ndash; " + tr("website:") + " photoqt.org &ndash; " + tr("Licensed under GPLv2 or later, without any guarantee") + "</div>");

}

void About::makeHide() {

	if(isShown) animate();

}

void About::makeShow() {

	if(!isShown) animate();

}

void About::setRect(QRect rect) {

	rectShown = rect;
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect(rect.width()/2.0,rect.height()/2.0,1,1);

	if(isShown) {
		this->setGeometry(rectShown);
		center->setGeometry(QRect(100,50,rectShown.width()-200,rectShown.height()-100));
	} else
		this->setGeometry(rectHidden);

}


// The animation function
void About::animate() {

	QRect shown = QRect(100,50,rectShown.width()-200,rectShown.height()-100);

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		// The background is initially transparent but the geometry is full
		this->setStyleSheet(QString("background: rgba(0,0,0,0);"));
		this->setGeometry(rectShown);

		// Widget is shown
		isShown = true;

		// Animate widget
		ani->setDuration(400);
		ani->setStartValue(rectAni);
		ani->setEndValue(shown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		// Fade background in
		fadeBack->setDuration(200);
		fadeBack->setLoopCount(5);
		fadeBackIN = true;
		fadeBack->start();

		// Block all base functions
		emit blockFunc(true);

		// Make sure this widget is on top
		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		// Fade background out
		fadeBack->setDuration(100);
		fadeBack->setLoopCount(5);
		fadeBackIN = false;
		fadeBack->start();

		// Widget is hidden again
		isShown = false;

		// Start animation out
		ani->setDuration(300);
		ani->setStartValue(shown);
		ani->setEndValue(rectAni);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

		// Unblock all base functions
		emit blockFunc(false);

	}

}


// Every fade step for the background
void About::fadeStep() {

	// Fade in
	if(fadeBackIN) {
		backAlphaCur += backAlphaShow/5;
		if(backAlphaCur > backAlphaShow)
			backAlphaCur = backAlphaShow;
	// Fade out
	} else {
		backAlphaCur -= backAlphaShow/5;
		if(backAlphaCur < 0)
			backAlphaCur = 0;
	}

	// Update stylesheet
	this->setStyleSheet(QString("background: rgba(0,0,0,%1);").arg(backAlphaCur));

}

// Handle widget when animation is finished
void About::aniFinished() {

	// Move widget out of screen
	if(!isShown)
		this->setGeometry(rectHidden);


}

void About::openWebsite() {
	if(!QDesktopServices::openUrl(QUrl("http://photoqt.org")))
		std::cerr << "ERROR: Couldn't open website..." << std::endl;
}

// Click on background closes dialog
void About::mouseReleaseEvent(QMouseEvent *e) {

	if(!center->geometry().contains(e->pos()))
		close->animateClick();

}


void About::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
