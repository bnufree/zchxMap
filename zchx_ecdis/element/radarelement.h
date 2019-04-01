#ifndef RADARELE_H
#define RADARELE_H

#include "fixelement.h"

namespace qt
{
class  RadarPointElement: public FixElement<ZCHX::Data::ITF_RadarPoint>
{
public:
    RadarPointElement(const ZCHX::Data::ITF_RadarPoint &ele, zchxMapWidget* frame);

    enum RADAR_SHARE{
        Radar_Rect = 0,
        Radar_Ellipse,
    };

    std::pair<double, double> getPoint();

    const std::vector<std::pair<double, double> > &getPath() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    bool getShan() const;
    void setShan(bool shan);

    void setDrawAsAis(bool sts) {mDrawAsAis = sts;}
    bool getDrawAsAis() const {return mDrawAsAis;}

    bool  getDrawShapeAsRect() const {return mRadarShapeAsRect;}

    //0不闪，1闪
    uint getStatus() const;
    void setStatus(const uint &value);

    bool getNeedDrawBox() const;
    void setNeedDrawBox(bool needDrawBox);
    virtual void initFromSettings();
    //目标描绘
    void drawElement(QPainter *painter);
    void copyDataFromOther(std::shared_ptr<Element> other);
    //绘制轨迹点
    void drawTrack(QPainter *painter);
    void drawText(QPainter *painter, QPointF pos, int sideLen);
    void clicked(bool isDouble);
    void showToolTip(const QPoint &pos);
    QList<QAction*> getRightMenuAction();

private:
    bool m_shan;
    bool m_needDrawBox;
    uint m_status; //0不闪，1闪
    bool mRadarShapeAsRect; //true:矩形,false:圆形
    bool mDrawAsAis;  //是否将目标画为一个船舶图标
};

class  RadarAreaElement: public FixElement<ZCHX::Data::ITF_RadarArea>
{
public:
    explicit RadarAreaElement(const ZCHX::Data::ITF_RadarArea &ele, zchxMapWidget* v);
    double radarX() const;
    void setRadarX(double radarX);

    double radarY() const;
    void setRadarY(double radarY);

    int centerLineAngel() const;
    void setCenterLineAngel(int centerLineAngel);

    int radius() const;
    void setRadius(int radius);

    int maxScanRangeANgle() const;
    void setMaxScanRangeANgle(int maxScanRangeANgle);

    int numberofChannele() const;
    void setNumberofChannele(int numberofChannele);

    int maxWakePointsNumber() const;
    void setMaxWakePointsNumber(int maxWakePointsNumber);
    void drawElement(QPainter* painter);
    //指定点是否在雷达扫描区域内
    bool contains(const QPoint& pos) {return mShapePnts.contains(pos);}
    QPolygonF getShapePnts(double angleFromNorth) const;
    void updateGeometry(QPointF pos, qreal size) {}
    void copyDataFromOther(std::shared_ptr<Element> other);
private:
    QPolygonF mShapePnts;
};
}

#endif // RADARELE_H
