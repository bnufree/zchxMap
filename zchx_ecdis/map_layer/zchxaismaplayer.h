#ifndef ZCHXAISMAPLAYER_H
#define ZCHXAISMAPLAYER_H

#include "zchxMapLayer.h"

namespace qt {
class ZCHX_ECDIS_EXPORT zchxAisMapLayer : public MapLayer
{
    Q_OBJECT
public:
    explicit zchxAisMapLayer(zchxMapWidget *drawWidget, bool visible, QObject *parent = 0);
    //Ais数据显示配置
    void SetEnableShipTag(int val);
    void setHistoryTrackStyle(const QString &color, const int lineWidth); //历史尾迹样式
    void setPrepushTrackStyle(const QString &color, const int lineWidth); //预推轨迹样式

    void drawLayer(QPainter* painter);
    void setRealtimeTailTrack(const QString &id, const QList<ZCHX::Data::ITF_AIS> &data);
    void removeRealtimeTailTrack(const QString &id);
    void setPickUpAisInfo(const QString& id);
    void setAisData(const QList<ZCHX::Data::ITF_AIS> &data, bool check);
    void setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data);  //历史回放
    void setConsAisData(const ZCHX::Data::ITF_AIS &aisdata);  //施工船
    void updateCamera(const QList<ZCHX::Data::ITF_CameraDev> &list); //更新船载相机

    //历史轨迹
    void setSingleAisData(const QString& id, const QList<ZCHX::Data::ITF_AIS> &data);
    void removeAisHistoryData(const QString& id);
    void removeHistoryTrack(const QString &id);
signals:

public slots:
private:
    int                                                         mShipTagDisplayMode;    //配置船舶AIStag的显示配置
    QString                                                     m_sHistoryTrackStyle;
    int                                                         m_iHistoryTrackWidth;
    QString                                                     m_sPrepushTrackStyle;
    int                                                         m_iPrepushTrackWidth;
};
}
#endif // ZCHXAISMAPLAYER_H
