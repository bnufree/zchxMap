#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T13:46:36
#
#-------------------------------------------------

PSFW_3RDPARTYPATH = $${PWD}/3rdParty
IDE_APP_PATH = ../../bin

include($$PSFW_3RDPARTYPATH/zchx_ecdis/zchx_ecdis.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zchxMapTest
TEMPLATE = app
DESTDIR = ../../bin

SOURCES += main.cpp \
    testmainwindow.cpp

FORMS += \
    testmainwindow.ui

HEADERS += \
    testmainwindow.h

