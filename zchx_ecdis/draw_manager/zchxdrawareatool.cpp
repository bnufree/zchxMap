#include "zchxdrawareatool.h"
#include "zchxmapframe.h"

using namespace qt;
zchxDrawAreaTool::zchxDrawAreaTool(zchxMapWidget* w, QObject *parent) : zchxDrawTool(w, qt::eTool::DRAWMEASUREAREA, parent)
{

}

void zchxDrawAreaTool::show(QPainter *painter)
{
    if(!isReady()) return;

    QFont font;
    font.setPixelSize(14);
    painter->setFont(font);
    painter->setPen(QPen(Qt::red,2));
    painter->setBrush(QBrush(Qt::green, Qt::CrossPattern));

    double total_area = 0;
    //通过三点计算三角形的面积和(0,1,2)+(0,2,3)+(0,3,4)
    QPolygonF polyf;
    ZCHX::Data::LatLon p0 = mPoints[0];
    for(int i=0; i<mPoints.size(); ++i)
    {
        //画点
        PainterPair chk(painter);
        painter->setPen(QPen(Qt::yellow,2));
        QPointF pi = mWidget->framework()->LatLon2Pixel(mPoints[i]).toPointF();
        painter->drawEllipse(pi, 5, 5);
        polyf.append(pi);
        if(i == 0) continue;
        ZCHX::Data::LatLon p1 = mPoints[i];
        if((i+1) >= mPoints.size()) continue;
        ZCHX::Data::LatLon p2 = mPoints[i+1];
        //areaNum+= MercatorBounds::AreaOnEarth(area0,area1,area2) / 1000000.000;//单位平千米
        total_area += zchxMapDataUtils::AreaOnEarth(p0, p1, p2) / 342990.400;//单位平方海里
    }
    //首先画线
    if(polyf.size() < 3)
    {
        painter->drawPolyline(polyf);
    } else {
        polyf.append(polyf.first());
        painter->drawPolygon(polyf);
        //标记面积数
        QPointF last_pnt = mWidget->framework()->LatLon2Pixel(mPoints.last()).toPointF();
        painter->drawText(last_pnt, QString("Area:%1 nmi²").arg(total_area));
    }

}

