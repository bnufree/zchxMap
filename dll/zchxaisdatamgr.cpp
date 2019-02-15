#include "zchxaisdatamgr.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"

zchxAisDataMgr::zchxAisDataMgr(zchxMapWidget* w, QObject *parent) : QObject(parent),mDisplayWidget(w)
{
    mMaxConcernNum = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_CONCERN_NUM, 10).toInt();
    mMaxTailTrackNum = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_TAILTRACK_NUM, 10).toInt();
    mReplaceTrackWherOver = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_TRACK, true).toBool();
}

void zchxAisDataMgr::showRadarPoint(QPainter *painter)
{
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
}

void zchxAisDataMgr::setRadarPointData(const std::vector<DrawElement::RadarPointElement > &data)
{
    QMutexLocker locker(&mDataMutex);
    m_RadarPoint = data;
}

bool zchxAisDataMgr::updateRadarActiveItem(const QPoint &pt)
{
    return false;
}


bool zchxAisDataMgr::setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    //检查当前配置的默认最大显示的历史轨迹数,默认是10,超出范围时默认是删除第一个使得数目符合最大值
    if(m_aisTraceMap.size() < mMaxTailTrackNum || m_aisTraceMap.contains(id))
    {
        //如果ID值的历史轨迹已经存在,则更新轨迹数据,同时将轨迹点选择清除
        m_aisTraceMap.insert(id, data);
        m_aisIndexMap.insert(id, -1);
    } else {
        //超出最大值的情况
        if(!mReplaceTrackWherOver)
        {
            QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多%1条历史轨迹.").arg(mMaxTailTrackNum));
            return false;
        } else
        {
            //移除当前Map中的第一条船舶.注意这里map并不是按照添加的时间先后顺序删除,而是按照key的字母顺序删除
            QString key = m_aisTraceMap.firstKey();
            m_aisTraceMap.remove(key);
            m_aisIndexMap.remove(key);
            //添加新的数据,没有选择点
            m_aisTraceMap.insert(id, data);
            m_aisIndexMap.insert(id, -1);
        }
    }

    return true;
}

void zchxAisDataMgr::removeAisHistoryData(QString id)
{
    m_aisTraceMap.remove(id);
    m_aisIndexMap.remove(id);
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
    std::shared_ptr<DrawElement::AisElement>item = m_aisMap.value(id, NULL);
    if(!item) return;
    item->setIsActive(true);
    //将当前的船舶居中放大显示
    std::pair<double, double> ll = item->getLatLon();
    mDisplayWidget->setCenterAtTargetLL(ll.first, ll.second);
    //更新当前选择的图元目标
    mDisplayWidget->setCurrentSelectedItem(item);
}

DrawElement::AisElement *zchxAisDataMgr::getCurrentAis()
{
    if(m_curActivShip.startsWith("AIS") && m_aisMap.contains(m_curActivShip))
    {
        return m_aisMap[m_curActivShip];
    }
    return NULL;
}


void zchxAisDataMgr::setAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    //获取当前选中的ais, 如果数据有更新需要重新推送消息
    DrawElement::AisElement *curritem = getCurrentAis();
    //m_aisMap.clear();
    //clearAisData(QDateTime::currentMSecsSinceEpoch(), 1); //1S清理一次
    QList<QString> dataId;
    for(const ZCHX::Data::ITF_AIS &aisdatas : data)
    {
        dataId.append(aisdatas.id);
    }
    QList<QString> oldDataId = m_aisMap.uniqueKeys();
    for(int number = 0; number < oldDataId.size(); ++number)
    {
        QString dataShipId = oldDataId.at(number);
        if(!dataId.contains(dataShipId))
        {
            DrawElement::AisElement *item = m_aisMap.value(dataShipId, 0);
            if(item) delete item;
            m_aisMap.remove(dataShipId);
        }
    }

    // 船舶历史轨迹
    QList<QString> oldTraceDataId = m_aisTraceMap.uniqueKeys();
    for(int number = 0; number < oldTraceDataId.size(); ++number)
    {
        QString dataShipId = oldTraceDataId.at(number);
        if(!dataId.contains(dataShipId))
        {
            m_aisTraceMap.remove(dataShipId);
            m_aisIndexMap.remove(dataShipId);
            emit signalSendHistoryTrail(dataShipId, false);
        }
    }

    QStringList wklist;
    for (const ZCHX::Data::ITF_AIS &aisdata : data)
    {
        DrawElement::AisElement *item = NULL;
        if(m_aisMap.contains(aisdata.id)) //目标已经存在, 则更新数据
        {
            item = m_aisMap[aisdata.id];
            item->setData(aisdata);

            //当前选中的对象数据有变化, 重新推送一条通知
            if(item == curritem)
            {
                //emit signalIsSelected4Ais(item->getData(), false);
            }

            if(m_curTrackShip == aisdata.id)
            {
                //qDebug()<<"目标跟踪id :: "<<m_curTrackShip;
                m_curTrackShip = aisdata.id; //设置当前选中跟踪船舶的
                emit signalIsSelected4TrackRadarOrbit(aisdata, false);
            }
        }
        else //目标不存在, 则添加数据
        {
            item = new DrawElement::AisElement(aisdata);
            item->m_framework = m_framework.get();
            item->m_drawWidget = this;
            m_aisMap.insert(aisdata.id, item);
//            if(item == curritem)
//            {
//                emit signalIsSelected4Ais(item->getData(), false);
//            }
        }
        Q_ASSERT(item);
        item->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
        if(mAisConcernList.contains(item->getData().id))
        {
            wklist.append(item->getData().id);
            item->setIsConcern(true);
        }
    }
    mAisConcernList = wklist;
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
    //TODO: 判断是否需要移除过长时间没有数据更新的对象
}

void zchxAisDataMgr::setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    //获取当前选中的ais, 如果数据有更新需要重新推送消息
    DrawElement::AisElement *curritem = getHistoryCurrentAis();
    QList<QString> dataId;
    for(const ZCHX::Data::ITF_AIS &aisdatas : data)
    {
        dataId.append(aisdatas.id);
    }
    QList<QString> oldDataId = m_historyAisMap.uniqueKeys();
    for(int number = 0; number<oldDataId.size(); ++number)
    {
        QString dataShipId = oldDataId.at(number);
        if(!dataId.contains(dataShipId))
        {
            DrawElement::AisElement *item = m_historyAisMap.value(dataShipId, 0);
            if(item) delete item;
            m_historyAisMap.remove(dataShipId);
        }
    }
    for(const ZCHX::Data::ITF_AIS &aisdata : data)
    {
        DrawElement::AisElement *item = NULL;
        if(m_historyAisMap.contains(aisdata.id)) //目标已经存在, 则更新数据
        {
            item = m_historyAisMap[aisdata.id];
            item->setData(aisdata);

            //当前选中的对象数据有变化, 重新推送一条通知
            if(item == curritem)
            {
                emit signalIsSelected4Ais(item->getData(), false);
            }

            if(m_curTrackShip == aisdata.id)
            {
                //qDebug()<<"目标跟踪id :: "<<m_curTrackShip;
                m_curTrackShip = aisdata.id; //设置当前选中跟踪船舶的
                emit signalIsSelected4TrackRadarOrbit(aisdata, false);
            }
        }
        else //目标不存在, 则添加数据
        {
            item = new DrawElement::AisElement(aisdata);
            item->m_framework = m_framework.get();
            item->m_drawWidget = this;;
            m_historyAisMap.insert(aisdata.id, item);
        }
        Q_ASSERT(item);
        item->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
    }
    if (m_bCameraTargerTrack &&m_cameraTrackTarget.type == 1)
    {
        DrawElement::AisElement *item = m_historyAisMap.value(m_cameraTrackTarget.id, NULL);
        if(item)
        {
            m_cameraTrackTarget.id = item->getStrID(); //设置当前选中的船舶的
            m_cameraTrackTarget.lon = item->lon();
            m_cameraTrackTarget.lat = item->lat();
            m_cameraTrackTarget.type = 1;
            emit signalIsEcdisCameraTrackTarget(m_cameraTrackTarget);
        }
        else // 目标丢失(千米目标丢失的给移除了)
        {
            m_cameraTrackTarget.type = 0;
        }
    }
}

void zchxAisDataMgr::setConsAisData(const ZCHX::Data::ITF_AIS &aisdata)
{
    DrawElement::AisElement *item = NULL;
    if(m_aisMap.contains(aisdata.id)) //目标已经存在, 则更新数据
    {
        item = m_aisMap[aisdata.id];
        item->setData(aisdata);
    }
    else //目标不存在, 则添加数据
    {
        item = new DrawElement::AisElement(aisdata);
        item->m_framework = m_framework.get();
        item->m_drawWidget = this;
        item->setForceImage(true);
        item->setDrawTargetInfo(false);
        m_aisMap.insert(aisdata.id, item);
    }
    Q_ASSERT(item);
    item->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
}

void zchxAisDataMgr::setClearHistoryData(bool states)
{
    if(states)
    {
        foreach (QString key, m_historyAisMap.keys()) {
            DrawElement::AisElement* item = m_historyAisMap.value(key, 0);
            if(item) delete item;
        }
        m_historyAisMap.clear();
        m_HistoryRadarPoint.clear();
    }
    else
    {
        m_aisTraceMap.clear();
    }
}


std::shared_ptr<DrawElement::AisElement> zchxAisDataMgr::getCurrentAis()
{
    if(!mDisplayWidget || !mDisplayWidget->getCurrentSelectedElement()) return NULL;
    std::shared_ptr<DrawElement::Element> item = mDisplayWidget->getCurrentSelectedElement();
    if(item->getElementType() != ZCHX::Data::ELEMENT_AIS) return NULL;
    return std::shared_ptr<DrawElement::AisElement>(static_cast<DrawElement::AisElement*>(item.get()));
}

DrawElement::AisElement *zchxAisDataMgr::getHistoryCurrentAis()
{
    if(m_curActivShip.startsWith("AIS") && m_historyAisMap.contains(m_curActivShip))
    {
        return m_historyAisMap[m_curActivShip];
    }
    return NULL;
}

void zchxAisDataMgr::updateAisElement(const QList<ZCHX::Data::ITF_AIS> &data)
{

}
