#ifndef CUSTOMCONFIRM_H
#define CUSTOMCONFIRM_H

#include "../customelements/custompushbutton.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlabel.h"
#include "../widgets/mywidget.h"

#include <QTextEdit>

class CustomConfirm : public MyWidget {

	Q_OBJECT

public:
	// This class is used a couple times to have a quick and easy way of displaying a confirmation widget
	CustomConfirm(QString title, QString text, QString yesText = "", QString noText = "", QSize size = QSize(600,400), QString borderColor = "rgb(130,130,130)", QWidget *parent = 0);
	~CustomConfirm();

	// A yes and a no is possible (with custom text of course)
	CustomPushButton *yes;
	CustomPushButton *no;

	// Possible to show checkbox
	CustomCheckBox *dontShowAgain;
	void setDontShowAgain();

signals:
	// Click on the buttons
	void confirmed();
	void rejected();

};

#endif // CUSTOMCONFIRM_H
