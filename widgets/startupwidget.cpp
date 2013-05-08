#include "startupwidget.h"

StartUpWidget::StartUpWidget(QWidget *parent) : QWidget(parent) {

	// Setting the background
	this->setObjectName("startup");
	this->setStyleSheet("QWidget#startup { background:rgba(0,0,0,190); }");

	// A close button to close widget
	CustomPushButton *close = new CustomPushButton(tr("Okay, I got enough now. Lets start!"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->setMargin(8);
	butLay->addStretch();
	butLay->addWidget(close);
	butLay->addStretch();
	QWidget *closeWidget = new QWidget(this);
	closeWidget->setStyleSheet("background: transparent; border-top: 1px solid white");
	closeWidget->setLayout(butLay);
	connect(close, SIGNAL(clicked()), this, SLOT(animate()));

	// the main scroll widget for all content
	scrollbar = new CustomScrollbar;
	QScrollArea *scroll = new QScrollArea;
	QVBoxLayout *lay = new QVBoxLayout(scroll);
	QWidget *scrollWidg = new QWidget(scroll);
	scrollWidg->setLayout(lay);
	scroll->setWidget(scrollWidg);
	scroll->setWidgetResizable(true);
	QVBoxLayout *scrollLay = new QVBoxLayout;
	scrollLay->addWidget(scroll);
	scrollLay->addWidget(closeWidget);
	this->setLayout(scrollLay);
	scroll->setVerticalScrollBar(scrollbar);

	// The animation, and some booleans
	ani = new QPropertyAnimation(this,"geometry");
	isShown = false;

	// The title is set depending on update or fresh install
	title = new QLabel;
	title->setStyleSheet("background: none; color: white; font-size: 20pt; font-weight: bold");
	title->setAlignment(Qt::AlignCenter);
	title->setTextInteractionFlags(Qt::TextSelectableByMouse);

	// The logo
	QLabel *logo = new QLabel;
	QPixmap splash(":/img/splashwithoutversion.png");
	splash = splash.scaledToHeight(100);
	logo->setPixmap(splash);
	logo->setAlignment(Qt::AlignCenter);

	// The introduction is set depending on update or fresh install
	customIntro = new QLabel;
	customIntro->setStyleSheet("background: none; color: white; font-size: 12pt");
	customIntro->setWordWrap(true);
	customIntro->setTextInteractionFlags(Qt::TextSelectableByMouse);

	// the description is the same in both cases
	QLabel *desc = new QLabel;
	desc->setStyleSheet("background: none; color: white; font-size: 12pt");
	desc->setWordWrap(true);
	desc->setTextInteractionFlags(Qt::TextSelectableByMouse);

	QString descTxt = "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Many File Formats") + "</span><br>" + tr("PhotoQt can recognise many image file formats. In fact it can work with any file format your Qt setup can handle. There's a default list, that should work at most setups. However, this list can be adjusted in the settings if needs be (usually there shouldn't!). If you do adjust it, then please don't blame me, if PhotoQt doesn't recognise all image formats you want anymore. If you \"break\" anything (as much as you can break it), usually the best is to re-set the default settings again.") + "<br><br>";
	descTxt += "<div align=\"center\"><img src=\":/img/startupfileformats.png\"></div><bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Many Settings Possible") + "</span><br>" + tr("PhotoQt has an extensive settings area. You can call it by the default shortcut \"e\" or through the dropdown menu at the top edge towards the top right corner. You can adjust (almost) everything, and it's' certainly worth having a look there. Each setting usually comes with a little text explaining what it does and what it can do.") + "<br><bR>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Thumbnails") + "</span><br>" + tr("What would be an image viewer without thumbnails support? It would only be half as good. Whenever you load an image, PhotoQt loads the other images in the directory in the background. It lines them up in a row at the bottom edge (move your mouse there to see them). There are many settings just for the thumbnails, like size, liftup, en-/disabled, type, filename, permanently shown/hidden, etc. PhotoQt's quite flexible with that.") + "<br><br>";
	descTxt += "<div align=\"center\"><img src=\":/img/startupthumbs.png\"></div><bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Shortcuts") + "</span><br>" + tr("One of the many strengths of PhotoQt is the ability to easily set a shortcut for almost anything. Even mouse shortcuts are possible!") + "<br><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Exif Information") + "</span><br>" + tr("Most images store some information about the image in the file. PhotoQt can read and display a good bit of that. You can find the information in the slide-in window at the left edge.") + "<bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Background") + "</span><br>" + tr("The background of PhotoQt is now much more flexible. You can have a (half) transparent background with composite, set a background colour, use a screenshot instead of composite (single monitor setup only) or even set a background image. It's up to you.") + "<br><br>";
	descTxt += "<div align=\"center\"><img src=\":/img/startupbackground.png\"></div><bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Zooming") + "</span><br>" + tr("Of course, zooming is possible as well. PhotoQt supports plain zoom in/out, zoom to fit and also actual size.") + "<br><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Rotating and Flipping") + "</span><br>" + tr("Images can also be rotated and/or flipped. Rotation is possible in 90&deg; angles, and flipping can be done vertically and horizontally.") + "<br><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Basic File Actions") + "</span><br>" + tr("Basic file actions like renaming a file, or moving/copying it or deleting it can be done from inside of PhotoQt. However, you can only operate on a single file at a time. So for bigger operations, it'd be a better solution to use your preferred file manager.") + "<br><br>";
	descTxt += "<div align=\"center\"><img src=\":/img/startupfileaction.png\"></div><bR><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Slideshow") + "</span><br>" + tr("PhotoQt also brings a slideshow feature. When you start a slideshow, it starts at the currently displayed image. There are a couple simply settings like transition and speed, and you can also set a music file that is played in the background. When the slideshow takes longer than the music file, then PhotoQt starts the music file all over from the beginning. At anytime during the slideshow, you can move the mouse cursor to the top edge of the screen to get a little bar, where you can pause/exit the slideshow and adjust the music volume.") + "<br><br>";
	descTxt += "<span style=\"font-size: 15pt; font-weight: bold\">" + tr("Localisation") + "</span><br>" + tr("PhotoQt comes with a number of translations. Many have taken some of their time to create/update one of them. Not all of them are (yet) complete, so why don't you help?") + "<br><br><br>";
	descTxt += tr("There are many many more features. Best is, you just give it a go. Don't forget to check the settings to turn PhotoQt into \"your own\".") + "<br><br><br>" + tr("Enjoy :-)") + "<bR><br><br>";

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
	introTxt += tr("This app is now over one year old, and has developed quite a bit in that time. It will never reach the state \"finished\", because this doesn't exist. This app is a journey, changing and improving constantly. So please forgive me, if a bug sneaked in here or there again, they are never intended, I promise!!") + "<br><br>";
	introTxt += tr("A lot (and I mean a lot) has changed since the last version(s). PhotoQt was pretty much completely re-written and re-structured. So here below you find a short overview of what PhotoQt has to offer. Or you can just get started.") + "</b><br>";

	customIntro->setText(introTxt);

}

void StartUpWidget::setInstallMsg() {

	title->setText(tr("PhotoQt was successfully installed"));

	QString introTxt = "<b>" + tr("Welcome to PhotoQt. PhotoQt is an image viewer, aimed at being fast, highly customisable and good looking.") + "<br><br>";
	introTxt += tr("This app is now over one year old, and has developed quite a bit in that time. It will never reach the state \"finished\", because this doesn't exist. This app is a journey, changing and improving constantly. So please forgive me, if a bug sneaked in here or there again, they are never intended, I promise!!") + "<br><br><br>";
	introTxt += tr("Here below you find a short overview of what PhotoQt has to offer. If you don't want to read it, you can also simply close this window and get started. But for the ones that are a little curious and want to know in advance a little:");


	customIntro->setText(introTxt);



}


void StartUpWidget::makeShow() {

	if(!isShown) animate();

}

void StartUpWidget::makeHide() {

	if(isShown) animate();

}

void StartUpWidget::setRect(QRect rect) {

	rectShown = rect;
	rectHidden = QRect(0,-10,10,10);
	aniStart = QRect(rect.width()/2.0,rect.height()/2.0,1,1);

	if(isShown) this->setGeometry(rectShown);
	else this->setGeometry(rectHidden);

}


// the animation function
void StartUpWidget::animate() {

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(aniStart);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		ani->setDuration(300);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(aniStart);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

		emit finished();

	}

}

// When the animation has finished
void StartUpWidget::aniFinished() {

	if(!isShown) {
		this->setGeometry(rectHidden);
	}


}

void StartUpWidget::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


StartUpWidget::~StartUpWidget() { }
