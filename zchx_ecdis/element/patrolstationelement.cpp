#include "patrolstationelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
PastrolStationElement::PastrolStationElement(const ZCHX::Data::ITF_PastrolRadarStation &data, zchxMapWidget* f)
    :FixElement(data, ZCHX::Data::ELE_PATROL_STATION, ZCHX::LAYER_PATROL_SITE, f)
{
}

void PastrolStationElement::copyDataFromOther(std::shared_ptr<Element> other)
{
    if(!other) return;
    PastrolStationElement *src = dynamic_cast<PastrolStationElement*>(other.get());
    if(src)
    {
        this->setData(src->data());
    }
}

void PastrolStationElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return ;
    QString img_name;
    if(data().type == ZCHX::Data::ECDIS_STATION_PATROL)
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

void PastrolStationElement::showToolTip(const QPoint &pos)
{
    ZCHX::Data::ITF_PastrolRadarStation data = this->data();
    QString infoList;
    infoList.append(QObject::tr("站点名称：%1").arg(data.getName()) + "\n");
    infoList.append(QObject::tr("站点ID：%1").arg(data.sbid) + "\n");
    infoList.append(QObject::tr("经度：%1").arg(FLOAT_STRING(data.getLon(), 6)) + "\n");
    infoList.append(QObject::tr("纬度：%1").arg(FLOAT_STRING(data.getLat(), 6)));
    QToolTip::showText(pos, infoList);
}
}

