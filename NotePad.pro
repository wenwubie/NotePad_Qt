#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T22:33:13
#
#-------------------------------------------------

QT       += core gui

TARGET = NotePad
TEMPLATE = app


SOURCES += main.cpp \
    MainWindowUI.cpp \
    MainWindowSlots.cpp \
    FindDialog.cpp \
    ReplaceDialog.cpp \
    AboutDialog.cpp \
    AppConfig.cpp

HEADERS  += MainWindow.h \
    FindDialog.h \
    ReplaceDialog.h \
    AboutDialog.h \
    AppConfig.h

RESOURCES += \
    Res.qrc
