#ifndef STRUCTUREELE_H
#define STRUCTUREELE_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
//结构物
class StructureElement : public Element
{
public:
    explicit StructureElement(const ZCHX::Data::ITF_Structure &ele, zchxMapWidget* f);

    void setData(const ZCHX::Data::ITF_Structure &ele);

    std::pair<double, double> point() const;
    void setPoint(const std::pair<double, double> &point);

//    int id() const;
//    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_Structure data() const;

    void updateGeometry(QPointF, int){}
//    bool contains(int range, double x, double y) const;
    void drawElement(QPainter *painter);

private:
    std::pair<double, double> m_point; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_Structure m_data;
};
}


#endif // STRUCTUREELE_H
