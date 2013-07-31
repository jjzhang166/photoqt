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

#include "settingstabshortcutschangecommand.h"

ShortcutChangeCommand::ShortcutChangeCommand(QWidget *parent) : QWidget(parent) {

	this->setStyleSheet("background: rgba(0,0,0,0)");

	// The geometries for the widget
	rectShown = QRect();
	rectHidden = QRect(0,-10,10,10);
	rectAni = QRect();

	// The central widget containing all the contents
	center = new QWidget(this);
	center->setObjectName("center");
	center->setStyleSheet("QWidget#center { background: rgba(0,0,0,200); border-radius: 20px; }");

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

	// The main layout
	QVBoxLayout *lay = new QVBoxLayout;

	// The title label
	QLabel *title = new QLabel("<center>" + tr("Set Command/Executeable") + "</center>");
	title->setWordWrap(true);
	title->setStyleSheet("font-size: 15pt; color: white; font-weight: bold; background: transparent");

	// A description of what's to be done here
	QLabel *desc = new QLabel(tr("Here you can change the executeable. A click on the button allows you to browse for one.") + "<br>" + tr("You can use two placeholders: %f = current file, %d = current directory"));
	desc->setWordWrap(true);
	desc->setStyleSheet("color: white; background: transparent;");

	// A button to browse for an executeable
	cmd = new CustomLineEdit;
	CustomPushButton *cmdSelect = new CustomPushButton("...");
	QHBoxLayout *cmdLay = new QHBoxLayout;
	cmdLay->addWidget(cmd);
	cmdLay->addWidget(cmdSelect);
	connect(cmdSelect, SIGNAL(clicked()), this, SLOT(selectCmd()));

	// Ok/Cancel
	CustomPushButton *ok = new CustomPushButton(tr("Okay, set it!"));
	CustomPushButton *cancel = new CustomPushButton(tr("Stop, I changed my mind!"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(ok);
	butLay->addWidget(cancel);
	butLay->addStretch();
	connect(ok, SIGNAL(clicked()), this, SLOT(saveChanges()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(abortChanges()));

	lay->addStretch();
	lay->addWidget(title);
	lay->addSpacing(10);
	lay->addWidget(desc);
	lay->addSpacing(5);
	lay->addLayout(cmdLay);
	lay->addSpacing(5);
	lay->addLayout(butLay);
	lay->addStretch();

	// Setting the main layout
	center->setLayout(lay);

}


// The animation function
void ShortcutChangeCommand::animate() {

	QRect shown = QRect((rectShown.width()-400)/2,(rectShown.height()-200)/2,400,200);

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		// The background is initially transparent but the geometry is full
		this->setStyleSheet(QString("background: rgba(0,0,0,0);"));
		this->setGeometry(rectShown);

		// Widget is shown
		isShown = true;

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

	}
}

// After animation finished
void ShortcutChangeCommand::aniFinished() {

	// We need to set rectHidden, otherwise the widget will be invisible (100% opacity), but still there possibly blocking the gui
	if(!isShown)
		this->setGeometry(rectHidden);

}

// Each step the fading timer calls this function
void ShortcutChangeCommand::fadeStep() {

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

// Animate widget in
void ShortcutChangeCommand::animateCmd(QString c, QString identify) {

	command = c;
	cmd->setText(c);
	id = identify;
	cat = "extern";
	this->animate();
	cmd->setFocus();
	cmd->selectAll();

}

// Save changes
void ShortcutChangeCommand::saveChanges() {

	emit commandChanged(id,cmd->text());

	this->animate();

}

// Cancel
void ShortcutChangeCommand::abortChanges() {

	emit commandCanceled(id);

	this->animate();

}

// Browse for executeable
void ShortcutChangeCommand::selectCmd() {

	QString newExe = QFileDialog::getOpenFileName(0,tr("Select executeable"),command);

	if(newExe != "") {
		cmd->setText(newExe);
		command = newExe;
	}

}

void ShortcutChangeCommand::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

ShortcutChangeCommand::~ShortcutChangeCommand() { }
