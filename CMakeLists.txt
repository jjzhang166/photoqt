cmake_minimum_required(VERSION 2.8)
PROJECT(photoqt)

FIND_PACKAGE(Qt5Widgets REQUIRED)

#### SOURCES ####
# toplevel
SET(photoqt_SOURCES main.cpp mainwindow.cpp globalsettings.h globalvariables.h setupwidgets.h)
# settings/
set(p "settings")
SET(photoqt_SOURCES ${photoqt_SOURCES} ${p}/settings.cpp ${p}/settingstabexif.cpp ${p}/settingstabexiftiles.cpp ${p}/settingstablookandfeel.cpp ${p}/settingstabother.cpp ${p}/settingstabothercontext.cpp ${p}/settingstabothercontexttiles.cpp ${p}/settingstabotherlanguagetiles.cpp ${p}/settingstabshortcuts.cpp ${p}/settingstabshortcutschangecommand.cpp ${p}/settingstabshortcutskeydetect.cpp ${p}/settingstabshortcutstiles.cpp ${p}/settingstabthumbnail.cpp ${p}/shortcuts.h ${p}/settingstabotherfiletypestiles.cpp)
# thumbnails/
set(p "thumbnails")
SET(photoqt_SOURCES ${photoqt_SOURCES} ${p}/thumbnailpixmapitem.cpp ${p}/thumbnails.cpp ${p}/thumbnailview.cpp ${p}/threadforthumbnails.h)
# graphics/
set(p "graphics")
SET(photoqt_SOURCES ${photoqt_SOURCES} ${p}/graphicsview.cpp ${p}/graphicsitem.cpp ${p}/imagereader.cpp ${p}/graphicsviewlay.cpp ${p}/cropwidget.cpp ${p}/cropbar.cpp)
# widgets/
set(p "widgets")
SET(photoqt_SOURCES ${photoqt_SOURCES} ${p}/aboutwidget.cpp ${p}/customconfirm.cpp ${p}/dropdownmenu.cpp ${p}/dropdownmenuitem.cpp ${p}/detailswidget.cpp ${p}/filehandling.cpp ${p}/startupwidget.cpp ${p}/mywidget.cpp)
# filterimages/
set(p "filterimages")
SET(photoqt_SOURCES ${photoqt_SOURCES} ${p}/filterimagessetup.cpp ${p}/filterimagesdisplay.cpp)
# customelements/
set(p "customelements")
SET(photoqt_SOURCES ${photoqt_SOURCES} ${p}/customcheckbox.cpp ${p}/customcombobox.cpp ${p}/customlineedit.cpp ${p}/custompushbutton.cpp ${p}/customradiobutton.cpp ${p}/customscrollbar.cpp ${p}/customslider.cpp ${p}/customspinbox.cpp ${p}/customlabel.cpp ${p}/customtabwidget.cpp ${p}/customline.h)
# [other]/
SET(photoqt_SOURCES ${photoqt_SOURCES} slideshow/slideshowbar.cpp slideshow/slideshowsettings.cpp flowlayout/flowlayout.cpp wallpaper/wallpaper.cpp)

#### HEADER ####
# toplevel
SET(photoqt_HEADERS mainwindow.h globalsettings.h globalvariables.h setupwidgets.h)
# settings/
set(p "settings")
SET(photoqt_HEADERS ${photoqt_HEADERS} ${p}/shortcuts.h ${p}/settings.h ${p}/settingstabexif.h ${p}/settingstabexiftiles.h ${p}/settingstablookandfeel.h ${p}/settingstabother.h ${p}/settingstabothercontext.h ${p}/settingstabothercontexttiles.h ${p}/settingstabotherlanguagetiles.h ${p}/settingstabshortcuts.h ${p}/settingstabshortcutschangecommand.h ${p}/settingstabshortcutskeydetect.h ${p}/settingstabshortcutstiles.h ${p}/settingstabthumbnail.h ${p}/settingstabotherfiletypestiles.h)
# thumbnails/
set(p "thumbnails")
SET(photoqt_HEADERS ${photoqt_HEADERS} ${p}/threadforthumbnails.h ${p}/thumbnailpixmapitem.h ${p}/thumbnails.h ${p}/thumbnailview.h)
# graphics/
set(p "graphics")
SET(photoqt_HEADERS ${photoqt_HEADERS} ${p}/graphicsview.h ${p}/graphicsitem.h ${p}/imagereader.h ${p}/graphicsviewlay.h ${p}/cropwidget.h ${p}/cropbar.h)
# widgets/
set(p "widgets")
SET(photoqt_HEADERS ${photoqt_HEADERS} ${p}/aboutwidget.h ${p}/customconfirm.h ${p}/dropdownmenu.h ${p}/dropdownmenuitem.h ${p}/detailswidget.h ${p}/filehandling.h ${p}/startupwidget.h ${p}/mywidget.h)
# filterimages/
set(p "filterimages")
SET(photoqt_HEADERS ${photoqt_HEADERS} ${p}/filterimagessetup.h ${p}/filterimagesdisplay.h)
# customelements/
set(p "customelements")
SET(photoqt_HEADERS ${photoqt_HEADERS} ${p}/customcheckbox.h ${p}/customcombobox.h ${p}/customlineedit.h ${p}/custompushbutton.h ${p}/customradiobutton.h ${p}/customscrollbar.h ${p}/customslider.h ${p}/customspinbox.h ${p}/customlabel.h ${p}/customtabwidget.h ${p}/customline.h)
# [other]/
SET(photoqt_HEADERS ${photoqt_HEADERS} slideshow/slideshowbar.h slideshow/slideshowsettings.h flowlayout/flowlayout.h wallpaper/wallpaper.h)


OPTION(EXIV2 "Use exiv2 library" ON)
OPTION(GM "Use graphicsmagick library" ON)
OPTION(QTONLY "ONLY USE QT-ONLY FEATURES" OFF)

###################################
###### REMOVE BEFORE RELEASE ######
###################################
SET(CMAKE_BUILD_TYPE Debug)

# Set sources
SET(photoqt_RESOURCES resLang.qrc resImg.qrc)

# add resource file
QT5_ADD_RESOURCES(photoqt_RESOURCES_RCC ${photoqt_RESOURCES})

# moc files
QT5_WRAP_CPP(photoqt_HEADERS_MOC ${photoqt_HEADERS})

# include, add defiunitions and include dirs
#INCLUDE(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDES})
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# And add the executeable
ADD_EXECUTABLE(photoqt ${photoqt_SOURCES} ${photoqt_HEADERS_MOC} ${photoqt_RESOURCES_RCC})
qt5_use_modules(photoqt Core Sql Widgets Multimedia)


message("")

if(NOT QTONLY)

	if(EXIV2)
		# Link Exiv Libraries
		TARGET_LINK_LIBRARIES(photoqt "exiv2")
		ADD_DEFINITIONS(-DEXIV2)
		message("** Exiv2 enabled")
	elseif(NOT EXIV2)
		message("** Exiv2 DISABLED")
	endif(EXIV2)

	if(GM)
		if(UNIX)
			INCLUDE_DIRECTORIES("/usr/include/GraphicsMagick/")
		endif(UNIX)
		TARGET_LINK_LIBRARIES(photoqt "GraphicsMagick++")
		ADD_DEFINITIONS(-DGM)
		message("** Graphicsmagick enabled")
	elseif(NOT GM)
		message("** Graphicsmagick DISABLED")
	endif(GM)

elseif(QTONLY)
	message("** All non-Qt features DISABLED")
endif(NOT QTONLY)

message("")



if(UNIX)

	# Set the install prefix to /usr/
	SET(PRODUCT_INSTALL_PREFIX /usr)

	# Install executeable
	INSTALL(
		TARGETS photoqt
		DESTINATION bin/
	)

	# Install desktop file
	INSTALL(
		FILES photoqt.desktop
		DESTINATION share/applications
	)

	# And install all the icons
	INSTALL(
		FILES icons/16x16/apps/photoqt.png
		DESTINATION share/icons/hicolor/16x16/apps/
	)
	INSTALL(
		FILES icons/32x32/apps/photoqt.png
		DESTINATION share/icons/hicolor/32x32/apps/
	)
	INSTALL(
		FILES icons/48x48/apps/photoqt.png
		DESTINATION share/icons/hicolor/48x48/apps/
	)
	INSTALL(
		FILES icons/64x64/apps/photoqt.png
		DESTINATION share/icons/hicolor/64x64/apps/
	)
	INSTALL(
		FILES icons/128x128/apps/photoqt.png
		DESTINATION share/icons/hicolor/128x128/apps/
	)
endif(UNIX)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

