#pragma once

#include "IDrawElement.hpp"
#include <QtCore>

namespace DrawElement
{

enum SHIP_ITEM {
    SHIP_ITEM_LABEL = 0,
    SHIP_ITEM_NAME,
    SHIP_ITEM_MMSI,
    SHIP_ITEM_LATLON,
    SHIP_ITEM_SOG,
    SHIP_ITEM_COG,
    SHIP_ITEM_MULTILINE,
    SHIP_ITEM_COUNT,
};

class  AisElement: public Element
{
public:
    AisElement();
    AisElement(const ZCHX::Data::ITF_AIS &ele);

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

    void drawElement(QPainter *painter);
    void drawTargetInformation(const QList<bool> &bList, QPainter *painter);
    void drawCollide(const ZCHX::Data::ITF_AIS& targetAis, QPainter *painter);
    void drawCPA(QPainter *painter);
    void drawShipImage(QPainter *painter);
    void drawShipTriangle(QPainter *painter, const QColor& fillColor);

    bool isEmpty() const;
    void updateGeometry(QPointF pos, int size);

    QPixmap getShipImage();
    QPixmap getCameraImage();
    std::vector<QPointF> getTrack();
    std::vector<QPointF> getTouchdown();

    bool needDrawImage() const;
    void setDrawTargetInfo(bool val);
    virtual void initFromSettings();
    void setTrackList(const QList<ZCHX::Data::ITF_AIS>& list) {mTrackList = list; mBigDisplayTrackIndex = -1;}
    void clearTrackList() {mTrackList.clear(); mBigDisplayTrackIndex = -1;}
    void setBigDisplayTrackIndex(int index) {mBigDisplayTrackIndex = index;}
    int  getBigDisplayTrackIndex() {return mBigDisplayTrackIndex;}
    QList<ZCHX::Data::ITF_AIS> getTrackList() const {return mTrackList;}

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
    QList<ZCHX::Data::ITF_AIS> mTrackList;          //船舶的尾迹点
    int    mBigDisplayTrackIndex;                        //当前放大显示的尾迹点

//    bool    m_isFleet;

    std::vector<ZCHX::Data::ITF_CameraDev> m_cameraDev; //船舶的相机列表
};
}
