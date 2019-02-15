#include "zchxradardatamgr.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"

zchxRadarDataMgr::zchxRadarDataMgr(zchxMapWidget* w, QObject *parent) : QObject(parent),mDisplayWidget(w)
{
    mMaxConcernNum = zchxEcdis::Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_CONCERN_NUM, 10).toInt();
    mMaxTailTrackNum = zchxEcdis::Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_TAILTRACK_NUM, 10).toInt();
}

void zchxRadarDataMgr::showRadarPoint(QPainter *painter)
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

void zchxRadarDataMgr::setRadarPointData(const std::vector<DrawElement::RadarPointElement > &data)
{
    QMutexLocker locker(&mDataMutex);
    m_RadarPoint = data;
}

bool zchxRadarDataMgr::updateRadarActiveItem(const QPoint &pt)
{
    return false;
}


void zchxRadarDataMgr::setRadarAreaData(const std::vector<DrawElement::RadarAreaElement> &data)
{
    m_RadarArea = data;
}

void zchxRadarDataMgr::setRadarFeatureZoneData(const std::vector<DrawElement::RadarFeatureZone> &data)
{
    m_radarFeatureZone = data;
}

void zchxRadarDataMgr::setHistoryRadarPointData(const std::vector<DrawElement::RadarPointElement> &data)
{
    //存储上一次雷达的转向(绘制雷达的当前转向)
    m_pRadarPointHistory.clear();
    if(m_HistoryRadarPoint.size() > 1)
    {
        for(int i=0; i < m_HistoryRadarPoint.size(); ++i)
        {
            DrawElement::RadarPointElement &item  = m_HistoryRadarPoint[i];
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
            DrawElement::RadarPoint *pRadarPointItem = NULL;
            for(int i=0; i< m_HistoryRadarPoint.size(); ++i)
            {
                DrawElement::RadarPoint &item = m_HistoryRadarPoint[i];
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
        DrawElement::RadarPoint &item  = m_HistoryRadarPoint[i];
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
