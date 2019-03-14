#include "ipcelement.h"
#include "zchxmapframe.h"

namespace qt
{
IPCElement::IPCElement(const ZCHX::Data::IPCastDevice &data, zchxMapWidget* f)
    :Element(data.lat, data.lon, f, ZCHX::Data::ELE_IPC)
{
    m_data = data;
}

ZCHX::Data::IPCastDevice IPCElement::getData() const
{
    return m_data;
}

void IPCElement::setData(const ZCHX::Data::IPCastDevice &data)
{
    m_data = data;
}

void IPCElement::setStatus(int sts)
{
    m_data.status = sts;
}

int IPCElement::status() const
{
    return m_data.status;
}

void IPCElement::drawElement(QPainter *painter)
{
    return;
}

}
