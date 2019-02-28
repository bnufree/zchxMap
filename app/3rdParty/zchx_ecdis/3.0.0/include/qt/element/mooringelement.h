#ifndef MOORINGELEMENT_H
#define MOORINGELEMENT_H

#include "moveelement.h"

namespace qt {
//锚泊
class MooringElement : public MoveElement
{
public:
    explicit MooringElement(const ZCHX::Data::ITF_Mooring &ele, zchxMapFrameWork* f);

    void setData(const ZCHX::Data::ITF_Mooring &ele);

    virtual std::vector<std::pair<double, double> > path() const;
    virtual void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_Mooring data() const;

    virtual std::vector<std::pair<double, double> > & getPath();

    void updateOldPath();

    void updateGeometry(QPointF, int){}
    void drawElement(QPainter *painter);
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_Mooring m_data;
};
}


#endif // MOORINGELEMENT_H
