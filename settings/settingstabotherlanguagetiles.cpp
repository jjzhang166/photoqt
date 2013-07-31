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

#include "settingstabotherlanguagetiles.h"

SettingsTabOtherLanguageTiles::SettingsTabOtherLanguageTiles(QString lang, QString code, QWidget *parent) : QWidget(parent) {

	// The different stylesheets
	css = "font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt;";
	cssBackgroundNorm = "background: rgba(255,255,255,120);";
	cssBackgroundHov = "background: rgba(255,255,255,210);";
	this->setStyleSheet(css + cssBackgroundNorm);

	this->setFixedSize(90,90);

	// Language in proper native language
	back = new QLabel("<center>" + lang + "</center>");
	back->setStyleSheet("background: none;");
	back->setWordWrap(true);
	langCode = code;

	this->setToolTip(lang);

	// Button to select language
	button = new CustomRadioButton;
	connect(button,SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
	QHBoxLayout *buttonLay = new QHBoxLayout;
	buttonLay->addStretch();
	buttonLay->addWidget(button);
	buttonLay->addStretch();

	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget(back);
	lay->addLayout(buttonLay);

	this->setCursor(Qt::PointingHandCursor);

	this->setLayout(lay);

}

// Toggle of button also adjusts stylesheet
void SettingsTabOtherLanguageTiles::buttonToggled(bool tgld) {

	if(tgld)
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(css + cssBackgroundNorm);

}

void SettingsTabOtherLanguageTiles::mouseMoveEvent(QMouseEvent *) {
	this->setStyleSheet(css + cssBackgroundHov);
}

void SettingsTabOtherLanguageTiles::enterEvent(QEvent *) {
	this->setStyleSheet(css + cssBackgroundHov);
}

void SettingsTabOtherLanguageTiles::leaveEvent(QEvent *) {
	if(button->isChecked())
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(css + cssBackgroundNorm);
}

void SettingsTabOtherLanguageTiles::mousePressEvent(QMouseEvent *) {
	button->setChecked(true);
}

void SettingsTabOtherLanguageTiles::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabOtherLanguageTiles::~SettingsTabOtherLanguageTiles() { }
