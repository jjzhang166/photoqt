#ifndef FILTERIMAGESSETUP_H
#define FILTERIMAGESSETUP_H

#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../widgets/mywidget.h"

class FilterImagesSetup : public MyWidget {

	Q_OBJECT

public:
	// Widget to set an image filter (by extension)
	FilterImagesSetup(QWidget *parent = 0);
	~FilterImagesSetup();

	CustomPushButton *enter;
	CustomPushButton *cancel;

private:
	// The central widget containing all the content
	QWidget *center;

	CustomLineEdit *edit;

private slots:
	void okayClicked();

	void editTextChanged();

signals:
	// Set/Remove filter
	void setFilter(QStringList);
	void removeFilter();

};


#endif // FILTERIMAGESSETUP_H
