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
    zchxmaploadthread.cpp \
    zchxutils.cpp \
    zchxMapDataUtils.cpp \    
    profiles.cpp \
    zchxmapframe.cpp \
    slider_ctrl.cpp \
    proxystyle.cpp \
    info_dialog.cpp \
    dialog/localmarkdlg.cpp \
    dialog/structureinfodialog.cpp \
    dialog/warningzoneparadialog.cpp \
    element/IDrawElement.cpp \
    element/drawelement.cpp \
    map_layer/zchxmaplayermgr.cpp \
    map_layer/zchxMapLayer.cpp \
    dialog/zchxcameralistdlg.cpp \
    element/aiselement.cpp \
    element/radarelement.cpp \
    zchxroutedatamgr.cpp \
    zchxshipplandatamgr.cpp \
    element/rodelement.cpp \
    element/ipcelement.cpp \
    dialog/zchxecdisdlgmgr.cpp \
    dialog/defenceinfodialog.cpp \
    element/videotargetelement.cpp \
    element/islandlineelement.cpp \
    element/shipalarmascendelement.cpp \
    element/structureelement.cpp \
    element/moveelement.cpp \
    element/patrolstationelement.cpp \
    draw_manager/zchxdrawareatool.cpp \
    draw_manager/zchxdrawtool.cpp \
    draw_manager/zchxdrawangletool.cpp \
    draw_manager/zchxdrawdistool.cpp \
    draw_manager/zchxdrawlocalmarktool.cpp \
    draw_manager/zchxdrawcameranetgridtool.cpp \
    element/radarvideoelement.cpp \
    dialog/radarfeaturezone.cpp \
    draw_manager/zchxdrawzonetool.cpp \
    dialog/channelinfodialog.cpp \
    dialog/mooringinfodialog.cpp \
    dialog/cardmouthinfodialog.cpp \
    draw_manager/zchxeditzonetool.cpp \
    element/zchxmoveelements.cpp \
    element/zchxfixelements.cpp \
    element/cameraelement.cpp \
    element/gridelement.cpp \
    map_layer/zchxaismaplayer.cpp \
    dialog/coastdatainfodialog.cpp \
    map_layer/zchxradarvideolayer.cpp \
    map_layer/zchxshipalarmascendlayer.cpp

HEADERS  += mainwindow.h \
    zchxmapthread.h \
    zchxtileimagethread.h \
    qhttpget.h \
    zchxmapwidget.h \
    zchxmaploadthread.h \
    zchx_ecdis_global.h \
    zchxutils.hpp \
    zchxMapDataUtils.h \
    element/IDrawElement.hpp \
    element/drawelement.hpp \
    profiles.h \
    zchxmapframe.h \
    slider_ctrl.hpp \
    proxystyle.hpp \
    info_dialog.hpp \
    dialog/localmarkdlg.h \
    dialog/structureinfodialog.h \
    dialog/warningzoneparadialog.h \
    map_layer/zchxmaplayermgr.h \
    map_layer/zchxmaplayer_p.h \
    map_layer/zchxMapLayer.h \
    dialog/zchxcameralistdlg.h \
    dialog/defenceinfodialog.h \
    element/aiselement.hpp \
    element/cameraelement.h \
    element/radarelement.h \
    zchxroutedatamgr.h \
    zchxshipplandatamgr.h \
    element/rodelement.h \
    element/ipcelement.h \
    dialog/zchxecdisdlgmgr.h \
    element/videotargetelement.h \
    element/islandlineelement.h \
    element/shipalarmascendelement.h \
    element/structureelement.h \
    element/moveelement.h \
    element/patrolstationelement.h \
    draw_manager/zchxecdismousedefines.h \
    draw_manager/zchxdrawareatool.h \
    draw_manager/zchxdrawtool.h \
    draw_manager/zchxdrawangletool.h \
    draw_manager/zchxdrawdistool.h \
    draw_manager/zchxdrawlocalmarktool.h \
    draw_manager/zchxdrawcameranetgridtool.h \
    draw_manager/zchxdrawtoolutil.h \
    element/radarvideoelement.h \
    dialog/radarfeaturezone.h \
    draw_manager/zchxdrawzonetool.h \
    dialog/channelinfodialog.h \
    dialog/mooringinfodialog.h \
    dialog/cardmouthinfodialog.h \
    draw_manager/zchxeditzonetool.h \
    element/zchxmoveelements.h \
    element/fixelement.h \
    element/zchxfixelements.h \
    element/gridelement.h \
    map_layer/zchxaismaplayer.h \
    dialog/coastdatainfodialog.h \
    map_layer/zchxradarvideolayer.h \
    map_layer/zchxshipalarmascendlayer.h

FORMS    += mainwindow.ui \
    dialog/localmarkdlg.ui \
    dialog/structureinfodialog.ui \
    dialog/warningzoneparadialog.ui \
    dialog/radarfeaturezone.ui \
    dialog/defenceinfodialog.ui \
    dialog/channelinfodialog.ui \
    dialog/mooringinfodialog.ui \
    dialog/cardmouthinfodialog.ui \
    dialog/coastdatainfodialog.ui

RESOURCES += res/resources.qrc
DISTFILES += \
    res/element/camera1.png \
    res/element/camera2.png \
    configuration/maplayers.xml
RESOURCES += $$PWD/opengl/opengl.qrc

#生成安装文件到第三方�?
#1)指定三方库目�?
DESTDIRRoot = $$TargetRoot/app/3rdParty/zchx_ecdis/3.0.0
#DESTDIRRoot = H:/workspace/2018_WBS_V2/5.code/trunk/kakou_framework_new_ecdis/Tools/3rdParty/zchx_ecdis/3.0.0
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

LibFilesInclude5.path = $$DESTDIRRoot/include/qt/draw_manager
LibFilesInclude5.files = $$PWD/draw_manager/*.h $$PWD/draw_manager/*.hpp
INSTALLS += LibFilesInclude5


#5)安装lib文件
LibFilesLib.path = $$DESTDIRRoot/lib/
LibFilesLib.files += $$DESTDIR/lib$${TARGET}.dll.a
INSTALLS += LibFilesLib
#6)安装dll文件
dlltarget.path = $$DESTDIRRoot/bin/$$CONFIG_NAME
dlltarget.files =  $$DESTDIR/$${TARGET}.dll
INSTALLS += dlltarget
#8)install layerfile
layertarget.path = $$DESTDIRRoot/bin/mapdata
layertarget.files = $$PWD/configuration/*.*
INSTALLS += layertarget

# ��release�汾����pdb��Ϣ�������
QMAKE_CXXFLAGS_RELEASE *= $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE *= $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

