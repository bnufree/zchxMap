#include "cameraelement.h"
#include "zchxmapframe.h"
#include "map_layer/zchxMapLayer.h"
#include <QPainter>

namespace DrawElement
{
CameraElement::CameraElement(const ZCHX::Data::ITF_CameraDev &data, Element* depends)
    :Element(data.nLatLon.lat, data.nLatLon.lon), mDependElement(depends)
{
    m_data = data;
    uuid = data.nUUID;
}

const ZCHX::Data::ITF_CameraDev &CameraElement::data() const
{
    return m_data;
}

int CameraElement::getGanPid() const
{
    if(m_data.mRod) return m_data.mRod->nUUID;
    return -1;
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
    if(!painter) return;
    if(!isLayervisible()) return;
    //取得当前的地图层级
    if(!m_framework) return;

    //根据相机的种类进行显示．如果枪机和球机等有杆，则不显示相机，否则直接显示相机
//    int type = data().nType;
//    if(type == ZCHX::Data::CAMERA_BALL || type == ZCHX::Data::CAMERA_GUN || type == ZCHX::Data::CAMERA_PANORAMA)
//    {
//        if(data().szSite.size() > 0 || data.mRod) return;
//    }
    if(!mDependElement) return;
    //开始显示
    int curScale = m_framework->GetDrawScale();
    QPointF pos = m_framework->LatLon2Pixel(lat(),lon()).toPointF();
    QPixmap image = ZCHX::Utils::getImage(this->data().nType, this->data().nStatus, curScale);

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

