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

#include "customscrollbar.h"

CustomScrollbar::CustomScrollbar(QWidget *parent) : QScrollBar(parent) {

	// These two booleans store cases when the scrollbar has to stay visible
	hovered = false;
	clicked = false;

	// On demand the scrollbar can be forced to stay visible
	dontfadeout = false;

	this->setCursor(Qt::OpenHandCursor);

	// Some fade out values
	alphaShow = 200;
	alphaCur = 200;
	fadeOut = new QTimeLine;
	fadeOut->setDuration(50);
	fadeOut->setLoopCount(15);
	connect(fadeOut, SIGNAL(valueChanged(qreal)), this, SLOT(fadeOutStep()));

	// The fadeout is started with a delay
	hideTimer = new QTimer;
	hideTimer->setSingleShot(true);
	hideTimer->setInterval(1000);
	connect(hideTimer, SIGNAL(timeout()), this, SLOT(hideTimerTimeout()));

	// Set the stylesheet
	setScrollbarShown();

	// When the page is scrolled in any ways, the scrollbar is shown
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(setScrollbarShown()));

}

// The timeout before the fadeout is over
void CustomScrollbar::hideTimerTimeout() {

	if(!dontfadeout)
		fadeOut->start();

}

// Never fade scrollbar out
void CustomScrollbar::keepAlwaysVisible(bool keepVisible) {

	dontfadeout = keepVisible;

}

// Each fadeout step runs this function
void CustomScrollbar::fadeOutStep() {

	alphaCur -= alphaShow/fadeOut->loopCount();
	if(alphaCur < 0)
		alphaCur = 0;

	QString css = "";

	if(this->orientation() == Qt::Vertical) {

		css = "QScrollBar:vertical {";
			css += "border: none;";
			css += QString("background: rgba(50,50,50,%1);").arg(alphaCur);
			css += "width: 8px;";
		css += "}";
		css += "QScrollBar::handle:vertical {";
		css += QString("background: rgba(0,0,0,%1);").arg(alphaCur);
			css += QString("border: 1px solid rgba(255,255,255,%1);").arg(alphaCur);
			css += "border-radius: 5px";
		css += "}";
		css += "QScrollBar::add-line:vertical {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::sub-line:vertical {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += QString("QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {");
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {";
			css += "background: none;";
		css += "}";

	} else {

		css = "QScrollBar:horizontal {";
			css += "border: none;";
			css += QString("background: rgba(50,50,50,%1);").arg(alphaCur);
			css += "height: 8px;";
		css += "}";
		css += "QScrollBar::handle:horizontal {";
		css += QString("background: rgba(0,0,0,%1);").arg(alphaCur);
			css += QString("border: 1px solid rgba(255,255,255,%1);").arg(alphaCur);
			css += "border-radius: 5px";
		css += "}";
		css += "QScrollBar::add-line:horizontal {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::sub-line:horizontal {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += QString("QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal {");
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {";
			css += "background: none;";
		css += "}";

	}

	this->setStyleSheet(css);

}

// Make the scrollbar visible
void CustomScrollbar::setScrollbarShown() {

	hideTimer->stop();
	fadeOut->stop();
	alphaCur = alphaShow;

	QString css = "";

	if(this->orientation() == Qt::Vertical) {

		css = "QScrollBar:vertical {";
			css += "border: none;";
			css += QString("background: rgba(50,50,50,%1);").arg(alphaShow);
			css += "width: 8px;";
		css += "}";
		css += "QScrollBar::handle:vertical {";
		css += QString("background: rgba(0,0,0,%1);").arg(alphaShow);
			css += QString("border: 1px solid rgba(255,255,255,%1);").arg(alphaShow);
			css += "border-radius: 5px";
		css += "}";
		css += "QScrollBar::add-line:vertical {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::sub-line:vertical {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += QString("QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {");
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {";
			css += "background: none;";
		css += "}";

	} else {

		css = "QScrollBar:horizontal {";
			css += "border: none;";
			css += QString("background: rgba(50,50,50,%1);").arg(alphaShow);
			css += "height: 8px;";
		css += "}";
		css += "QScrollBar::handle:horizontal {";
		css += QString("background: rgba(0,0,0,%1);").arg(alphaShow);
			css += QString("border: 1px solid rgba(255,255,255,%1);").arg(alphaShow);
			css += "border-radius: 5px";
		css += "}";
		css += "QScrollBar::add-line:horizontal {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::sub-line:horizontal {";
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += QString("QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal {");
			css += "border: none;";
			css += "background: transparent;";
		css += "}";
		css += "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {";
			css += "background: none;";
		css += "}";

	}

	this->setStyleSheet(css);

	if(!clicked && !hovered)
		hideTimer->start();

}

void CustomScrollbar::mousePressEvent(QMouseEvent *e) {

	clicked = true;

	this->setCursor(Qt::ClosedHandCursor);

	QScrollBar::mousePressEvent(e);

}

void CustomScrollbar::mouseReleaseEvent(QMouseEvent *e) {

	clicked = false;

	this->setCursor(Qt::OpenHandCursor);

	QScrollBar::mouseReleaseEvent(e);

}

void CustomScrollbar::enterEvent(QEvent *e) {

	hovered = true;

	setScrollbarShown();

	QScrollBar::enterEvent(e);

}

void CustomScrollbar::leaveEvent(QEvent *e) {

	hovered = false;

	hideTimer->start();

	QScrollBar::leaveEvent(e);

}


CustomScrollbar::~CustomScrollbar() { }
