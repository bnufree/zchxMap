#ifndef SEABEDPIPLINEELEMENT_H
#define SEABEDPIPLINEELEMENT_H

#include "moveelement.h"

namespace qt {
//海岸数据
class SeabedPipeLineElement : public MoveElement
{
public:
    explicit SeabedPipeLineElement(const ZCHX::Data::ITF_SeabedPipeLine &ele, zchxMapFrameWork* f);
    void setData(const ZCHX::Data::ITF_SeabedPipeLine &ele);
    ZCHX::Data::ITF_SeabedPipeLine data() const;

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);
    std::vector<std::pair<double, double> >& getPath();

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
    ZCHX::Data::ITF_SeabedPipeLine m_data;
};
}


#endif // SEABEDPIPLINEELEMENT_H
