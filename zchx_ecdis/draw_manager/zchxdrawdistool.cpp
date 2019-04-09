#include "zchxdrawdistool.h"
#include "zchxmapframe.h"

using namespace qt;
zchxDrawDistanceTool::zchxDrawDistanceTool(zchxMapWidget* w, QObject *parent) : zchxDrawTool(w, qt::eTool::DRAWDISTANCE, parent)
{

}

void zchxDrawDistanceTool::show(QPainter *painter)
{
    if(!isReady()) return;

    PainterPair chk(painter);
    painter->setPen(QPen(Qt::red,2));
    QPolygonF poly;
    foreach (ZCHX::Data::LatLon ll, mPoints) {
        poly.append(mWidget->framework()->LatLon2Pixel(ll).toPointF());
    }
    painter->drawPolyline(poly);

    double total_distance = 0;
    for(int i=0; i<mPoints.size(); ++i)
    {
        //画点
        PainterPair chk(painter);
        QFont font = painter->font();
        font.setPixelSize(14);
        painter->setFont(font);
        painter->setPen(QPen(Qt::red,2));
        double distance = 0;
        QPointF pi = mWidget->framework()->LatLon2Pixel(mPoints[i]).toPointF();
        if(i>= 1){
            distance = zchxMapDataUtils::DistanceOnEarth(mPoints[i-1], mPoints[i]) / 1852.000;
        }
        total_distance += distance;
        QString total_dis_text = "", line_dis_text = "";
        if(0 == i) {
            total_dis_text = "0";
        } else {
            total_dis_text = tr("total:%1 nmi").arg(total_distance);
            line_dis_text = tr("line:%1 nmi").arg(distance);
        }
        //标记距离值
        painter->drawText(pi, total_dis_text);
        painter->drawText(QPointF(pi.x(), pi.y() + 15), line_dis_text);

        //画点
        painter->setPen(QPen(Qt::yellow,1));
        painter->drawEllipse(pi, 3, 3);

    }
}
