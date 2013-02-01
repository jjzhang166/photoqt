#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QObject>

class Wallpaper {

public:
	Wallpaper();
	~Wallpaper();

	void setWallpaper(QString filename);

private:
	void detectWM();

	void setKDE();
	void setGNOME();
	void setXFCE();
	void setOTHER();

};

#endif // WALLPAPER_H
