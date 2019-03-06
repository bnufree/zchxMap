#include "zchxdrawangletool.h"
#include "zchxmapframe.h"

using namespace qt;
using namespace ZCHX::Data;
zchxDrawAngleTool::zchxDrawAngleTool(zchxMapWidget* w, QObject *parent) : zchxDrawTool(w, qt::eTool::DRAWDIRANGLE, parent)
{

}

void zchxDrawAngleTool::appendPoint(const QPointF &pnt)
{
    if(mPoints.size() < 2){
        mPoints.append(pnt);
    } else {
        mPoints[1] = pnt;
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
    double dr = zchxMapDataUtils::getDistancePixel(mPoints[0], mPoints[1]);
    double angle4TrueNorth = zchxMapDataUtils::DegToRad((mWidget->framework()->GetRotateAngle()));
    double antArc = zchxMapDataUtils::TwoVectorsAngle(mPoints[0], Point2D(mPoints[0].x(),mPoints[0].y() - dr), mPoints[1]);
    antArc = zchxMapDataUtils::AngleIn2PI(antArc - angle4TrueNorth);

    //计算两点之间的地球距离
    LatLon start_latlon = mWidget->framework()->Pixel2LatLon(mPoints[0]);
    LatLon end_latlon = mWidget->framework()->Pixel2LatLon(mPoints[1]);
    double distance = zchxMapDataUtils::DistanceOnEarth(start_latlon, end_latlon) / 1852.000;
    double startAng = angle4TrueNorth / GLOB_PI * 180;
    double lenAng = antArc / GLOB_PI * 180;
    QRectF rectangle(mPoints[0].x() - dr, mPoints[0].y() - dr, dr * 2, dr * 2);
    int startAngle = (90 - startAng)* 16;
    int spanAngle = (-lenAng) * 16;
    painter->setPen(QPen(Qt::red,2));
    painter->drawPie(rectangle, startAngle, spanAngle);
    if (lenAng < 180)
    {
        painter->drawText(mPoints[1] + QPointF(10,10), QString("∠%1°").arg(lenAng));
        painter->drawText(mPoints[1] + QPointF(10,25), QString("distance:%1 nmi").arg(distance));
    }
    else
    {
        painter->drawText(mPoints[1] - QPointF(120,25), QString("∠%1°").arg(lenAng));
        painter->drawText(mPoints[1] - QPointF(120,10), QString("distance:%1 nmi").arg(distance));
    }

}
