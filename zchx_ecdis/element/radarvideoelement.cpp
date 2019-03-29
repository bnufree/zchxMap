#include "radarvideoelement.h"
#include "zchxmapframe.h"
#include "map_layer/zchxmaplayermgr.h"
#include <QPainter>

using namespace ZCHX::Data;

namespace qt {

void RadarVideoGlowElement::drawOutline(QPainter *painter, const QPointF& center, double in, double out)
{
    if(!painter) return;
    PainterPair chk(painter);
    painter->setPen(QColor(200,200,100,200));
    painter->drawEllipse(center, out, in);
}

void RadarVideoGlowElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter) || !m_data.showvideo) return;

    LatLon ll0 = ZCHX::Utils::distbear_to_latlon(data().lat, m_data.lon, m_data.distance, 0);
    LatLon ll90 = ZCHX::Utils::distbear_to_latlon(data().lat, m_data.lon, m_data.distance, 90);
    LatLon ll180 = ZCHX::Utils::distbear_to_latlon(data().lat, m_data.lon, m_data.distance, 180);
    LatLon ll270 = ZCHX::Utils::distbear_to_latlon(data().lat, m_data.lon, m_data.distance, 270);
    Point2D pos0 = mView->framework()->LatLon2Pixel(ll0);
    Point2D pos90 = mView->framework()->LatLon2Pixel(ll90);
    Point2D pos180 = mView->framework()->LatLon2Pixel(ll180);
    Point2D pos270 = mView->framework()->LatLon2Pixel(ll270);

    double dMinDrawX1 = fmin(pos0.x, pos90.x);
    double dMinDrawY1 = fmin(pos0.y, pos90.y);
    double dMaxDrawX1 = fmax(pos0.x, pos90.x);
    double dMaxDrawY1 = fmax(pos0.y, pos90.y);
    double dMinDrawX2 = fmin(pos180.x, pos270.x);
    double dMinDrawY2 = fmin(pos180.y, pos270.y);
    double dMaxDrawX2 = fmax(pos180.x, pos270.x);
    double dMaxDrawY2 = fmax(pos180.y, pos270.y);

    double dMinDrawX = fmin(dMinDrawX1, dMinDrawX2);
    double dMinDrawY = fmin(dMinDrawY1, dMinDrawY2);
    double dMaxDrawX = fmax(dMaxDrawX1, dMaxDrawX2);
    double dMaxDrawY = fmax(dMaxDrawY1, dMaxDrawY2);
    double dWidth = qAbs(dMaxDrawX-dMinDrawX);
    double dHeight = qAbs(dMaxDrawY-dMinDrawY);

    QPointF centerPos = mView->framework()->LatLon2Pixel(data().lat, data().lon).toPointF();
    double dRadiusIn = dWidth/2.0;
    double dRadiusOut = qSqrt(dRadiusIn*dRadiusIn + dRadiusIn*dRadiusIn);
    //绘制外圈
    drawOutline(painter, centerPos, dRadiusOut, dRadiusOut);
    if(data().type == ZCHX::Data::ITF_RadarVideoGLow::RadarVideo)
    {
        QRectF objRect(0, 0, dWidth, dHeight);
        objRect.moveCenter(centerPos);
        //绘制回波图片
        double angleFromNorth = mView->framework()->GetRotateAngle(); //计算当前正北方向的方向角
        PainterPair chk(painter);
        double translateX = objRect.x() + objRect.width()/2.0;
        double translateY = objRect.y() + objRect.height()/2.0;
        painter->translate(translateX, translateY);
        painter->rotate((int)(angleFromNorth) % 360);
        painter->translate(-translateX,-translateY);
        painter->drawPixmap(centerPos.x() - dWidth / 2, centerPos.y() - dHeight / 2, dWidth, dHeight, data().videoPixmap);
    } else if(data().type == ZCHX::Data::ITF_RadarVideoGLow::RadarGlow)
    {
        QRectF objRect(dMinDrawX,dMinDrawY,dWidth,dHeight);
        QRectF objSourceRect(0, 0, ZCHX::Data::RadarVideoPixmapWidth, ZCHX::Data::RadarVideoPixmapHeight);
        //绘制余辉图片
        for(int i = 0;i<m_data.afterglowType;i++)
        {
            if(m_data.afterglowPixmap[i].isNull() || i == m_data.afterglowIndex)
            {
                continue;
            }
            painter->drawPixmap(objRect,m_data.afterglowPixmap[i],objSourceRect);
        }
        //绘制当前的
        if(!m_data.afterglowPixmap[m_data.afterglowIndex].isNull())
        {
            painter->drawPixmap(objRect,m_data.afterglowPixmap[m_data.afterglowIndex],objSourceRect);
        }
    }
}
}

