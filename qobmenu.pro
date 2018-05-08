#-------------------------------------------------
#
# Project created by QtCreator 2017-11-10T11:12:43
#
#-------------------------------------------------

QT       += core gui xml x11extras dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qobmenu
TEMPLATE = app
DESTDIR = usr/bin
CONFIG += qt \
          release
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
INCLUDEPATH +=build

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0




SOURCES += \
        main.cpp \
        menuapplications.cpp \
        menufolders.cpp \
        menuprogrammes.cpp \
        menugen.cpp \
        menusystem.cpp \
        findbutton.cpp \
        menurecent.cpp \
        openexec.cpp \
#        menuform.cpp \
        menuclassic.cpp\
        power/powerdlg.cpp \
        power/powermain.cpp \
        utils/edir.cpp \
        utils/desktopfile.cpp \
    panel_adaptor.cpp
 #        utils/x11utills.cpp
HEADERS += \
            menuapplications.h \
            menufolders.h \
            menuprogrammes.h \
            menugen.h \
            menusystem.h \
            power/powerdlg.h \
            power/powermain.h \
            menurecent.h \
            findbutton.h \
            openexec.h \
#            menuform.h \
            menuclassic.h \
           utils/desktopfile.h \
           utils/edir.h \
           panel_adaptor.h
 #utils/x11utills.h


#FORMS += \
#        menuform.ui


#---------------------------------------------
#                   TRANSLATIONS
#---------------------------------------------
TRANSLATIONS    =usr/share/elokab/translations/ar/epanel.ts\
                 usr/share/elokab/translations/fr/epanel-fm.ts\
                 usr/share/elokab/translations/en/epanel-fm.ts\


#---------------------------------------------
#                   INSTALL
#---------------------------------------------

 MKDIR = mkdir -p /etc/xdg/menus
 MKDIR = mkdir -p /etc/xdg/elokab
 MKDIR = mkdir -p /usr/share/desktop-directories
 MKDIR = mkdir -p /usr/share/elokab

#---config files---------
 menusConfig.files =etc/xdg/menus/*
 menusConfig.path=/etc/xdg/menus/

 obConfig.files =etc/xdg/elokab/*
 obConfig.path=/etc/xdg/elokab/
#---DATA files---------------
 dsDirectoriesData.files=usr/share/desktop-directories/*
 dsDirectoriesData.path=/usr/share/desktop-directories/

 elokabData.files=usr/share/elokab/*
 elokabData.path=/usr/share/elokab

 INSTALLS +=    obConfig \
                menusConfig \
                dsDirectoriesData \
                elokabData
