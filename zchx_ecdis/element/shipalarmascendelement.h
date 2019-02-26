#ifndef SHIPALARMASCENDELEMENT_H
#define SHIPALARMASCENDELEMENT_H

#include "moveelement.h"
namespace qt {
// 预警追溯
class ShipAlarmAscendElement : public MoveElement
{
public:
    explicit ShipAlarmAscendElement(const ZCHX::Data::ITF_ShipAlarmAscend &ele, zchxMapFrameWork* f);

    void setData(const ZCHX::Data::ITF_ShipAlarmAscend &ele);

    ZCHX::Data::ITF_ShipAlarmAscend data() const;

    void updateGeometry(QPointF, int){}
    void drawElement(QPainter *painter);

private:
    ZCHX::Data::ITF_ShipAlarmAscend m_data;
};
}


#endif // SHIPALARMASCENDELEMENT_H
