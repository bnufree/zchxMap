#ifndef CHANNELELEMENT_H
#define CHANNELELEMENT_H

#include "moveelement.h"

namespace qt {
//航道
class ChannelElement : public MoveElement
{
public:
    explicit ChannelElement(const ZCHX::Data::ITF_Channel &ele, zchxMapFrameWork* f);

    void setData(const ZCHX::Data::ITF_Channel &ele);
    ZCHX::Data::ITF_Channel data() const;

    virtual std::vector<std::pair<double, double> > path() const;
    virtual void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    void setLineSelected(int i, bool selectStatus);
    virtual std::vector<std::pair<double, double> > & getPath();
    void updateOldPath();
    void updateGeometry(QPointF, int){}
    void drawElement(QPainter *painter);
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_Channel m_data;
};
}


#endif // CHANNELELEMENT_H
