#ifndef AISELEMENT_H
#define AISELEMENT_H

#include "fixelement.h"

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

Q_DECLARE_FLAGS(SHIP_ITEMs, SHIP_ITEM)
Q_DECLARE_OPERATORS_FOR_FLAGS(SHIP_ITEMs)

class ZCHX_ECDIS_EXPORT AisHistoryElement : public FixElement<ZCHX::Data::ITF_AIS_TRACK>
{
    Q_OBJECT
public:
    explicit AisHistoryElement(const ZCHX::Data::ITF_AIS_TRACK& ele, zchxMapWidget* w);
    void     drawElement(QPainter *painter);
    void     drawActive(QPainter * painter);
    void     setHistoryTrackStyle(const QString &color, const int lineWidth);
    bool     contains(const QPoint &pos);
    void     setBigDisplayIndex(int i) {mBigDisplayHistoryIndex = 1;}
private:
    void    drawHistoryTrackPolyLine(std::vector<QPointF>& pts, QPainter* painter);
    void    drawHistoryTrackPoint(QPainter *painter);

private:
    int                                                         mBigDisplayHistoryIndex;                        //当前放大显示的历史迹点
    QString                                                     m_sHistoryTrackStyle;
    int                                                         m_iHistoryTrackWidth;
};

class ZCHX_ECDIS_EXPORT AisElement : public FixElement<ZCHX::Data::ITF_AIS>
{
    Q_OBJECT
public:
    explicit  AisElement(const ZCHX::Data::ITF_AIS &ele, zchxMapWidget* w);

    const std::vector<std::pair<double, double> > &getPath() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    RADARTYPE getType() const;

    void setData(const ZCHX::Data::ITF_AIS &data);

    qint64 getUtc() const;
	uint getStatus() const;
    void setStatus(const uint &value);
    bool hasCamera() const;
    std::vector<ZCHX::Data::ITF_CameraDev> getCameraData() const;
    void setCameraData(const std::vector<ZCHX::Data::ITF_CameraDev> &camera);
    void appendCamera(const ZCHX::Data::ITF_CameraDev& data);

    void drawElement(QPainter *painter);
    void drawTargetInformation(QPainter *painter);
    void drawCollide(QPainter *painter);
    void drawCPA(QPainter *painter);
    void drawShipImage(QPainter *painter);
    void drawShipTriangle(QPainter *painter, const QColor& fillColor);
    void drawMeet(QPainter* painter);
    //历史轨迹
    void drawHistoryTrack(QPainter* painter);
    void drawHistoryTrackPolyLine(std::vector<QPointF>& pts, QPainter* painter);
    void drawHistoryTrackPoint(QPainter* painter);
    //画实时船舶轨迹
    void drawRealtimeTailTrack(QPainter* painter);
    //模拟外推
    void drawExtrapolation(QPainter* painter);
    void updateGeometry(QPointF pos, qreal size);
    bool isEmpty() const;
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

//    void setHistoryTrackList(const QList<ZCHX::Data::ITF_AIS>& list) {mHistoryTrackList = list; mBigDisplayHistoryIndex = -1;}
//    void clearHistoryTrackList() {mHistoryTrackList.clear(); mBigDisplayHistoryIndex = -1;}
//    QList<ZCHX::Data::ITF_AIS> getHistoryTrackList() const {return mHistoryTrackList;}
//    void setBigDisplayHistoryTrackIndex(int index) {mBigDisplayHistoryIndex  = index;}
//    int  getBigDisplayHistoryTrackIndex() {return mBigDisplayHistoryIndex;}

    //void setIsExtrapolate(bool val) {m_isExtrapolate = val;}
    //bool getIsExtrapolate() const {return m_isExtrapolate;}
    //void setExtrapolateTime(double val) {m_ExtrapolateTime = val;}
    //double getExtrapolate() const {return m_ExtrapolateTime;}

    void setHistoryTrackStyle(const QString &color, const int lineWidth);
    void setPrepushTrackStyle(const QString &color, const int lineWidth);
    void clicked(bool isDouble);
    void showToolTip(const QPoint& pos);
    QList<QAction*> getRightMenuAction();
    //标签显示
    void setLabelDisplayMode(SHIP_ITEMs mode) {mLabelDisplayMode = mode;}
    //碰撞船舶
    void setCollideAis(const ZCHX::Data::ITF_AIS& data) {mCollideAis = data;}
signals:
public slots:
    void slotOpenCameraList();
    void slotSetFleet();
    void slotSetCPATrack();
    void slotSetBlackList();
    void slotSetWhiteList();
private:

private:
    uint                        m_status; //0不闪，1闪
    QPolygonF                   m_polygon;
    QPixmap                     m_shipImage;
    QPixmap                     m_cameraImage;
    QPointF                     m_cameraPos;
    bool                        m_drawTargetInfo;
    QList<ZCHX::Data::ITF_AIS>  mRealtimeTailTrackList;             //船舶的尾迹点
    QString                     m_sPrepushTrackStyle;
    int                         m_iPrepushTrackWidth;
    QString                     m_sHistoryTrackStyle;
    int                         m_iHistoryTrackWidth;
    int                         mLabelDisplayMode;
    ZCHX::Data::ITF_AIS         mCollideAis;                        //碰撞船舶信息
    std::vector<ZCHX::Data::ITF_CameraDev> m_cameraDev; //船舶的相机列表
};
}

#endif   //AISELEMENT_H
