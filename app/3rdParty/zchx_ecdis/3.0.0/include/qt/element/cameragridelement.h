#ifndef CAMERAGRIDELE_H
#define CAMERAGRIDELE_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
//相机网格
class CameraGridElement : public Element
{
public:
    explicit CameraGridElement(const ZCHX::Data::ITF_NetGrid&ele, zchxMapWidget* f);
    void setData(const ZCHX::Data::ITF_NetGrid &ele);
    ZCHX::Data::ITF_NetGrid data() const;

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);
    void updateGeometry(QPointF, int){}

    bool contains(const QPoint& pos) const;
    void drawElement(QPainter *painter);

private:
    ZCHX::Data::ITF_NetGrid m_data;
};
}


#endif // COASTELE_H
