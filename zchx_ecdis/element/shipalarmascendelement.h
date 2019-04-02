#ifndef SHIPALARMASCENDELE_H
#define SHIPALARMASCENDELE_H

#include "fixelement.h"

namespace qt {
// 预警追溯
class ZCHX_ECDIS_EXPORT ShipAlarmAscendElement : public FixElement<ZCHX::Data::ITF_ShipAlarmAscend>
{
public:
    explicit ShipAlarmAscendElement(const ZCHX::Data::ITF_ShipAlarmAscend &ele, zchxMapWidget* f)
        :FixElement<ZCHX::Data::ITF_ShipAlarmAscend>(ele, ZCHX::Data::ELE_SHIP_ALARM_ASCEND, ZCHX::LAYER_ALARMASCEND, f) {}
    void copyDataFromOther(std::shared_ptr<Element> other);
    void drawElement(QPainter *painter);

private:
};
}


#endif // SHIPALARMASCENDELE_H
