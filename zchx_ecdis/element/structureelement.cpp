#include "structureelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
StructureElement::StructureElement(const ZCHX::Data::ITF_Structure &ele, zchxMapFrameWork* f)
    :Element(0,0,f, ZCHX::Data::ELEMENT_STRUCTURE)
{
    setData(ele);
    setIsUpdate(true);
}

void StructureElement::setData(const ZCHX::Data::ITF_Structure &ele)
{
    m_point = ele.point;
    //m_type = (RADARTYPE)ele.type;
    elelat = ele.point.first;
    elelon = ele.point.second;
    m_data = ele;
    m_name = ele.name.toStdString();
    setIsUpdate(true);
}

std::pair<double, double> StructureElement::point() const
{
    return m_data.point;
}

void StructureElement::setPoint(const std::pair<double, double> &point)
{
    m_data.point = point;
}

int StructureElement::id() const
{
    return uuid;
}

void StructureElement::setId(int id)
{
    uuid = id;
}

std::string StructureElement::name() const
{
    return m_name;
}

void StructureElement::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_Structure StructureElement::data() const
{
    return m_data;
}

//bool StructureElement::contains(int range, double x, double y) const
//{
//    return false;
//}

void StructureElement::drawElement(QPainter *painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_STRUCTURE)) return;
    ZCHX::Data::Point2D  curPos = m_framework->LatLon2Pixel(point().first, point().second);

    QPixmap devicePix;
    if(!devicePix.load(":/navig64/device.png")) return;

    int posX = curPos.x - devicePix.width() / 2;
    int poxY = curPos.y - devicePix.height() / 2;
    painter->drawPixmap(posX, poxY, devicePix.width(), devicePix.height(), devicePix);
    QString name = QString::fromStdString(this->name());
    painter->drawText(posX + 30, poxY, name);
}
}

