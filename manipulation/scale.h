#ifndef SCALE_H
#define SCALE_H

#include <QFileDialog>
#include <QSignalMapper>
#include <QGridLayout>
#include <QImageReader>

#include "../customelements/customlabel.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customslider.h"
#include "../widgets/customconfirm.h"
#include "../widgets/mywidget.h"

class Scale : public MyWidget {

	Q_OBJECT

public:
	Scale(bool verbose, QWidget *parent = 0);
	~Scale();

	void scale(QString filename, QSize s);

	CustomPushButton *enterInPlace;
	CustomConfirm *confirmNotSupported;
	CustomConfirm *confirmInPlace;
	CustomConfirm *confirmNew;

	// First we need to enable the spinboxes and then we show the widget
	void makeShow() { widthSpin->setEnabled(true); heightSpin->setEnabled(true); MyWidget::makeShow(); }

private:
	bool verbose;

	CustomLabel *curSize;
	QString currentfile;
	double widthByHeightRatio;

	CustomSpinBox *widthSpin;
	CustomSpinBox *heightSpin;
	CustomCheckBox *keepratio;

	CustomSlider *quality;

	bool ignoreSizeChange;
	QString lastEdit;

public slots:
	void disableAllSpinBoxAndClose();

private slots:
	void sizeChanged();
	void keepRatioClicked();

	void enterClicked();

	void doScale(QString filename);

};




#endif // SCALE_H
