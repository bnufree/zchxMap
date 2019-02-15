#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T13:46:36
#
#-------------------------------------------------

QT       += core gui network positioning core_private opengl svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zchx_ecdis
TEMPLATE = lib
DESTDIR = ../../bin

DEFINES *= ZCHX_ECDIS_PLUGIN


SOURCES += mainwindow.cpp \
    zchxmapthread.cpp \
    zchxtileimagethread.cpp \
    qhttpget.cpp \
    zchxmapwidget.cpp \
    zchxmapdownloadthread.cpp \
    zchxmapdownloadrunfunction.cpp \
    zchxmaploadthread.cpp \
    zchxutils.cpp \
    zchxMapDataUtils.cpp \    
    profiles.cpp \
    zchxmapframe.cpp \
    slider_ctrl.cpp \
    proxystyle.cpp \
    cardmouthinfodialog.cpp \
    channelinfodialog.cpp \
    coastdatainfodialog.cpp \
    defenceinfodialog.cpp \
    info_dialog.cpp \
    localmarkdlg.cpp \
    mooringinfodialog.cpp \
    structureinfodialog.cpp \
    warningzoneparadialog.cpp \
    zchxdrawmultibeam.cpp \
    zchxdrawradarvideo.cpp \
    element/IDrawElement.cpp \
    element/drawelement.cpp \
    map_layer/zchxmaplayermgr.cpp \
    map_layer/zchxMapLayer.cpp \
    camera_mgr/zchxcameradatasmgr.cpp \
    camera_mgr/zchxcameralistdlg.cpp \
    element/aiselement.cpp \
    element/camerarodelement.cpp \
    element/cameraelement.cpp \
    element/radarelement.cpp \
    zchxradardatamgr.cpp \
    zchxaisdatamgr.cpp \
    zchxuserdefinesdatamgr.cpp \
    zchxroutedatamgr.cpp \
    zchxshipplandatamgr.cpp

HEADERS  += mainwindow.h \
    zchxmapthread.h \
    zchxtileimagethread.h \
    qhttpget.h \
    zchxmapwidget.h \
    zchxmapdownloadthread.h \
    zchxmapdownloadrunfunction.h \
    zchxmaploadthread.h \
    zchx_ecdis_global.h \
    zchxutils.hpp \
    zchxMapDataUtils.h \
    element/IDrawElement.hpp \
    element/IDrawElement_p.hpp \
    element/drawelement.hpp \
    profiles.h \
    zchxmapframe.h \
    slider_ctrl.hpp \
    proxystyle.hpp \
    cardmouthinfodialog.h \
    channelinfodialog.h \
    coastdatainfodialog.h \
    defenceinfodialog.h \
    info_dialog.hpp \
    localmarkdlg.h \
    mooringinfodialog.h \
    structureinfodialog.h \
    warningzoneparadialog.h \
    zchxdrawmultibeam.h \
    zchxdrawradarvideo.h \
    map_layer/zchxmaplayermgr.h \
    map_layer/zchxmaplayer_p.h \
    map_layer/zchxMapLayer.h \
    camera_mgr/zchxcameradatasmgr.h \
    camera_mgr/zchxcameralistdlg.h \
    element/aiselement.hpp \
    element/camerarodelement.h \
    element/cameraelement.h \
    element/radarelement.h \
    zchxradardatamgr.h \
    zchxaisdatamgr.h \
    zchxuserdefinesdatamgr.h \
    zchxroutedatamgr.h \
    zchxshipplandatamgr.h

FORMS    += mainwindow.ui \
    cardmouthinfodialog.ui \
    channelinfodialog.ui \
    coastdatainfodialog.ui \
    defenceinfodialog.ui \
    localmarkdlg.ui \
    mooringinfodialog.ui \
    structureinfodialog.ui \
    warningzoneparadialog.ui

RESOURCES += res/resources.qrc
DISTFILES += \
    res/element/camera1.png \
    res/element/camera2.png
RESOURCES += $$PWD/opengl/opengl.qrc
