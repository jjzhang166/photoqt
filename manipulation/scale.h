#ifndef SCALE_H
#define SCALE_H

#include <QFileDialog>
#include <QSignalMapper>
#include <QGridLayout>
#include <QImageReader>

#include "../customelements/customlabel.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customcheckbox.h"
#include "../widgets/customconfirm.h"
#include "../widgets/mywidget.h"

class Scale : public MyWidget {

	Q_OBJECT

public:
	Scale(bool verbose, QWidget *parent = 0);
	~Scale();

	void scale(QString filename, QSize s);

	CustomPushButton *enterInPlace;
	CustomConfirm *confirmInPlace;
	CustomConfirm *confirmNew;

private:
	bool verbose;

	CustomLabel *curSize;
	QString currentfile;
	double widthByHeightRatio;

	CustomSpinBox *widthSpin;
	CustomSpinBox *heightSpin;
	CustomCheckBox *keepratio;

	bool ignoreSizeChange;
	QString lastEdit;

private slots:
	void sizeChanged();
	void keepRatioClicked();

	void enterClicked();

	void doScale(QString filename);

};




#endif // SCALE_H
