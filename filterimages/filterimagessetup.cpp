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

#include "filterimagessetup.h"

FilterImagesSetup::FilterImagesSetup(QWidget *parent) : MyWidget("default","default",parent) {

	this->setVisibleArea(QSize(500,270));

	// Title
	CustomLabel *title = new CustomLabel(tr("Filter Images in Current Directory"));
	title->setAlignment(Qt::AlignCenter);
	title->setFontSize(15);
	title->setBold(true);
	title->setWordWrap(true);

	// A short description
	CustomLabel *desc = new CustomLabel(tr("Enter here the term you want to search for. Seperate multiple terms by a space.") + "<br>" + tr("If you want to limit a term to file extensions, prepend a dot '.' to the term."));
	desc->setAlignment(Qt::AlignCenter);
	desc->setWordWrap(true);

	// LineEdit to enter extensions
	edit = new CustomLineEdit;

	// Different buttons
	enter = new CustomPushButton(tr("Filter"));
	enter->setEnabled(false);	// only enabled if text entered
	cancel = new CustomPushButton(tr("Cancel"));
	CustomPushButton *remove = new CustomPushButton(tr("Remove Filter"));
	remove->setFontSize("7pt");

	// Connect some buttons
	connect(enter, SIGNAL(clicked()), this, SLOT(okayClicked()));
	connect(edit, SIGNAL(returnPressed()), enter, SIGNAL(clicked()));
	connect(edit, SIGNAL(textEdited(QString)), this, SLOT(editTextChanged()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(disableAndClose()));
	connect(remove, SIGNAL(clicked()), this, SIGNAL(removeFilter()));
	connect(remove, SIGNAL(clicked()), cancel, SLOT(animateClick()));

	// Add buttons to layout
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(enter);
	butLay->addWidget(cancel);
	butLay->addSpacing(20);
	butLay->addWidget(remove);
	butLay->addStretch();

	// main layout
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget(title);
	lay->addWidget(desc);
	lay->addSpacing(8);
	lay->addWidget(edit);
	lay->addSpacing(20);
	lay->addLayout(butLay);

	// Set layout to central widget
	this->setWidgetLayout(lay);

	edit->setFocus();
	edit->selectAll();


}

// Disable enter button if nothing entered (no filter possible)
void FilterImagesSetup::editTextChanged() {

	enter->setEnabled(edit->text().trimmed().length() > 0);

}

// Set filter and reload directory
void FilterImagesSetup::okayClicked() {

	emit setFilter(curDir, edit->text().toLower().split(" ",QString::SkipEmptyParts));
	disableAndClose();

}

FilterImagesSetup::~FilterImagesSetup() { }
