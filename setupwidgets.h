#ifndef SETUP_WIDGETS_H
#define SETUP_WIDGETS_H

// Some global variables
class SetupWidgets {

public:

	// Initially all these widgets aren't set up
	SetupWidgets() {
		filehandling = false;
		startup = false;
		about = false;
		wallpaper = false;
		filterimages = false;
		slideshow = false;
		slideshowbar = false;
		settings = false;
		errormagick = false;
		menu = false;
	}

	bool menu;
	bool filehandling;
	bool startup;
	bool about;
	bool wallpaper;
	bool filterimages;
	bool slideshow;
	bool slideshowbar;
	bool settings;
	bool errormagick;

};


#endif // SETUP_WIDGETS_H
