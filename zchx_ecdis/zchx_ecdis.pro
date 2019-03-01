#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T13:46:36
#
#-------------------------------------------------

defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}

QT       += core gui network positioning core_private opengl svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zchx_ecdis
TARGET = $$qtLibraryName($$TARGET)
TEMPLATE = lib
CONFIG += shared dll

#根据不同的编译清空生成不同的输出路径
CONFIG(release, debug|release) {
  DEFINES *= RELEASE _RELEASE NDEBUG
  CONFIG_NAME = Release
} else {
  DEFINES *= DEBUG _DEBUG
  CONFIG_NAME = Debug
}

TargetRoot=$$dirname(PWD)
BINARIES_PATH = $$TargetRoot/out/$$CONFIG_NAME
DESTDIR = $$BINARIES_PATH
warning("dest:" + $$DESTDIR)
#翻译文件
TRANSLATIONS += $$PWD/translations/zchx_ecdis_zh_CN.ts
exists($$PWD/translations/zchx_ecdis_zh_CN.ts){
    mkpath($$BINARIES_PATH/translations)
    mkpath($$BINARIES_PATH/resources)
    system(lrelease $$PWD/translations/zchx_ecdis_zh_CN.ts -qm $$BINARIES_PATH/translations/zchx_ecdis_zh_CN.qm)
    system(rcc $$PWD/res/resources.qrc --binary -o $$BINARIES_PATH/resources/zchx_ecdis.rcc)
}

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
    dialog/zchxcameralistdlg.cpp \
    element/aiselement.cpp \
    element/cameraelement.cpp \
    element/radarelement.cpp \
    zchxroutedatamgr.cpp \
    zchxshipplandatamgr.cpp \
    data_manager/zchxecdisdatamgr.cpp \
    data_manager/zchxaisdatamgr.cpp \
    data_manager/zchxradardatamgr.cpp \
    data_manager/zchxdatamgrfactory.cpp \
    data_manager/zchxcameradatamgr.cpp \
    data_manager/zchxroddatamgr.cpp \
    element/rodelement.cpp \
    element/ipcelement.cpp \
    dialog/zchxecdisdlgmgr.cpp \
    element/cameraviewelement.cpp \
    element/videotargetelement.cpp \
    data_manager/zchxvideotargetdatamgr.cpp \
    data_manager/zchxcameraviewdatamgr.cpp \
    data_manager/zchxuserdefinesdatamgr.cpp \
    element/warningzoneelement.cpp \
    element/coastelement.cpp \
    element/seabedpiplineelement.cpp \
    element/islandlineelement.cpp \
    element/areanetelement.cpp \
    element/cardmouthelement.cpp \
    element/channelelement.cpp \
    element/mooringelement.cpp \
    element/shipalarmascendelement.cpp \
    element/structureelement.cpp \
    element/moveelement.cpp \
    element/patrolstationelement.cpp \
    element/cameragridelement.cpp \
    data_manager/zchxcameragridmgr.cpp \
    draw_manager/zchxdrawareatool.cpp \
    draw_manager/zchxdrawtool.cpp \
    draw_manager/zchxdrawangletool.cpp \
    draw_manager/zchxdrawdistool.cpp

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
    dialog/zchxcameralistdlg.h \
    element/aiselement.hpp \
    element/cameraelement.h \
    element/radarelement.h \
    zchxroutedatamgr.h \
    zchxshipplandatamgr.h \
    data_manager/zchxecdisdatamgr.h \
    data_manager/zchxaisdatamgr.h \
    data_manager/zchxradardatamgr.h \
    data_manager/zchxdatamgrfactory.h \
    data_manager/zchxdatamanagers.h \
    data_manager/zchxcameradatamgr.h \
    data_manager/zchxroddatamgr.h \
    element/rodelement.h \
    element/ipcelement.h \
    dialog/zchxecdisdlgmgr.h \
    element/cameraviewelement.h \
    element/videotargetelement.h \
    data_manager/zchxcameraviewdatamgr.h \
    data_manager/zchxvideotargetdatamgr.h \
    data_manager/zchxuserdefinesdatamgr.h \
    element/warningzoneelement.h \
    element/coastelement.h \
    element/seabedpiplineelement.h \
    element/islandlineelement.h \
    element/areanetelement.h \
    element/cardmouthelement.h \
    element/channelelement.h \
    element/mooringelement.h \
    element/shipalarmascendelement.h \
    element/structureelement.h \
    element/moveelement.h \
    element/patrolstationelement.h \
    data_manager/zchxtemplatedatamgr.h \
    element/cameragridelement.h \
    data_manager/zchxcameragridmgr.h \
    draw_manager/zchxecdismousedefines.h \
    draw_manager/zchxdrawareatool.h \
    draw_manager/zchxdrawtool.h \
    draw_manager/zchxdrawangletool.h \
    draw_manager/zchxdrawdistool.h

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

#生成安装文件到第三方�?
#1)指定三方库目�?
DESTDIRRoot = $$TargetRoot/app/3rdParty/zchx_ecdis/3.0.0
#DESTDIRRoot = H:/workspace/2018_WBS_V2/5.code/trunk/kakou_framework/Tools/3rdParty/zchx_ecdis/3.0.0
#2)安装翻译文件
qminstall.files = $$BINARIES_PATH/translations/zchx_ecdis_zh_CN.qm
qminstall.path = $$DESTDIRRoot/translations/
INSTALLS += qminstall
#3)安装资源文件
rccinstall.files = $$BINARIES_PATH/resources/zchx_ecdis.rcc
rccinstall.path = $$DESTDIRRoot/resources/
INSTALLS += rccinstall
#4)安装头文�?
LibFilesInclude.path = $$DESTDIRRoot/include/qt
LibFilesInclude.files = $$PWD/*.h $$PWD/*.hpp
INSTALLS += LibFilesInclude

LibFilesInclude1.path = $$DESTDIRRoot/include/qt/camera_mgr
LibFilesInclude1.files = $$PWD/camera_mgr/*.h $$PWD/camera_mgr/*.hpp
INSTALLS += LibFilesInclude1

LibFilesInclude2.path = $$DESTDIRRoot/include/qt/data_manager
LibFilesInclude2.files = $$PWD/data_manager/*.h $$PWD/data_manager/*.hpp
INSTALLS += LibFilesInclude2

LibFilesInclude3.path = $$DESTDIRRoot/include/qt/element
LibFilesInclude3.files = $$PWD/element/*.h $$PWD/element/*.hpp
INSTALLS += LibFilesInclude3

LibFilesInclude4.path = $$DESTDIRRoot/include/qt/map_layer
LibFilesInclude4.files = $$PWD/map_layer/*.h $$PWD/map_layer/*.hpp
INSTALLS += LibFilesInclude4

#5)安装lib文件
LibFilesLib.path = $$DESTDIRRoot/lib/
LibFilesLib.files += $$DESTDIR/lib$${TARGET}.dll.a
INSTALLS += LibFilesLib
#6)安装dll文件
dlltarget.path = $$DESTDIRRoot/bin/$$CONFIG_NAME
dlltarget.files =  $$DESTDIR/$${TARGET}.dll
INSTALLS += dlltarget

