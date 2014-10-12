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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "widgets/aboutwidget.h"
#include "graphics/graphicsview.h"
#include "globalvariables.h"
#include "globalsettings.h"
#include "thumbnails/thumbnails.h"
#include "graphics/graphicsitem.h"
#include "settings/settings.h"
#include "widgets/dropdownmenu.h"
#include "widgets/filehandling.h"
#include "widgets/detailswidget.h"
#include "slideshow/slideshowsettings.h"
#include "slideshow/slideshowbar.h"
#include "widgets/startupwidget.h"
#include "wallpaper/wallpaper.h"
#include "graphics/imagereader.h"
#include "setupwidgets.h"
#include "graphics/graphicsviewlay.h"
#include "filterimages/filterimagessetup.h"
#include "filterimages/filterimagesdisplay.h"
#include "manipulation/scale.h"
#include "widgets/quicksettings.h"

#include <QDesktopWidget>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QtSql>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QLabel>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QImageReader>
#include <QTimer>
#include <QShortcut>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QScreen>


class MainWindow : public QMainWindow {
	Q_OBJECT

private:
	void setupTrayIcon();

public:
	explicit MainWindow(QWidget *parent = 0, bool verbose = false);
	~MainWindow();

	// Global variables and settings
	GlobalVariables *globVar;
	GlobalSettings *globSet;

	// Which widgets have been setup
	SetupWidgets *setupWidgets;

	// The timer is started at startup to wait until window is completely set up
	QTimer *startUpTimer;

	// The layout of viewBig
	ViewBigLay *viewBigLay;

private:
	// The current screenshot
	QList<QPixmap> screenshots;
	QLabel *bglabel;

	// The graphicsscenes and -views
	QGraphicsScene sceneBig;
	GraphicsView *viewBig;
	Thumbnails *viewThumbs;

	// The big main graphic item
	GraphicsItem *graphItem;
	GraphicsItem *cropItem;

	// The settings dialog
	Settings *set;

	// All "normal" key shortcuts
	QMap<QString,QList<QVariant> > allKeySHdo;
	QMap<QString,QShortcut* > allKeySH;
	// All system key shortcuts. These function as normal shrtcuts, unless a widget like the settings or open dialogs are open, then they are applied to these widgets. Therefore some shortcuts have always to be set up (just not always connected to a normal shortcut)
	QMap<QString,QList<QVariant> > systemKeySHdo;
	QMap<QString,QShortcut* > systemKeySH;

	// The main dropdown menu top right corner
	DropDownMenu *menu;

	// The tray icon
	QSystemTrayIcon *trayIcon;
	// The tray icon menu
	QMenu *trayIconMenu;

	// The filehandling widget
	FileHandling *filehandling;

	// Showing some about information
	About *about;

	// The two slideshow widgets
	SlideShow *slideshow;
	SlideShowBar *slideshowbar;

	// Showing some exif information
	Details *details;

	// The timer that ensures only one instance of Photo is running a time
	QTimer *globalRunningProgTimer;

	// This widget is shown after an update/fresh install
	StartUpWidget *startup;

	// Wallpaper settings
	Wallpaper *wallpaper;

	// Image filtering
	FilterImagesSetup *filterImagesSetup;
	FilterImagesDisplay *filterImagesDisplay;



	Scale *scaleimage;

	QuickSettings *quickset;



	// imagereader combining QImageReader and GraphicsMagic
	ImageReader *imageReader;


	// Adjust all the geometries (QRects and stuff)
	void adjustGeometries();

	// Show the startup widget (after update/fresh install)
	void showStartupUpdateInstallMsg();

	// Load a new image from the open file dialog
	void loadNewImgFromOpen(QString path, bool hideImageFilterLabel = true);

	// Open a new file
	void openFile();

	// This function slips the current big image vertically/horizontally
	void rotateFlip(bool rotateNotFlipped, QString direction, int rotateSpecificAmount = 0);

	// Set the background of Photo
	void setBackground();

	// Zoom the current image; if a string is set, the boolean is ignored
	void zoom(bool zoomin, QString ignoreBoolean = "");

	// Setup a widget
	void setupWidget(QString what);

private slots:

	// This is called, whenever the settings have changed, and updates all sub-widgets
	// This function is needed together with the updateSettings() function to avoid a crash on startup
	void applySettings(QMap<QString,bool> applySet = QMap<QString,bool>(), bool justApplyAllOfThem = false);

	// If a widget (like open or settings) is opened, all other functions are suspended
	void blockFunc(bool);

	// Draw the big main image
	void drawImage();

	// Some images store a orientation in their exif data
	void getOrientationFromExif(int degree, bool flipHor);

	// Timeout of the global timer (see above)
	void globalRunningProgTimerTimeout();

	// A click on the main image graphicsview
	void gotViewBigClick(QPoint pos);

	// Load a new image in current dir from the thumbnail view
	void loadNewImgFromThumbs(QString path);

	// A menuitem has been clicked
	void menuClicked(QString txt, int close);

	// The mouse has been moved over the main image view (called from viewBig)
	void mouseMoved(int x, int y);

	// Move in the current directory (1=right, 0=left)
	void moveInDirectory(int direction);

	// After a file has been manipulated (renamed, deleted, moved), the current dir is reloaded
	void reloadDir(QString t);

	// Restore default settings
	void restoreDefaultSettings();

	// Filter images
	void removeImageFilter();
	void setImageFilter(QString dir, QStringList filter);

	// After the settings window is closed we might have to call openFile().
	// We cannot use one of the already existing functions, since this call is only to
	// be done if the settings window has been open and closed!
	void settingsClosed() { if(globVar->currentfile == "") openFile(); }

	// Setup the shortcuts
	void setupShortcuts();

	// Called by shortcuts to execute something
	void shortcutDO(QString todo = "", bool mouseSH = false);

	// After the user closed the startup widget
	void startupInstallUpdateMsgClosed();

	// On startup, this timer calls drawImage() as soon as the mainwindow finished setup
	void startuptimer();

	// Start and Stop a slideshow
	void startSlideShow();
	void stopSlideShow();

	// A system shortcut has been called (needed for widgets like about or settings)
	void systemShortcutDO(QString todo);

	// Take screenshots (called at startup, and when PhotoQt is minimised to system tray and re-opened)
	void takeScreenshots();

	// Click on a tray icon menu item
	void trayAcDo(QSystemTrayIcon::ActivationReason rsn = QSystemTrayIcon::Trigger);

	// Update the main image scene rect (called from graphicsitem.cpp)
	void updateSceneBigRect();

	// The settings have been updated, so the map is passed to suib-widgets
	void updateSettings(QMap<QString,QVariant>);

protected:
	void closeEvent(QCloseEvent *);
	void resizeEvent(QResizeEvent *);
	void keyReleaseEvent(QKeyEvent *);

};

#endif // MAINWINDOW_H
