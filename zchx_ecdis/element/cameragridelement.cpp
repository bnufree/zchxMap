#include "cameragridelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
CameraGridElement::CameraGridElement(const ZCHX::Data::ITF_NetGrid &ele, zchxMapWidget* f)
    :Element(0,0,f, ZCHX::Data::ELE_CAMERA_GRID)
{
    setData(ele);
}

void CameraGridElement::setData(const ZCHX::Data::ITF_NetGrid &ele)
{
    m_data = ele;
    setIsUpdate(true);
}


int CameraGridElement::id() const
{
    return m_data.id;
}

void CameraGridElement::setId(int id)
{
    m_data.id = id;
}

std::string CameraGridElement::name() const
{
    return m_data.name.toStdString();
}

void CameraGridElement::setName(const std::string &name)
{
    m_data.name = QString::fromStdString(name);
}

ZCHX::Data::ITF_NetGrid CameraGridElement::data() const
{
    return m_data;
}


bool CameraGridElement::contains(const QPoint& pos) const
{
    if(!mView->framework()) return false;
    //计算左上和右下的经纬度围成的矩形范围
    QPointF top_left = mView->framework()->LatLon2Pixel(m_data.leftTopLat, m_data.leftTopLon).toPointF();
    QPointF bottom_right = mView->framework()->LatLon2Pixel(m_data.rightLowerLat, m_data.rightLowerLon).toPointF();
    QRectF rect(top_left, bottom_right);
    return rect.contains(pos);
}

void CameraGridElement::drawElement(QPainter *painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CAMERANETGRID)) return;
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::darkGray,1,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    //画矩形
    QPointF top_left = mView->framework()->LatLon2Pixel(m_data.leftTopLat, m_data.leftTopLon).toPointF();
    QPointF bottom_right = mView->framework()->LatLon2Pixel(m_data.rightLowerLat, m_data.rightLowerLon).toPointF();
    QRectF rectf(top_left, bottom_right);
    painter->drawRect(rectf);

    //画文字
    QPointF center = rectf.center();
    QString text = QString::number(m_data.id);
    QPen pen = painter->pen();
    pen.setColor(Qt::red);
    painter->setPen(pen);
    int txt_width = painter->fontMetrics().width(text);
    int txt_height = painter->fontMetrics().height();
    QPointF txt_pos = center + QPointF(-0.5 * txt_width, 0.5 * txt_height);

    painter->drawText(txt_pos, text );
}
}

