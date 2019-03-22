#include "gridelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

using namespace qt;

void GridElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    switch (data().mType) {
    case ZCHX::Data::NetGrid_Camera:
        drawCameraGrid(painter);
        break;
    case ZCHX::Data::NetGrid_Ten:
        drawTenGrid(painter);
        break;
    case ZCHX::Data::NetGrid_Thirty:
        drawThirtyGrid(painter);
        break;
    default:
        break;
    }
}

void GridElement::drawCameraGrid(QPainter *painter)
{
    if(!painter) return;
    foreach (ZCHX::Data::ITF_NetGridPolygon poly, data().mNetGridList) {
        PainterPair chk(painter);
        painter->setPen(QPen(data().mColor,1,Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        //画矩形
        QPointF top_left = mView->framework()->LatLon2Pixel(poly.mLatLonList.first().lat, poly.mLatLonList.first().lon).toPointF();
        QPointF bottom_right = mView->framework()->LatLon2Pixel(poly.mLatLonList.last().lat, poly.mLatLonList.last().lon).toPointF();
        QRectF rectf(top_left, bottom_right);
        painter->drawRect(rectf);


        //画文字
        QPointF center = rectf.center();
        QString text = QString::number(poly.id);
        QPen pen = painter->pen();
        pen.setColor(Qt::red);
        painter->setPen(pen);
        int txt_width = painter->fontMetrics().width(text);
        int txt_height = painter->fontMetrics().height();
        QPointF txt_pos = center + QPointF(-0.5 * txt_width, 0.5 * txt_height);

        painter->drawText(txt_pos, text );
    }
//    if(m_data.mNetGridList.size() > 0)
//    {
//        ZCHX::Data::ITF_NetGridPolygon poly = m_data.mNetGridList.last();
//        qDebug()<<poly.id<<poly.name<<FLOAT_STRING(poly.mLatLonList.first().lat, 10)<<FLOAT_STRING(poly.mLatLonList.first().lon, 10)
//                <<FLOAT_STRING(poly.mLatLonList.last().lat, 10) <<FLOAT_STRING(poly.mLatLonList.last().lon, 10);
//    }
}

void GridElement::drawTenGrid(QPainter *painter)
{
    if(!painter) return;
    PainterPair chk(painter);
    painter->setPen(QPen(data().mColor,1,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    bool text_pos_found = false;
    QPoint text_pos;
    foreach (ZCHX::Data::ITF_NetGridPolygon poly, data().mNetGridList) {
        if(poly.mLatLonList.size() == 0) continue;
        QPolygon polygon;
        for(ZCHX::Data::LatLon ll : poly.mLatLonList)
        {
            polygon.append(framework()->LatLon2Pixel(ll.lat, ll.lon).toPoint());
        }
        painter->drawPolygon(polygon);
        if(!text_pos_found) {
            text_pos = polygon.first();
            text_pos_found = true;
        }
    }
    if(framework()->Zoom() >= 10 && text_pos_found)
    {
        QString name = m_data.getName();
        if(name.contains("N"))
        {
            painter->drawText(text_pos, name);
        }
        else
        {
            painter->translate(text_pos);
            painter->rotate(90);
            painter->drawText(QPoint(0, 0), name);
        }
    }
}

void GridElement::drawThirtyGrid(QPainter *painter)
{
    if(!painter) return;
    PainterPair chk(painter);
    painter->setPen(QPen(data().mColor,1,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    bool text_pos_found = false;
    QPoint text_pos;
    foreach (ZCHX::Data::ITF_NetGridPolygon poly, data().mNetGridList) {
        if(poly.mLatLonList.size() == 0) continue;
        QPolygon polygon;
        for(ZCHX::Data::LatLon ll : poly.mLatLonList)
        {
            polygon.append(framework()->LatLon2Pixel(ll.lat, ll.lon).toPoint());
        }
        painter->drawPolygon(polygon);
        if(!text_pos_found) {
            text_pos = polygon.boundingRect().center();
            text_pos_found = true;
        }
    }

    if(framework()->Zoom() >= 6 && text_pos_found)
    {
        QString id = m_data.getName();
        QFont font("times", 24);
        QFontMetricsF fm(font);
        QRectF textRectF =  fm.boundingRect(id);
        textRectF.moveCenter(text_pos);
        painter->drawText(textRectF.center(), id);
    }
}

