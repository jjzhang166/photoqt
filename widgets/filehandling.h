#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "../customelements/customscrollbar.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QtDebug>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QTimeLine>
#include <QDateTime>
#include <QDir>
#include <QUrl>
#include <QSignalMapper>
#include <QTreeView>
#include <QFileSystemModel>

#include <QApplication>

class FileHandling : public QWidget {

	Q_OBJECT

public:
	FileHandling(QWidget *parent = 0);
	~FileHandling();

	// The animation for the content widget
	QPropertyAnimation *ani;

	// Boolean about current geometry/position
	bool isShown;

	// The different QRects
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// The current loaded file
	QString currentfile;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	// The current type of dialog opened
	QString dialogType;



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
	QString renameSaveCSS;
	QPushButton *renameSave;
	QPushButton *renameCancel;



	/**********************************
	/////////// DELETE ////////////////
	**********************************/
	QWidget *deleteWidget;
	QVBoxLayout *deleteLay;
	QLabel *deleteTitle;
	QLabel *deleteFilename;
	QLabel *deleteQuestion;
	QPushButton *deleteYes;
	QPushButton *deleteYesHard;
	QPushButton *deleteNo;



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
	QString moveSaveCSS;
	QPushButton *moveSave;
	QPushButton *moveCancel;

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
	QString copySaveCSS;
	QPushButton *copySave;
	QPushButton *copyCancel;



	// Open the dialog as type 't'
	void openDialog(QString t);

	// Set the different widgets to be used
	void setRename();
	void setDelete();
	void setMove();
	void setCopy();

public slots:
	// The animation functions
	void animate();
	void aniFinished();

	// Each fade step calls this function
	void fadeStep();

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

	// Block all function in mainwindow and activate system keys
	void blockFunc(bool block);

	// Reload the current dir after change has been done
	void reloadDir(QString t);

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

	void mouseReleaseEvent(QMouseEvent *);

};

#endif // FILEHANDLING_H
