# -------------------------------------------------
# Project created by QtCreator 2010-09-02T11:03:29
# -------------------------------------------------

TARGET = ztasklist
TEMPLATE = app
DESTDIR = ./bin/
CODEC= UTF-8

QT+=xml network webkit

SOURCES += src/main.cpp \
    src/taskdialog.cpp \
    src/trayicon.cpp \
    src/tasklistwidget.cpp \
    src/taskdelegate.cpp \
    src/programsettings.cpp \
    src/settingsdialog.cpp \
    src/task.cpp \
    src/taskcontextmenu.cpp \
    src/taskmodel.cpp \
    src/taskfiltermodel.cpp \
    src/transparentbutton.cpp \
    src/aboutdialog.cpp \
    src/menubuttonaction.cpp \
    src/filterbutton.cpp \
    src/clearlineedit.cpp \
    src/updatedialog.cpp \
    src/datamanager.cpp \
    src/timewatcher.cpp \
    src/mainwindow.cpp \
    src/taskeditwidget.cpp \
    src/trackingtaskmodel.cpp \
    src/note.cpp \
    src/noteeditwidget.cpp \
    src/markuptextedit.cpp \
    src/noteeditdialog.cpp \
    src/notemodel.cpp \
    src/trackingnotemodel.cpp \
    src/content.cpp \
    src/notelistwidget.cpp \
    src/notefiltermodel.cpp

HEADERS += src/taskdialog.h \
    src/trayicon.h \
    src/tasklistwidget.h \
    src/taskdelegate.h \
    src/settingsdialog.h \
    src/programsettings.h \
    src/task.h \
    src/taskcontextmenu.h \
    src/taskmodel.h \
    src/iconwrapper.h \
    src/taskfiltermodel.h \
    src/transparentbutton.h \
    src/aboutdialog.h \
    src/menubuttonaction.h \
    src/filterbutton.h \
    src/clearlineedit.h \
    src/updatedialog.h \
    src/datamanager.h \
    src/timewatcher.h \
    src/mainwindow.h \
    src/taskeditwidget.h \
    src/trackingtaskmodel.h \
    src/note.h \
    src/noteeditwidget.h \
    src/markuptextedit.h \
    src/noteeditdialog.h \
    src/notemodel.h \
    src/trackingnotemodel.h \
    src/content.h \
    src/notelistwidget.h \
    src/notefiltermodel.h

FORMS += forms/taskdialog.ui \
    forms/settingsdialog.ui \
    forms/synchronizedialog.ui \
    forms/aboutdialog.ui \
    forms/updatedialog.ui \
    forms/mainwindow.ui \
    forms/taskeditwidget.ui \
    forms/noteeditwidget.ui \
    forms/noteeditdialog.ui

RESOURCES += resources/resources.qrc\
             langs/langs.qrc

include(src/another/another.pri)

OBJECTS_DIR = src/objects
MOC_DIR = src/objects
UI_DIR = src/objects
RCC_DIR = src/objects

desktop.path=/usr/share/applications
desktop.files=install/ztasklist.desktop
icon.path=/usr/share/pixmaps
icon.files=resources/icons/icon.png
target.path=/usr/bin/

INSTALLS+=desktop icon target
TRANSLATIONS += langs/ztasklist_ru_RU.ts
