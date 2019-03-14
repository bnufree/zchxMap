#ifndef ZCHXFIXELEMENTS_H
#define ZCHXFIXELEMENTS_H

#include "fixelement.h"

namespace qt {

//海图绘制元素
class EllipseElement : public FixElement<ZCHX::Data::ITF_EleEllipse>
{
public:
    explicit  EllipseElement(const ZCHX::Data::ITF_EleEllipse &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_EleEllipse>(ele, ZCHX::Data::ELE_ELLIPSE, ZCHX::LAYER_ELLIPSE, w) {}
};

//三角形
class TriangleElement : public FixElement<ZCHX::Data::ITF_EleTriangle>
{
    explicit  TriangleElement(const ZCHX::Data::ITF_EleTriangle &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_EleTriangle>(ele, ZCHX::Data::ELE_TRIANGLE, ZCHX::LAYER_Triangle, w) {}
};

//直线
class LineElement : public FixElement<ZCHX::Data::ITF_EleLine>
{
    explicit  LineElement(const ZCHX::Data::ITF_EleLine &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_EleLine>(ele, ZCHX::Data::ELE_LINE,ZCHX::LAYER_Line, w) {}
};



//矩形
class RectElement : public FixElement<ZCHX::Data::ITF_EleRect>
{
    explicit  RectElement(const ZCHX::Data::ITF_EleRect &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_EleRect>(ele, ZCHX::Data::ELE_RECT, ZCHX::LAYER_Rect, w) {}
};

/*标准图元*/


//位置标注
class LocalMarkElement :public FixElement<ZCHX::Data::ITF_LocalMark>
{
public:
    explicit  LocalMarkElement(const ZCHX::Data::ITF_LocalMark &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_LocalMark>(ele, ZCHX::Data::ELE_LOCALMARK, ZCHX::LAYER_LOCALMARK,w) {}
};

//导航
class  NavigationElement : public FixElement<ZCHX::Data::ITF_Navigation>
{
public:
    explicit  NavigationElement(const ZCHX::Data::ITF_Navigation &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_Navigation>(ele, ZCHX::Data::ELE_NAVIGATION, ZCHX::LAYER_NAVIGATION_MARK, w) {}

};

//危险圈
class DangerousCircleElement : public FixElement<ZCHX::Data::ITF_DangerousCircle>
{
public:
    explicit DangerousCircleElement(const ZCHX::Data::ITF_DangerousCircle &data, zchxMapWidget* w)
    :FixElement<ZCHX::Data::ITF_DangerousCircle>(data, ZCHX::Data::ELE_DANGREOUS, ZCHX::LAYER_DANGEROUS_CIRLE, w) {}
};

//特征区域
class RadarFeatureZoneElement : public FixElement<ZCHX::Data::ITF_RadarFeaturesZone>
{
public:
    explicit RadarFeatureZoneElement(const ZCHX::Data::ITF_RadarFeaturesZone &data, zchxMapWidget* w)
    :FixElement<ZCHX::Data::ITF_RadarFeaturesZone>(data, ZCHX::Data::ELE_RADAR_FEATURE_ZONE, ZCHX::LAYER_RADAR_FRETURE_AREA, w) {}
    void drawElement(QPainter *painter);
};

//相机视场
class CameraViewElement : public FixElement<ZCHX::Data::ITF_CameraView>
{
public:
    explicit CameraViewElement(const ZCHX::Data::ITF_CameraView &data, zchxMapWidget* w)
    :FixElement<ZCHX::Data::ITF_CameraView>(data, ZCHX::Data::ELE_CAMERA_VIEW, ZCHX::LAYER_CAMERA_VIEW, w) {}
};

//AIS基站
class AISBaseStationElement : public FixElement<ZCHX::Data::ITF_AISBASESTATION>
{

public:
    explicit AISBaseStationElement(const ZCHX::Data::ITF_AISBASESTATION &data, zchxMapWidget* w)
    :FixElement<ZCHX::Data::ITF_AISBASESTATION>(data, ZCHX::Data::ELE_AIS_BASE_STATION, ZCHX::LAYER_AIS_Station, w) {}
    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
    void setData(const ZCHX::Data::ITF_AISBASESTATION &data);
    virtual void clicked(bool isDouble ) {}
    virtual void showToolTip(const QPoint& pos);
private:
    QPolygonF   m_polygon;

};

//AIS雷达融合
class AISRadarFuseElement : public FixElement<ZCHX::Data::ITF_AIS>
{
public:
    explicit AISRadarFuseElement(const ZCHX::Data::ITF_AIS &data, zchxMapWidget* w)
    :FixElement<ZCHX::Data::ITF_AIS>(data, ZCHX::Data::ELE_AIS_FUSION, ZCHX::LAYER_AIS_FUSION, w) {}
    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
    void setData(const ZCHX::Data::ITF_AIS &data);

private:
    QPolygonF m_polygon;
};

//CDMA设备
class CDMAElement : public FixElement<ZCHX::Data::ITF_AIS>
{
public:
    explicit CDMAElement(const ZCHX::Data::ITF_AIS &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_AIS>(data, ZCHX::Data::ELE_CDMA, ZCHX::LAYER_CDMA_TARGET, w)
        ,m_oldCog(0)
    {

    }

    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
private:
    QPolygonF m_polygon;
    qreal m_oldCog; //上一次的航向

};

//北斗设备
class BigDipperElement : public FixElement<ZCHX::Data::ITF_AIS>
{
public:
    explicit BigDipperElement(const ZCHX::Data::ITF_AIS &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_AIS>(data, ZCHX::Data::ELE_BIG_DIPPER, ZCHX::LAYER_BIGDIPPER_TARGET, w)
        ,m_oldCog(0)
    {

    }
    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
private:
    QRectF m_bdRect;
    qreal m_oldCog; //上一次的航向
};

}

#endif // ZCHXFIXELEMENTS_H
