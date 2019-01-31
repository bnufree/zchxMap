#include "zchxradardatamgr.h"

zchxRadarDataMgr::zchxRadarDataMgr(zchxMapWidget* w, QObject *parent) : QObject(parent),mDisplayWidget(w)
{

}

void zchxRadarDataMgr::showRadarPoint(QPainter *painter)
{
    std::vector<std::shared_ptr<DrawElement::RadarPointElement>>::iterator it;
    for(it = m_RadarPoint.begin(); it != m_RadarPoint.end(); ++it)
    {
        std::shared_ptr<DrawElement::RadarPointElement> item = (*it);
        item.drawElement(painter);
    }
}

void zchxRadarDataMgr::setRadarPointData(const std::vector<std::shared_ptr<DrawElement::RadarPointElement> > &data)
{
    //存储上一次雷达的转向(绘制雷达的当前转向)
    m_pRadarPointHistory.clear();
    if(m_RadarPoint.size() > 1)
    {
        for(int i=0; i < m_RadarPoint.size(); ++i)
        {
            DrawElement::RadarPoint &item  = m_RadarPoint[i];
            double point = item.getData().cog;
            int radar_number = item.getData().trackNumber;
            m_pRadarPointHistory.insert(radar_number, point);
        }
    }

    //更新雷达关注和尾迹的列表ID
    QList<int> wklist;
    for(int i=0; i < m_RadarPoint.size(); ++i)
    {
        DrawElement::RadarPoint &item  = m_RadarPoint[i];
        if(mRadarConcernList.contains(item.getData().trackNumber))
        {
            item.setIsConcern(true);
            wklist.append(item.getData().trackNumber);
        }
    }
    mRadarConcernList = wklist;
    wklist.clear();

    for(int i=0; i < m_RadarPoint.size(); ++i)
    {
        DrawElement::RadarPoint &item  = m_RadarPoint[i];
        if(mRadarTailTrackList.contains(item.getData().trackNumber))
        {
            item.setIsTailTrack(true);
            wklist.append(item.getData().trackNumber);
        }
    }
    mRadarTailTrackList = wklist;
    wklist.clear();

    for(int i=0; i < data.size(); ++i)
    {
        DrawElement::RadarPoint item  = data[i];
    }

    m_RadarPoint = data;
    if (m_bCameraTargerTrack)
    {
        if(m_cameraTrackTarget.type == 2) // 上次数据为雷达
        {
            bool isFind = false;
            DrawElement::RadarPoint *pRadarPointItem = NULL;
            for(int i=0; i< m_RadarPoint.size(); ++i)
            {
                DrawElement::RadarPoint &item = m_RadarPoint[i];
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

    for(int i=0; i < m_RadarPoint.size(); ++i)
    {
        DrawElement::RadarPoint &item  = m_RadarPoint[i];
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

        //qDebug()<<"radar property:"<<item.getData().trackNumber<<item.getDrawAsAis()<<item.getDrawShape()<<item.getFillingColor().name()<<item.getBorderColor().name();
    }
}
