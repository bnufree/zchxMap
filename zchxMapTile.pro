#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T13:46:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zchxMapTile
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    zchxecdisutils.cpp \
    zchxmapthread.cpp \
    zchxtileimagethread.cpp \
    qhttpget.cpp \
    zchxmapwidget.cpp

HEADERS  += mainwindow.h \
    zchxecdisutils.h \
    zchxmapthread.h \
    zchxtileimagethread.h \
    qhttpget.h \
    zchxmapwidget.h

FORMS    += mainwindow.ui
