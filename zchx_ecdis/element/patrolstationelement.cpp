#include "patrolstationelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
PastrolStationElement::PastrolStationElement(const ZCHX::Data::ITF_PastrolStation &data, zchxMapFrameWork* f)
    :Element(data.ll.lat,data.ll.lon, f, ZCHX::Data::ELEMENT_PATROL_STATION)
{
    m_data = data;
    uuid = data.uuid;
    setIsUpdate(true);
}

ZCHX::Data::ITF_PastrolStation PastrolStationElement::data() const
{
    return m_data;
}

void PastrolStationElement::setData(const ZCHX::Data::ITF_PastrolStation &data)
{
    m_data = data;
    setIsUpdate(true);
}

//bool PastrolStationElement::contains(int range, double x, double y) const
//{
//    return false;
//}

void PastrolStationElement::drawElement(QPainter *painter)
{
    QString layer_name, img_name;
    if(data().type == ZCHX::Data::ECDIS_STATION_RADAR)
    {
        layer_name = ZCHX::LAYER_RADAR_SITE;
        img_name = ":/element/XlzVo.png";
    } else {
        layer_name = ZCHX::LAYER_PATROL_SITE;
        img_name = ":/element/LdVo.png";
    }
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(layer_name)) return;

    int curScale = m_framework->GetDrawScale();
    QPixmap img = ZCHX::Utils::getImage(img_name, Qt::green, curScale);
    ZCHX::Data::Point2D pos = m_framework->LatLon2Pixel(data().ll.lat,data().ll.lon);
    QRect rect(pos.x - img.width() / 2, pos.y - img.height() / 2, img.width(), img.height());
    if(getIsActive())
    {
        PainterPair chk(painter);
        painter->setPen(QPen(Qt::red,2));
        painter->drawRect(rect.x() -5, rect.y() -5, rect.width()+10, rect.height()+10);
    }
    painter->drawPixmap(rect, img);
}
}

