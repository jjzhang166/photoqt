#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "../widgets/mywidget.h"
#include "../customelements/custompushbutton.h"

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
	QLabel *renameTitle;
	QLabel *renameOldName;
	QLineEdit *renameNewName;
	QLabel *renameOldEnding;
	QLabel *renameNewNameExists;



	/**********************************
	/////////// DELETE ////////////////
	**********************************/
	QWidget *deleteWidget;
	QVBoxLayout *deleteLay;
	QLabel *deleteTitle;
	QLabel *deleteFilename;
	QLabel *deleteQuestion;


	/********************************
	/////////// MOVE ////////////////
	********************************/
	QWidget *moveWidget;
	QVBoxLayout *moveLay;
	QLabel *moveTitle;
	QTreeView *moveTree;
	CustomScrollbar *moveScrollbar;
	QFileSystemModel *moveTreeModel;
	QLabel *moveNewNameExists;
	QLabel *moveNewNameLabel;
	QLineEdit *moveNewName;
	QLabel *moveNewNameEnding;

	/********************************
	/////////// COPY ////////////////
	********************************/
	QWidget *copyWidget;
	QVBoxLayout *copyLay;
	QLabel *copyTitle;
	QTreeView *copyTree;
	CustomScrollbar *copyScrollbar;
	QFileSystemModel *copyTreeModel;
	QLabel *copyNewNameExists;
	QLabel *copyNewNameLabel;
	QLineEdit *copyNewName;
	QLabel *copyNewNameEnding;


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
