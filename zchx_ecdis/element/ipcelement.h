#ifndef IPCELE_H
#define IPCELE_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt
{

class  IPCElement : public Element
{
public:
    explicit IPCElement(const ZCHX::Data::IPCastDevice &data, zchxMapWidget* f);
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

#endif // IPCELE_H
