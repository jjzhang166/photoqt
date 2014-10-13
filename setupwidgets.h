/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
		scaleimage = false;
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
