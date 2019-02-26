#include "warningzoneelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
WarningZoneElement::WarningZoneElement(const ZCHX::Data::ITF_WarningZoneElement &ele, zchxMapFrameWork* f)
    :Element(0,0,f,ZCHX::Data::ELEMENT_WARNING_ZONE)
{
    setData(ele);
    setIsUpdate(true);
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

void WarningZoneElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int WarningZoneElement::id() const
{
    return uuid;
}

void WarningZoneElement::setId(int id)
{
    uuid = id;
}

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

void WarningZoneElement::changePathPoint(double lat, double lon)
{
    if(0 <= m_activePathPoint  && m_activePathPoint < m_data.path.size())
    {
        m_data.path[m_activePathPoint].first = lat;
        m_data.path[m_activePathPoint].second = lon;
    }
}

void WarningZoneElement::moveTo(double lat, double lon)
{
    for(int i= 0; i< m_data.path.size(); ++i)
    {
        m_data.path[i].first  = m_path[i].first  + lat;
        m_data.path[i].second = m_path[i].second + lon;
    }
}

void WarningZoneElement::updateOldPath()
{
    m_path = m_data.path;
}

int WarningZoneElement::activePathPoint() const
{
    return m_activePathPoint;
}

void WarningZoneElement::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
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

void WarningZoneElement::addCtrlPoint(std::pair<double, double> ps)
{
    m_data.path.push_back(ps);
    m_path = m_data.path;
    m_activePathPoint = -1;
}

bool WarningZoneElement::contains(int range, double x, double y) const
{
    if(!framework()) return false;
    std::vector<std::pair<double,double>> tmp_path = path();
    for(int i=0; i<tmp_path.size()-1;++i)
    {
        std::pair<double, double> p1 = tmp_path[i];
        std::pair<double, double> p2 = tmp_path[i+1];
        Point2D start = framework()->LatLon2Pixel(p1.first, p1.second);
        Point2D end = framework->LatLon2Pixel(p2.first, p2.second);

        //检查3点是否共线
        int p1x = start.x, p1y = start.y;
        int p2x = end.x, p2y = end.y;
        int check = (p1x - x) *(p2y - y) - (p2x - x) * (p1y - y);
        //qDebug()<<"start:"<<p1x<<" "<<p1y<<" end:"<<p2x<<" "<<p2y<<" cur:"<<x<<" "<<y<<" area:"<<check;
        if(abs(check) < range)
        {
            return true;
        }
    }

    return false;
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
        Point2D  curPos = m_framework->LatLon2Pixel(ll.first, ll.second);
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
    MapStyle colorType = m_framework->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::gray, 2 ,Qt::DashLine));
    }
    else
    {
        painter->setPen(QPen(QColor(ele.getDefenceColor()),2,Qt::DashLine));
    }
    painter->setBrush(QBrush(Qt::blue, Qt::Dense7Pattern));
    painter->drawPolygon(polygon);
    painter->drawText(polygon.boundingRect().center(),QString::fromStdString(name()));
}
}

