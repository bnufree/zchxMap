#ifndef PATROLSTATIONELE_H
#define PATROLSTATIONELE_H

#include "fixelement.h"

namespace qt {
//结构物
class ZCHX_ECDIS_EXPORT PastrolStationElement : public FixElement<ZCHX::Data::ITF_PastrolRadarStation>
{
public:
    explicit PastrolStationElement(const ZCHX::Data::ITF_PastrolRadarStation &data, zchxMapWidget* f);
    void updateGeometry(QPointF, qreal){}
    void drawElement(QPainter *painter);
    void copyDataFromOther(std::shared_ptr<Element> other);

private:
};
}


#endif // STRUCTUREELE_H
