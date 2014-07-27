#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include "../customelements/customlabel.h"
#include "../customelements/customline.h"
#include "../widgets/mywidget.h"

#include <QDesktopServices>
#include <QUrl>

class About : public MyWidget {

	Q_OBJECT

public:
	About(QWidget *parent = 0);

	void setLicense(QString version);

private:
	// A button to close the widget
	CustomPushButton *close;

	// The license at top
	CustomLabel *license;

private slots:
	void openWebsite();

};

#endif // ABOUTWIDGET_H
