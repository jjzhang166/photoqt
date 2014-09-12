#ifndef SCALE_H
#define SCALE_H

#include "../customelements/customlabel.h"
#include "../customelements/customspinbox.h"
#include "../customelements/customcheckbox.h"

#include "../widgets/mywidget.h"

class Scale : public MyWidget {

	Q_OBJECT

public:
	Scale(QWidget *parent = 0);
	~Scale();

	void scale(QSize s);

private:
	CustomLabel *curSize;
	double widthByHeightRatio;

	CustomSpinBox *widthSpin;
	CustomSpinBox *heightSpin;
	CustomCheckBox *keepratio;

	bool ignoreSizeChange;
	QString lastEdit;

private slots:
	void sizeChanged();

};




#endif // SCALE_H
