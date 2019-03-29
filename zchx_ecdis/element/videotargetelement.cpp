#include "videotargetelement.h"
#include "zchxmapframe.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
#include <QPainter>

namespace qt {
VideoTargetElement::VideoTargetElement(const ZCHX::Data::ITF_VideoTarget &data, zchxMapWidget* work)
    :FixElement<ZCHX::Data::ITF_VideoTarget>(data, ZCHX::Data::ELE_VIDEO_TARGET, ZCHX::LAYER_WARNING_TARGET, work)
{
    mTargetIImgList<< ":/element/ren.png"
                      << ":/element/che.png"
                      << ":/element/chuan.png"
                      << ":/element/no.png";
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
    if(!isDrawAvailable(painter)) return ;

    QPixmap image   = ZCHX::Utils::getImage(mTargetIImgList.value(getTargetType()), Qt::green, mView->framework()->GetDrawScale());
    std::pair<double, double> ll = getLatLon();

    ZCHX::Data::Point2D pos = mView->framework()->LatLon2Pixel(ll.first,ll.second);
    QRect rect(0,0,image.width(),image.height());
    rect.moveCenter(QPoint(pos.x, pos.y));
    updateBouningRect(pos.toPointF(), image.width(), image.height());
    updateGeometry(pos.toPointF(), getDrawScaleSize());

    painter->drawPixmap(rect, image);
    painter->drawText(QPointF(rect.x() + rect.width() +3,rect.y()),getObjId());
    drawActive(painter);
}
}

