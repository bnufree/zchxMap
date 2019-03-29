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
    void drawElement(QPainter* painter);

};

//三角形
class TriangleElement : public FixElement<ZCHX::Data::ITF_EleTriangle>
{
    explicit  TriangleElement(const ZCHX::Data::ITF_EleTriangle &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_EleTriangle>(ele, ZCHX::Data::ELE_TRIANGLE, ZCHX::LAYER_TRIANGLE, w) {}
};

//直线
class LineElement : public FixElement<ZCHX::Data::ITF_EleLine>
{
    explicit  LineElement(const ZCHX::Data::ITF_EleLine &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_EleLine>(ele, ZCHX::Data::ELE_LINE,ZCHX::LAYER_LINE, w) {}
};



//矩形
class RectElement : public FixElement<ZCHX::Data::ITF_EleRect>
{
    explicit  RectElement(const ZCHX::Data::ITF_EleRect &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_EleRect>(ele, ZCHX::Data::ELE_RECT, ZCHX::LAYER_RECT, w) {}
};

/*标准图元*/


//位置标注
class LocalMarkElement :public FixElement<ZCHX::Data::ITF_LocalMark>
{
public:
    explicit  LocalMarkElement(const ZCHX::Data::ITF_LocalMark &ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_LocalMark>(ele, ZCHX::Data::ELE_LOCALMARK, ZCHX::LAYER_LOCALMARK,w) {}
    void drawElement(QPainter *painter);
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
class ZCHX_ECDIS_EXPORT AISBaseStationElement : public FixElement<ZCHX::Data::ITF_AISBASESTATION>
{

public:
    explicit AISBaseStationElement(const ZCHX::Data::ITF_AISBASESTATION &data, zchxMapWidget* w)
    :FixElement<ZCHX::Data::ITF_AISBASESTATION>(data, ZCHX::Data::ELE_AIS_BASE_STATION, ZCHX::LAYER_AIS_STATION, w) {}
    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
    virtual void clicked(bool isDouble ) {}
    virtual void showToolTip(const QPoint& pos);
private:
    QPolygonF   m_polygon;

};

//AIS雷达融合
class ZCHX_ECDIS_EXPORT AISRadarFuseElement : public FixElement<ZCHX::Data::ITF_AIS>
{
public:
    explicit AISRadarFuseElement(const ZCHX::Data::ITF_AIS &data, zchxMapWidget* w)
    :FixElement<ZCHX::Data::ITF_AIS>(data, ZCHX::Data::ELE_AIS_FUSION, ZCHX::LAYER_AIS_FUSION, w) {}
    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);

private:
    QPolygonF m_polygon;
};

//CDMA设备
class ZCHX_ECDIS_EXPORT CDMAElement : public FixElement<ZCHX::Data::ITF_AIS>
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
class ZCHX_ECDIS_EXPORT BigDipperElement : public FixElement<ZCHX::Data::ITF_AIS>
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

class ZCHX_ECDIS_EXPORT CameraRegionElement : public FixElement<ZCHX::Data::ITF_CameraRegion>
{
public:
    explicit CameraRegionElement(const ZCHX::Data::ITF_CameraRegion &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_CameraRegion>(data, ZCHX::Data::ELE_CAMERA_REGION, ZCHX::LAYER_CAMERA_REGION, w)
        ,m_isVisited(false)
    {}

    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
    inline bool isVisited() const {return m_isVisited;}
    inline void setIsVisited(bool isVisited) {m_isVisited = isVisited;}
private:
    void drawRegion(QPainter* painter, const QColor& pen, const QColor& brush);
    void drawText(QPainter *painter, bool adjust_text_pos,  const QString& ststext, const QString& photoText = QString());

private:
    QPolygonF m_Region;
    bool m_isVisited; //开始盘点后，标记此盘点区域是否被进入过

};

class ZCHX_ECDIS_EXPORT DramCircularElement : public FixElement<ZCHX::Data::ITF_DramCircular>
{
public:
    explicit DramCircularElement(const ZCHX::Data::ITF_DramCircular &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_DramCircular>(data, ZCHX::Data::ELE_DRAM_CIRCUAL, ZCHX::LAYER_VESSEL_TARGET, w)
    {}

    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
private:

private:
    QPolygonF m_polygon;

};

class ZCHX_ECDIS_EXPORT VesselTargetElement : public FixElement<ZCHX::Data::ITF_VesselTargetData>
{
public:
    explicit VesselTargetElement(const ZCHX::Data::ITF_VesselTargetData &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_VesselTargetData>(data, ZCHX::Data::ELE_VESSEL_TARGET, ZCHX::LAYER_VESSEL_TARGET, w)
    {}

    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
private:

private:
    QPolygonF m_polygon;

};

class ZCHX_ECDIS_EXPORT VesselTrackElement : public FixElement<ZCHX::Data::ITF_VesselTrackData>
{
public:
    explicit VesselTrackElement(const ZCHX::Data::ITF_VesselTrackData &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_VesselTrackData>(data, ZCHX::Data::ELE_VESSEL_TRACK, ZCHX::LAYER_VESSEL_TRACK, w)
    {}

    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
private:

private:
    QRectF m_trackRect;
};

class ZCHX_ECDIS_EXPORT VesselTrackLineElement : public FixElement<ZCHX::Data::ITF_VoyageTrackInfo>
{
public:
    explicit VesselTrackLineElement(const ZCHX::Data::ITF_VoyageTrackInfo &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_VoyageTrackInfo>(data, ZCHX::Data::ELE_VESSEL_TRACK_LINE, ZCHX::LAYER_VESSEL_TRACK, w)
    {}

    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);
private:

private:
    QPolygonF m_Line;
};

class ZCHX_ECDIS_EXPORT HistoryTraceElement : public FixElement<ZCHX::Data::ITF_AIS>
{
public:
    explicit HistoryTraceElement(const ZCHX::Data::ITF_AIS &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_AIS>(data, ZCHX::Data::ELE_AIS_HIRTORY, ZCHX::LAYER_AIS_TRACK, w)
    {}

    void drawElement(QPainter *painter) ;
private:

private:
};

class ZCHX_ECDIS_EXPORT WeatherWindWavesElement : public FixElement<ZCHX::Data::ITF_WeatherWindWaves>
{
public:
    explicit WeatherWindWavesElement(const ZCHX::Data::ITF_WeatherWindWaves &data, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_WeatherWindWaves>(data, ZCHX::Data::ELE_WEATHER_WAVE, ZCHX::LAYER_WEATHER, w)
    {
        weatherImg.load(":/zchxemap/images/weather.png");
    }

    void drawElement(QPainter *painter) ;
    void updateGeometry(QPointF pos, qreal size);

private:
   QPixmap weatherImg;

};

}

#endif // ZCHXFIXELEMENTS_H
