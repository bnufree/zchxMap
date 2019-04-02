#include "shipalarmascendelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {

void ShipAlarmAscendElement::copyDataFromOther(std::shared_ptr<Element> other)
{
    if(!other) return;
    ShipAlarmAscendElement *src = dynamic_cast<ShipAlarmAscendElement*>(other.get());
    if(src)
    {
        this->setData(src->data());
    }
}

void ShipAlarmAscendElement::drawElement(QPainter *painter)
{
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_ALARMASCEND)) return;
    QPointF point = mView->framework()->LatLon2Pixel(m_data.lat, m_data.lon).toPointF();

    if (m_data.alarmType > 0)
    {
        QPixmap pixmap(ZCHX::Utils::getAlarmImg(m_data.alarmType));
        updateBouningRect(point, pixmap.width(), pixmap.height());
        updateGeometry(point, getDrawScaleSize());
        painter->drawPixmap(point.x() - pixmap.width() / 2, point.y() - pixmap.height() / 2,
                            pixmap.width(), pixmap.height(), pixmap);
        drawActive(painter);
    }
}
}

