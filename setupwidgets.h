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
		manipulate = false;
		slideshow = false;
		slideshowbar = false;
		cropimage = false;
		settings = false;
		errormagick = false;
		menu = false;
		quicksettings = false;
	}

	bool menu;
	bool filehandling;
	bool startup;
	bool about;
	bool wallpaper;
	bool filterimages;
	bool manipulate;
	bool slideshow;
	bool slideshowbar;
	bool cropimage;
	bool settings;
	bool errormagick;
	bool scaleimage;
	bool quicksettings;

};


#endif // SETUP_WIDGETS_H
