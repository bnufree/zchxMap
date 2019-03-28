#include "ipcelement.h"
#include "zchxmapframe.h"

using namespace qt;
IPCElement::IPCElement(const ZCHX::Data::IPCastDevice &data, zchxMapWidget* f)
    :FixElement<ZCHX::Data::IPCastDevice>(data, ZCHX::Data::ELE_IPC, ZCHX::LAYER_CAMERA, f)
{
}

void IPCElement::setStatus(int sts)
{
    m_data.status = sts;
}

int IPCElement::status() const
{
    return m_data.status;
}
