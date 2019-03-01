#include "zchxcameradatamgr.h"
#include "zchxmapframe.h"
#include <QDebug>

namespace qt {
zchxCameraDataMgr::zchxCameraDataMgr(zchxMapWidget* w, QObject *parent) : zchxEcdisDataMgr(w, DATA_MGR_CAMERA, parent)
{

}


void zchxCameraDataMgr::setCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    foreach (ZCHX::Data::ITF_CameraDev cam, data) {
        std::shared_ptr<CameraElement> ele = m_CameraDev[cam.szCamName];
        if(ele) {
            ele->setData(cam);
        } else {
            m_CameraDev[cam.szCamName] = std::shared_ptr<CameraElement>(new CameraElement(cam, mDisplayWidget->framework()));
        }
    }
}

void zchxCameraDataMgr::show(QPainter* painter, double offset_x, double offset_y)
{
    if(!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CAMERA)) return;
    QMap<QString, std::shared_ptr<CameraElement>>::iterator it = m_CameraDev.begin();
    for(; it != m_CameraDev.end(); ++it)
    {
        std::shared_ptr<CameraElement> item = (*it);
        item->drawElement(painter);
    }
}

bool zchxCameraDataMgr::updateActiveItem(const QPoint &pt)
{

}

void zchxCameraDataMgr::updateCameraStatus(const QString &id, int sts)
{
    std::shared_ptr<CameraElement> ele = m_CameraDev[id];
    if(ele){
        ele->setStatus(sts);
    }
}

}


