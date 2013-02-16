#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QFileInfo>
#include <QProcess>
#include <QtDebug>
#include <QButtonGroup>
#include <QDesktopWidget>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

#include "../customelements/custompushbutton.h"
#include "../customelements/customscrollbar.h"
#include "../customelements/customcombobox.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customcheckbox.h"

class Wallpaper : public QWidget {

	Q_OBJECT

public:
	Wallpaper(QMap<QString,QVariant> set, QWidget *parent = 0);
	~Wallpaper();

	QMap<QString,QVariant> globSet;

	void setWallpaper(QString file);
	void dontSetWallpaper();

	// Boolean about current geometry/position
	bool isShown;

	// The different QRects
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// The central widget
	QWidget *center;


private:
	// The animation for the content widget
	QPropertyAnimation *ani;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	CustomPushButton *ok;
	CustomPushButton *cancel;

	CustomComboBox *wm;
	QLabel *wmMessage;

	QString filename;
	QLabel *filenameLabel;

	QButtonGroup *gnomeButGrp;
	QLabel *gnomePicOpsLabel;
	QMap<QString,CustomRadioButton*> gnomePictureOptions;

	QMap<int,CustomCheckBox*> xfceMonitorSelect;
	QLabel *xfceMonitorLabel;
	QButtonGroup *xfceButGrp;
	QLabel *xfcePicOpsLabel;
	QMap<QString,CustomRadioButton*> xfcePictureOptions;

	void detectWM();

	void setKDE();
	void setGNOME();
	void setXFCE();
	void setOTHER();

private slots:
	// The animation functions
	void animate();

	void aniFinished();

	// Each fade step calls this function
	void fadeStep();

	void wmSelected();

	void goAheadAndSetWallpaper();

signals:
	// Block all function in mainwindow and activate system keys
	void blockFunc(bool block);

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

	void mouseReleaseEvent(QMouseEvent *);

};

#endif // WALLPAPER_H
