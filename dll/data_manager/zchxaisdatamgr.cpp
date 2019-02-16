#include "zchxaisdatamgr.h"

zchxAisDataMgr::zchxAisDataMgr(zchxMapWidget* w, QObject *parent) : zchxEcdisDataMgr(w, parent)
{
    mMaxConcernNum = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_CONCERN_NUM, 10).toInt();
    mMaxTailTrackNum = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_TAILTRACK_NUM, 10).toInt();
    mReplaceTrackWherOver = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_TRACK, true).toBool();
}

void zchxAisDataMgr::show(QPainter *painter)
{
#if 0
    if(!mDisplayWidget || !mDisplayWidget->getMapLayerMgr()) return;
    if(!mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_RADAR)) return;
    QMutexLocker locker(&mDataMutex);
    std::vector<DrawElement::RadarPointElement>::iterator it;
    for(it = m_RadarPoint.begin(); it != m_RadarPoint.end(); ++it)
    {
        DrawElement::RadarPointElement& item = (*it);
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_RADAR_CURRENT))
        {
            item.setIsOpenMeet(mDisplayWidget->getIsOpenMeet());
            //检查当前点是否在矩形区域内
            if(!mDisplayWidget->rect().contains(item.getCurrentPos().toPoint())) continue;
            //预警状态, 闪烁; 0为无预警
            if(item.getStatus() > 0)
            {
                item.drawFlashRegion(painter, item.getCurrentPos(), item.getStatus(), item.getData().warnStatusColor);
            }
            item.drawElement(painter);
        }
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_RADAR_TRACK) && item.getIsTailTrack())
        {
            item.drawTrack(painter);
        }
    }
#endif
}

bool zchxAisDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}


bool zchxAisDataMgr::setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    if(!isTrack(id)) return false;
    std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(id, NULL);
    if(item)
    {
        item->setTrackList(data);
        return true;
    }
    return false;
}

void zchxAisDataMgr::removeAisHistoryData(QString id)
{
    removeTrack(id);
}

void zchxAisDataMgr::removeTrack(const QString &id)
{
    if(isTrack(id))
    {
        std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(id, 0);
        if(item) item->clearTrackList();
        zchxEcdisDataMgr::removeTrack(id);
        emit mDisplayWidget->signalSendHistoryTrail(id, false);
    }
}

void zchxAisDataMgr::removeConcern(const QString &id)
{
    if(isConcern(id))
    {
        zchxEcdisDataMgr::removeConcern(id);
    }
}


void zchxAisDataMgr::SetPickUpAisInfo(QString id)
{
    mDisplayWidget->setCurrentSelectedItem(0);
    mDisplayWidget->setCurPickupType(ZCHX::Data::ECDIS_PICKUP_AIS);
    //将当前所有的船舶选择状态清空
    QHash<QString, std::shared_ptr<DrawElement::AisElement>>::const_iterator it = m_aisMap.begin();
    for(; it != m_aisMap.end(); ++it)
    {
        it.value()->setIsActive(false);
    }
    //将当前id的船舶设定为active状态
    if(id.isEmpty()) return;
    std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(id, NULL);
    if(!item) return;
    item->setIsActive(true);
    //将当前的船舶居中放大显示
    std::pair<double, double> ll = item->getLatLon();
    mDisplayWidget->setCenterAtTargetLL(ll.first, ll.second);
    //更新当前选择的图元目标
    mDisplayWidget->setCurrentSelectedItem(item);
}

void zchxAisDataMgr::setAisData(const QList<ZCHX::Data::ITF_AIS> &data, bool check )
{
    if(check)
    {
        //删除当前实时数据中不存在的旧数据
        QList<QString> newDataId;
        for(const ZCHX::Data::ITF_AIS &aisdatas : data)
        {
            newDataId.append(aisdatas.id);
        }
        QList<QString> oldDataId = m_aisMap.uniqueKeys();
        for(int number = 0; number < oldDataId.size(); ++number)
        {
            QString dataShipId = oldDataId.at(number);
            if(!newDataId.contains(dataShipId))
            {
                //取消原来数据的关注和尾迹
                removeConcern(dataShipId);
                removeTrack(dataShipId);
                std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(dataShipId, 0);
                if(item) item.reset();
                //删除对应的实时数据
                m_aisMap.remove(dataShipId);
            }
        }
    }

    //更新实时AIS数据
    for (const ZCHX::Data::ITF_AIS &aisdata : data)
    {
        //更新item对应的数据
        std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(aisdata.id, 0);
        if(!item) {
            item = std::shared_ptr<DrawElement::AisElement>(new DrawElement::AisElement(aisdata));
            item->setFrameWork(mDisplayWidget->framework());
        } else {
            item->setData(aisdata);
        }
        if(isConcern(aisdata.id)){
            item->setIsConcern(true);
        } else {
            item->setIsConcern(false);
        }
        if(isTrack(aisdata.id)){
            item->setIsTailTrack(true);
        } else {
            item->setIsTailTrack(false);
        }
        if(item == getCurrentAis()) {
            item->setIsActive(true);
        } else {
            item->setIsActive(false);
        }
        if(aisdata.is_construction_ship)
        {
            item->setForceImage(true);
            item->setDrawTargetInfo(false);
        }
        item->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
    }
#if 0
    if (m_bCameraTargerTrack &&m_cameraTrackTarget.type == 1)
    {
        DrawElement::AisElement *item = m_aisMap.value(m_cameraTrackTarget.id, NULL);
        if(item)
        {
                m_cameraTrackTarget.id = item->getStrID(); //设置当前选中的船舶的
                m_cameraTrackTarget.lon = item->lon();
                m_cameraTrackTarget.lat = item->lat();
                m_cameraTrackTarget.type = 1;
                emit signalIsEcdisCameraTrackTarget(m_cameraTrackTarget);
                //releaseDrawStatus();
        }
        else // 目标丢失(千米目标丢失的给移除了)
        {
            m_cameraTrackTarget.type = 0;
            //m_bCameraTargerTrack = false;
        }
    }
#endif
    //TODO: 判断是否需要移除过长时间没有数据更新的对象
}

//将历史和实时的数据进行合并,切换显示模式的时候 清除对应的数据
void zchxAisDataMgr::setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    setAisData(data);
}

void zchxAisDataMgr::setConsAisData(const ZCHX::Data::ITF_AIS &aisdata)
{
    setAisData(QList<ZCHX::Data::ITF_AIS>() <<aisdata, false);
}


std::shared_ptr<DrawElement::AisElement> zchxAisDataMgr::getCurrentAis()
{
    if(!mDisplayWidget || !mDisplayWidget->getCurrentSelectedElement()) return NULL;
    std::shared_ptr<DrawElement::Element> item = mDisplayWidget->getCurrentSelectedElement();
    if(item->getElementType() != ZCHX::Data::ELEMENT_AIS) return NULL;
    return std::shared_ptr<DrawElement::AisElement>(static_cast<DrawElement::AisElement*>(item.get()));
}

void zchxAisDataMgr::setClearHistoryData(bool states)
{
    if(states)
    {
        //清空历史数据
        m_aisMap.clear();
        //m_HistoryRadarPoint.clear();
    }
    else
    {
        //清空历史尾迹
        while (mTrackList.size() > 0) {
            removeTrack(mTrackList.first());
        }
    }
}

