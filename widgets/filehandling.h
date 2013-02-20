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

	QPushButton *renameSave;
	QPushButton *renameCancel;

	QPushButton *deleteYes;
	QPushButton *deleteYesHard;
	QPushButton *deleteNo;

	QPushButton *moveSave;
	QPushButton *moveCancel;

	QPushButton *copySave;
	QPushButton *copyCancel;

	void makeHide();
	void makeShow();
	bool isVisible() { return isShown; }
	void setRect(QRect rect);


private:


	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	// The animation for the content widget
	QPropertyAnimation *ani;

	// Boolean about current geometry/position
	bool isShown;

	// The different QRects
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;


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
	QString moveSaveCSS;

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
