#ifndef ZCHXAISDATAMGR_H
#define ZCHXAISDATAMGR_H

#include "zchxecdisdatamgr.h"
#include <QMutex>

class zchxAisDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxAisDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void    show(QPainter* painter);
    bool    updateActiveItem(const QPoint& pt);
    void    removeTrack(const QString& id);
    void    removeConcern(const QString& id);

    //单船历史轨迹数据更新
    bool setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data);
    void removeAisHistoryData(QString id);
    //实时数据管理
    void SetPickUpAisInfo(QString id);          //设定当前选中的船舶ID
    std::shared_ptr<DrawElement::AisElement> getCurrentAis(); //当前地图窗口是否选择了ais
    void setAisData(const QList<ZCHX::Data::ITF_AIS> &data, bool check = true);
    void clearDisplayData() {m_aisMap.clear();}
    void setConsAisData(const ZCHX::Data::ITF_AIS &aisdata);  //施工船显示.施工船强制显示为图片
    //历史数据
    void setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data);
    void setClearHistoryData(bool states);


signals:

public slots:


private:
    QMutex              mDataMutex;
    QHash<QString, std::shared_ptr<DrawElement::AisElement>>    m_aisData;              //船舶模拟数据
    QHash<QString, std::shared_ptr<DrawElement::AisElement>>    m_aisMap;               //实时AIS对象
    //船舶的轨迹合并到了船舶图元,这里暂且注销
    //QMap<QString, QList<ZCHX::Data::ITF_AIS>>                   m_aisTraceMap;          //船舶历史轨迹
    //QMap<QString, int>                                          m_aisIndexMap;          //当前船舶历史轨迹线上放大显示的轨迹点下标
    QHash<QString, std::shared_ptr<DrawElement::AisElement>>    m_historyAisMap;        //AIS历史对象,历史回放时使用
};

#endif // zchxAisDataMgr_H
