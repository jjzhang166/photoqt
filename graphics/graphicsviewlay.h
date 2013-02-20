#ifndef VIEWBIGLAY_H
#define VIEWBIGLAY_H

#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QSignalMapper>
#include <QMouseEvent>
#include <QFileInfo>


// A custom label (the "x" top right corner) for closing Photo
class QuickInfoLabel : public QLabel {
	Q_OBJECT

public:
	QuickInfoLabel(QWidget *parent = 0, QString objName = "");
	QMenu *c;
	QAction *dohideFilepath;
	QAction *dohide;
	QString version;

	QMap<QString,QVariant> globSet;

protected:
	void mouseReleaseEvent(QMouseEvent *e);
	void contextMenuEvent(QContextMenuEvent *ev);
signals:
	void clicked();
};


// This item is used for displaying the big image
class ViewBigLay : public QVBoxLayout {

	Q_OBJECT

public:
	ViewBigLay(QMap<QString,QVariant> set);
	~ViewBigLay();

	QMap<QString,QVariant> globSet;

	void setPosition(QString pos);
	void updateInfo(QString currentfile, int countpos, int counttot);

	bool slideshowRunning;
	bool slideshowHide;

public slots:
	void hideItem();

private:
	QuickInfoLabel *quickInfoCounterBOT;
	QLabel *quickInfoSepBOT;
	QuickInfoLabel *quickInfoFilenameBOT;

	QuickInfoLabel *quickInfoCounterTOP;
	QuickInfoLabel *quickInfoFilenameTOP;
	QuickInfoLabel *closeWindowX;

	QLabel *quickInfoSepTOP;

signals:
	void clickOnX(QString);
	void updateSettings(QMap<QString,QVariant>);

};

#endif // VIEWBIGLAY_H
