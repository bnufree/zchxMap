#ifndef COASTELEMENT_H
#define COASTELEMENT_H

#include "moveelement.h"

namespace qt {
//海岸数据
class CoastElement : public MoveElement
{
public:
    explicit CoastElement(const ZCHX::Data::ITF_CoastData &ele, zchxMapFrameWork* f);
    void setData(const ZCHX::Data::ITF_CoastData &ele);
    ZCHX::Data::ITF_CoastData data() const;

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);
    std::vector<std::pair<double, double> >& getPath();
    void updateOldPath();
    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);
    void updateGeometry(QPointF, int){}

    void drawElement(QPainter *painter);

private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_CoastData m_data;
};
}


#endif // COASTELEMENT_H
