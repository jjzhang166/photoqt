#ifndef FILTERIMAGESSETUP_H
#define FILTERIMAGESSETUP_H

#include "../customelements/customlineedit.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customradiobutton.h"
#include "../widgets/mywidget.h"

class FilterImagesSetup : public MyWidget {

	Q_OBJECT

public:
	// Widget to set an image filter (by extension)
	FilterImagesSetup(QWidget *parent = 0);
	~FilterImagesSetup();

	CustomPushButton *enter;
	CustomPushButton *cancel;

	void makeShow(QString dir) { curDir = dir; edit->setEnabled(true); edit->setFocus(); edit->selectAll(); MyWidget::makeShow(); }

private:
	// The central widget containing all the content
	QWidget *center;

	QString curDir;

	CustomLineEdit *edit;

private slots:
	void disableAndClose() { edit->setEnabled(false); makeHide(); }

	void okayClicked();

	void editTextChanged();

signals:
	// Set/Remove filter
	void setFilter(QString dir, QStringList searchwords);
	void removeFilter();

};


#endif // FILTERIMAGESSETUP_H
