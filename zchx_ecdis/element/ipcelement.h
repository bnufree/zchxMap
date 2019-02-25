#ifndef IPCELEMENT_H
#define IPCELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt
{

class  IPCElement : public Element
{
public:
    explicit IPCElement(const ZCHX::Data::IPCastDevice &data, zchxMapFrameWork* f);
    ZCHX::Data::IPCastDevice getData() const;
    void setData(const ZCHX::Data::IPCastDevice &data);
    void setStatus(int sts);
    int status() const;
    virtual void drawElement(QPainter *painter);
    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::IPCastDevice m_data;
};
}

#endif // IPCELEMENT_H
