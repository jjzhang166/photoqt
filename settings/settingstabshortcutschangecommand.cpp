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

#include "settingstabshortcutschangecommand.h"

ShortcutChangeCommand::ShortcutChangeCommand(QWidget *parent) : MyWidget(parent) {

	// The main layout
	QVBoxLayout *lay = new QVBoxLayout;

	// The title label
	QLabel *title = new QLabel("<center>" + tr("Set Command/Executeable") + "</center>");
	title->setWordWrap(true);
	title->setStyleSheet("font-size: 15pt; color: white; font-weight: bold; background: transparent");

	// A description of what's to be done here
	QLabel *desc = new QLabel(tr("Here you can change the executeable. A click on the button allows you to browse for one.") + "<br>" + tr("You can use two placeholders: %f = current file, %d = current directory"));
	desc->setWordWrap(true);
	desc->setStyleSheet("color: white; background: transparent;");

	// A button to browse for an executeable
	cmd = new CustomLineEdit;
	CustomPushButton *cmdSelect = new CustomPushButton("...");
	QHBoxLayout *cmdLay = new QHBoxLayout;
	cmdLay->addWidget(cmd);
	cmdLay->addWidget(cmdSelect);
	connect(cmdSelect, SIGNAL(clicked()), this, SLOT(selectCmd()));

	// Ok/Cancel
	CustomPushButton *ok = new CustomPushButton(tr("Okay, set it!"));
	CustomPushButton *cancel = new CustomPushButton(tr("Stop, I changed my mind!"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(ok);
	butLay->addWidget(cancel);
	butLay->addStretch();
	connect(ok, SIGNAL(clicked()), this, SLOT(saveChanges()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(abortChanges()));

	lay->addStretch();
	lay->addWidget(title);
	lay->addSpacing(10);
	lay->addWidget(desc);
	lay->addSpacing(5);
	lay->addLayout(cmdLay);
	lay->addSpacing(5);
	lay->addLayout(butLay);
	lay->addStretch();

	// Setting the main layout
	setWidgetLayout(lay);

}

// Animate widget in
void ShortcutChangeCommand::animateCmd(QString c, QString identify) {

	command = c;
	cmd->setText(c);
	id = identify;
	cat = "extern";
	makeShow();
	cmd->setFocus();
	cmd->selectAll();

}

// Save changes
void ShortcutChangeCommand::saveChanges() {

	emit commandChanged(id,cmd->text());

	makeHide();

}

// Cancel
void ShortcutChangeCommand::abortChanges() {

	emit commandCanceled(id);

	makeHide();

}

// Browse for executeable
void ShortcutChangeCommand::selectCmd() {

	QString newExe = QFileDialog::getOpenFileName(0,tr("Select executeable"),command);

	if(newExe != "") {
		cmd->setText(newExe);
		command = newExe;
	}

}

ShortcutChangeCommand::~ShortcutChangeCommand() { }
