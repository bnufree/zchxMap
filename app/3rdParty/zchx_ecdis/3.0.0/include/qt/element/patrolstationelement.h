#ifndef PATROLSTATIONELE_H
#define PATROLSTATIONELE_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
//结构物
class  PastrolStationElement : public Element
{
public:
    explicit PastrolStationElement(const ZCHX::Data::ITF_PastrolStation &data, zchxMapWidget* f);
    ZCHX::Data::ITF_PastrolStation data() const;
    void setData(const ZCHX::Data::ITF_PastrolStation &data);

    void updateGeometry(QPointF, qreal){}
//    bool contains(int range, double x, double y) const;
    void drawElement(QPainter *painter);
    std::string name() const {return m_data.name.toStdString();}

private:
    ZCHX::Data::ITF_PastrolStation m_data;
};
}


#endif // STRUCTUREELE_H
