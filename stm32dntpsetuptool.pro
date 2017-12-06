#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T09:59:51
#
#-------------------------------------------------

QT       += core gui
QT += network widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stm32dntpsetuptool
TEMPLATE = app


SOURCES += src/main.cpp\
        src/setuptool.cpp \
    src/ntpserver.cpp \
    src/settingsform.cpp

HEADERS  += src/setuptool.h \
    src/ntpserver.h \
    src/settingsform.h

FORMS    += src/setuptool.ui \
    src/settingsform.ui
