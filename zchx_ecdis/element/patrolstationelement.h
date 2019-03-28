#ifndef PATROLSTATIONELE_H
#define PATROLSTATIONELE_H

#include "fixelement.h"

namespace qt {
//结构物
class  PastrolStationElement : public FixElement<ZCHX::Data::ITF_PastrolRadarStation>
{
public:
    explicit PastrolStationElement(const ZCHX::Data::ITF_PastrolRadarStation&data, zchxMapWidget* f);
    void drawElement(QPainter *painter);

private:
};
}


#endif // STRUCTUREELE_H
