#include "zchxdrawangletool.h"
#include "zchxmapframe.h"

using namespace qt;
using namespace ZCHX::Data;
zchxDrawAngleTool::zchxDrawAngleTool(zchxMapWidget* w, QObject *parent) : zchxDrawTool(w, qt::eTool::DRAWDIRANGLE, parent)
{

}

void zchxDrawAngleTool::appendPoint(const QPointF &pnt)
{
    if(!mWidget) return;
    ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(pnt);
    if(mPoints.size() < 2){
        mPoints.append(ll);
    } else {
        mPoints[1] = ll;
    }
}

void zchxDrawAngleTool::show(QPainter *painter)
{
    if(!isReady() || mPoints.size() < 2) return;
    PainterPair chk(painter);
    painter->setRenderHint(QPainter::Antialiasing);
    QFont font;
    font.setPixelSize(14);
    painter->setFont(font);

    //计算角度
    QPointF p0 = mWidget->framework()->LatLon2Pixel(mPoints[0]).toPointF();
    QPointF p1 = mWidget->framework()->LatLon2Pixel(mPoints[1]).toPointF();
    double dr = zchxMapDataUtils::getDistancePixel(p0, p1);
    double angle4TrueNorth = zchxMapDataUtils::DegToRad((mWidget->framework()->GetRotateAngle()));
    double antArc = zchxMapDataUtils::TwoVectorsAngle(p0, Point2D(p0.x(),p0.y() - dr), p1);
    antArc = zchxMapDataUtils::AngleIn2PI(antArc - angle4TrueNorth);

    //计算两点之间的地球距离
    LatLon start_latlon = mPoints[0];
    LatLon end_latlon = mPoints[1];
    double distance = zchxMapDataUtils::DistanceOnEarth(start_latlon, end_latlon) / 1852.000;
    double startAng = angle4TrueNorth / GLOB_PI * 180;
    double lenAng = antArc / GLOB_PI * 180;
    QRectF rectangle(p0.x() - dr, p0.y() - dr, dr * 2, dr * 2);
    int startAngle = (90 - startAng)* 16;
    int spanAngle = (-lenAng) * 16;
    painter->setPen(QPen(Qt::red,2));
    painter->drawPie(rectangle, startAngle, spanAngle);
    if (lenAng < 180)
    {
        painter->drawText(p1 + QPointF(10,10), QString("∠%1°").arg(lenAng));
        painter->drawText(p1 + QPointF(10,25), QString("distance:%1 nmi").arg(distance));
    }
    else
    {
        painter->drawText(p1 - QPointF(120,25), QString("∠%1°").arg(lenAng));
        painter->drawText(p1 - QPointF(120,10), QString("distance:%1 nmi").arg(distance));
    }

}
