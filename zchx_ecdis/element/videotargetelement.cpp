#include "videotargetelement.h"
#include "zchxmapframe.h"
#include "map_layer/zchxmaplayermgr.h"
#include <QPainter>

namespace qt {
VideoTargetElement::VideoTargetElement(const ZCHX::Data::ITF_VideoTarget &data, zchxMapFrameWork* work)
    :Element(data.objectMapPosY, data.objectMapPosX, work, ZCHX::Data::ELEMENT_VIDEO_TARGET)
{
    m_data = data;
    uuid = data.uuid;
    mTargetIImgList<< ":/element/ren.png"
                      << ":/element/che.png"
                      << ":/element/chuan.png"
                      << ":/element/no.png";
}

const ZCHX::Data::ITF_VideoTarget &VideoTargetElement::getData() const
{
    return m_data;
}

void VideoTargetElement::setData(const ZCHX::Data::ITF_VideoTarget &data)
{
    m_data = data;
}

uint VideoTargetElement::getTargetStatus() const
{
    return m_data.objectState;
}

uint VideoTargetElement::getTargetType() const
{
    return m_data.objectType;
}

uint VideoTargetElement::getAlarmType() const
{
    return m_data.alarmType;
}

QString VideoTargetElement::getObjId() const
{
    return m_data.objectID;
}

QString VideoTargetElement::getAlarmColor() const
{
    return m_data.warn_color;
}

void VideoTargetElement::drawElement(QPainter *painter)
{
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_WARNING_TARGET)) return ;
    if(!framework()) return;

    QPixmap image   = ZCHX::Utils::getImage(mTargetIImgList.value(getTargetType()), Qt::green, m_framework->GetDrawScale());
    std::pair<double, double> ll = getLatLon();

    Point2D pos = m_framework->LatLon2Pixel(ll.first,ll.second);
    QRect rect(0,0,image.width(),image.height());
    rect.moveCenter(QPoint(pos.x, pos.y));

    painter->drawPixmap(rect, image);
    if(getIsActive())
    {
        PainterPair chk(painter);
        painter->setPen(QPen(Qt::red,2));
        painter->drawRect(rect.x() -5, rect.y() -5, rect.width()+10, rect.height()+10);
    }
    painter->drawText(QPointF(rect.x() + rect.width() +3,rect.y()),getObjId());
}
}

