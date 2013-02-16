#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QWidget>
#include <QScrollArea>
#include <QFileInfo>
#include <QtDebug>
#include <QButtonGroup>
#include <QProcess>
#include "customconfirm.h"
#include "../customelements/customscrollbar.h"
#include "../customelements/customradiobutton.h"

class Wallpaper : public QWidget {

	Q_OBJECT

public:
	Wallpaper(QWidget *parent = 0);
	~Wallpaper();

	void setWallpaper(QString file);


	// The animation for the content widget
	QPropertyAnimation *ani;

	// Boolean about current geometry/position
	bool isShown;

	// The different QRects
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// The fade parameters for the background
	int backAlphaShow;
	int backAlphaCur;
	QTimeLine *fadeBack;
	bool fadeBackIN;

	// The central widget
	QWidget *center;


	QLabel *filenameLabel;




	CustomRadioButton *imgCenter;
	CustomRadioButton *imgAlignLeft;
	CustomRadioButton *imgAlignRight;
	CustomRadioButton *imgAlignTop;
	CustomRadioButton *imgAlignBottom;

	CustomRadioButton *imgScaleToFit;
	CustomRadioButton *imgScaleToWidth;
	CustomRadioButton *imgScaleToHeight;
	CustomRadioButton *imgFitIntoView;


	// SCALE TO FIT (KEEP PROPORTIONS)
	// SCALE TO WIDTH (KEEP PROPORTIONS)
	// SCALE TO HEIGHT (KEEP PROPORTIONS)
	// FIT INTO VIEW
	// TILED


	QString filename;

	CustomConfirm *kdeUnsupported;

private:

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

signals:
	// Block all function in mainwindow and activate system keys
	void blockFunc(bool block);

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

	void mouseReleaseEvent(QMouseEvent *);

};

#endif // WALLPAPER_H
