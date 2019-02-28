#ifndef SHIPALARMASCENDELEMENT_H
#define SHIPALARMASCENDELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
// 预警追溯
class ShipAlarmAscendElement : public Element
{
public:
    explicit ShipAlarmAscendElement(const ZCHX::Data::ITF_ShipAlarmAscend &ele, zchxMapFrameWork* f);

    void setData(const ZCHX::Data::ITF_ShipAlarmAscend &ele);

    ZCHX::Data::ITF_ShipAlarmAscend data() const;

    void updateGeometry(QPointF, int){}
    void drawElement(QPainter *painter);
    std::string name() const;

private:
    ZCHX::Data::ITF_ShipAlarmAscend m_data;
};
}


#endif // SHIPALARMASCENDELEMENT_H
