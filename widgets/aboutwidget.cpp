#include "aboutwidget.h"

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
	center->setStyleSheet("background: rgba(0,0,0,200); border-radius: 10px; font-size: 12pt;");

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
	logo->setPixmap(QPixmap(":/img/splash.png"));
	QHBoxLayout *logoLay = new QHBoxLayout;
	logo->setStyleSheet("background: transparent");
	logoLay->addStretch();
	logoLay->addWidget(logo);
	logoLay->addStretch();
	central->addSpacing(20);
	central->addLayout(logoLay);
	central->addSpacing(30);

	// The main text block
	QString txt = tr("Photo is a simple image viewer, designed to be good looking, highly configurable, yet easy to use and fast.") +  "<br><br>";
	txt += tr("\"Photo\" is not a very creative name for an image viewer. There are so many better names possible. So why is Photo called \"Photo\"? Well, as I started out programming what should become version 0.1, I never really thought of actually developing it as far as it is now. I actually wasn't sure if I should upload it somewhere at all. I eventually decided to give it a go, and I uploaded it at qt-apps.org. And, well, of course, if you want to upload a project there (as probably everywhere else too), you need to give it a name. And the first thing that came to my mind was \"photo\", just to satisfy the upload form of qt-apps.org...") + "<br><br>";
	txt += tr("I'm not a trained programmer. I'm a simple Maths student that loves doing stuff like this. All I know about programming I taught myself over the years. So Photo is certainly not done in the best of ways. Many things could probably be improved (from a programming point of view). But it's as good as I can do it. And I'm constantly learning and improving things. So you sometimes have to be a little patient with me :)") + "<br><br>";
	txt += tr("I heard a number of times people saying, that Photo is a \"copy\" of Picasa's image viewer. Well, it's not. In fact, I myself have never used Picasa. I have seen it in use though by others, and I can't deny that it influenced the basic design idea a little. But I'm not trying to do something \"like Picasa\". I try to do my own thing, and to do it as good as I can. If some consider it to be a replacement for Picasa, well, I feel a little honoured (since Picasa is created by \"proper\" programmers).") + "<br><bR>";
	txt += tr("Don't forget to check out the website:") + " http://PhotoQt.org<br><br><br>";
	txt += tr("If there's one thing I want to achieve with Photo, then it is, that you find it useful. If I accomplished that, then I'm glad. There's no other reason I'm doing that. I don't want money or anything, but just be helpful.") + "<br><br><br><br>";
	txt += tr("If you find a bug or if you have a question or suggestion, tell me. I'm open to any feedback I get :)") + "<br><br>";
	txt += "<center><h2>" + tr("Thanks to everybody who<br>contributed to Photo<br>and/or translated Photo to another language!<br><br>You guys rock!! :-)") + "</h2></center><br><br>";
	txt += tr("You want to join the team and do something, e.g. translating Photo to another language? Drop me and email (Lukas@photoqt.org), and for translations, check the project page on Transifex: http://transifex.com/p/photo.") + "<br><br><br>";

	QLabel *text = new QLabel(txt);
	text->setTextInteractionFlags(Qt::TextSelectableByMouse);
	text->setStyleSheet("color: white");
	text->setWordWrap(true);
	central->addWidget(text);
	central->addStretch();

	// A close button at the bottom
	//: This string is written onto the "close" button of the about widget
	close = new CustomPushButton(tr("Okay, I got enough of that..."));
	close->removeRoundedCorners();
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

	license->setText("<div style=\"font-size: 8pt\">" + tr("Photo %1, by Lukas Spies, 2013 (Lukas@photoqt.org)").arg(version) + " &ndash; " + tr("website:") + " photoqt.org &ndash; " + tr("Licensed under GPL, without any guarantee") + "</div>");

}

// The animation function
void About::animate() {

	QRect shown = QRect(100,50,rectShown.width()-200,rectShown.height()-100);

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

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
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

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
