#ifndef ZCHXRADARDATAMGR_H
#define ZCHXRADARDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>
#include <QMutex>

class zchxMapWidget;

class zchxRadarDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxRadarDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void    setRadarPointData(const std::vector<DrawElement::RadarPointElement > &data);
    void    setHistoryRadarPointData(const std::vector<DrawElement::RadarPointElement> &data);
    void    showRadarPoint(QPainter* painter);
    int     getMaxConcernNum() const {return mMaxConcernNum;}
    int     getMaxTailTrackNum() const {return mMaxTailTrackNum;}
    bool    updateRadarActiveItem(const QPoint& pt);

    //雷达扫描区域
    void    setRadarAreaData(const std::vector<DrawElement::RadarAreaElement> &data);
    //雷达特征区域
    void    setRadarFeatureZoneData(const std::vector<DrawElement::RadarFeatureZone> &data);

signals:

public slots:


private:
    std::vector<DrawElement::RadarPointElement > m_RadarPoint;
    std::vector<DrawElement::RadarPointElement> m_HistoryRadarPoint;   //雷达历史点数据
    QMap<int, double>   m_pRadarPointHistory;       //雷达的转向数据(历史数据使用)
    int                 mMaxConcernNum;             //最大关注列表数
    int                 mMaxTailTrackNum;           //最大尾迹数据数
    zchxMapWidget       *mDisplayWidget;
    QMutex              mDataMutex;

    //雷达扫描区域点数据
    std::vector<DrawElement::RadarAreaElement> m_RadarArea;            //雷达扫描区域数据
    std::vector<DrawElement::RadarFeatureZone> m_radarFeatureZone;
};

#endif // ZCHXRADARDATAMGR_H
