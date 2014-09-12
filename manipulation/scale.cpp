#include "scale.h"

Scale::Scale(QWidget *parent) : MyWidget(parent) {

	this->setVisibleArea(QSize(500,260));

	ignoreSizeChange = false;

	CustomLabel *title = new CustomLabel(tr("<center><h1>Scale Image</h1></center>"));
	curSize = new CustomLabel("<center>" + tr("Current Size:") + "</center>");



	QHBoxLayout *widthLay = new QHBoxLayout;
	QHBoxLayout *heightLay = new QHBoxLayout;
	QVBoxLayout *spinLay = new QVBoxLayout;
	QHBoxLayout *spinCheckLay = new QHBoxLayout;

	CustomLabel *widthLabel = new CustomLabel(tr("New Width:"));
	widthLabel->setFixedWidth(120);
	widthLabel->setAlignment(Qt::AlignRight);
	widthSpin = new CustomSpinBox;
	widthSpin->setObjectName("w");
	widthSpin->setMaximum(999999);
	widthSpin->setFixedWidth(70);
	widthSpin->setBackground("white", "black");
	widthSpin->setBorder("black",1);
	widthSpin->setFontColor("black");
	widthLay->addWidget(widthLabel);
	widthLay->addWidget(widthSpin);
	connect(widthSpin, SIGNAL(valueChanged(int)), this, SLOT(sizeChanged()));

	CustomLabel *heightLabel = new CustomLabel(tr("New Height:"));
	heightLabel->setFixedWidth(120);
	heightLabel->setAlignment(Qt::AlignRight);
	heightSpin = new CustomSpinBox;
	heightSpin->setObjectName("h");
	heightSpin->setMaximum(999999);
	heightSpin->setFixedWidth(70);
	heightSpin->setBackground("white", "black");
	heightSpin->setBorder("black",1);
	heightSpin->setFontColor("black");
	heightLay->addWidget(heightLabel);
	heightLay->addWidget(heightSpin);
	connect(heightSpin, SIGNAL(valueChanged(int)), this, SLOT(sizeChanged()));

	spinLay->addLayout(widthLay);
	spinLay->addLayout(heightLay);

	keepratio = new CustomCheckBox(tr("Keep Aspect Ratio"));
	keepratio->setChecked(true);
	connect(keepratio, SIGNAL(clicked()), this, SLOT(sizeChanged()));

	spinCheckLay->addStretch();
	spinCheckLay->addLayout(spinLay);
	spinCheckLay->addWidget(keepratio);
	spinCheckLay->addStretch();


	CustomPushButton *enterInPlace = new CustomPushButton(tr("Scale in place"));
	CustomPushButton *enterNew = new CustomPushButton(tr("Scale in new file"));
	CustomPushButton *cancel = new CustomPushButton(tr("Don't Scale"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addStretch();
	butLay->addWidget(enterInPlace);
	butLay->addWidget(enterNew);
	butLay->addWidget(cancel);
	butLay->addStretch();



	QVBoxLayout *lay = new QVBoxLayout;

	lay->addWidget(title);
	lay->addSpacing(15);
	lay->addWidget(curSize);
	lay->addSpacing(20);
	lay->addLayout(spinCheckLay);
	lay->addSpacing(20);
	lay->addLayout(butLay);

	lay->addStretch();


	this->setWidgetLayout(lay);


}


void Scale::scale(QSize s) {

	widthByHeightRatio = (1.0*s.width())/(1.0*s.height());

	curSize->setText("<center>" + tr("Current Size:") + QString(" %1 x %2").arg(s.width()).arg(s.height()) + "</center>");
	widthSpin->setValue(s.width());
	heightSpin->setValue(s.height());
	keepratio->setChecked(true);

	makeShow();

}

void Scale::sizeChanged() {

	if(keepratio->isChecked() && !ignoreSizeChange) {

		QString obj = ((CustomSpinBox *)sender())->objectName();
		if(obj != "w" && obj != "h" && lastEdit != "")
			obj = lastEdit;
		lastEdit = "";

		ignoreSizeChange = true;
		if(obj == "w")
			heightSpin->setValue(widthSpin->value()/widthByHeightRatio);
		else if(obj == "h")
			widthSpin->setValue(heightSpin->value()*widthByHeightRatio);
		ignoreSizeChange = false;

	} else
		lastEdit = ((CustomSpinBox *)sender())->objectName();

}



Scale::~Scale() { }
