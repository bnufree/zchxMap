
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
#Debug 和 Release都使用Release的文件  现在不做区别
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
    INSTALLS += ZCHX_ECDIS_install ZCHX_ECDIS_translation ZCHX_ECDIS_resource

    warning("app path:" + $${IDE_APP_PATH})
}

!exists( $${ZCHX_ECDIS_PATH} ) {
    warning("Cann't find ZCHX_ECDIS DIR =======================" + $$ZCHX_ECDIS_PATH)
}else{
    warning("find ZCHX_ECDIS DIR =======================" + $$ZCHX_ECDIS_PATH)
}
