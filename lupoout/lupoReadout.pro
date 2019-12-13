#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T04:43:45
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = lupo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp\
    caenvmeA32.cpp \
    lupoLib.cpp

HEADERS  +=  \
    caenvmeA32.h \
    lupoLib.h



unix:!macx: LIBS += -L/usr/lib/ -lCAENVME

INCLUDEPATH += $$/usr/lib/
DEPENDPATH += $$/usr/lib/
