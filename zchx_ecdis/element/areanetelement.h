#ifndef AREANETELEMENT_H
#define AREANETELEMENT_H

#include "moveelement.h"

namespace qt {
//地理区域网络
class AreaNetElement : public MoveElement
{
public:
    explicit AreaNetElement(const ZCHX::Data::ITF_AreaNet &ele, zchxMapFrameWork* f);

    void setData(const ZCHX::Data::ITF_AreaNet &ele);
    ZCHX::Data::ITF_AreaNet data() const;

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);
    std::vector<std::pair<double, double> > & getPath();
    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);
    void updateGeometry(QPointF, int){}
//    bool contains(int range, double x, double y) const;
    void drawElement(QPainter *painter);

private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_AreaNet m_data;
};
}


#endif // AREANETELEMENT_H
