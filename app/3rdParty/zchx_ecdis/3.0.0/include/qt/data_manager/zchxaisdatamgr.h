#ifndef ZCHXAISDATAMGR_H
#define ZCHXAISDATAMGR_H

#include "zchxecdisdatamgr.h"
#include <QMutex>

namespace qt {
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
    std::shared_ptr<AisElement> getCurrentAis(); //当前地图窗口是否选择了ais
    void setAisData(const QList<ZCHX::Data::ITF_AIS> &data, bool check = true);
    void clearDisplayData() {m_aisMap.clear();}
    void setConsAisData(const ZCHX::Data::ITF_AIS &aisdata);  //施工船显示.施工船强制显示为图片
    //历史数据
    void setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data);
    void setClearHistoryData(bool states);
    //Ais数据显示配置
    void SetEnableShipTag(int val);
    void setHistoryTrackStyle(const QString &color, const int lineWidth); //历史尾迹样式
    void setPrepushTrackStyle(const QString &color, const int lineWidth); //预推轨迹样式
signals:

public slots:


private:
    QMutex              mDataMutex;
    QHash<QString, std::shared_ptr<AisElement>>    m_aisData;              //船舶模拟数据
    QHash<QString, std::shared_ptr<AisElement>>    m_aisMap;               //实时AIS对象
    //船舶的轨迹合并到了船舶图元,这里暂且注销
    //QMap<QString, QList<ZCHX::Data::ITF_AIS>>                   m_aisTraceMap;          //船舶历史轨迹
    //QMap<QString, int>                                          m_aisIndexMap;          //当前船舶历史轨迹线上放大显示的轨迹点下标
    QHash<QString, std::shared_ptr<AisElement>>    m_historyAisMap;        //AIS历史对象,历史回放时使用
    int                                                         mShipTagDisplayMode;    //配置船舶AIStag的显示配置
    QString                                                     m_sHistoryTrackStyle;
    int                                                         m_iHistoryTrackWidth;
    QString                                                     m_sPrepushTrackStyle;
    int                                                         m_iPrepushTrackWidth;
};
}

#endif // zchxAisDataMgr_H
