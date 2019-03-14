#include "zchxroddatamgr.h"
//#include "zchxcameralistdlg.h"
#include "zchxmapframe.h"
#include <QDebug>

namespace qt {
zchxRodDataMgr::zchxRodDataMgr(zchxMapWidget* w,  QObject *parent) : zchxEcdisDataMgr(w, ZCHX::DATA_MGR_ROD, parent)
{

}

void zchxRodDataMgr::show(QPainter* painter)
{
    QMap<QString, std::shared_ptr<RodElement>>::iterator it;
    for(it = m_CameraRod.begin(); it != m_CameraRod.end(); ++it)
    {
        std::shared_ptr<RodElement> item = it.value();
        if(item) item->drawElement(painter);
    }
}

bool zchxRodDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}

void zchxRodDataMgr::setRodData(const QList<ZCHX::Data::ITF_CameraRod> &list)
{
    //默认rod的状态为OK
    foreach (ZCHX::Data::ITF_CameraRod rod, list) {
        std::shared_ptr<RodElement> ele = m_CameraRod[rod.szID];
        if(ele) {
            ele->setData(rod);
        } else {
            m_CameraRod[rod.szID] = std::shared_ptr<RodElement>(new RodElement(rod, mDisplayWidget));
        }
    }
}

void zchxRodDataMgr::updateRodStatus(const QString &id, ZCHX::Data::CAMERAROD_STATUS sts)
{
    std::shared_ptr<RodElement> ele = m_CameraRod[id];
    if(ele) {
        ele->setStatus(sts);
    }
}

RodElement* zchxRodDataMgr::getRod(const QString &id)
{
    RodElement * target = 0;
    std::shared_ptr<RodElement> ele = m_CameraRod[id];
    if(ele) {
        target = ele.get();
    }
    return target;
}

void zchxRodDataMgr::updateCamera(const QList<ZCHX::Data::ITF_CameraDev>& list)
{
    foreach (ZCHX::Data::ITF_CameraDev cam, list) {
        std::shared_ptr<RodElement> ele = m_CameraRod[cam.mParentEleID];
        if(ele) {
            std::shared_ptr<CameraElement> cam_ele(new CameraElement(cam, mDisplayWidget));
            ele->addChild(cam_ele);
            ele->updateElementStatus(qint64(cam_ele.get()), cam.nStatus == 1);
        }
    }
}

QList<ZCHX::Data::ITF_CameraDev> zchxRodDataMgr::getCameraOfRod(const QString& id)
{
    QList<ZCHX::Data::ITF_CameraDev> list;
    std::shared_ptr<RodElement> ele = m_CameraRod[id];
    if(ele) {
        return ele->getCameraList();
    }

    return list;
}

//相机状态1：正常（在线）、2：掉线（不在线）、512：异常
void zchxRodDataMgr::updateCameraStatus(const QString& rod, const QString& cam_name, int status)
{
    std::shared_ptr<RodElement> ele = m_CameraRod[rod];
    if(!ele) return;

    std::list<std::shared_ptr<Element>> wklist = ele->getChildren(ZCHX::Data::ELE_CAMERA);
    foreach (std::shared_ptr<Element> ele_cam, wklist) {
        CameraElement *cam = static_cast<CameraElement*>(ele_cam.get());
        if(cam && cam->data().szCamName == cam_name) {
            cam->setStatus(status);
            ele->updateElementStatus(qint64(cam), cam->data().nStatus == 1);
            break;
        }
    }
}

QList<ZCHX::Data::IPCastDevice> zchxRodDataMgr::getIPCastDeviceOfRod(const QString &id)
{
    QList<ZCHX::Data::IPCastDevice> list;
    std::shared_ptr<RodElement> ele = m_CameraRod[id];
    if(ele) {
        return ele->getIPCList();
    }

    return list;

}

void zchxRodDataMgr::updateIPCastDeviceList(const QList<ZCHX::Data::IPCastDevice>& list)
{
    foreach (ZCHX::Data::IPCastDevice ipc, list) {
        std::shared_ptr<RodElement> ele = m_CameraRod[ipc.rodID];
        if(ele) {
            std::shared_ptr<IPCElement> ipc_ele(new IPCElement(ipc, mDisplayWidget));
            ele->addChild(ipc_ele);
            ele->updateElementStatus(qint64(ipc_ele.get()), ipc.status != -1);
        }
    }
}

void zchxRodDataMgr::updateIpcStatus(const QString &rod, const QString &ipc, int status)
{
    std::shared_ptr<RodElement> ele = m_CameraRod[rod];
    if(!ele) return;

    std::list<std::shared_ptr<Element>> wklist = ele->getChildren(ZCHX::Data::ELE_IPC);
    foreach (std::shared_ptr<Element> ele_ipc, wklist) {
        IPCElement *ipc_ele = static_cast<IPCElement*>(ele_ipc.get());
        if(ipc_ele && ipc_ele->getData().name == ipc) {
            ipc_ele->setStatus(status);
            ele->updateElementStatus(qint64(ipc_ele), ipc_ele->getData().status != -1);
            break;
        }
    }
}
}


