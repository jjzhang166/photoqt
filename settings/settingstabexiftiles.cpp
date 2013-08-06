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

#include "settingstabexiftiles.h"
#include <iostream>

SettingsTabExifTiles::SettingsTabExifTiles(QString exifTxt, QString exifIntern, QWidget *parent) : QWidget(parent) {

	// The standard default stylesheet
	css = "border-radius: 5px; padding: 1px;";
	cssOff = "border-radius: 5px; padding: 1px; font-size: 7pt;";

	// Some special stylesheet for hovered and normal
	cssBackgroundNorm = "background: rgba(255,255,255,150);";
	cssBackgroundHov = "background: rgba(255,255,255,200);";

	cssBackgroundOffNorm = "background: rgba(255,255,255,100);";
	cssBackgroundOffHov = "background: rgba(255,255,255,150);";

	// Main Layout
	QVBoxLayout *lay = new QVBoxLayout;

	this->setMouseTracking(true);
	this->setCursor(Qt::PointingHandCursor);

	// Store the exif key value
	intern = exifIntern;

	// The back label is being styled
	back = new CustomLabel("<center>" + exifTxt + "</center>");
	back->setFontColor("black");
	back->setFontSize("7pt");
	back->setBold(true);
	back->setToolTip(exifTxt);

	// The checkbox for diabling or enabling this tile
	enabled = new CustomCheckBox;
	enabled->setIndicatorSize(16);
	QHBoxLayout *enabledLay = new QHBoxLayout;
	enabledLay->addStretch();
	enabledLay->addWidget(enabled);
	enabledLay->addStretch();
	connect(enabled, SIGNAL(clicked()), this, SLOT(checkboxClicked()));

	this->setFixedSize(90,90);

	lay->addWidget(back);
	lay->addSpacing(10);
	lay->addLayout(enabledLay);
	this->setLayout(lay);

	// Default css
	this->setStyleSheet(cssOff + cssBackgroundOffNorm);

}

// Set this tile checked
void SettingsTabExifTiles::setChecked(bool chkd) {

	if(chkd) {
		this->setStyleSheet(css + cssBackgroundNorm);
	} else {
		this->setStyleSheet(cssOff + cssBackgroundOffNorm);
	}

	enabled->setChecked(chkd);

}

void SettingsTabExifTiles::setEnabled(bool en) {

	if(!en) {
		enabled->setEnabled(false);

		cssBackgroundNorm = "background: rgba(255,255,255,100);";
		cssBackgroundHov = "background: rgba(255,255,255,100);";

		cssBackgroundOffNorm = "background: rgba(255,255,255,100);";
		cssBackgroundOffHov = "background: rgba(255,255,255,100);";

		this->setCursor(Qt::ArrowCursor);
	} else {

		enabled->setEnabled(true);

		cssBackgroundNorm = "background: rgba(255,255,255,150);";
		cssBackgroundHov = "background: rgba(255,255,255,200);";

		cssBackgroundOffNorm = "background: rgba(255,255,255,100);";
		cssBackgroundOffHov = "background: rgba(255,255,255,150);";

		this->setCursor(Qt::PointingHandCursor);

	}

}

// Click on the checkbox
void SettingsTabExifTiles::checkboxClicked() {

	if(!enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundNorm);
	else
		setChecked(true);

}

void SettingsTabExifTiles::mouseMoveEvent(QMouseEvent *) {

	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(cssOff + cssBackgroundOffHov);

}

void SettingsTabExifTiles::enterEvent(QEvent *) {

	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(cssOff + cssBackgroundOffHov);

}

void SettingsTabExifTiles::leaveEvent(QEvent *) {

	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundNorm);
	else
		this->setStyleSheet(cssOff + cssBackgroundOffNorm);

}

void SettingsTabExifTiles::mousePressEvent(QMouseEvent *) {

	if(!enabled->isEnabled()) return;

	enabled->setChecked(!enabled->isChecked());
	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(css + cssBackgroundNorm);

}


void SettingsTabExifTiles::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabExifTiles::~SettingsTabExifTiles() { }
