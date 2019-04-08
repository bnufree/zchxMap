
ZCHX_ECDIS_NAME        = zchx_ecdis
ZCHX_ECDIS_VERSION     = 3.0.0
######################################################################
# 3rdparty path
######################################################################
QT       += core gui network positioning core_private opengl svg xml

ZCHX_ECDIS_PATH = $${PSFW_3RDPARTYPATH}/$${ZCHX_ECDIS_NAME}/$${ZCHX_ECDIS_VERSION}

exists( $${ZCHX_ECDIS_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    ZCHX_ECDIS_HEADERS   = $${ZCHX_ECDIS_PATH}/include
    ZCHX_ECDIS_LIBS      = $${ZCHX_ECDIS_PATH}/lib

    INCLUDEPATH += $${ZCHX_ECDIS_HEADERS}
    CONFIG(release, debug|release) {
        LIBS +=-L$${ZCHX_ECDIS_LIBS} \
                     -llibzchx_ecdis

        ZCHX_ECDIS_install.files += $${ZCHX_ECDIS_PATH}/bin/Release/*
    }else{
        LIBS +=-L$${ZCHX_ECDIS_LIBS} \
                     -llibzchx_ecdisd

        ZCHX_ECDIS_install.files += $${ZCHX_ECDIS_PATH}/bin/Debug/*
    }
    ZCHX_ECDIS_install.path = $${IDE_APP_PATH}/
    INSTALLS += ZCHX_ECDIS_install

# 安装翻译文件
    ZCHX_ECDIS_translation.files = $${ZCHX_ECDIS_PATH}/translations/*.qm
    ZCHX_ECDIS_translation.path =  $$IDE_BUILD_TREE/Share/sccmms/translations/
    #message($$ZCHX_ECDIS_translation.path)

# 安装资源文件
    ZCHX_ECDIS_resource.files = $${ZCHX_ECDIS_PATH}/resources/*.rcc
    ZCHX_ECDIS_resource.path = $$IDE_BUILD_TREE/Share/sccmms/resource/

#安装补丁文件
    ZCHX_ECDIS_patch.files = $${ZCHX_ECDIS_PATH}/bin/mapdata/*
    ZCHX_ECDIS_patch.path = $${IDE_APP_PATH}/mapdata/

    INSTALLS += ZCHX_ECDIS_install ZCHX_ECDIS_translation ZCHX_ECDIS_resource ZCHX_ECDIS_patch

    warning("app path:" + $${IDE_APP_PATH})
}
!exists( $${ZCHX_ECDIS_PATH} ) {
    warning("Cann't find ZCHX_ECDIS DIR =======================" + $$ZCHX_ECDIS_PATH)
}else{
    warning("find ZCHX_ECDIS DIR =======================" + $$ZCHX_ECDIS_PATH)
}
