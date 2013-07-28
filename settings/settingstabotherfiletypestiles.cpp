#include "settingstabotherfiletypestiles.h"

SettingsTabOtherFileTypesTiles::SettingsTabOtherFileTypesTiles(QString ftype, QWidget *parent) : QWidget(parent) {

	// The standard default stylesheet
	css = "font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt;";
	cssOff = "font-weight: bold; color: rgb(30,30,30); border-radius: 5px; padding: 1px; font-size: 8pt;";

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
	filetype = ftype;

	// The back label is being styled
	back = new QLabel("<center>*" + ftype + "</center>");
	back->setWordWrap(true);
	back->setStyleSheet("background: transparent");

	// The checkbox for diabling or enabling this tile
	enabled = new CustomCheckBox;
	enabled->setIndicatorSize(16);
	QHBoxLayout *enabledLay = new QHBoxLayout;
	enabledLay->addStretch();
	enabledLay->addWidget(enabled);
	enabledLay->addStretch();
	connect(enabled, SIGNAL(clicked()), this, SLOT(checkboxClicked()));

	this->setFixedSize(80,60);

	lay->addWidget(back);
	lay->addSpacing(10);
	lay->addLayout(enabledLay);
	this->setLayout(lay);

	// Default css
	this->setStyleSheet(cssOff + cssBackgroundOffNorm);

}

// Set this tile checked
void SettingsTabOtherFileTypesTiles::setChecked(bool chkd) {

	if(chkd) {
		this->setStyleSheet(css + cssBackgroundNorm);
	} else {
		this->setStyleSheet(cssOff + cssBackgroundOffNorm);
	}

	enabled->setChecked(chkd);

}

// Click on the checkbox
void SettingsTabOtherFileTypesTiles::checkboxClicked() {

	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundNorm);
	else
		setChecked(true);

}

void SettingsTabOtherFileTypesTiles::mouseMoveEvent(QMouseEvent *) {

	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(cssOff + cssBackgroundOffHov);

}

void SettingsTabOtherFileTypesTiles::enterEvent(QEvent *) {

	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(cssOff + cssBackgroundOffHov);

}

void SettingsTabOtherFileTypesTiles::leaveEvent(QEvent *) {

	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundNorm);
	else
		this->setStyleSheet(cssOff + cssBackgroundOffNorm);

}

void SettingsTabOtherFileTypesTiles::mousePressEvent(QMouseEvent *) {

	enabled->setChecked(!enabled->isChecked());
	if(enabled->isChecked())
		this->setStyleSheet(css + cssBackgroundHov);
	else
		this->setStyleSheet(css + cssBackgroundNorm);

}


void SettingsTabOtherFileTypesTiles::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

SettingsTabOtherFileTypesTiles::~SettingsTabOtherFileTypesTiles() { }
