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

#include "dropdownmenu.h"

DropDownMenu::DropDownMenu(QWidget *parent) : QWidget(parent) {

	this->setStyleSheet("border-radius: 8px; border-top-right-radius: 0px; border-top-left-radius: 0px; background-color: rgba(0, 0, 0, 220)");

	allItems.clear();

	// The sensitivity of the menu
	ySensitivity = 10;
	xSensitivity = 3;

	// Per default, the menu is shown at the top
	posAtBottom = true;

	// This is the dimension of the menu
	rectShown = QRect(0,0,10,10);
	rectHidden = QRect(rectShown.x(),-rectShown.height(),rectShown.width(),rectShown.height());
	this->setGeometry(rectHidden);

	// The animation
	ani = new QPropertyAnimation(this,"geometry");

	// Is the menu shown?
	isShown = false;

	// The layout holding all the labels
	mLay = new QVBoxLayout;

	// And set the standard functions
	setHeading(tr("General Functions"));
	setStandard(tr("Open File"), ":/img/open.png","open", 1);
	setStandard(tr("Settings"), ":/img/setup.png","settings", 1);
	setStandard(tr("Set as Wallpaper"), ":/img/setup.png","wallpaper", 1);
	setSlideshow();
	setStandard(tr("Filter Images in Folder"),":/img/mesh.png","filterImages", 1);
	setStandard(tr("Show Details"), ":/img/exif.png","hideMeta", 0);
	setStandard(tr("About PhotoQt"), ":/img/about.png","about", 1);
	setStandard(tr("Hide (System Tray)"), ":/img/quit.png","hide", 1);
	setStandard(tr("Quit"), ":/img/quit.png","close", 1);

	// And set the layout
	this->setLayout(mLay);


}

// Slideshow Settings/Quickstart
void DropDownMenu::setSlideshow() {

	MainMenuItem *item1 = new MainMenuItem(tr("Start Slideshow"),":/img/slideshow","slideshow",1);
	MainMenuItem *item2 = new MainMenuItem(tr("(Quickstart)"),"","slideshowQuick",1);

	allItems.insert("slideshow",item1);
	allItems.insert("slideshowQuick",item2);

	QHBoxLayout *slideLay = new QHBoxLayout;

	slideLay->addWidget(item1);
	slideLay->addWidget(item2);
	slideLay->addStretch();

	connect(item1, SIGNAL(clicked()), this, SLOT(menuClicked()));
	connect(item2, SIGNAL(clicked()), this, SLOT(menuClicked()));

	mLay->addSpacing(5);
	mLay->addLayout(slideLay);

}

// Set a standard function
void DropDownMenu::setStandard(QString txt, QString icon, QString data, int active) {

	QHBoxLayout *itemLay = new QHBoxLayout;

	MainMenuItem *item = new MainMenuItem(txt,icon,data,active);

	allItems.insert(data,item);

	itemLay->addWidget(item);
	itemLay->addSpacing(50);

	mLay->addLayout(itemLay);

	connect(item, SIGNAL(clicked()), this, SLOT(menuClicked()));

}

// Add a seperator
void DropDownMenu::setSep() {

	MainMenuItem *item = new MainMenuItem("","","",0,false);

	mLay->addWidget(item);

}

// Ser a heading
void DropDownMenu::setHeading(QString txt) {

	MainMenuItem *item = new MainMenuItem("<center><i>" + txt + "</i></span></center>","","",0,false);

	mLay->addWidget(item);

}


void DropDownMenu::setRect(QRect rect) {

	rectShown = rect;

	rectHidden = (rect.y() == 0)
			? QRect(rect.x(),-rect.height(),rect.width(),rect.height())
			: QRect(rect.x(),rect.y()+rect.height(),rect.width(),rect.height());

	this->setStyleSheet(QString("border-radius: 8px; border-%1-right-radius: 0px; border-%2-left-radius: 0px; background-color: rgba(0, 0, 0, 220)").arg(rect.y() == 0 ? "top" : "bottom").arg(rect.y() == 0 ? "top" : "bottom"));

	isShown ? this->setGeometry(rectShown) : this->setGeometry(rectHidden);

}

void DropDownMenu::setSensitivity(int xSens, int ySens) {

	xSensitivity = xSens;
	ySensitivity = ySens;

}

void DropDownMenu::makeShow() {

	if(!isShown) animate();

}

void DropDownMenu::makeHide() {

	if(isShown) animate();

}


// Animate the menu in/out
void DropDownMenu::animate() {

	// Open widget
	if(!isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(rectHidden);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		this->raise();

	// Close widget
	} else if(isShown) {

		if(ani->state() != QPropertyAnimation::Stopped)
			ani->targetObject()->setProperty(ani->propertyName(),ani->endValue());

		ani->setDuration(500);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(rectHidden);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

	}


}

// If an item was clicked
void DropDownMenu::menuClicked() {

	MainMenuItem *item = (MainMenuItem*) sender();

	if(item->isEnabled()) {

		QString txt = item->data;

		int close = item->close;

		emit itemClicked(txt,close);

	}

}

// Enabling styling of widget
void DropDownMenu::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}



DropDownMenu::~DropDownMenu() { }
