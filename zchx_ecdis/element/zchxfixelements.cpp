#include "zchxfixelements.h"
#include "zchxmapframe.h"
#include "map_layer/zchxmaplayermgr.h"
#include <QToolTip>

using namespace qt;

void RadarFeatureZoneElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    QPolygonF polygon;
    for(int i=0; i<data().pointList.size();++i)
    {
        QPointF pos = this->framework()->LatLon2Pixel(data().pointList[i]).toPointF();
        polygon.append(pos);
        if(getIsActive())
        {
            PainterPair chk(painter);
            painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
            painter->setBrush(Qt::white);
            painter->drawEllipse(pos,5,5);
        }
    }
    //封闭
    if(polygon.first() != polygon.last())
    {
        polygon.append(polygon.first());
    }
    PainterPair chk(painter);
    painter->setPen(QPen(QColor(Qt::red),1,Qt::DashLine));
    painter->setBrush(QBrush(Qt::blue, Qt::Dense7Pattern));
    painter->drawPolygon(polygon);
    painter->drawText(polygon.boundingRect().center(), data().getName());
}

void AISBaseStationElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;

    PainterPair chk(painter);
    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());
    painter->setPen(QPen(Qt::black,1));
    if(!m_data.status)
        painter->setBrush(Qt::yellow);
    else
        painter->setBrush(Qt::gray);

    Element::drawElement(painter);
    painter->drawPolygon(m_polygon);
}

void AISBaseStationElement::updateGeometry(QPointF pos, qreal size)
{
    m_polygon.clear();
    qreal half = size / 2.0;
    m_polygon << QPointF(pos.x() - half , pos.y() + half)
              << QPointF(pos.x() + half , pos.y() + half )
              << QPointF(pos.x() + half /2.0 , pos.y() - (half + 10))
              << QPointF(pos.x() - half /2.0 , pos.y() - (half + 10));

    m_boundingRect =  m_polygon.boundingRect().normalized();
    Element::updateGeometry(pos, size);
}

void AISBaseStationElement::setData(const ZCHX::Data::ITF_AISBASESTATION &data)
{
    FixElement::setData(data);
    setID(m_data.aisId);
    Element::setLatLon(m_data.getLat(),m_data.getLon());
    Element::setDisplayLat(m_data.getLat());
    Element::setDisplayLon(m_data.getLon());
}

void AISBaseStationElement::showToolTip(const QPoint &pos)
{
    ZCHX::Data::ITF_AISBASESTATION data = this->data();
    QStringList infoList;
    infoList.append(QObject::tr("基站名称：%1").arg(data.aisName));
    infoList.append(QObject::tr("基站ID：%1").arg(data.aisId));
    infoList.append(QObject::tr("基站IP：%1").arg(data.ipAddress));
    infoList.append(QObject::tr("基站港口：%1").arg(data.position));
    infoList.append(QObject::tr("基站经度：%1").arg(FLOAT_STRING(data.getLon(), 6)));
    infoList.append(QObject::tr("基站纬度：%1").arg(FLOAT_STRING(data.getLat(), 6)));
    QToolTip::showText(pos, infoList.join("\n"));
}

void AISRadarFuseElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    PainterPair chk(painter);
    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());
    drawFlashRegion(painter, pos, m_data.warn_status, m_data.warnStatusColor);
    painter->setBrush(Qt::NoBrush);

    //绘制正方形
    QPolygonF radar;
    radar.append(QPointF(pos.x()-2  , pos.y()+4.5 ));
    radar.append(QPointF(pos.x()-2  , pos.y()+8.5 ));
    radar.append(QPointF(pos.x()+2  , pos.y()+8.5 ));
    radar.append(QPointF(pos.x()+2  , pos.y()+4.5 ));
    radar.append(QPointF(pos.x()-2  , pos.y()+4.5 ));
    painter->setPen(QPen(QColor(255,0,255)));
    painter->drawPolygon(radar);

    //绘制三角形
    QPolygonF ais;
    ais.append(QPointF(pos.x()  , pos.y()-8.5 ));
    ais.append(QPointF(pos.x()-5.5  , pos.y()+8.5 ));
    ais.append(QPointF(pos.x()+5.5  , pos.y()+8.5 ));
    ais.append(QPointF(pos.x()  , pos.y()-8.5 ));
    painter->setPen(QPen(QColor(0,0,64)));
    painter->drawPolygon(ais);
    drawActive(painter);
    drawFocus(painter);
}

void AISRadarFuseElement::updateGeometry(QPointF pos, qreal size)
{
    m_polygon = QPolygonF();
    m_polygon.append(QPointF(pos.x()-20  , pos.y()       ));
    m_polygon.append(QPointF(pos.x()     , pos.y()-20    ));
    m_polygon.append(QPointF(pos.x()+20  , pos.y()       ));
    m_polygon.append(QPointF(pos.x()     , pos.y()+20    ));
    m_polygon.append(QPointF(pos.x()-20  , pos.y()       ));

    m_boundingRect =  m_polygon.boundingRect().normalized();
    Element::updateGeometry(pos, size);
}

void AISRadarFuseElement::setData(const ZCHX::Data::ITF_AIS &data)
{
    FixElement::setData(data);
    setID(data.id);
    Element::setLatLon(m_data.getLat(),m_data.getLon());
    Element::setDisplayLat(m_data.getLat());
    Element::setDisplayLon(m_data.getLon());

}

void CDMAElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    QPointF pos = getCurrentPos();
    updateGeometry(pos, getDrawScaleSize());

    drawFlashRegion(painter, pos, m_data.warn_status, m_data.warnStatusColor);

    //先画轨迹
    if(MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CDMA_TRACK)){
        PainterPair chk(painter);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::black));
        painter->drawPolygon(QPolygonF(QVector<QPointF>::fromStdVector(framework()->convert2QtPonitList(m_data.getPath()))));
    }
    {
        PainterPair chk(painter);
        double angleFromNorth = m_data.cog + framework()->GetRotateAngle(); //计算当前正北方向的方向角
        painter->translate(pos.x(), pos.y());
        painter->rotate(angleFromNorth);
        painter->translate(-pos.x(), -pos.y());
        painter->setBrush(Qt::green);
        painter->drawPolygon(m_polygon);
    }
    //绘制速度方向线
    drawSpeedDirectionLine(painter, QPointF(pos.x(), pos.y()-5), m_data.sog, m_data.cog, m_data.cog - m_oldCog);
    m_oldCog = m_data.cog;

    drawActive(painter);
    drawFocus(painter);
}


void CDMAElement::updateGeometry(QPointF pos, qreal size)
{
    m_polygon.clear();
    m_polygon.append(QPointF(pos.x()-5  , pos.y()       ));
    m_polygon.append(QPointF(pos.x()     , pos.y()-5    ));
    m_polygon.append(QPointF(pos.x()+5  , pos.y()       ));
    m_polygon.append(QPointF(pos.x()     , pos.y()+5    ));
    m_polygon.append(QPointF(pos.x()-5  , pos.y()       ));
    m_boundingRect =  m_polygon.boundingRect().normalized();
    Element::updateGeometry(pos, size);
}


void BigDipperElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    QPointF pos = getCurrentPos();
    updateGeometry(pos, getDrawScaleSize());

    drawFlashRegion(painter, pos, m_data.warn_status, m_data.warnStatusColor);
    //先画轨迹
    if(MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_BIGDIPPER)){
        PainterPair chk(painter);
        painter->setPen(QPen(Qt::black));
        painter->drawPolygon(QPolygonF(QVector<QPointF>::fromStdVector(framework()->convert2QtPonitList(m_data.getPath()))));
    }
    {
        PainterPair chk(painter);
        double angleFromNorth = m_data.cog + framework()->GetRotateAngle(); //计算当前正北方向的方向角
        painter->translate(pos.x(), pos.y());
        painter->rotate(angleFromNorth);
        painter->translate(-pos.x(), -pos.y());
        painter->setBrush(Qt::magenta);
        painter->drawEllipse(m_bdRect);
    }
    //绘制速度方向线
    drawSpeedDirectionLine(painter, QPointF(pos.x(), pos.y()-5), m_data.sog, m_data.cog, m_data.cog - m_oldCog);
    m_oldCog = m_data.cog;

    drawActive(painter);
    drawFocus(painter);
}


void BigDipperElement::updateGeometry(QPointF pos, qreal size)
{
    m_bdRect = QRectF(0, 0, 10, 10);
    m_bdRect.moveCenter(pos);
    m_boundingRect =  m_bdRect.adjusted(-1,-1,1,1);
    Element::updateGeometry(pos, size);
}

void CameraRegionElement::drawRegion(QPainter *painter, const QColor& penColor, const QColor& brushColor)
{
     PainterPair chk(painter);
     QPen pen = painter->pen();
     pen.setColor(penColor);
     painter->setPen(pen);
     painter->setBrush(QBrush(brushColor,Qt::SolidPattern));//设置画刷形式
     painter->drawPolygon(m_Region, Qt::FillRule::WindingFill);
}

void CameraRegionElement::drawText(QPainter *painter, bool adjust_text_pos, const QString &ststext, const QString &photoText)
{
    PainterPair chk(painter);

    QFont font = painter->font();
    font.setPointSize(16);
    painter->setFont(font);

    QPen textPen = painter->pen(); //文字画笔
    textPen.setColor(Qt::black);
    textPen.setWidth(10);
    painter->setPen(textPen);

    painter->setBrush(Qt::NoBrush);

    QRect textRect = painter->fontMetrics().boundingRect(ststext);
    if(m_boundingRect.width() >= textRect.width())
    {
        QPoint pos = getViewPos().toPoint();
        if(adjust_text_pos) pos.setY(pos.y() - textRect.height());
        textRect.moveCenter(pos);
        painter->drawText(textRect, Qt::AlignCenter, ststext);
        if(photoText.length() > 0)
        {
            QPoint center = textRect.center();
            QPoint photoCenter = QPoint(center.x(), center.y() + textRect.height());
            QRect photoTextRect =  painter->fontMetrics().boundingRect(photoText);
            photoTextRect.moveCenter(photoCenter);
            painter->drawText(photoTextRect, Qt::AlignCenter, photoText);
        }
    }

}

void CameraRegionElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    Element::drawElement(painter);
    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());

    PainterPair chk(painter);
    switch (m_data.Status) {
    case ZCHX::Data::DEFAULT:
    {
        drawRegion(painter, Qt::blue, QColor(0,0,255,255*0.6));
        drawText(painter, false,  QObject::tr("未盘点"));
        break;
    }
    case ZCHX::Data::PHOTO_FINISH:
    {
        drawRegion(painter, Qt::yellow, QColor(255,255,0,255*0.6));
        drawText(painter, false, QObject::tr("拍照完成"), QObject::tr("照片数量：%1").arg(m_data.photoNumbers));
        break;
    }
    case ZCHX::Data::CHECK_FINISH:
    {
        drawRegion(painter, Qt::green, QColor(0,255,0,255*0.6));
        drawText(painter, false, QObject::tr("盘点完成"), QObject::tr("船舶数量：%1").arg(m_data.shipNumbers));
        break;
    }
    case ZCHX::Data::CHECKING:
    {
        drawRegion(painter, Qt::red, QColor(255,0,255,255*0.6));
        drawText(painter, false, QObject::tr("盘点中"));
        break;
    }
    default:
        break;
    }

    //泊位
    drawText(painter, true, m_data.Berth);
}

void CameraRegionElement::updateGeometry(QPointF pos, qreal size)
{
    m_Region.clear();
    foreach (ZCHX::Data::LatLon  point, m_data.CameraPointList)
    {
        m_Region.append(framework()->LatLon2Pixel(point).toPointF());
    }
    m_boundingRect = m_Region.boundingRect().adjusted(-1,-1,-1,-1);
    Element::updateGeometry(pos, size);
}

void DramCircularElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return ;
    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(Qt::red);
    painter->drawPolygon(m_polygon);
}

void DramCircularElement::updateGeometry(QPointF pos, qreal size)
{
    m_polygon.clear();
    qreal half = size / 2.0;
    m_polygon << QPointF(pos.x() - half , pos.y() + half )
              << QPointF(pos.x() + half , pos.y() + half )
              << QPointF(pos.x() + half , pos.y() - half )
              << QPointF(pos.x() - half , pos.y() - half );

    m_boundingRect =  m_polygon.boundingRect().normalized();
    Element::updateGeometry(pos, size);

}

void VesselTargetElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());
    PainterPair chk(painter);

    switch (m_data.Status) {
    case ZCHX::Data::NO_INDENTIFY:
        painter->setBrush(Qt::yellow);
        break;
    case ZCHX::Data::INDENTIFY:
        painter->setBrush(Qt::blue);
        break;
    case ZCHX::Data::UNABLE_INDENTIFY:
        painter->setBrush(Qt::red);
        break;
    default:
        painter->setBrush(Qt::red);
        break;
    }
    Element::drawElement(painter);
    painter->drawPolygon(m_polygon);
}

void VesselTargetElement::updateGeometry(QPointF pos, qreal size)
{
    m_polygon.clear();
    qreal half = size / 2.0;
    m_polygon << QPointF(pos.x() - half ,pos.y() + half)
              << QPointF(pos.x() + half, pos.y() + half )
              << QPointF(pos.x(), pos.y() - (half + 10));

    m_boundingRect =  m_polygon.boundingRect().normalized();
    Element::updateGeometry(pos, size);
}


void VesselTrackLineElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter) || m_data.m_isHideen) return;

    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::blue,2));
    painter->drawPolyline(m_Line);
}

void VesselTrackLineElement::updateGeometry(QPointF pos, qreal size)
{
    m_Line.clear();
    foreach (ZCHX::Data::ITF_TrackPointInfo info, m_data.m_trackPointList) {
        m_Line.append(framework()->LatLon2Pixel(info.m_lat,info.m_lon).toPointF());
    }
    m_boundingRect =  m_Line.boundingRect().normalized();
    Element::updateGeometry(pos, size);
}


void VesselTrackElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter) || m_data.m_isHidden) return;
    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::red,1));
    painter->setBrush(QBrush(QColor(240,255,255),Qt::SolidPattern));

    painter->drawEllipse(m_trackRect);
}

void VesselTrackElement::updateGeometry(QPointF pos, qreal size)
{
    m_trackRect = QRectF(0, 0, 8, 8);
    m_trackRect.moveCenter(pos);
    m_boundingRect =  m_trackRect.adjusted(-1,-1,1,1);
    Element::updateGeometry(pos, size);
}


void HistoryTraceElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    PainterPair chk(painter);
    Element::drawElement(painter);

    painter->setBrush(Qt::NoBrush);
    QPen  pen = QPen(QColor(Qt::GlobalColor::blue));
    pen.setWidth(0.5);
    painter->setPen(pen);
    painter->drawEllipse(getViewPos(), 3, 3);
    drawActive(painter);
    drawFocus(painter);
}


void WeatherWindWavesElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    PainterPair chk(painter);
    Element::drawElement(painter);

    QPointF pos = getViewPos();
    updateGeometry(pos, getDrawScaleSize());
    painter->setBrush(Qt::NoBrush);
    painter->drawPixmap(m_boundingRect.toRect(),weatherImg);
    drawActive(painter);
    drawFocus(painter);
}


void WeatherWindWavesElement::updateGeometry(QPointF pos, qreal size)
{
    m_boundingRect = QRectF(0,0,weatherImg.width(),weatherImg.height());
    m_boundingRect.moveCenter(pos);
    Element::updateGeometry(pos, size);
}







