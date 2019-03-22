#ifndef FIXELEMENT_H
#define FIXELEMENT_H

#include "IDrawElement.hpp"

namespace qt {
template <typename T>
class ZCHX_ECDIS_EXPORT FixElement : public Element
{
public:
    explicit  FixElement(const T& data, ZCHX::Data::ELETYPE type, const QString& layer, zchxMapWidget* w)
        :Element(data.getLat(), data.getLon(), w, type)
    {
        setData(data);
        setLayer(layer);
    }

    T data() const {return m_data;}
    virtual void setData(const T& data) {
        m_data = data;
        setIsUpdate(true);
        setID(m_data.getName());
    }
    std::string name() const {return m_data.getName().toStdString();}
    void updateGeometry(QPointF, qreal){}

protected:
    T       m_data;
};

}

#endif // FIXELEMENT_H
