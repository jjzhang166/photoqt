#ifndef WALLPAPER_H
#define WALLPAPER_H

#include "../widgets/mywidget.h"
#include "../customelements/customline.h"

#include <QFileInfo>
#include <QProcess>
#include <QButtonGroup>
#include <QDesktopWidget>
#include <QLabel>

#include "../customelements/custompushbutton.h"
#include "../customelements/customcombobox.h"
#include "../customelements/customradiobutton.h"
#include "../customelements/customcheckbox.h"
#include "../customelements/customlabel.h"

class Wallpaper : public MyWidget {

	Q_OBJECT

public:
	Wallpaper(QMap<QString,QVariant> set, bool v, QWidget *parent = 0);
	~Wallpaper();

	QMap<QString,QVariant> globSet;

	bool verbose;

	void setWallpaper(QString file);
	void dontSetWallpaper();

	void accept();


private:
	CustomPushButton *ok;
	CustomPushButton *cancel;

	CustomComboBox *wm;
	CustomLabel *wmMessage;

	QString filename;
	CustomLabel *filenameLabel;

	QButtonGroup *gnomeButGrp;
	CustomLabel *gnomePicOpsLabel;
	QMap<QString,CustomRadioButton*> gnomePictureOptions;

	QButtonGroup *xfceButGrp;
	CustomLabel *xfcePicOpsLabel;
	QMap<QString,CustomRadioButton*> xfcePictureOptions;


	CustomRadioButton *otherFeh;
	CustomRadioButton *otherNitrogen;
	QMap<QString,CustomRadioButton*> otherFehOptions;
	QMap<QString,CustomRadioButton*> otherNitrogenOptions;
	QButtonGroup *otherFehGrp;
	QButtonGroup *otherNitrogenGrp;


	QMap<int,CustomCheckBox*> wmMonitorSelect;
	CustomLabel *wmMonitorLabel;

	void detectWM();

	void setKDE();
	void setGNOME();
	void setXFCE();
	void setRazor();
	void setOTHER();

private slots:
	void wmSelected();

	void goAheadAndSetWallpaper();

	void swapFehNitrogen();

};

#endif // WALLPAPER_H
