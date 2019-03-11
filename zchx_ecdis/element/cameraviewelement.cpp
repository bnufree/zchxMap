#include "cameraviewelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "../zchxmapframe.h"

namespace qt
{
CameraViewElement::CameraViewElement(const ZCHX::Data::ITF_CameraView &data, zchxMapWidget* f)
    :Element(data.lat,data.lon, f, ZCHX::Data::ELE_CAMERA_VIEW)
{
    m_data = data;
}

ZCHX::Data::ITF_CameraView CameraViewElement::getData() const
{
    return m_data;
}

void CameraViewElement::setData(const ZCHX::Data::ITF_CameraView &data)
{
    m_data = data;
}

void CameraViewElement::drawElement(QPainter *painter)
{
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CAMERA_VIEW)) return;
}
}

