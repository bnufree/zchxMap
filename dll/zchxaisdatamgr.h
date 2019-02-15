#ifndef ZCHXAISDATAMGR_H
#define ZCHXAISDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>
#include <QMutex>

class zchxMapWidget;

class zchxAisDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxAisDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void    setRadarPointData(const std::vector<DrawElement::RadarPointElement > &data);
    void    showRadarPoint(QPainter* painter);
    int     getMaxConcernNum() const {return mMaxConcernNum;}
    int     getMaxTailTrackNum() const {return mMaxTailTrackNum;}
    bool    updateRadarActiveItem(const QPoint& pt);

    //单船历史轨迹数据更新
    bool setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data);
    void removeAisHistoryData(QString id);
    //实时数据管理
    void SetPickUpAisInfo(QString id);          //设定当前选中的船舶ID
    std::shared_ptr<DrawElement::AisElement> getCurrentAis(); //当前地图窗口是否选择了ais


signals:

public slots:


private:
    std::vector<DrawElement::RadarPointElement > m_RadarPoint;
    int                 mMaxConcernNum;             //最大关注列表数
    int                 mMaxTailTrackNum;           //最大尾迹数据数
    bool                mReplaceTrackWherOver;      //是否自动覆盖轨迹点显示当超出最大轨迹线数目

    zchxMapWidget       *mDisplayWidget;
    QMutex              mDataMutex;

    QHash<QString, std::shared_ptr<DrawElement::AisElement>> m_aisData;             //船舶模拟数据
    QHash<QString, std::shared_ptr<DrawElement::AisElement>> m_aisMap;              //实时AIS对象
    QMap<QString, QList<ZCHX::Data::ITF_AIS> > m_aisTraceMap;       //船舶历史轨迹
    QMap<QString, int> m_aisIndexMap;                               //当前船舶历史轨迹线上放大显示的轨迹点下标
    QHash<QString, std::shared_ptr<DrawElement::AisElement>> m_historyAisMap;       //AIS历史对象,历史回放时使用
};

#endif // zchxAisDataMgr_H
