#pragma once

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt
{

enum SHIP_ITEM {
    SHIP_ITEM_LABEL = 0x01,
    SHIP_ITEM_NAME = 0x02,
    SHIP_ITEM_MMSI = 0x04,
    SHIP_ITEM_LATLON = 0x08,
    SHIP_ITEM_SOG = 0x10,
    SHIP_ITEM_COG = 0x20,
    SHIP_ITEM_MULTILINE = 0x40,
    SHIP_ITEM_DEFAULT = SHIP_ITEM_LABEL | SHIP_ITEM_NAME | SHIP_ITEM_MMSI | SHIP_ITEM_LATLON | SHIP_ITEM_SOG | SHIP_ITEM_COG | SHIP_ITEM_MULTILINE,
};

class  AisElement: public Element
{
public:
    AisElement(zchxMapWidget* frame);
    AisElement(const ZCHX::Data::ITF_AIS &ele, zchxMapWidget* frame);

    const std::vector<std::pair<double, double> > &getPath() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    RADARTYPE getType() const;

    const ZCHX::Data::ITF_AIS &getData() const;
    void setData(const ZCHX::Data::ITF_AIS &data);

    qint64 getUtc() const;
	uint getStatus() const;
    void setStatus(const uint &value);
    bool hasCamera() const;
    std::vector<ZCHX::Data::ITF_CameraDev> getCameraData() const;
    void setCameraData(const std::vector<ZCHX::Data::ITF_CameraDev> &camera);
    void appendCamera(const ZCHX::Data::ITF_CameraDev& data);

    void drawElement(QPainter *painter);
    void drawTargetInformation(int mode, QPainter *painter);
    void drawCollide(const ZCHX::Data::ITF_AIS& targetAis, QPainter *painter);
    void drawCPA(QPainter *painter);
    void drawShipImage(QPainter *painter);
    void drawShipTriangle(QPainter *painter, const QColor& fillColor);
    //历史轨迹
    void drawHistoryTrack(QPainter* painter);
    void drawHistoryTrackPolyLine(std::vector<QPointF>& pts, QPainter* painter);
    void drawHistoryTrackPoint(QPainter* painter);
    //画实时船舶轨迹
    void drawRealtimeTailTrack(QPainter* painter);
    //模拟外推
    void drawExtrapolation(QPainter* painter);

    bool isEmpty() const;
    void updateGeometry(QPointF pos, int size);

    QPixmap getShipImage();
    QPixmap getCameraImage();
    std::vector<QPointF> getTrack();
    std::vector<QPointF> getTouchdown();

    bool needDrawImage() const;
    void setDrawTargetInfo(bool val);
    virtual void initFromSettings();

    void setRealtimeTailTrackList(const QList<ZCHX::Data::ITF_AIS>& list) {mRealtimeTailTrackList = list;}
    void clearRealtimeTailTrackList() {mRealtimeTailTrackList.clear();}
    QList<ZCHX::Data::ITF_AIS> getRealtimeTailTrackList() const {return mRealtimeTailTrackList;}

    void setHistoryTrackList(const QList<ZCHX::Data::ITF_AIS>& list) {mHistoryTrackList = list; mBigDisplayHistoryIndex = -1;}
    void clearHistoryTrackList() {mHistoryTrackList.clear(); mBigDisplayHistoryIndex = -1;}
    QList<ZCHX::Data::ITF_AIS> getHistoryTrackList() const {return mHistoryTrackList;}
    void setBigDisplayHistoryTrackIndex(int index) {mBigDisplayHistoryIndex  = index;}
    int  getBigDisplayHistoryTrackIndex() {return mBigDisplayHistoryIndex;}

    void setIsExtrapolate(bool val) {m_isExtrapolate = val;}
    bool getIsExtrapolate() const {return m_isExtrapolate;}
    void setExtrapolateTime(double val) {m_ExtrapolateTime = val;}
    double getExtrapolate() const {return m_ExtrapolateTime;}

    void setHistoryTrackStyle(const QString &color, const int lineWidth);
    void setPrepushTrackStyle(const QString &color, const int lineWidth);
    //bool contains(QPointF pos) const;
    void doubleClickNow();

//    bool getFleet();
//    void setFleet(bool val);
private:

private:
    ZCHX::Data::ITF_AIS m_data;
    uint m_status; //0不闪，1闪
    QPolygonF m_polygon;
    QPixmap m_shipImage;
    QPixmap m_cameraImage;
    QPointF m_cameraPos;
    bool    m_drawTargetInfo;
    QList<ZCHX::Data::ITF_AIS> mRealtimeTailTrackList;          //船舶的尾迹点
    int    mBigDisplayHistoryIndex;                        //当前放大显示的历史迹点
    QList<ZCHX::Data::ITF_AIS> mHistoryTrackList;          //船舶的历史轨迹点

    //模拟外推
    bool    m_isExtrapolate;
    double  m_ExtrapolateTime;
    QString                                                     m_sHistoryTrackStyle;
    int                                                         m_iHistoryTrackWidth;
    QString                                                     m_sPrepushTrackStyle;
    int                                                         m_iPrepushTrackWidth;

//    bool    m_isFleet;

    std::vector<ZCHX::Data::ITF_CameraDev> m_cameraDev; //船舶的相机列表
};
}
