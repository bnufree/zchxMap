#include "patrolstationelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
PastrolStationElement::PastrolStationElement(const ZCHX::Data::ITF_PastrolRadarStation &data, zchxMapWidget* f)
    :FixElement(data, ZCHX::Data::ELE_PATROL_STATION, ZCHX::LAYER_PATROL_SITE, f)
{
}

void PastrolStationElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return ;
    QString img_name;
    if(data().type == ZCHX::Data::ECDIS_STATION_RADAR)
    {
        img_name = ":/element/XlzVo.png";
    } else {
        img_name = ":/element/LdVo.png";
    }

    int curScale = getDrawScaleSize();
    QPixmap img = ZCHX::Utils::getImage(img_name, Qt::green, curScale);
    QPoint pos = getCurrentPos().toPoint();
    QRect rect(0, 0, img.width(), img.height());
    rect.moveCenter(pos);
    m_boundingRect = rect;
    updateGeometry(pos, curScale);
    painter->drawPixmap(rect, img);
    drawActive(painter);
}
}

