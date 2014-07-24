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

#include "filterimagessetup.h"

FilterImagesSetup::FilterImagesSetup(QWidget *parent) : QWidget(parent) {

	this->setObjectName("back");
	this->setStyleSheet("#back { background: rgba(0,0,0,0) }");

	// The geometries for the widget
	rectShown = QRect();
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect();

	// The central widget containing all the contents
	center = new QWidget(this);
	center->setObjectName("center");
	center->setStyleSheet("#center { background: rgba(0,0,0,200); border-radius: 20px; border: 2px solid rgb(130,130,130); }");

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

	// Title
	QLabel *title = new QLabel(tr("Filter Images in Current Directory"));
	title->setAlignment(Qt::AlignCenter);
	title->setStyleSheet("background: transparent; color: white; font-size: 15pt");
	title->setWordWrap(true);

	// A short description
	QLabel *desc = new QLabel(tr("Enter here the <b>extension</b> of the files you want to show. Seperate multiple extensions by a space."));
	desc->setAlignment(Qt::AlignCenter);
	desc->setStyleSheet("background: transparent; color: white");
	desc->setWordWrap(true);

	// LineEdit to enter extensions
	edit = new CustomLineEdit;

	// Different buttons
	enter = new CustomPushButton(tr("Filter"));
	enter->setEnabled(false);	// only enabled if text entered
	cancel = new CustomPushButton(tr("Cancel"));
	CustomPushButton *remove = new CustomPushButton(tr("Remove Filter"));
	remove->setFontSize("7pt");

	// Connect some buttons
	connect(enter, SIGNAL(clicked()), this, SLOT(okayClicked()));
	connect(edit, SIGNAL(returnPressed()), enter, SIGNAL(clicked()));
	connect(edit, SIGNAL(textEdited(QString)), this, SLOT(editTextChanged()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));
	connect(remove, SIGNAL(clicked()), this, SIGNAL(removeFilter()));
	connect(remove, SIGNAL(clicked()), cancel, SLOT(animateClick()));

	// Add buttons to layout
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(enter);
	butLay->addWidget(cancel);
	butLay->addSpacing(20);
	butLay->addWidget(remove);
	butLay->addStretch();

	// main layout
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget(title);
	lay->addWidget(desc);
	lay->addWidget(edit);
	lay->addLayout(butLay);

	// Set layout to central widget
	center->setLayout(lay);


}

// Disable enter button if nothing entered (no filter possible)
void FilterImagesSetup::editTextChanged() {

	enter->setEnabled(edit->text().trimmed().length() > 0);

}

// Set filter and reload directory
void FilterImagesSetup::okayClicked() {

	emit setFilter(edit->text().toLower().split(" ",QString::SkipEmptyParts));
	animate();

}

// Set rect's
void FilterImagesSetup::setRect(QRect rect) {

	rectShown = rect;
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect(rect.width()/2.0,rect.height()/2.0,1,1);

	if(isShown) this->setGeometry(rectShown);
	else this->setGeometry(rectHidden);

}

void FilterImagesSetup::makeHide() {
	if(isShown) animate();
}

void FilterImagesSetup::makeShow() {
	if(!isShown) animate();
}

// The animation function
void FilterImagesSetup::animate() {

	QRect shown = QRect((rectShown.width()/2.0)-200, (rectShown.height()/2.0)-80,400,160);

	// Open widget
	if(!isShown) {

		edit->setEnabled(true);

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
void FilterImagesSetup::aniFinished() {

	// We need to set rectHidden, otherwise the widget will be invisible (100% opacity), but still there, i.e. it blocks the whole gui
	if(!isShown) {
		edit->setEnabled(false);
		this->setGeometry(rectHidden);
	} else {
		edit->setFocus();
		edit->selectAll();
	}

}

// Each step the fading timer calls this function
void FilterImagesSetup::fadeStep() {

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

// Hide window on click on background
void FilterImagesSetup::mouseReleaseEvent(QMouseEvent *e) {

	if(!center->geometry().contains(e->pos())) cancel->animateClick();

}

void FilterImagesSetup::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


FilterImagesSetup::~FilterImagesSetup() { }
