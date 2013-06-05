#include "customlabel.h"

CustomLabel::CustomLabel(const QString &text, QWidget *parent) : QLabel(text, parent) {

	css = "QLabel {color: white; } QLabel:disabled { color: grey; }";

	this->setStyleSheet(css);

}

void CustomLabel::setEnabled(bool e) {

	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect;

	if(e)
		eff->setOpacity(1);
	else
		eff->setOpacity(0.3);

	this->setGraphicsEffect(eff);


	QLabel::setEnabled(e);

}

void CustomLabel::setCSS(QString c) {

	css = css;
	if(this->isEnabled())
		this->setStyleSheet(css + "QLabel { background: rgba(255,255,255,200); }");
	else
		this->setStyleSheet(css + "QLabel { background: transparent; }");

}

CustomLabel::~CustomLabel() { }
