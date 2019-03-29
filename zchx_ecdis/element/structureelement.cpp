#include "structureelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {

void StructureElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    ZCHX::Data::Point2D  curPos = mView->framework()->LatLon2Pixel(m_data.getLat(), m_data.getLon());

    QPixmap devicePix;
    if(!devicePix.load(":/navig64/device.png")) return;
    updateBouningRect(curPos.toPointF(), devicePix.width(), devicePix.height());
    updateGeometry(curPos.toPointF(), getDrawScaleSize());

    int posX = curPos.x - devicePix.width() / 2;
    int poxY = curPos.y - devicePix.height() / 2;
    painter->drawPixmap(posX, poxY, devicePix.width(), devicePix.height(), devicePix);
    QString name = QString::fromStdString(this->name());
    painter->drawText(posX + 30, poxY, name);
}
}

