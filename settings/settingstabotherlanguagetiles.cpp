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
	css = "border-radius: 5px;";
	cssBackgroundNorm = "background: rgba(255,255,255,120);";
	cssBackgroundHov = "background: rgba(255,255,255,210);";
	this->setStyleSheet(css + cssBackgroundNorm);

	this->setFixedSize(90,90);

	// Display translators in smaller font => everything's visible, nothing hidden
	if(lang.contains("(")) {
		QList<QString> lang_parts = lang.split("(");
		lang = lang_parts.at(0);
		lang_parts.removeFirst();
		while(lang_parts.length() > 1) {
			lang += "(" + lang_parts.at(0);
			lang_parts.removeFirst();
		}
		lang = lang + "<br><span style=\"font-size: 6pt\">(" + lang_parts.at(0) + "</span>";
	}

	// Language in proper native language
	back = new CustomLabel("<center>" + lang + "</center>");
	back->setBold(true);
	back->setFontColor("black");
	back->setPadding(0);
	back->setFontSize(8);
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
	this->setStyleSheet(css + (tgld ? cssBackgroundHov : cssBackgroundNorm));
}

void SettingsTabOtherLanguageTiles::mouseMoveEvent(QMouseEvent *) {
	this->setStyleSheet(css + cssBackgroundHov);
}

void SettingsTabOtherLanguageTiles::enterEvent(QEvent *) {
	this->setStyleSheet(css + cssBackgroundHov);
}

void SettingsTabOtherLanguageTiles::leaveEvent(QEvent *) {
	this->setStyleSheet(css + (button->isChecked() ? cssBackgroundHov : cssBackgroundNorm));
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
