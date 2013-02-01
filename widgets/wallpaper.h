#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QObject>
#include <QtDebug>
#include <QProcess>
#include "customconfirm.h"

class Wallpaper : public QWidget {

public:
	Wallpaper(QWidget *parent = 0);
	~Wallpaper();

	void setWallpaper(QString file);

	QString filename;

	CustomConfirm *kdeUnsupported;

private:

	void detectWM();
	void setKDE();
	void setGNOME();
	void setXFCE();
	void setOTHER();

};

#endif // WALLPAPER_H
