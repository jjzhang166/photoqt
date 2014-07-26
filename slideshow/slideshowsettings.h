#ifndef SLIDESHOWSETTINGS_H
#define SLIDESHOWSETTINGS_H

#include "../widgets/mywidget.h"

#include "../customelements/customscrollbar.h"
#include "../customelements/customslider.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customline.h"

#include <QDir>
#include <QFileDialog>

class SlideShow : public MyWidget {

	Q_OBJECT

public:
	SlideShow(QMap<QString,QVariant> set, bool verbose = false, QWidget *parent = 0);
	~SlideShow();

	bool verbose;

	QMap<QString,QVariant> globSet;

	// Adjust time
	CustomSlider *timeSlider;
	// Adjust transition
	CustomSlider *trans;

	// Adjust music
	CustomCheckBox *musicEnable;
	CustomLineEdit *musicPath;

	// Adjust quickinfo labels
	CustomCheckBox *hideQuickInfo;

public slots:
	// Start slideshow
	void andStart();

private slots:
	// Browse filesystem for music file
	void browseForMusic();

signals:
	// Start the slideshow
	void startSlideShow();

};

#endif // SLIDESHOWSETTINGS_H
