#-------------------------------------------------
#
# Project created by QtCreator 2012-08-21T23:07:27
#
#-------------------------------------------------

QT       += core gui sql phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview.cpp \
    graphicsitem.cpp \
    customelements/customcheckbox.cpp \
    customelements/custompushbutton.cpp \
    customelements/customradiobutton.cpp \
    customelements/customscrollbar.cpp \
    customelements/customslider.cpp \
    customelements/customspinbox.cpp \
    widgets/aboutwidget.cpp \
    widgets/exifwidget.cpp \
    widgets/filehandling.cpp \
    flowlayout/flowlayout.cpp \
    settings/settings.cpp \
    settings/settingstabexif.cpp \
    settings/settingstablookandfeel.cpp \
    settings/settingstabother.cpp \
    settings/settingstabshortcuts.cpp \
    settings/settingstabshortcutskeydetect.cpp \
    settings/settingstabshortcutstiles.cpp \
    settings/settingstabthumbnail.cpp \
    thumbnails/thumbnailpixmapitem.cpp \
    thumbnails/thumbnailview.cpp \
    thumbnails/thumbnails.cpp \
    customelements/customcombobox.cpp \
    settings/settingstabexiftiles.cpp \
    settings/settingstabotherlanguagetiles.cpp \
    customelements/customlineedit.cpp \
    settings/settingstabshortcutschangecommand.cpp \
    widgets/customconfirm.cpp \
    settings/settingstabothercontext.cpp \
    settings/settingstabothercontexttiles.cpp \
    slideshow/slideshowsettings.cpp \
    slideshow/slideshowbar.cpp \
    widgets/dropdownmenu.cpp \
    widgets/startupwidget.cpp \
    widgets/dropdownmenuitem.cpp \
    widgets/wallpaper.cpp \
    imagemagick.cpp

HEADERS  += mainwindow.h \
    graphicsview.h \
    globalvariables.h \
    globalsettings.h \
    thumbnails/thumbnails.h \
    graphicsitem.h \
    thumbnails/thumbnailpixmapitem.h \
    thumbnails/thumbnailview.h \
    settings/settings.h \
    shortcuts.h \
    settings/settingstablookandfeel.h \
    settings/settingstabthumbnail.h \
    settings/settingstabexif.h \
    settings/settingstabother.h \
    customelements/customscrollbar.h \
    widgets/filehandling.h \
    customelements/customcheckbox.h \
    customelements/customslider.h \
    customelements/customradiobutton.h \
    customelements/custompushbutton.h \
    customelements/customspinbox.h \
    widgets/aboutwidget.h \
    threadforthumbnails.h \
    widgets/exifwidget.h \
    settings/settingstabshortcutstiles.h \
    settings/settingstabshortcutskeydetect.h \
    settings/settingstabshortcuts.h \
    flowlayout/flowlayout.h \
    customelements/customcombobox.h \
    settings/settingstabexiftiles.h \
    settings/settingstabotherlanguagetiles.h \
    customelements/customlineedit.h \
    settings/settingstabshortcutschangecommand.h \
    widgets/customconfirm.h \
    settings/settingstabothercontext.h \
    settings/settingstabothercontexttiles.h \
    slideshow/slideshowsettings.h \
    slideshow/slideshowbar.h \
    widgets/dropdownmenu.h \
    widgets/startupwidget.h \
    widgets/dropdownmenuitem.h \
    widgets/wallpaper.h \
    imagemagick.h

TRANSLATIONS += lang/photo_en.ts

RESOURCES += \
    resImg.qrc \
    resLang.qrc

LIBS += -lexiv2
LIBS += /usr/lib/libMagick++-Q16.so

INCLUDEPATH += /usr/include/ImageMagick
