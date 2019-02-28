#ifndef COASTELEMENT_H
#define COASTELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
//海岸数据
class CoastElement : public Element
{
public:
    explicit CoastElement(const ZCHX::Data::ITF_CoastData &ele, zchxMapFrameWork* f);
    void setData(const ZCHX::Data::ITF_CoastData &ele);
    ZCHX::Data::ITF_CoastData data() const;

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);
    void updateGeometry(QPointF, int){}

    bool contains(int range, double x, double y) const;
    void drawElement(QPainter *painter);

private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_CoastData m_data;
};
}


#endif // COASTELEMENT_H
