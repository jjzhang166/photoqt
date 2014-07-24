#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <QObject>
#include <QRect>
#include <QMap>

// Some global variables
class GlobalVariables {

public:
	GlobalVariables() {

		rotation = 0;

		skipTrayIcon = false;

		windowMaximised = true;

		blocked = false;

		zoomed = false;
		zoomedByMouse = true;
		zoomedImgAtLeastOnce = false;

		systemSh << "Escape" << "Enter" << "Return" << "Ctrl+s" << "Left" << "Right" << "Up" << "Down" << "Alt+Up" << "Alt+Left" << "Alt+Right" << "Alt+1" << "Alt+2" << "Alt+3" << "Alt+4" << "Alt+5" << "Ctrl+Tab" << "Ctrl+Shift+Tab";

		flipVer = false;
		flipHor = false;

		exifRead = false;

		zoomToActualSize = false;

		restoringFromTrayNoResize = 0;

		startupMessageInstallUpdateShown = 0;

		store_rotation.clear();
		store_flipHor.clear();
		store_flipVer.clear();

		fileBeforeEmptyFilter = "";

	}

	bool verbose;

	// The current file
	QString currentfile;

	// Are we ignoring the tray icon and just exit
	bool skipTrayIcon;

	// The current rotation of the image
	int rotation;

	// Is current image zoomed? By Mouse or key combo?
	bool zoomed;
	bool zoomedByMouse;

	// Is a widget opened?
	bool blocked;

	// This list holds all shortcuts that are to be set up as system shortcuts
	QStringList systemSh;

	// Was the current image zoomed in any way
	bool zoomedImgAtLeastOnce;

	// Is the current image flipped?
	bool flipVer;
	bool flipHor;

	// The "read" is in past tense
	bool exifRead;

	// Currently zoomed to actual size
	bool zoomToActualSize;

	// When restoring PhotoQt from system tray, it emits a resizeEvent -> ignore it
	uint restoringFromTrayNoResize;

	// Is a slideshow running
	bool slideshowRunning;

	// Is the startup message shown (displayed after update/fresh install)
	int startupMessageInstallUpdateShown;

	bool windowMaximised;

	// These two QMap's store the rotation/flipping of each image while it's being displayed (nothing changed permanently)
	QMap<QString,int> store_rotation;
	QMap<QString,bool> store_flipHor;
	QMap<QString,bool> store_flipVer;

	QString fileBeforeEmptyFilter;

};

#endif // GLOBALVARIABLES_H
