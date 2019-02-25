#include "cameraelement.h"
#include "zchxmapframe.h"
#include "map_layer/zchxmaplayermgr.h"
#include <QPainter>

namespace qt {
CameraElement::CameraElement(const ZCHX::Data::ITF_CameraDev &data, zchxMapFrameWork* frame)
    :Element(data.nLatLon.lat, data.nLatLon.lon, frame, ZCHX::Data::ELEMENT_CAMERA)
    ,mParent(0)
{
    m_data = data;
    uuid = data.nUUID;
}

const ZCHX::Data::ITF_CameraDev &CameraElement::getData() const
{
    return m_data;
}

void CameraElement::setData(const ZCHX::Data::ITF_CameraDev &dev)
{
    m_data = dev;
}

int CameraElement::getParentID() const
{
    if(mParent) return mParent->getUuid();
    return -1;
}

void CameraElement::setParent(Element *ele)
{
    mParent = ele;
}

Element* CameraElement::getParent()
{
    return mParent;
}

uint CameraElement::getStatus() const
{
    return m_data.nStatus;
}

ZCHX::Data::CAMERATYPE CameraElement::getType() const
{
    return m_data.nType;
}

void CameraElement::setStatus(uint status)
{
    m_data.nStatus = status;
}

bool CameraElement::isNormal() const
{
    return m_data.nStatus == 1;
}

bool CameraElement::isBug() const
{
    return m_data.nStatus == 2;
}

bool CameraElement::isError() const
{
    return m_data.nStatus == 512;
}

QPixmap CameraElement::getImage(zchxMapFrameWork *f) const
{
    if(!f)
        f = m_framework;
    if(f)
    {
        int curScale = f->Zoom();
        return ZCHX::Utils::getImage(m_data.nType, m_data.nStatus, curScale);
    }
    return QPixmap();
}

double CameraElement::getMaxTrackRange() const
{
    return m_data.nMaxRange;
}

void CameraElement::drawElement(QPainter *painter)
{
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CAMERA)) return;
    if(mParent) return; //当前目标悬挂在其他图元上不显示
    //取得当前的地图层级
    if(!m_framework) return;
    //开始显示
    int curScale = m_framework->GetDrawScale();
    QPointF pos = m_framework->LatLon2Pixel(lat(),lon()).toPointF();
    QPixmap image = ZCHX::Utils::getImage(getData().nType, getData().nStatus, curScale);

    QRectF rect(0,0, image.width(),image.height());
    rect.moveCenter(pos);

    if(getIsActive())
    {
        PainterPair chk(painter);
        painter->setPen(QPen(Qt::red,2));
        painter->drawRect(rect.adjusted(-5, -5, 5, 5));
    }
    if(getIsHover())
    {
        //通过推算经纬度计算半径
        PainterPair chk(painter);
        double dr = ZCHX::Utils::getDistanceByPixel(m_framework, lat(), lon(), getMaxTrackRange(), 0, pos);
        painter->setBrush(QBrush(QColor(0,255,255,100)));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(pos, dr, dr);
    }
    painter->drawPixmap(rect.toRect(), image);
}
}

