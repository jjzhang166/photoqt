#include "wallpaper.h"

Wallpaper::Wallpaper(QWidget *parent) : QWidget(parent) {

	filename = "";

	kdeUnsupported = new CustomConfirm("KDE not (yet) supported","Currenlty, it's not possible to change the Wallpaper in KDE other than from the KDE settings dialogs. This is said to change in KDE 4.10 (though I don't know for certain).","Oh, okay...","",QSize(450,200),this->parentWidget());
	kdeUnsupported->showBorder("red",2);

}

void Wallpaper::setWallpaper(QString file) {

	if(file != "") {

		filename = file;
		detectWM();

	}

}


void Wallpaper::detectWM() {

	if(QString(getenv("KDE_FULL_SESSION")).toLower() == "true")
		setKDE();
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "gnome" || QString(getenv("XDG_CURRENT_DESKTOP")).toLower() == "unity" || QString(getenv("DESKTOP_SESSION")).toLower() == "ubuntu")
		setGNOME();
	else if(QString(getenv("DESKTOP_SESSION")).toLower() == "xfce4")
		setXFCE();

}


void Wallpaper::setKDE() {

	qDebug() << "KDE";

	kdeUnsupported->animate();

}

void Wallpaper::setGNOME() {

	qDebug() << "GNOME:" << QString("gsettings set org.gnome.desktop.background picture-uri file://%1").arg(filename);

	QProcess proc;
	proc.execute(QString("gsettings set org.gnome.desktop.background picture-uri file://%1").arg(filename));

}

void Wallpaper::setXFCE() {

	qDebug() << "XFCE";

	QProcess proc;
	if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/image-path -s \"%1\"").arg(filename)) == 1) {
		qDebug() << "Property not found, trying to create it";
		if(proc.execute("xfconf-query -c xfce4-desktop -n -p /backdrop/screen0/monitor0/image-path -t string -s ''") == 1)
			qDebug() << "ERROR: Unable to create property '/backdrop/screen0/monitor0/image-path'!";
		else {
			if(proc.execute(QString("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/image-path -s \"%1\"").arg(filename)) == 1)
				qDebug() << "ERROR: Unable to create property '/backdrop/screen0/monitor0/image-path' needed to set background!";
			else
				qDebug() << "Image set...";
		}
	} else
		qDebug() << "Property found, image set...";

}

void Wallpaper::setOTHER() {

}










Wallpaper::~Wallpaper() { }
