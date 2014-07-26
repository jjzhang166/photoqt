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

#include "filterimagesdisplay.h"

FilterImagesDisplay::FilterImagesDisplay(QWidget *parent) : QWidget(parent) {

	// No background, visible but ss non-obstrusive as possible
	this->setStyleSheet("background: none");

	// Quick way to remove filter
	CustomLabel *removeFilter = new CustomLabel(" x ");
	removeFilter->setToolTip(tr("Remove Image Filter"));
	removeFilter->setCursor(Qt::PointingHandCursor);
	removeFilter->setBold(true);
	removeFilter->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	connect(removeFilter, SIGNAL(clicked()), this, SLOT(removeClicked()));

	// Display visible extensions
	display = new CustomLabel("---");
	display->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	display->setToolTip(tr("Click to change filter"));
	display->setWordWrap(false);
	display->setCursor(Qt::PointingHandCursor);
	connect(display, SIGNAL(clicked()), SIGNAL(changeFilter()));

	// Put everything into layout
	QHBoxLayout *lay = new QHBoxLayout;
	lay->addWidget(removeFilter);
	lay->addWidget(display);
	lay->addStretch();
	this->setLayout(lay);

}

// Remove filter
void FilterImagesDisplay::removeClicked() {

	this->setVisible(false);
	emit removeImageFilter();

}

// Display a filter
void FilterImagesDisplay::showFilter(QStringList filter) {

	display->setText(filter.join(", "));
	this->setVisible(true);
	this->adjustSize();

}

// Make widget styleable
void FilterImagesDisplay::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


FilterImagesDisplay::~FilterImagesDisplay() { }
