#include "zchxradardatamgr.h"
#include "zchxmapframe.h"

namespace qt {
zchxRadarDataMgr::zchxRadarDataMgr(zchxMapWidget* w, QObject *parent) : zchxEcdisDataMgr(w, ZCHX_DATA_MGR_RADAR, parent)
{
    mMaxConcernNum = Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_CONCERN_NUM, 10).toInt();
    mMaxTailTrackNum = Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_TAILTRACK_NUM, 10).toInt();
}

void zchxRadarDataMgr::removeTrack(const QString &id)
{
    if(isTrack(id))
    {
        zchxEcdisDataMgr::removeTrack(id);
        //emit mDisplayWidget->signalSendHistoryTrail(id, false);
    }
}

void zchxRadarDataMgr::removeConcern(const QString &id)
{
    if(isConcern(id))
    {
        zchxEcdisDataMgr::removeConcern(id);
    }
}

void zchxRadarDataMgr::show(QPainter *painter)
{
    if(!mDisplayWidget || !mDisplayWidget->getMapLayerMgr()) return;
    if(!mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_RADAR)) return;
    QMutexLocker locker(&mDataMutex);
    QMap<QString, std::shared_ptr<RadarPointElement>>::iterator it = m_RadarPoint.begin();
    for(; it != m_RadarPoint.end(); ++it)
    {
        std::shared_ptr<RadarPointElement> item = it.value();
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_RADAR_CURRENT))
        {
            item->setIsOpenMeet(mDisplayWidget->getIsOpenMeet());
            //检查当前点是否在矩形区域内
            if(!mDisplayWidget->rect().contains(item->getCurrentPos().toPoint())) continue;
            //预警状态, 闪烁; 0为无预警
            if(item->getStatus() > 0)
            {
                item->drawFlashRegion(painter, item->getCurrentPos(), item->getStatus(), item->getData().warnStatusColor);
            }
            item->drawElement(painter);
        }
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_RADAR_TRACK) && item->getIsTailTrack())
        {
            item->drawTrack(painter);
        }
    }
}

void zchxRadarDataMgr::setRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint> &data)
{
    QMutexLocker locker(&mDataMutex);
    //删除当前实时数据中不存在的旧数据
    QList<QString> newDataId;
    for(ZCHX::Data::ITF_RadarPoint radar : data)
    {
        newDataId.append(QString::number(radar.trackNumber));
    }
    QList<QString> oldDataId = m_RadarPoint.uniqueKeys();
    for(int number = 0; number < oldDataId.size(); ++number)
    {
        QString trackID = oldDataId.at(number);
        if(!newDataId.contains(trackID))
        {
            //取消原来数据的关注和尾迹
            removeConcern(trackID);
            removeTrack(trackID);
            std::shared_ptr<RadarPointElement> item = m_RadarPoint.value(trackID, 0);
            if(item) item.reset();
            //删除对应的实时数据
            m_RadarPoint.remove(trackID);
        }
    }

    //更新实时雷达数据
    for (const ZCHX::Data::ITF_RadarPoint &aisdata : data)
    {
        //更新item对应的数据
        QString id = QString::number(aisdata.trackNumber);
        std::shared_ptr<RadarPointElement> item = m_RadarPoint.value(id, 0);
        if(!item) {
            item = std::shared_ptr<RadarPointElement>(new RadarPointElement(aisdata));
            item->setFrameWork(mDisplayWidget->framework());
        } else {
            item->setData(aisdata);
        }
        if(isConcern(id)){
            item->setIsConcern(true);
        } else {
            item->setIsConcern(false);
        }
        if(isTrack(id)){
            item->setIsTailTrack(true);
        } else {
            item->setIsTailTrack(false);
        }
        if(item == mDisplayWidget->getCurrentSelectedElement()) {
            item->setIsActive(true);
        } else {
            item->setIsActive(false);
        }
        item->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
    }
#if 0
    if (m_bCameraTargerTrack &&m_cameraTrackTarget.type == 1)
    {
        AisElement *item = m_aisMap.value(m_cameraTrackTarget.id, NULL);
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

bool zchxRadarDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}


void zchxRadarDataMgr::setRadarAreaData(const std::vector<RadarAreaElement> &data)
{
    m_RadarArea = data;
}

void zchxRadarDataMgr::setRadarFeatureZoneData(const std::vector<RadarFeatureZone> &data)
{
    m_radarFeatureZone = data;
}

void zchxRadarDataMgr::setHistoryRadarPointData(const std::vector<RadarPointElement> &data)
{
    //存储上一次雷达的转向(绘制雷达的当前转向)
    m_pRadarPointHistory.clear();
    if(m_HistoryRadarPoint.size() > 1)
    {
        for(int i=0; i < m_HistoryRadarPoint.size(); ++i)
        {
            RadarPointElement &item  = m_HistoryRadarPoint[i];
            double point = item.getData().cog;
            int radar_number = item.getData().trackNumber;
            m_pRadarPointHistory.insert(radar_number, point);
        }
    }
    m_HistoryRadarPoint = data;
#if 0

    if (m_bCameraTargerTrack)
    {
        if(m_cameraTrackTarget.type == 2) // 上次数据为雷达
        {
            bool isFind = false;
            RadarPoint *pRadarPointItem = NULL;
            for(int i=0; i< m_HistoryRadarPoint.size(); ++i)
            {
                RadarPoint &item = m_HistoryRadarPoint[i];
                pRadarPointItem = &item;
                if(pRadarPointItem)
                {
                    ZCHX::Data::ITF_RadarPoint radarPonintData = pRadarPointItem->getData();
                    QString numberid = QString::number(radarPonintData.trackNumber);
                    if(numberid == m_cameraTrackTarget.id)
                    {
                        isFind = true;
                        std::vector<std::pair<double, double>> path = radarPonintData.path;
                        int point = path.size();
                        std::pair<double, double> ll = path[point -1];

                        m_cameraTrackTarget.id = QString::number(radarPonintData.trackNumber); //设置当前选中的船舶的
                        m_cameraTrackTarget.lon = ll.second;
                        m_cameraTrackTarget.lat = ll.first;
                        m_cameraTrackTarget.type = 2;
                        emit signalIsEcdisCameraTrackTarget(m_cameraTrackTarget);
                        //releaseDrawStatus();
                        break;
                    }
                }
            }
            if (!isFind) // 目标丢失
            {
                m_cameraTrackTarget.type = 0;
                //m_bCameraTargerTrack = false;
            }
        }
    }
    //判断是否用鼠标选择了船
    bool ok = false;
    uint trackNumber =  m_curActivShip.toInt(&ok);
    if(!ok)
    {
        return;
    }
    for(int i=0; i < m_HistoryRadarPoint.size(); ++i)
    {
        RadarPoint &item  = m_HistoryRadarPoint[i];
        const ZCHX::Data::ITF_RadarPoint& point = item.getData();
        if(point.trackNumber == trackNumber)
        {
            item.setIsActive(true);
            //emit signalIsSelected4RadarPoint(point,true);
            return;
        }
        else
        {
            item.setIsActive(false);
        }
    }
#endif
}
}
