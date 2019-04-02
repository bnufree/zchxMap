#ifndef IPCELE_H
#define IPCELE_H

#include "fixelement.h"

namespace qt
{

class ZCHX_ECDIS_EXPORT IPCElement : public FixElement<ZCHX::Data::IPCastDevice>
{
public:
    explicit IPCElement(const ZCHX::Data::IPCastDevice &data, zchxMapWidget* f);
    void setStatus(int sts);
    int status() const;
    virtual void drawElement(QPainter *painter) {}
    void updateGeometry(QPointF pos, qreal size) {}
    void copyDataFromOther(std::shared_ptr<Element> other);
private:
};
}

#endif // IPCELE_H
