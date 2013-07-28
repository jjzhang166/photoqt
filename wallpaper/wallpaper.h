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
#include <QButtonGroup>
#include <QDesktopWidget>

#include "../customelements/custompushbutton.h"
#include "../customelements/customscrollbar.h"
#include "../customelements/customcombobox.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customcheckbox.h"

class Wallpaper : public QWidget {

	Q_OBJECT

public:
	Wallpaper(QMap<QString,QVariant> set, bool v, QWidget *parent = 0);
	~Wallpaper();

	QMap<QString,QVariant> globSet;

	bool verbose;

	void setWallpaper(QString file);
	void dontSetWallpaper();

	void makeShow();
	void makeHide();
	bool isVisible() { return isShown; }
	void setRect(QRect rect);

	void accept();


private:

	// Boolean about current geometry/position
	bool isShown;

	// The different QRects
	QRect rectShown;
	QRect rectHidden;
	QRect rectAni;

	// The central widget
	QWidget *center;

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

	QButtonGroup *xfceButGrp;
	QLabel *xfcePicOpsLabel;
	QMap<QString,CustomRadioButton*> xfcePictureOptions;


	CustomRadioButton *otherFeh;
	CustomRadioButton *otherNitrogen;
	QMap<QString,CustomRadioButton*> otherFehOptions;
	QMap<QString,CustomRadioButton*> otherNitrogenOptions;
	QButtonGroup *otherFehGrp;
	QButtonGroup *otherNitrogenGrp;


	QMap<int,CustomCheckBox*> wmMonitorSelect;
	QLabel *wmMonitorLabel;

	void detectWM();

	void setKDE();
	void setGNOME();
	void setXFCE();
	void setRazor();
	void setOTHER();

private slots:
	// The animation functions
	void animate();

	void aniFinished();

	// Each fade step calls this function
	void fadeStep();

	void wmSelected();

	void goAheadAndSetWallpaper();

	void swapFehNitrogen();

signals:
	// Block all function in mainwindow and activate system keys
	void blockFunc(bool block);

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

	void mouseReleaseEvent(QMouseEvent *);

};

#endif // WALLPAPER_H
