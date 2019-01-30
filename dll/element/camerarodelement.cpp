#include "camerarodelement.h"
#include "zchxmapframe.h"

namespace DrawElement
{
CameraRodElement::CameraRodElement(const ZCHX::Data::ITF_CameraRod &data)
    :Element(data.nLatLon.lat,data.nLatLon.lon)
{
    m_data = data;
    uuid = data.nUUID;
}

ZCHX::Data::ITF_CameraRod CameraRodElement::data() const
{
    return m_data;
}

void CameraRodElement::setStatus(ZCHX::Data::CAMERAROD_STATUS st)
{
    m_data.nStatus = st;
}

ZCHX::Data::CAMERAROD_STATUS CameraRodElement::status() const
{
    return m_data.nStatus;
}

void CameraRodElement::drawElement(QPainter *painter)
{
    if(!painter) return;
    if(!isLayervisible()) return;
    /*绘制图片元素*/
    int curScale = m_framework->GetDrawScale();
    QPixmap cameraRadImg        = ZCHX::Utils::getImage(":/element/gan_normal.png", Qt::green, curScale);
    QPixmap cameraRodWarringImg = ZCHX::Utils::getImage(":/element/gan_bug.png", Qt::yellow, curScale);
    QPixmap cameraRodErrImg     = ZCHX::Utils::getImage(":/element/gan_error.png", Qt::red, curScale);
    QPixmap cameraRodFocus      = ZCHX::Utils::getImage(":/element/gan_bg.png", Qt::yellow, curScale);

    ZCHX::Data::ITF_CameraRod data =  this->data();
    Point2D pos = m_framework->LatLon2Pixel(data.nLatLon.lat,data.nLatLon.lon);
    QRect rect(pos.x - cameraRadImg.width() / 2, pos.y - cameraRadImg.height() / 2, cameraRadImg.width(),cameraRadImg.height());
    if(getIsActive())
    {
        PainterPair chk(painter);
        painter->setPen(QPen(Qt::red,2));
        painter->drawRect(rect.x() -5, rect.y() -5, rect.width()+5, rect.height()+5);
    }

    if(getIsHover())
    {
        //通过推算经纬度计算半径
        LatLon drll(0, 0);
        ZCHX::Utils::distbear_to_latlon(data.nLatLon.lat,data.nLatLon.lon,800,0,drll.lat,drll.lon);
        Point2D drPos = m_framework->LatLon2Pixel(drll);
        double dr = sqrt(abs(drPos.y - pos.y) * abs(drPos.y-pos.y) + abs(drPos.x - pos.x) * abs(drPos.x - pos.x));
        PainterPair chk(painter);
        painter->setBrush(QBrush(QColor(0,255,255,125)));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(pos.x,pos.y),dr,dr);
    }
    if(getIsFocus()) //是否聚焦状态
    {
        painter->drawPixmap(rect,cameraRodFocus);
    }

    ZCHX::Data::CAMERAROD_STATUS status = this->status();
    if(ZCHX::Data::CAMERAROD_WARRING == status)
    {
        painter->drawPixmap(rect, cameraRodWarringImg);
    }
    else if(ZCHX::Data::CAMERAROD_ERROR == status)
    {
        painter->drawPixmap(rect, cameraRodErrImg);
    }
    else
    {
        painter->drawPixmap(rect, cameraRadImg);
    }
}
}
