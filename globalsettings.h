#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QtDebug>
#include <QColor>
#include <QStringList>

#if defined(Q_WS_X11)
#include <QX11Info>
#endif

// All the global settings
class GlobalSettings : public QObject {

	Q_OBJECT

public:
	// The current version
	QString version;

	// The language selected and available languages
	QString language;
	QStringList availableLang;

	// Is composite enabled?
	bool composite;

	// Set the background color
	int bgColorRed;
	int bgColorGreen;
	int bgColorBlue;
	int bgColorAlpha;

	// Background image in use?
	bool backgroundImageScreenshot;
	bool backgroundImageUse;
	QString backgroundImagePath;
	bool backgroundImageScale;
	bool backgroundImageStretch;
	bool backgroundImageCenter;

	// Hide to tray icon?
	bool trayicon;
	// Smooth Transition for changing images
	int transition;
	// Loop through folder?
	bool loopthroughfolder;
	// Menu sensitivity
	int menusensitivity;
	// Close on click on background exits?
	bool closeongrey;
	// Border around big image
	int borderAroundImg;

	// Are quickinfos hidden?
	bool hidecounter;
	bool hidefilepathshowfilename;
	bool hidefilename;
	bool hidex;

	// Some settings of the slideshow
	int slideShowTime;
	int slideShowTransition;
	QString slideShowMusicFile;
	bool slideShowHideQuickinfo;

	// The Size of the thumbnail squares
	int thumbnailsize;
	// Thumbnails at the bottom or top?
	QString thumbnailposition;
	// Enable thumbnail cache
	bool thumbnailcache;
	// Are files used for caching (use database if false)
	bool thbcachefile;
	// Border between thumbnails
	int thumbnailBorderAround;
	// Lift hovered/selected thumbnails by x pixels
	int thumbnailLiftUp;
	// Are the thumbnails fading out or always visible?
	bool thumbnailKeepVisible;
	// Don't load actual thumbnail but just display the filename
	bool thumbnailFilenameInstead;
	int thumbnailFilenameInsteadFontSize;
	// Thumbnails can be disabled altogether
	bool thumbnailDisable;

	// The currently known filetypes
	QString knownFileTypes;

	// Some exif settings
	bool exifenablemousetriggering;
	QString exifrotation;
	QString exifgpsmapservice;
	int exiffontsize;

	// Which Exif data is shown?
	bool exiffilename;
	bool exiffiletype;
	bool exiffilesize;
	bool exifdimensions;
	bool exifmake;
	bool exifmodel;
	bool exifsoftware;
	bool exifphototaken;
	bool exifexposuretime;
	bool exifflash;
	bool exifiso;
	bool exifscenetype;
	bool exifflength;
	bool exiffnumber;
	bool exiflightsource;
	bool exifgps;

	// Create a map that is given to all subwidgets
	QMap<QString,QVariant> toSignalOut() {

		QMap<QString,QVariant> map;

		map.insert("Version",version);
		map.insert("Language",language);

		map.insert("KnownFileTypes",knownFileTypes);

		map.insert("Composite",composite);
		map.insert("BgColorRed",bgColorRed);
		map.insert("BgColorGreen",bgColorGreen);
		map.insert("BgColorBlue",bgColorBlue);
		map.insert("BgColorAlpha",bgColorAlpha);

		map.insert("BackgroundImageScreenshot",backgroundImageScreenshot);
		map.insert("BackgroundImageUse",backgroundImageUse);
		map.insert("BackgroundImagePath",backgroundImagePath);
		map.insert("BackgroundImageScale",backgroundImageScale);
		map.insert("BackgroundImageStretch",backgroundImageStretch);
		map.insert("BackgroundImageCenter",backgroundImageCenter);

		map.insert("TrayIcon",trayicon);
		map.insert("Transition",transition);
		map.insert("LoopThroughFolder",loopthroughfolder);
		map.insert("MenuSensitivity",menusensitivity);
		map.insert("CloseOnGrey",closeongrey);
		map.insert("BorderAroundImg",borderAroundImg);

		map.insert("HideCounter",hidecounter);
		map.insert("HideFilepathShowFilename",hidefilepathshowfilename);
		map.insert("HideFilename",hidefilename);
		map.insert("HideX",hidex);

		map.insert("ThumbnailSize",thumbnailsize);
		map.insert("ThumbnailPosition",thumbnailposition);
		map.insert("ThumbnailCache",thumbnailcache);
		map.insert("ThbCacheFile",thbcachefile);
		map.insert("ThumbnailBorderAround",thumbnailBorderAround);
		map.insert("ThumbnailLiftUp",thumbnailLiftUp);
		map.insert("ThumbnailKeepVisible",thumbnailKeepVisible);
		map.insert("ThumbnailFilenameInstead",thumbnailFilenameInstead);
		map.insert("ThumbnailFilenameInsteadFontSize",thumbnailFilenameInsteadFontSize);
		map.insert("ThumbnailDisable",thumbnailDisable);

		map.insert("SlideShowTime",slideShowTime);
		map.insert("SlideShowTransition",slideShowTransition);
		map.insert("SlideShowMusicFile",slideShowMusicFile);
		map.insert("SlideShowHideQuickinfo",slideShowHideQuickinfo);

		map.insert("ExifEnableMouseTriggering",exifenablemousetriggering);
		map.insert("ExifFontSize",exiffontsize);
		map.insert("ExifFilename",exiffilename);
		map.insert("ExifFiletype",exiffiletype);
		map.insert("ExifFilesize",exiffilesize);
		map.insert("ExifDimensions",exifdimensions);
		map.insert("ExifMake",exifmake);
		map.insert("ExifModel",exifmodel);
		map.insert("ExifSoftware",exifsoftware);
		map.insert("ExifPhotoTaken",exifphototaken);
		map.insert("ExifExposureTime",exifexposuretime);
		map.insert("ExifFlash",exifflash);
		map.insert("ExifIso",exifiso);
		map.insert("ExifSceneType",exifscenetype);
		map.insert("ExifFLength",exifflength);
		map.insert("ExifFNumber",exiffnumber);
		map.insert("ExifLightSource",exiflightsource);
		map.insert("ExifGps",exifgps);
		map.insert("ExifRotation",exifrotation);
		map.insert("ExifGPSMapService",exifgpsmapservice);

		return map;

	}

	// Set the default settings
	void setDefault() {

		version = "0.9beta";

		knownFileTypes = "*.bmp,*.gif,*.tif,*.jpg,*.jpeg,*.jpeg2000,*.png,*.pbm,*.pgm,*.ppm,*.xbm,*.xpm,*.tiff";

		language = "en";
		bgColorRed = 0;
		bgColorGreen = 0;
		bgColorBlue = 0;
		bgColorAlpha = 190;

		backgroundImageScreenshot = false;
		backgroundImageUse = false;
		backgroundImagePath = "";
		backgroundImageScale = false;
		backgroundImageStretch = false;
		backgroundImageCenter = false;

		composite = false;
		trayicon = false;
		transition = 0;
		loopthroughfolder = true;
		menusensitivity = 6;
		closeongrey = false;
		borderAroundImg = 5;

		hidecounter = false;
		hidefilepathshowfilename = true;
		hidefilename = false;
		hidex = false;

		thumbnailsize = 80;
		thumbnailposition = "Bottom";
		thumbnailcache = true;
		thbcachefile = false;
		thumbnailBorderAround = 0;
		thumbnailLiftUp = 10;
		thumbnailKeepVisible = false;
		thumbnailDisable = false;

		thumbnailFilenameInstead = false;
		thumbnailFilenameInsteadFontSize = 8;

		slideShowTime = 5;
		slideShowTransition = 4;
		slideShowMusicFile = "";
		slideShowHideQuickinfo = true;


		exifenablemousetriggering = true;
		exiffontsize = 8;
		exiffilename = true;
		exiffiletype = true;
		exiffilesize = true;
		exifdimensions = true;
		exifmake = true;
		exifmodel = true;
		exifsoftware = true;
		exifphototaken = true;
		exifexposuretime = true;
		exifflash = true;
		exifiso = true;
		exifscenetype = true;
		exifflength = true;
		exiffnumber = true;
		exiflightsource = true;
		exifgps = true;
		exifrotation = "Never";
		exifgpsmapservice = "bing.com/maps";
	}

	// Read the current settings
	void readSettings() {

		setDefault();

		QFile file(QDir::homePath() + "/.photo/settings");

		if(!file.open(QIODevice::ReadOnly))

			qDebug() << "ERROR reading settings";

		else {

			// Read file
			QTextStream in(&file);
			QString all = in.readAll();

			if(all.contains("Language="))
				language = all.split("Language=").at(1).split("\n").at(0);

			if(all.contains("KnownFileTypes=")) {
				QString temp = all.split("KnownFileTypes=").at(1).split("\n").at(0);
				if(temp.length() > 3)
					knownFileTypes = temp;
			}

			if(all.contains("Composite=1"))
				composite = true;
			else if(all.contains("Composite=0"))
				composite = false;
			else {
#if defined(Q_WS_X11)

				if(QX11Info::isCompositingManagerRunning())
					composite = true;
				else
					composite = false;

#endif
			}

			if(all.contains("BgColorRed="))
				bgColorRed = all.split("BgColorRed=").at(1).split("\n").at(0).toInt();
			if(all.contains("BgColorGreen="))
				bgColorGreen = all.split("BgColorGreen=").at(1).split("\n").at(0).toInt();
			if(all.contains("BgColorBlue="))
				bgColorBlue = all.split("BgColorBlue=").at(1).split("\n").at(0).toInt();
			if(all.contains("BgColorAlpha="))
				bgColorAlpha = all.split("BgColorAlpha=").at(1).split("\n").at(0).toInt();

			if(all.contains("BackgroundImageScreenshot="))
				backgroundImageScreenshot = bool(all.split("BackgroundImageScreenshot=").at(1).split("\n").at(0).toInt());

			if(all.contains("BackgroundImagePath="))
				backgroundImagePath = all.split("BackgroundImagePath=").at(1).split("\n").at(0);
			if(all.contains("BackgroundImageUse="))
				backgroundImageUse = bool(all.split("BackgroundImageUse=").at(1).split("\n").at(0).toInt());
			if(all.contains("BackgroundImageScale="))
				backgroundImageScale = bool(all.split("BackgroundImageScale=").at(1).split("\n").at(0).toInt());
			if(all.contains("BackgroundImageStretch="))
				backgroundImageStretch = bool(all.split("BackgroundImageStretch=").at(1).split("\n").at(0).toInt());
			if(all.contains("BackgroundImageCenter="))
				backgroundImageCenter = bool(all.split("BackgroundImageCenter=").at(1).split("\n").at(0).toInt());


			if(all.contains("TrayIcon=1"))
				trayicon = true;
			else if(all.contains("TrayIcon=0"))
				trayicon = false;

			if(all.contains("Transition="))
				transition = all.split("Transition=").at(1).split("\n").at(0).toInt();

			if(all.contains("LoopThroughFolder=1"))
				loopthroughfolder = true;
			else if(all.contains("LoopThroughFolder=0"))
				loopthroughfolder = false;

			if(all.contains("MenuSensitivity="))
				menusensitivity = all.split("MenuSensitivity=").at(1).split("\n").at(0).toInt();

			if(all.contains("CloseOnGrey=1"))
				closeongrey = true;
			else if(all.contains("CloseOnGrey=0"))
				closeongrey = false;

			if(all.contains("BorderAroundImg="))
				borderAroundImg = all.split("BorderAroundImg=").at(1).split("\n").at(0).toInt();

			if(all.contains("HideCounter=1"))
				hidecounter = true;
			else if(all.contains("HideCounter=0"))
				hidecounter = false;

			if(all.contains("HideFilepathShowFilename=1"))
				hidefilepathshowfilename = true;
			else if(all.contains("HideFilepathShowFilename=0"))
				hidefilepathshowfilename = false;

			if(all.contains("HideFilename=1"))
				hidefilename = true;
			else if(all.contains("HideFilename=0"))
				hidefilename = false;

			if(all.contains("HideX=1"))
				hidex = true;
			else if(all.contains("HideX=0"))
				hidex = false;

			if(all.contains("ThumbnailSize="))
				thumbnailsize = all.split("ThumbnailSize=").at(1).split("\n").at(0).toInt();

			if(all.contains("ThumbnailPosition="))
				thumbnailposition = all.split("ThumbnailPosition=").at(1).split("\n").at(0);

			if(all.contains("ThumbnailCache=1"))
				thumbnailcache = true;
			else if(all.contains("ThumbnailCache=0"))
				thumbnailcache = false;

			if(all.contains("ThbCacheFile=1"))
				thbcachefile = true;
			else if(all.contains("ThbCacheFile=0"))
				thbcachefile = false;

			if(all.contains("ThumbnailBorderAround="))
				thumbnailBorderAround = all.split("ThumbnailBorderAround=").at(1).split("\n").at(0).toInt();

			if(all.contains("ThumbnailLiftUp="))
				thumbnailLiftUp = all.split("ThumbnailLiftUp=").at(1).split("\n").at(0).toInt();

			if(all.contains("ThumbnailKeepVisible=1"))
				thumbnailKeepVisible = true;
			else if(all.contains("ThumbnailKeepVisible=0"))
				thumbnailKeepVisible = false;

			if(all.contains("ThumbnailFilenameInstead=1"))
				thumbnailFilenameInstead = true;
			else if(all.contains("ThumbnailFilenameInstead=0"))
				thumbnailFilenameInstead = false;

			if(all.contains("ThumbnailFilenameInsteadFontSize="))
				thumbnailFilenameInsteadFontSize = all.split("ThumbnailFilenameInsteadFontSize=").at(1).split("\n").at(0).toInt();

			if(all.contains("ThumbnailDisable=1"))
				thumbnailDisable = true;
			else if(all.contains("ThumbnailDisable=0"))
				thumbnailDisable = false;


			if(all.contains("SlideShowTime="))
				slideShowTime = all.split("SlideShowTime=").at(1).split("\n").at(0).toInt();

			if(all.contains("SlideShowTransition="))
				slideShowTransition = all.split("SlideShowTransition=").at(1).split("\n").at(0).toInt();

			if(all.contains("SlideShowMusicFile="))
				slideShowMusicFile = all.split("SlideShowMusicFile=").at(1).split("\n").at(0).trimmed();

			if(all.contains("SlideShowHideQuickinfo="))
				slideShowHideQuickinfo = bool(all.split("SlideShowHideQuickinfo=").at(1).split("\n").at(0).toInt());


			if(all.contains("ExifEnableMouseTriggering=1"))
				exifenablemousetriggering = true;
			else if(all.contains("ExifEnableMouseTriggering=0"))
				exifenablemousetriggering = false;

			if(all.contains("ExifFontSize="))
				exiffontsize = all.split("ExifFontSize=").at(1).split("\n").at(0).toInt();

			if(all.contains("ExifFilename=1"))
				exiffilename = true;
			else if(all.contains("ExifFilename=0"))
				exiffilename = false;

			if(all.contains("ExifFiletype=1"))
				exiffiletype = true;
			else if(all.contains("ExifFiletype=0"))
				exiffiletype = false;

			if(all.contains("ExifFilesize=1"))
				exiffilesize = true;
			else if(all.contains("ExifFilesize=0"))
				exiffilesize = false;

			if(all.contains("ExifDimensions=1"))
				exifdimensions = true;
			else if(all.contains("ExifDimensions=0"))
				exifdimensions = false;

			if(all.contains("ExifMake=1"))
				exifmake = true;
			else if(all.contains("ExifMake=0"))
				exifmake = false;

			if(all.contains("ExifModel=1"))
				exifmodel = true;
			else if(all.contains("ExifModel=0"))
				exifmodel = false;

			if(all.contains("ExifSoftware=1"))
				exifsoftware = true;
			else if(all.contains("ExifSoftware=0"))
				exifsoftware = false;

			if(all.contains("ExifPhotoTaken=1"))
				exifphototaken = true;
			else if(all.contains("ExifPhotoTaken=0"))
				exifphototaken = false;

			if(all.contains("ExifExposureTime=1"))
				exifexposuretime = true;
			else if(all.contains("ExifExposureTime=0"))
				exifexposuretime = false;

			if(all.contains("ExifFlash=1"))
				exifflash = true;
			else if(all.contains("ExifFlash=0"))
				exifflash = false;

			if(all.contains("ExifIso=1"))
				exifiso = true;
			else if(all.contains("ExifIso=0"))
				exifiso = false;

			if(all.contains("ExifSceneType=1"))
				exifscenetype = true;
			else if(all.contains("ExifSceneType=0"))
				exifscenetype = false;

			if(all.contains("ExifFLength=1"))
				exifflength = true;
			else if(all.contains("ExifFLength=0"))
				exifflength = false;

			if(all.contains("ExifFNumber=1"))
				exiffnumber = true;
			else if(all.contains("ExifFNumber=0"))
				exiffnumber = false;

			if(all.contains("ExifLightSource=1"))
				exiflightsource = true;
			else if(all.contains("ExifLightSource=0"))
				exiflightsource = false;

			if(all.contains("ExifGps=1"))
				exifgps = true;
			else if(all.contains("ExifGps=0"))
				exifgps = false;

			if(all.contains("ExifRotation="))
				exifrotation = all.split("ExifRotation=").at(1).split("\n").at(0);

			if(all.contains("ExifGPSMapService="))
				exifgpsmapservice = all.split("ExifGPSMapService=").at(1).split("\n").at(0);

			file.close();

		}

	}

	// Save settings
	void saveSettings(QMap<QString,bool> applySet = QMap<QString,bool>()) {

		QFile file(QDir::homePath() + "/.photo/settings");

		if(!file.open(QIODevice::ReadWrite))

			qDebug() << "ERROR saving settings";

		else {

			file.close();
			file.remove();
			file.open(QIODevice::ReadWrite);

			QTextStream out(&file);

			QString cont = "Version=" + version + "\n";
			cont += QString("Language=%1\n").arg(language);
			cont += QString("KnownFileTypes=%1\n").arg(knownFileTypes);

			cont += "\n[Look]\n";

			cont += QString("Composite=%1\n").arg(int(composite));
			cont += QString("BgColorRed=%1\n").arg(bgColorRed);
			cont += QString("BgColorGreen=%1\n").arg(bgColorGreen);
			cont += QString("BgColorBlue=%1\n").arg(bgColorBlue);
			cont += QString("BgColorAlpha=%1\n").arg(bgColorAlpha);
			cont += QString("BackgroundImageScreenshot=%1\n").arg(backgroundImageScreenshot);
			cont += QString("BackgroundImageUse=%1\n").arg(backgroundImageUse);
			cont += QString("BackgroundImagePath=%1\n").arg(backgroundImagePath);
			cont += QString("BackgroundImageScale=%1\n").arg(backgroundImageScale);
			cont += QString("BackgroundImageStretch=%1\n").arg(backgroundImageStretch);
			cont += QString("BackgroundImageCenter=%1\n").arg(backgroundImageCenter);

			cont += "\n[Behaviour]\n";

			cont += QString("TrayIcon=%1\n").arg(int(trayicon));
			cont += QString("Transition=%1\n").arg(transition);
			cont += QString("LoopThroughFolder=%1\n").arg(int(loopthroughfolder));
			cont += QString("MenuSensitivity=%1\n").arg(menusensitivity);
			cont += QString("CloseOnGrey=%1\n").arg(int(closeongrey));
			cont += QString("BorderAroundImg=%1\n").arg(borderAroundImg);

			cont += "\n[Path]\n";

			cont += QString("HideCounter=%1\n").arg(int(hidecounter));
			cont += QString("HideFilepathShowFilename=%1\n").arg(int(hidefilepathshowfilename));
			cont += QString("HideFilename=%1\n").arg(int(hidefilename));
			cont += QString("HideX=%1\n").arg(int(hidex));

			cont += "\n[Thumbnail]\n";

			cont += QString("ThumbnailSize=%1\n").arg(thumbnailsize);
			cont += QString("ThumbnailPosition=%1\n").arg(thumbnailposition);
			cont += QString("ThumbnailCache=%1\n").arg(int(thumbnailcache));
			cont += QString("ThbCacheFile=%1\n").arg(int(thbcachefile));
			cont += QString("ThumbnailBorderAround=%1\n").arg(thumbnailBorderAround);
			cont += QString("ThumbnailLiftUp=%1\n").arg(thumbnailLiftUp);
			cont += QString("ThumbnailKeepVisible=%1\n").arg(thumbnailKeepVisible);
			cont += QString("ThumbnailFilenameInstead=%1\n").arg(int(thumbnailFilenameInstead));
			cont += QString("ThumbnailFilenameInsteadFontSize=%1\n").arg(thumbnailFilenameInsteadFontSize);
			cont += QString("ThumbnailDisable=%1\n").arg(thumbnailDisable);

			cont += "\n[Slideshow]\n";

			cont += QString("SlideShowTime=%1\n").arg(slideShowTime);
			cont += QString("SlideShowTransition=%1\n").arg(slideShowTransition);
			cont += QString("SlideShowMusicFile=%1\n").arg(slideShowMusicFile);
			cont += QString("SlideShowHideQuickinfo=%1\n").arg(int(slideShowHideQuickinfo));

			cont += "\n[Exif]\n";

			cont += QString("ExifEnableMouseTriggering=%1\n").arg(int(exifenablemousetriggering));
			cont += QString("ExifFontSize=%1\n").arg(exiffontsize);
			cont += QString("ExifFilename=%1\n").arg(int(exiffilename));
			cont += QString("ExifFiletype=%1\n").arg(int(exiffiletype));
			cont += QString("ExifFilesize=%1\n").arg(int(exiffilesize));
			cont += QString("ExifDimensions=%1\n").arg(int(exifdimensions));
			cont += QString("ExifMake=%1\n").arg(int(exifmake));
			cont += QString("ExifModel=%1\n").arg(int(exifmodel));
			cont += QString("ExifSoftware=%1\n").arg(int(exifsoftware));
			cont += QString("ExifPhotoTaken=%1\n").arg(int(exifphototaken));
			cont += QString("ExifExposureTime=%1\n").arg(int(exifexposuretime));
			cont += QString("ExifFlash=%1\n").arg(int(exifflash));
			cont += QString("ExifIso=%1\n").arg(int(exifiso));
			cont += QString("ExifSceneType=%1\n").arg(int(exifscenetype));
			cont += QString("ExifFLength=%1\n").arg(int(exifflength));
			cont += QString("ExifFNumber=%1\n").arg(int(exiffnumber));
			cont += QString("ExifLightSource=%1\n").arg(int(exiflightsource));
			cont += QString("ExifGps=%1\n").arg(int(exifgps));
			cont += QString("ExifRotation=%1\n").arg(exifrotation);
			cont += QString("ExifGPSMapService=%1\n").arg(exifgpsmapservice);

			out << cont;
			file.close();

			// Send update out
			emit updateSettingsData(toSignalOut());
			emit updateSettingsToApply(applySet);

		}

	}

public slots:
	// The settings have been updated (take over new values)
	void settingsUpdated(QMap<QString,QVariant> changedSet) {

		QMap<QString,bool> applySet;

		applySet.insert("background",false);
		applySet.insert("menu",false);
		applySet.insert("quickinfo",false);
		applySet.insert("redrawimg",false);
		applySet.insert("thumb",false);
		applySet.insert("exif",false);
		applySet.insert("open",false);

		if(changedSet.keys().contains("Version"))
			version = changedSet.value("Version").toString();

		if(changedSet.keys().contains("KnownFileTypes"))
			knownFileTypes = changedSet.value("KnownFileTypes").toString();;

		if(changedSet.keys().contains("Language"))
			language = changedSet.value("Language").toString();


		if(changedSet.keys().contains("BgColorRed")) {
			bgColorRed = changedSet.value("BgColorRed").toInt();
			applySet["background"] = true;
		}
		if(changedSet.keys().contains("BgColorGreen")) {
			bgColorGreen = changedSet.value("BgColorGreen").toInt();
			applySet["background"] = true;
		}
		if(changedSet.keys().contains("BgColorBlue")) {
			bgColorBlue = changedSet.value("BgColorBlue").toInt();
			applySet["background"] = true;
		}
		if(changedSet.keys().contains("BgColorAlpha")) {
			bgColorAlpha = changedSet.value("BgColorAlpha").toInt();
			applySet["background"] = true;
		}

		if(changedSet.keys().contains("BackgroundImageScreenshot")) {
			backgroundImageScreenshot = changedSet.value("BackgroundImageScreenshot").toBool();
			applySet["background"] = true;
		}

		if(changedSet.keys().contains("BackgroundImagePath")) {
			backgroundImagePath = changedSet.value("BackgroundImagePath").toString();
			applySet["background"] = true;
		}
		if(changedSet.keys().contains("BackgroundImageUse")) {
			backgroundImageUse = changedSet.value("BackgroundImageUse").toBool();
			applySet["background"] = true;
		}
		if(changedSet.keys().contains("BackgroundImageScale")) {
			backgroundImageScale = changedSet.value("BackgroundImageScale").toBool();
			applySet["background"] = true;
		}
		if(changedSet.keys().contains("BackgroundImageStretch")) {
			backgroundImageStretch = changedSet.value("BackgroundImageStretch").toBool();
			applySet["background"] = true;
		}
		if(changedSet.keys().contains("BackgroundImageCenter")) {
			backgroundImageCenter = changedSet.value("BackgroundImageCenter").toBool();
			applySet["background"] = true;
		}

		if(changedSet.keys().contains("Composite")) {
			composite = changedSet.value("Composite").toBool();
			applySet["background"] = true;
		}


		if(changedSet.keys().contains("TrayIcon"))
			trayicon = changedSet.value("TrayIcon").toBool();

		if(changedSet.keys().contains("Transition"))
			transition = changedSet.value("Transition").toInt();

		if(changedSet.keys().contains("LoopThroughFolder"))
			loopthroughfolder = changedSet.value("LoopThroughFolder").toBool();

		if(changedSet.keys().contains("MenuSensitivity")) {
			menusensitivity = changedSet.value("MenuSensitivity").toInt();
			applySet["menu"] = true;
		}
		if(changedSet.keys().contains("CloseOnGrey"))
			closeongrey = changedSet.value("CloseOnGrey").toBool();

		if(changedSet.keys().contains("BorderAroundImg")) {
			borderAroundImg = changedSet.value("BorderAroundImg").toInt();
			applySet["redrawimg"] = true;
		}

		if(changedSet.keys().contains("HideCounter")) {
			hidecounter = changedSet.value("HideCounter").toBool();
			applySet["quickinfo"] = true;
		}
		if(changedSet.keys().contains("HideFilename")) {
			hidefilename = changedSet.value("HideFilename").toBool();
			applySet["quickinfo"] = true;
		}
		if(changedSet.keys().contains("HideFilepathShowFilename")) {
			hidefilepathshowfilename = changedSet.value("HideFilepathShowFilename").toBool();
			applySet["quickinfo"] = true;
		}
		if(changedSet.keys().contains("HideX")) {
			hidex = changedSet.value("HideX").toBool();
			applySet["quickinfo"] = true;
		}

		if(changedSet.keys().contains("ThumbnailSize")) {
			thumbnailsize = changedSet.value("ThumbnailSize").toInt();
			applySet["thumb"] = true;
		}
		if(changedSet.keys().contains("ThumbnailPosition")) {
			thumbnailposition = changedSet.value("ThumbnailPosition").toString();
			applySet["thumb"] = true;
		}
		if(changedSet.keys().contains("ThumbnailCache"))
			thumbnailcache = changedSet.value("ThumbnailCache").toBool();
		if(changedSet.keys().contains("ThbCacheFile"))
			thbcachefile = changedSet.value("ThbCacheFile").toBool();
		if(changedSet.keys().contains("ThumbnailBorderAround")) {
			thumbnailBorderAround = changedSet.value("ThumbnailBorderAround").toInt();
			applySet["thumb"] = true;
		}
		if(changedSet.keys().contains("ThumbnailLiftUp")) {
			thumbnailLiftUp = changedSet.value("ThumbnailLiftUp").toInt();
			applySet["thumb"] = true;
		}
		if(changedSet.keys().contains("ThumbnailKeepVisible")) {
			thumbnailKeepVisible = changedSet.value("ThumbnailKeepVisible").toBool();
			applySet["thumb"] = true;
			applySet["redrawimg"] = true;
		}
		if(changedSet.keys().contains("ThumbnailFilenameInstead")) {
			thumbnailFilenameInstead = changedSet.value("ThumbnailFilenameInstead").toBool();
			applySet["thumb"] = true;
		}
		if(changedSet.keys().contains("ThumbnailFilenameInsteadFontSize")) {
			thumbnailFilenameInsteadFontSize = changedSet.value("ThumbnailFilenameInsteadFontSize").toInt();
			applySet["thumb"] = true;
		}
		if(changedSet.keys().contains("ThumbnailDisable")) {
			thumbnailDisable = changedSet.value("ThumbnailDisable").toBool();
			applySet["thumb"] = true;
		}


		if(changedSet.keys().contains("SlideShowTime"))
			slideShowTime = changedSet.value("SlideShowTime").toInt();

		if(changedSet.keys().contains("SlideShowTransition"))
			slideShowTransition = changedSet.value("SlideShowTransition").toInt();

		if(changedSet.keys().contains("SlideShowMusicFile"))
			slideShowMusicFile = changedSet.value("SlideShowMusicFile").toString();

		if(changedSet.keys().contains("SlideShowHideQuickinfo"))
			slideShowHideQuickinfo = changedSet.value("SlideShowHideQuickinfo").toBool();


		if(changedSet.keys().contains("ExifEnableMouseTriggering")) {
			exifenablemousetriggering = changedSet.value("ExifEnableMouseTriggering").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifFontSize")) {
			exiffontsize = changedSet.value("ExifFontSize").toInt();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifFilename")) {
			exiffilename = changedSet.value("ExifFilename").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifFiletype")) {
			exiffiletype = changedSet.value("ExifFiletype").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifFilesize")) {
			exiffilesize = changedSet.value("ExifFilesize").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifDimensions")) {
			exifdimensions = changedSet.value("ExifDimensions").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifMake")) {
			exifmake = changedSet.value("ExifMake").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifModel")) {
			exifmodel = changedSet.value("ExifModel").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifSoftware")) {
			exifsoftware = changedSet.value("ExifSoftware").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifPhotoTaken")) {
			exifphototaken = changedSet.value("ExifPhotoTaken").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifExposureTime")) {
			exifexposuretime = changedSet.value("ExifExposureTime").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifFlash")) {
			exifflash = changedSet.value("ExifFlash").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifIso")) {
			exifiso = changedSet.value("ExifIso").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifSceneType")) {
			exifscenetype = changedSet.value("ExifSceneType").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifFLength")) {
			exifflength = changedSet.value("ExifFLength").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifFNumber")) {
			exiffnumber = changedSet.value("ExifFNumber").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifLightSource")) {
			exiflightsource = changedSet.value("ExifLightSource").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifGps")) {
			exifgps = changedSet.value("ExifGps").toBool();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifRotation")) {
			exifrotation = changedSet.value("ExifRotation").toString();
			applySet["exif"] = true;
		}
		if(changedSet.keys().contains("ExifGPSMapService")) {
			exifgpsmapservice = changedSet.value("ExifGPSMapService").toString();
			applySet["exif"] = true;
		}

		saveSettings(applySet);

	}

signals:
	// Settings have been updated
	void updateSettingsData(QMap<QString,QVariant>);
	void updateSettingsToApply(QMap<QString,bool>);

};

#endif // GLOBALSETTINGS_H
