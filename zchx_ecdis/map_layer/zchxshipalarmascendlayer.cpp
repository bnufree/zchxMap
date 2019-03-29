#include "zchxshipalarmascendlayer.h"
#include "zchxmapframe.h"

using namespace qt;

zchxShipAlarmAscendLayer::zchxShipAlarmAscendLayer(zchxMapWidget *drawWidget, bool visible, QObject *parent)
    :MapLayer(drawWidget, ZCHX::LAYER_ALARMASCEND, ZCHX::TR_LAYER_ALARMASCEND, visible, parent)
{    
}

void zchxShipAlarmAscendLayer::drawLayer(QPainter *painter)
{
    if( !visible()) return;
    //追溯线
    QPolygonF points;
    for(std::shared_ptr<Element> ele : getElements())
    {
        if(!ele) continue;
        ShipAlarmAscendElement *item = dynamic_cast<ShipAlarmAscendElement*>(ele.get());
        if(item) {
            ZCHX::Data::ITF_ShipAlarmAscend data = item->data();
            points.append(getDrawWidget()->framework()->LatLon2Pixel(data.getLat(), data.getLon()).toPointF());
        }
    }
    if(points.size() >= 2)
    {
        PainterPair chk(painter);
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
        painter->drawPolyline(points);
    }
    MapLayer::drawLayer(painter);

}
