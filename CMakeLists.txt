cmake_minimum_required(VERSION 2.8)
PROJECT(photoqt)

### POSSIBLY NEEDED: SET(QT_QMAKE_EXECUTABLE qmake-qt4)
FIND_PACKAGE(Qt4 REQUIRED)

# Set Sources and Headers
SET(photoqt_SOURCES main.cpp mainwindow.cpp graphics/graphicsview.cpp graphics/graphicsitem.cpp widgets/aboutwidget.cpp widgets/customconfirm.cpp widgets/dropdownmenu.cpp widgets/dropdownmenuitem.cpp widgets/exifwidget.cpp widgets/filehandling.cpp widgets/startupwidget.cpp thumbnails/thumbnailpixmapitem.cpp thumbnails/thumbnails.cpp thumbnails/thumbnailview.cpp slideshow/slideshowbar.cpp slideshow/slideshowsettings.cpp settings/settings.cpp settings/settingstabexif.cpp settings/settingstabexiftiles.cpp settings/settingstablookandfeel.cpp settings/settingstabother.cpp settings/settingstabothercontext.cpp settings/settingstabothercontexttiles.cpp settings/settingstabotherlanguagetiles.cpp settings/settingstabshortcuts.cpp settings/settingstabshortcutschangecommand.cpp settings/settingstabshortcutskeydetect.cpp settings/settingstabshortcutstiles.cpp settings/settingstabthumbnail.cpp flowlayout/flowlayout.cpp customelements/customcheckbox.cpp customelements/customcombobox.cpp customelements/customlineedit.cpp customelements/custompushbutton.cpp customelements/customradiobutton.cpp customelements/customscrollbar.cpp customelements/customslider.cpp customelements/customspinbox.cpp wallpaper/wallpaper.cpp graphics/imagereader.cpp globalsettings.h globalvariables.h settings/shortcuts.h thumbnails/threadforthumbnails.h setupwidgets.h graphics/graphicsviewlay.cpp)

SET(photoqt_HEADERS mainwindow.h globalsettings.h globalvariables.h settings/shortcuts.h thumbnails/threadforthumbnails.h graphics/graphicsview.h graphics/graphicsitem.h widgets/aboutwidget.h widgets/customconfirm.h widgets/dropdownmenu.h widgets/dropdownmenuitem.h widgets/exifwidget.h widgets/filehandling.h widgets/startupwidget.h thumbnails/thumbnailpixmapitem.h thumbnails/thumbnails.h thumbnails/thumbnailview.h slideshow/slideshowbar.h slideshow/slideshowsettings.h settings/settings.h settings/settingstabexif.h settings/settingstabexiftiles.h settings/settingstablookandfeel.h settings/settingstabother.h settings/settingstabothercontext.h settings/settingstabothercontexttiles.h settings/settingstabotherlanguagetiles.h settings/settingstabshortcuts.h settings/settingstabshortcutschangecommand.h settings/settingstabshortcutskeydetect.h settings/settingstabshortcutstiles.h settings/settingstabthumbnail.h flowlayout/flowlayout.h customelements/customcheckbox.h customelements/customcombobox.h customelements/customlineedit.h customelements/custompushbutton.h customelements/customradiobutton.h customelements/customscrollbar.h customelements/customslider.h customelements/customspinbox.h wallpaper/wallpaper.h graphics/imagereader.h setupwidgets.h graphics/graphicsviewlay.h)

# Set sources
SET(photoqt_RESOURCES resLang.qrc resImg.qrc)
# and add sql
SET(QT_USE_QTSQL TRUE)



OPTION(WITH_PHONON "Use Phonon" ON)
OPTION(WITH_EXIV2 "Use exiv2 library" ON)
OPTION(WITH_GRAPHICSMAGICK "Use graphicsmagick library" ON)     ### NOT YET IMPLEMENETED
OPTION(WITH_QTONLY "ONLY USE QT-ONLY FEATURES" OFF)

if(NOT WITH_QTONLY AND WITH_PHONON)
        SET(QT_USE_PHONON TRUE)
endif(NOT WITH_QTONLY AND WITH_PHONON)


###################################
###### REMOVE BEFORE RELEASE ######
###################################
SET(CMAKE_BUILD_TYPE Debug)

# moc files
QT4_WRAP_CPP(photoqt_HEADERS_MOC ${photoqt_HEADERS})
QT4_ADD_RESOURCES(photoqt_RESOURCES_RCC ${photoqt_RESOURCES})

if(NOT WITH_QTONLY AND WITH_EXIV2)
        LINK_LIBRARIES(exiv2)
endif(NOT WITH_QTONLY AND WITH_EXIV2)

# include, add defiunitions and include dirs
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# And add the executeable
ADD_EXECUTABLE(photoqt ${photoqt_SOURCES} ${photoqt_HEADERS_MOC} ${photoqt_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(photoqt ${QT_LIBRARIES})


message("")

if(NOT WITH_QTONLY)

        if(WITH_EXIV2)
                # Link Exiv Libraries

                ADD_DEFINITIONS(-DWITH_EXIV2)
                message("** Exiv2 enabled")
        elseif(NOT WITH_EXIV2)
                message("** Exiv2 DISABLED")
        endif(WITH_EXIV2)

        if(WITH_PHONON)
                ADD_DEFINITIONS(-DWITH_PHONON)
                if(UNIX)
                        SET(QT_PHONON_INCLUDE_DIR /usr/include/phonon)
                endif(UNIX)
                message("** Phonon enabled")
        elseif(NOT WITH_PHONON)
                message("** Phonon DISABLED")
        endif(WITH_PHONON)

        if(WITH_GRAPHICSMAGICK)
                INCLUDE_DIRECTORIES("/usr/include/GraphicsMagick/")
                TARGET_LINK_LIBRARIES(photoqt "GraphicsMagick++")
                ADD_DEFINITIONS(-DWITH_GRAPHICSMAGICK)
                message("** Graphicsmagick enabled")
        elseif(NOT WITH_GRAPHICSMAGICK)
                message("** Graphicsmagick DISABLED")
        endif(WITH_GRAPHICSMAGICK)

elseif(WITH_QTONLY)
        message("** All non-Qt features DISABLED")
endif(NOT WITH_QTONLY)

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
    
