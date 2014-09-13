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

#include "customconfirm.h"

CustomConfirm::CustomConfirm(QString title, QString text, QString yesText, QString noText, QSize size, QString borderColor, QString backgroundColor, QWidget *parent) : MyWidget(borderColor,backgroundColor,parent) {

	this->setVisibleArea(size);

	// The main layout
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addStretch();
	lay->addSpacing(10);

	// The title of the widget with user set text
	CustomLabel *t = new CustomLabel(title);
	t->setFontSize(15);
	t->setBold(true);
	t->setAlignment(Qt::AlignCenter);
	lay->addWidget(t);
	lay->addSpacing(12);

	CustomLabel *desc = new CustomLabel(text);
	desc->setAlignment(Qt::AlignCenter);
	desc->setLineHeight("2pt");
	lay->addWidget(desc);
	lay->addSpacing(7);

	// This box is optional and can be enabled if needed
	dontShowAgain = new CustomCheckBox(tr("Don't ask me again..."));
	dontShowAgain->hide();
	QHBoxLayout *dontLay = new QHBoxLayout;
	dontLay->addStretch();
	dontLay->addWidget(dontShowAgain);
	dontLay->addStretch();
	lay->addLayout(dontLay);
	lay->addSpacing(7);

	// The yes and no buttons (an empty string hides a button)
	yes = new CustomPushButton(yesText,this);
	no = new CustomPushButton(noText, this);
	connect(yes, SIGNAL(clicked()), this, SLOT(animate()));
	connect(no, SIGNAL(clicked()), this, SLOT(animate()));
	connect(yes, SIGNAL(clicked()), this, SIGNAL(confirmed()));
	connect(no, SIGNAL(clicked()), this, SIGNAL(rejected()));
	QHBoxLayout *yesNoLay = new QHBoxLayout;
	yesNoLay->addStretch();
	if(yesText != "")
		yesNoLay->addWidget(yes);
	else
		yes->hide();
	if(noText != "")
		yesNoLay->addWidget(no);
	else
		no->hide();
	yesNoLay->addStretch();
	lay->addLayout(yesNoLay);



	lay->addSpacing(10);
	lay->addStretch();
	this->setWidgetLayout(lay);

}

// Enable the don't show again checkbox
void CustomConfirm::setDontShowAgain() {

	dontShowAgain->show();

}

CustomConfirm::~CustomConfirm() { }
