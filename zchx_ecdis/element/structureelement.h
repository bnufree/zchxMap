#ifndef STRUCTUREELE_H
#define STRUCTUREELE_H

#include "fixelement.h"

namespace qt {
//结构物
class ZCHX_ECDIS_EXPORT StructureElement : public FixElement<ZCHX::Data::ITF_Structure>
{
public:
    explicit StructureElement(const ZCHX::Data::ITF_Structure &ele, zchxMapWidget* f)
        :FixElement<ZCHX::Data::ITF_Structure>(ele, ZCHX::Data::ELE_STRUCTURE, ZCHX::LAYER_STRUCTURE, f) {}
    void drawElement(QPainter *painter);

private:
};
}


#endif // STRUCTUREELE_H
