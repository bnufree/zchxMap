#include "zchxdatamgrfactory.h"

namespace qt {
zchxDataMgrFactory* zchxDataMgrFactory::minstance = 0;
zchxDataMgrFactory::MGarbage zchxDataMgrFactory::Garbage;

zchxDataMgrFactory::zchxDataMgrFactory() : mWidget(0)
{

}

zchxDataMgrFactory::~zchxDataMgrFactory()
{

}

zchxDataMgrFactory* zchxDataMgrFactory::instance()
{
    if(minstance == 0)
    {
        minstance = new zchxDataMgrFactory;
    }
    return minstance;
}

void zchxDataMgrFactory::setDisplayWidget(zchxMapWidget *w)
{
    mWidget = w;
}

std::shared_ptr<zchxEcdisDataMgr> zchxDataMgrFactory::createManager(int type)
{
    switch (type) {
    case DATA_MGR_AIS:
        mMgrList[type] = std::shared_ptr<zchxAisDataMgr>(new zchxAisDataMgr(mWidget));
        break;
    case DATA_MGR_RADAR:
        mMgrList[type] = std::shared_ptr<zchxRadarDataMgr>(new zchxRadarDataMgr(mWidget));
        break;
    case DATA_MGR_CAMERA:
        mMgrList[type] = std::shared_ptr<zchxCameraDataMgr>(new zchxCameraDataMgr(mWidget));
        break;
    case DATA_MGR_CAMERA_VIEW:
        mMgrList[type] = std::shared_ptr<zchxCameraViewDataMgr>(new zchxCameraViewDataMgr(mWidget));
        break;
    case DATA_MGR_VIDEO_TARGET:
        mMgrList[type] = std::shared_ptr<zchxVideoTargetDataMgr>(new zchxVideoTargetDataMgr(mWidget));
        break;
    case DATA_MGR_ROD:
        mMgrList[type] = std::shared_ptr<zchxRodDataMgr>(new zchxRodDataMgr(mWidget));
        break;
    case DATA_MGR_WARNING_ZONE:
        mMgrList[type] = std::shared_ptr<zchxWarningZoneDataMgr>(new zchxWarningZoneDataMgr(mWidget));
        break;
    case DATA_MGR_COAST:
        mMgrList[type] = std::shared_ptr<zchxCoastDataMgr>(new zchxCoastDataMgr(mWidget));
        break;
    case DATA_MGR_SEABEDIPLINE:
        mMgrList[type] = std::shared_ptr<zchxSeabedPipLineDataMgr>(new zchxSeabedPipLineDataMgr(mWidget));
        break;
    case DATA_MGR_CHANNEL:
        mMgrList[type] = std::shared_ptr<zchxChannelDataMgr>(new zchxChannelDataMgr(mWidget));
        break;
    case DATA_MGR_CAMERA_NET_GRID:
        //mMgrList[type] = std::shared_ptr<zchxWarningZoneDataMgr>(new zchxWarningZoneDataMgr(mWidget));
        break;
    case DATA_MGR_STRUCTURE:
        mMgrList[type] = std::shared_ptr<zchxStructureDataMgr>(new zchxStructureDataMgr(mWidget));
        break;
    case DATA_MGR_AREANET:
        mMgrList[type] = std::shared_ptr<zchxAreaNetDataMgr>(new zchxAreaNetDataMgr(mWidget));
        break;
    case DATA_MGR_MOOR:
        mMgrList[type] = std::shared_ptr<zchxMooringDataMgr>(new zchxMooringDataMgr(mWidget));
        break;
    case DATA_MGR_CARDMOUTH:
        mMgrList[type] = std::shared_ptr<zchxCardMouthDataMgr>(new zchxCardMouthDataMgr(mWidget));
        break;
    case DATA_MGR_LOCAL_MARK:
        mMgrList[type] = std::shared_ptr<zchxLocalMarkDataMgr>(new zchxLocalMarkDataMgr(mWidget));
        break;
    case DATA_MGR_DANGEROUS:
        mMgrList[type] = std::shared_ptr<zchxDangerousDataMgr>(new zchxDangerousDataMgr(mWidget));
        break;
    case DATA_MGR_PASTROLSTATION:
        mMgrList[type] = std::shared_ptr<zchxPastrolStationDataMgr>(new zchxPastrolStationDataMgr(mWidget));
        break;
    case DATA_MGR_IPC:
        break;
    case DATA_MGR_ISLANDLINE:
        mMgrList[type] = std::shared_ptr<zchxIslandLineDataMgr>(new zchxIslandLineDataMgr(mWidget));
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
    return static_cast<zchxAisDataMgr*>(getManager(DATA_MGR_AIS).get());
}

zchxRadarDataMgr* zchxDataMgrFactory::getRadarDataMgr()
{
    return static_cast<zchxRadarDataMgr*>(getManager(DATA_MGR_RADAR).get());
}

zchxCameraDataMgr* zchxDataMgrFactory::getCameraDataMgr()
{
    return static_cast<zchxCameraDataMgr*>(getManager(DATA_MGR_CAMERA).get());
}

zchxRodDataMgr* zchxDataMgrFactory::getRodDataMgr()
{
    return static_cast<zchxRodDataMgr*>(getManager(DATA_MGR_ROD).get());
}

zchxCameraViewDataMgr* zchxDataMgrFactory::getCameraViewMgr()
{
    return static_cast<zchxCameraViewDataMgr*>(getManager(DATA_MGR_CAMERA_VIEW).get());
}

zchxVideoTargetDataMgr* zchxDataMgrFactory::getVideoDataMgr()
{
    return static_cast<zchxVideoTargetDataMgr*>(getManager(DATA_MGR_VIDEO_TARGET).get());
}

zchxPastrolStationDataMgr* zchxDataMgrFactory::getPastrolStationMgr()
{
    return static_cast<zchxPastrolStationDataMgr*>(getManager(DATA_MGR_PASTROLSTATION).get());
}

zchxWarningZoneDataMgr* zchxDataMgrFactory::getWarningZoneMgr()
{
    return static_cast<zchxWarningZoneDataMgr*>(getManager(DATA_MGR_WARNING_ZONE).get());
}

zchxCoastDataMgr*   zchxDataMgrFactory::getCoastMgr()
{
    return static_cast<zchxCoastDataMgr*>(getManager(DATA_MGR_COAST).get());
}

zchxSeabedPipLineDataMgr* zchxDataMgrFactory::getSeabedPiplineMgr()
{
    return static_cast<zchxSeabedPipLineDataMgr*>(getManager(DATA_MGR_SEABEDIPLINE).get());
}

zchxChannelDataMgr* zchxDataMgrFactory::getChannelMgr()
{
    return static_cast<zchxChannelDataMgr*>(getManager(DATA_MGR_CHANNEL).get());
}

zchxStructureDataMgr*   zchxDataMgrFactory::getStructureMgr()
{
    return static_cast<zchxStructureDataMgr*>(getManager(DATA_MGR_STRUCTURE).get());
}

zchxAreaNetDataMgr* zchxDataMgrFactory::getAreanetMgr()
{
    return static_cast<zchxAreaNetDataMgr*>(getManager(DATA_MGR_AREANET).get());
}

zchxMooringDataMgr* zchxDataMgrFactory::getMooringMgr()
{
    return static_cast<zchxMooringDataMgr*>(getManager(DATA_MGR_MOOR).get());
}

zchxCardMouthDataMgr*   zchxDataMgrFactory::getCardmouthMgr()
{
    return static_cast<zchxCardMouthDataMgr*>(getManager(DATA_MGR_CARDMOUTH).get());
}

zchxLocalMarkDataMgr*   zchxDataMgrFactory::getLocalmarkMgr()
{
    return static_cast<zchxLocalMarkDataMgr*>(getManager(DATA_MGR_LOCAL_MARK).get());
}

zchxDangerousDataMgr*   zchxDataMgrFactory::getDangerousMgr()
{
    return static_cast<zchxDangerousDataMgr*>(getManager(DATA_MGR_DANGEROUS).get());
}

zchxIslandLineDataMgr*   zchxDataMgrFactory::getIslandlineMgr()
{
    return static_cast<zchxIslandLineDataMgr*>(getManager(DATA_MGR_ISLANDLINE).get());
}

}


