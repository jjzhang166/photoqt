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

#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "../widgets/mywidget.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customlineedit.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QUrl>
#include <QSignalMapper>
#include <QTreeView>
#include <QFileSystemModel>

#include <QApplication>

class FileHandling : public MyWidget {

	Q_OBJECT

public:
	FileHandling(QWidget *parent = 0, bool verbose = false, QString cf = "");
	~FileHandling();

	bool verbose;

	// The current loaded file
	QString currentfile;

	// The current type of dialog opened
	QString dialogType;

	// Open the dialog as type 't'
	void openDialog(QString t);

	// Set the different widgets to be used
	void setRename();
	void setDelete();
	void setMove();
	void setCopy();

	CustomPushButton *renameSave;
	CustomPushButton *renameCancel;

	CustomPushButton *deleteYes;
	CustomPushButton *deleteYesHard;
	CustomPushButton *deleteNo;

	CustomPushButton *moveSave;
	CustomPushButton *moveCancel;

	CustomPushButton *copySave;
	CustomPushButton *copyCancel;

private:

	/**********************************
	/////////// RENAME ////////////////
	**********************************/
	QWidget *renameWidget;
	QVBoxLayout *renameLay;
	CustomLabel *renameTitle;
	CustomLabel *renameOldName;
	CustomLineEdit *renameNewName;
	CustomLabel *renameOldEnding;
	CustomLabel *renameNewNameExists;



	/**********************************
	/////////// DELETE ////////////////
	**********************************/
	QWidget *deleteWidget;
	QVBoxLayout *deleteLay;
	CustomLabel *deleteTitle;
	CustomLabel *deleteFilename;
	CustomLabel *deleteQuestion;


	/********************************
	/////////// MOVE ////////////////
	********************************/
	QWidget *moveWidget;
	QVBoxLayout *moveLay;
	CustomLabel *moveTitle;
	QTreeView *moveTree;
	CustomScrollbar *moveScrollbar;
	QFileSystemModel *moveTreeModel;
	CustomLabel *moveNewNameExists;
	CustomLabel *moveNewNameLabel;
	CustomLineEdit *moveNewName;
	CustomLabel *moveNewNameEnding;

	/********************************
	/////////// COPY ////////////////
	********************************/
	QWidget *copyWidget;
	QVBoxLayout *copyLay;
	CustomLabel *copyTitle;
	QTreeView *copyTree;
	CustomScrollbar *copyScrollbar;
	QFileSystemModel *copyTreeModel;
	CustomLabel *copyNewNameExists;
	CustomLabel *copyNewNameLabel;
	CustomLineEdit *copyNewName;
	CustomLabel *copyNewNameEnding;


public slots:
	// The animation function
	void aniFinished();

	/**********************************
	/////////// RENAME ////////////////
	**********************************/
	void doRename();
	void validateRenameFilename();

	/**********************************
	/////////// DELETE ////////////////
	**********************************/
	void doDelete(int harddelete = 0);

	/********************************
	/////////// MOVE ////////////////
	********************************/
	void doMove();

	/********************************
	/////////// COPY ////////////////
	********************************/
	void doCopy();

	void validateMoveAndCopyFilename();


signals:
	// Stop the Thumbnail creation of current directory
	void stopThbCreation();

	// Reload the current dir after change has been done
	void reloadDir(QString t);

};

#endif // FILEHANDLING_H
