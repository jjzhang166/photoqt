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
