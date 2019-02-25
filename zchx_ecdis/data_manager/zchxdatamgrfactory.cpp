#include "zchxdatamgrfactory.h"

namespace qt {
zchxDataMgrFactory::zchxDataMgrFactory(zchxMapWidget* w) : mWidget(w)
{

}

std::shared_ptr<zchxEcdisDataMgr> zchxDataMgrFactory::createManager(int type)
{
    switch (type) {
    case ZCHX_DATA_MGR_AIS:
        mMgrList[type] = std::shared_ptr<zchxAisDataMgr>(new zchxAisDataMgr(mWidget));
        break;
    case ZCHX_DATA_MGR_RADAR:
        mMgrList[type] = std::shared_ptr<zchxRadarDataMgr>(new zchxRadarDataMgr(mWidget));
        break;
    default:
        break;
    }

    return getManager(type);
}

std::shared_ptr<zchxEcdisDataMgr> zchxDataMgrFactory::getManager(int type)
{
    if(mMgrList.contains(type)) return mMgrList[type];
    return 0;
}

QList<std::shared_ptr<zchxEcdisDataMgr>> zchxDataMgrFactory::getManagers() const
{
    return mMgrList.values();
}

bool zchxDataMgrFactory::removeDataMgr(std::shared_ptr<zchxEcdisDataMgr> mgr)
{
    if(!mgr) return false;
    return mMgrList.remove(mgr->getType()) != 0;
}

bool zchxDataMgrFactory::appendDataMgr(std::shared_ptr<zchxEcdisDataMgr> mgr)
{
    if(!mgr) return false;
    if(mMgrList.contains(mgr->getType())) return true;
    mMgrList[mgr->getType()] = mgr;
    return true;
}

zchxAisDataMgr* zchxDataMgrFactory::getAisDataMgr()
{
    return static_cast<zchxAisDataMgr*>(getManager(ZCHX_DATA_MGR_AIS).get());
}

zchxRadarDataMgr* zchxDataMgrFactory::getRadarDataMgr()
{
    return static_cast<zchxRadarDataMgr*>(getManager(ZCHX_DATA_MGR_RADAR).get());
}

zchxCameraDataMgr* zchxDataMgrFactory::getCameraDataMgr()
{
    return static_cast<zchxCameraDataMgr*>(getManager(ZCHX_DATA_MGR_CAMERA).get());
}

zchxRodDataMgr* zchxDataMgrFactory::getRodDataMgr()
{
    return static_cast<zchxRodDataMgr*>(getManager(ZCHX_DATA_MGR_ROD).get());
}

zchxCameraViewDataMgr* zchxDataMgrFactory::getCameraViewMgr()
{
    return static_cast<zchxCameraViewDataMgr*>(getManager(ZCHX_DATA_MGR_CANMERA_VIEW).get());
}

zchxVideoTargetDataMgr* zchxDataMgrFactory::getVideoDataMgr()
{
    return static_cast<zchxVideoTargetDataMgr*>(getManager(ZCHX_DATA_MGR_VIDEO_TARGET).get());
}

}


