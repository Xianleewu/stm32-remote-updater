#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T00:50:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpserver
TEMPLATE = app


SOURCES += main.cpp\
        serverwindow.cpp

HEADERS  += serverwindow.h

FORMS    += serverwindow.ui

RESOURCES += \
    qdarkstyle/style.qrc
