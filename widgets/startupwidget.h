#ifndef STARTUPWIDGET_H
#define STARTUPWIDGET_H

#include "../widgets/mywidget.h"
#include "../customelements/custompushbutton.h"
#include "../customelements/customlabel.h"
#include "../customelements/customline.h"

class StartUpWidget : public MyWidget {

	Q_OBJECT

public:
	StartUpWidget(QWidget *parent = 0);
	~StartUpWidget();

	// There's a little difference between an update message and a fresh install message
	void setUpdateMsg();
	void setInstallMsg();

private:
	// The title and intro can be customised by above functions
	QLabel *title;
	QLabel *customIntro;

signals:
	// Block all function in mainwindow and activate system keys
	void finished();

};

#endif // STARTUPWIDGET_H
