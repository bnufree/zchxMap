#ifndef GRIDELEMENT_H
#define GRIDELEMENT_H

#include "fixelement.h"

namespace qt {
//网格
class ZCHX_ECDIS_EXPORT GridElement : public FixElement<ZCHX::Data::ITF_NetGrid>
{
public:
    explicit GridElement(const ZCHX::Data::ITF_NetGrid& ele, zchxMapWidget* w)
        :FixElement<ZCHX::Data::ITF_NetGrid>(ele, ZCHX::Data::ELE_GRID, ele.mLayer, w) {}
    void drawElement(QPainter *painter);

private:
    void drawCameraGrid(QPainter* painter);
    void drawTenGrid(QPainter* painter);
    void drawThirtyGrid(QPainter* painter);
};
}


#endif // GRIDELEMENT_H
