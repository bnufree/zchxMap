#include "cameraelement.h"
#include "zchxmapframe.h"
#include "map_layer/zchxmaplayermgr.h"
#include <QPainter>

using namespace qt ;

CameraElement::CameraElement(const ZCHX::Data::ITF_CameraDev &data, zchxMapWidget* frame)
    :FixElement<ZCHX::Data::ITF_CameraDev>(data, ZCHX::Data::ELE_CAMERA, ZCHX::LAYER_CAMERA, frame)
    ,mParent(0)
{
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

QPixmap CameraElement::getImage() const
{
    if(mView->framework())
    {
        int curScale = mView->framework()->Zoom();
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
    if(!isDrawAvailable(painter)) return;
    if(mParent) return; //当前目标悬挂在其他图元上不显示
    //开始显示
    int curScale = this->framework()->GetDrawScale();
    QPointF pos = this->framework()->LatLon2Pixel(data().getLat(), data().getLon()).toPointF();
    QPixmap image = ZCHX::Utils::getImage(data().nType, data().nStatus, curScale);

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
        double dr = ZCHX::Utils::getDistanceByPixel(mView->framework(), lat(), lon(), getMaxTrackRange(), 0, pos);
        painter->setBrush(QBrush(QColor(0,255,255,100)));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(pos, dr, dr);
    }
    painter->drawPixmap(rect.toRect(), image);
}

void CameraElement::clicked(bool isDouble)
{
    if(!mView) return;
    if(isDouble) {
        mView->signalIsDoubleClicked4CameraDev(data());
    } else {
        mView->signalIsSelected4CameraDev(data());
    }
}

