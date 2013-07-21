#include "customconfirm.h"

CustomConfirm::CustomConfirm(QString title, QString text, QString yesText, QString noText, QSize size, QWidget *parent) : QWidget(parent) {

	this->setObjectName("back");
	this->setStyleSheet("#back { background: rgba(0,0,0,0) }");

	// The geometries for the widget
	rectShown = QRect();
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect();

	// The central widget containing all the contents
	center = new QWidget(this);
	center->setObjectName("center");
	center->setStyleSheet("#center { background: rgba(0,0,0,200); border-radius: 20px; }");

	// The animation instances
	ani = new QPropertyAnimation(center, "geometry");
	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));
	isShown = false;
	this->setGeometry(rectHidden);

	// The fading instances
	fadeBack = new QTimeLine;
	fadeBack->setLoopCount(5);
	backAlphaShow = 130;
	backAlphaCur = 0;
	fadeBackIN = true;
	connect(fadeBack, SIGNAL(valueChanged(qreal)), this, SLOT(fadeStep()));

	// Save the user set size for later use
	widgetSize = size;

	// The main layout
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addStretch();
	lay->addSpacing(10);

	// The title of the widget with user set text
	QLabel *t = new QLabel(title);
	t->setObjectName("title");
	t->setWordWrap(true);
	t->setStyleSheet("#title { color: white; background: transparent; font-size: 15pt; font-weight: bold; }");
	t->setAlignment(Qt::AlignCenter);
	lay->addWidget(t);
	lay->addSpacing(12);

	// And the description
	QTextEdit *desc = new QTextEdit(text);
	desc->setObjectName("desc");
	desc->setAlignment(Qt::AlignCenter);
	desc->setReadOnly(true);
	desc->setTextInteractionFlags(Qt::NoTextInteraction);
	desc->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
	desc->viewport()->setCursor(Qt::ArrowCursor);
	desc->setCursor(Qt::ArrowCursor);
	desc->setStyleSheet("#desc {color: white; background: transparent; line-height: 2pt; }");
	lay->addWidget(desc);
	lay->addSpacing(7);

	// This box is optional and can be enabled if needed
	dontShowAgain = new CustomCheckBox(tr("Don't ask me again..."));
	dontShowAgain->hide();
	QHBoxLayout *dontLay = new QHBoxLayout;
	dontLay->addStretch();
	dontLay->addWidget(dontShowAgain);
	dontLay->addStretch();
	lay->addLayout(dontLay);
	lay->addSpacing(7);

	// The yes and no buttons (an empty string hides a button)
	yes = new CustomPushButton(yesText,this);
	no = new CustomPushButton(noText, this);
	connect(yes, SIGNAL(clicked()), this, SLOT(animate()));
	connect(no, SIGNAL(clicked()), this, SLOT(animate()));
	connect(yes, SIGNAL(clicked()), this, SIGNAL(confirmed()));
	connect(no, SIGNAL(clicked()), this, SIGNAL(rejected()));
	QHBoxLayout *yesNoLay = new QHBoxLayout;
	yesNoLay->addStretch();
	if(yesText != "")
		yesNoLay->addWidget(yes);
	else
		yes->hide();
	if(noText != "")
		yesNoLay->addWidget(no);
	else
		no->hide();
	yesNoLay->addStretch();
	lay->addLayout(yesNoLay);



	lay->addSpacing(10);
	lay->addStretch();
	center->setLayout(lay);

}

void CustomConfirm::setRect(QRect rect) {

	rectShown = rect;
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect(rect.width()/2.0,rect.height()/2.0,1,1);

	if(isShown) this->setGeometry(rectShown);
	else this->setGeometry(rectHidden);

}

// The animation function
void CustomConfirm::animate() {

	QRect shown = QRect((rectShown.width()-widgetSize.width())/2,(rectShown.height()-widgetSize.height())/2,widgetSize.width(),widgetSize.height());

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		// The background is initially transparent but the geometry is full
		this->setStyleSheet(QString("#back { background: rgba(0,0,0,0); }"));
		this->setGeometry(rectShown);

		// Widget is shown
		isShown = true;

		emit blockFunc(true);

		// Animate widget
		ani->setDuration(200);
		ani->setStartValue(rectAni);
		ani->setEndValue(shown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		// Fade background in
		fadeBack->setDuration(200);
		fadeBack->setLoopCount(5);
		fadeBackIN = true;
		fadeBack->start();

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

		emit blockFunc(false);

		// Start animation out
		ani->setDuration(300);
		ani->setStartValue(shown);
		ani->setEndValue(rectAni);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

	}
}

// After animation finished
void CustomConfirm::aniFinished() {

	// We need to set rectHidden, otherwise the widget will be invisible (100% opacity), but still there, i.e. it blocks the whole gui
	if(!isShown)
		this->setGeometry(rectHidden);

}

// Each step the fading timer calls this function
void CustomConfirm::fadeStep() {

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
	this->setStyleSheet(QString("#back { background: rgba(0,0,0,%1); }").arg(backAlphaCur));

}

// Show a border around the central widget
void CustomConfirm::showBorder(QString color, int width) {

	center->setStyleSheet(QString("#center { background: rgba(0,0,0,200); border-radius: 20px; border: %1px solid %2; }").arg(width).arg(color));

}

// Enable the don't show again checkbox
void CustomConfirm::setDontShowAgain() {

	dontShowAgain->show();

}

void CustomConfirm::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


CustomConfirm::~CustomConfirm() { }
