#ifndef ZCHXRADARDATAMGR_H
#define ZCHXRADARDATAMGR_H

#include "zchxecdisdatamgr.h"
#include <QMutex>

namespace qt {
class zchxRadarDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxRadarDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void    show(QPainter* painter);
    bool    updateActiveItem(const QPoint& pt);
    void    removeTrack(const QString& id);
    void    removeConcern(const QString& id);

    void    setRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint> &data);
    void    setHistoryRadarPointData(const std::vector<RadarPointElement> &data);
    void    showRadarPoint(QPainter* painter);

    //雷达扫描区域
    void    setRadarAreaData(const std::vector<RadarAreaElement> &data);
    //雷达特征区域
    void    setRadarFeatureZoneData(const std::vector<RadarFeatureZone> &data);

signals:

public slots:


private:
    QMap<QString, std::shared_ptr<RadarPointElement>>     m_RadarPoint;               //实时雷达目标数据
    std::vector<RadarPointElement>     m_HistoryRadarPoint;        //雷达历史点数据
    QMap<int, double>                               m_pRadarPointHistory;       //雷达的转向数据(历史数据使用)
    QMutex              mDataMutex;

    //雷达扫描区域点数据
    std::vector<RadarAreaElement>      m_RadarArea;            //雷达扫描区域数据
    std::vector<RadarFeatureZone>      m_radarFeatureZone;
};
}

#endif // ZCHXRADARDATAMGR_H
