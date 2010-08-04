TARGET      = qontacts
VERSION 	= 0.9.4
HEADERS     += mainwindow.h \
			contactslist.h \
			contactpreview.h \
			contactsmodel.h \
			contactdata.h \
			about.h
SOURCES     +=  main.cpp\
			mainwindow.cpp \
			contactslist.cpp \
			contactpreview.cpp \
			contactsmodel.cpp \
			about.cpp
FORMS       += mainwindow.ui \
			contactslist.ui \
			contactpreview.ui
LEXSOURCES  += #LEXS#
YACCSOURCES += #YACCS#

INCLUDEPATH +=
LIBS        +=
DEFINES     += VERSION=\\\"$$VERSION\\\"

# All generated files goes same directory
OBJECTS_DIR = build
MOC_DIR     = build
UI_DIR      = build

DESTDIR     = build
TEMPLATE    = app
DEPENDPATH  +=
VPATH       += src uis
CONFIG      -= 
CONFIG      += debug mobility
MOBILITY 	+= contacts
QT			= core gui maemo5

INSTALLS    += target
target.path  = /usr/bin/

INSTALLS    += desktop
desktop.path  = /usr/share/applications/hildon
desktop.files  = data/qontacts.desktop

INSTALLS    += service
service.path  = /usr/share/dbus-1/services
service.files  = data/qontacts.service

INSTALLS    += icon64
icon64.path  = /usr/share/icons/hicolor/64x64/apps
icon64.files  = data/64x64/qontacts.png

INSTALLS    += icon48
icon48.path  = /usr/share/icons/hicolor/48x48/apps
icon48.files  = data/48x48/qontacts.png

INSTALLS    += icon26
icon26.path  = /usr/share/icons/hicolor/26x26/apps
icon26.files  = data/48x48/qontacts.png

#
# Targets for debian source and binary package creation
#
debian-src.commands = dpkg-buildpackage -S -r -us -uc -d
debian-bin.commands = dpkg-buildpackage -b -r -uc -d
debian-all.depends = debian-src debian-bin

#
# Clean all but Makefile
#
compiler_clean.commands = -$(DEL_FILE) $(TARGET)

QMAKE_EXTRA_TARGETS += debian-all debian-src debian-bin compiler_clean
