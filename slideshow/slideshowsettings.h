#ifndef SLIDESHOWSETTINGS_H
#define SLIDESHOWSETTINGS_H

#include "../customelements/customscrollbar.h"
#include "../customelements/customslider.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QDir>
#include <QFileDialog>

class SlideShow : public QWidget {

	Q_OBJECT

public:
	SlideShow(QMap<QString,QVariant> set, QWidget *parent = 0, bool verbose = false);
	~SlideShow();

	bool verbose;

	QMap<QString,QVariant> globSet;

	void makeHide();
	void makeShow();
	bool isVisible() { return isShown; }
	void setRect(QRect rect);

	// Adjust time
	CustomSlider *timeSlider;
	// Adjust transition
	CustomSlider *trans;

#ifdef WITH_PHONON
	// Adjust music
	CustomCheckBox *musicEnable;
	CustomLineEdit *musicPath;
#endif

	// Adjust quickinfo labels
	CustomCheckBox *hideQuickInfo;

public slots:
	// Start slideshow
	void andStart();

private:

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

	// Central widget
	QWidget *center;

	// Adjust geometries
	void adjustGeometries();

private slots:
	// The animation functions
	void animate();
	void aniFinished();

	// Each fade step calls this function
	void fadeStep();

	// Browse filesystem for music file
	void browseForMusic();

signals:
	// Block all function in mainwindow and activate system keys
	void blockFunc(bool block);

	// Start the slideshow
	void startSlideShow();

protected:
	// Overriding the paint event to make widget styleable
	void paintEvent(QPaintEvent *);

	void mouseReleaseEvent(QMouseEvent *);
};

#endif // SLIDESHOWSETTINGS_H
