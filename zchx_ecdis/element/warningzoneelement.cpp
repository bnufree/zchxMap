#include "warningzoneelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
WarningZoneElement::WarningZoneElement(const ZCHX::Data::ITF_WarringZone &ele, zchxMapWidget* f)
    :MoveElement(f,ZCHX::Data::ELE_WARNING_ZONE)
{
    setData(ele);
}

void WarningZoneElement::setData(const ZCHX::Data::ITF_WarringZone &ele)
{
    m_path = ele.path;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
    setIsUpdate(true);
}

std::vector<std::pair<double, double> > WarningZoneElement::path() const
{
    return m_data.path;
}

std::vector<std::pair<double, double> > & WarningZoneElement::getPath()
{
    return m_data.path;
}

void WarningZoneElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

//int WarningZoneElement::id() const
//{
//    return uuid;
//}

//void WarningZoneElement::setId(int id)
//{
//    uuid = id;
//}

std::string WarningZoneElement::name() const
{
    return m_name;
}

void WarningZoneElement::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_WarringZone WarningZoneElement::data() const
{
    return m_data;
}


void WarningZoneElement::updateOldPath()
{
    m_path = m_data.path;
}



void WarningZoneElement::delPathPoint(int idx)
{
    if(m_data.path.size() > idx)
    {
        m_data.path.erase(m_data.path.begin()+idx);
        m_path = m_data.path;
        m_activePathPoint = -1;
    }
}

void WarningZoneElement::drawElement(QPainter *painter)
{
    //禁止显示防区信息
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_DEFENCE)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QPolygonF polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        ZCHX::Data::Point2D  curPos = mView->framework()->LatLon2Pixel(ll.first, ll.second);
        QPointF pos(curPos.x,curPos.y);
        polygon.append(pos);
        if(getIsActive())
        {
            PainterPair chk(painter);
            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
            painter->setBrush(Qt::white);
            painter->drawEllipse(pos, 5, 5);
        }
    }
    //闭合图形
    polygon.append(polygon.first());
    PainterPair chk(painter);
    MapStyle colorType = mView->framework()->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::gray, 2 ,Qt::DashLine));
    }
    else
    {
        painter->setPen(QPen(QColor(getDefenceColor()),2,Qt::DashLine));
    }
    painter->setBrush(QBrush(Qt::blue, Qt::Dense7Pattern));
    painter->drawPolygon(polygon);
    painter->drawText(polygon.boundingRect().center(),QString::fromStdString(name()));
}
}

