#include "islandlineelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
IslandLineElement::IslandLineElement(const ZCHX::Data::ITF_IslandLine &ele, zchxMapFrameWork* f)
    : Element(0, 0, f, ZCHX::Data::ELEMENT_ISLAND_LINE)
    , m_path(ele.path)
    , m_type((RADARTYPE)ele.type)
{
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    uuid = ele.uuid;
    m_id = ele.id;
    m_name = ele.name.toStdString();
    m_content = ele.content.toStdString();
}

std::vector<std::pair<double, double> > IslandLineElement::path() const
{
    return m_data.path;
}

void IslandLineElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int IslandLineElement::id() const
{
    return m_id;
}

void IslandLineElement::setId(int id)
{
    m_id = id;
}

std::string IslandLineElement::name() const
{
    return m_name;
}

void IslandLineElement::setName(const std::string &name)
{
    m_name = name;
}

std::string IslandLineElement::content() const
{
    return m_content;
}

void IslandLineElement::setContent(const std::string &content)
{
    m_content = content;
}

void IslandLineElement::setData(const ZCHX::Data::ITF_IslandLine &data)
{
    m_data = data;
}

ZCHX::Data::ITF_IslandLine IslandLineElement::data() const
{
    return m_data;
}

void IslandLineElement::changePathPoint(double lat, double lon)
{
    if(0 <= m_activePathPoint && m_activePathPoint < m_data.path.size())
    {
        m_data.path[m_activePathPoint].first = lat;
        m_data.path[m_activePathPoint].second = lon;
    }
}

void IslandLineElement::moveTo(double lat, double lon)
{
    for(int i= 0; i< m_data.path.size(); ++i)
    {
        m_data.path[i].first  = m_path[i].first  + lat;
        m_data.path[i].second = m_path[i].second + lon;
    }
}

void IslandLineElement::updateOldPath()
{
    m_path = m_data.path;
}

int IslandLineElement::activePathPoint() const
{
    return m_activePathPoint;
}

void IslandLineElement::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void IslandLineElement::delPathPoint(int idx)
{
    if(m_data.path.size() > idx)
    {
        m_data.path.erase(m_data.path.begin()+idx);
        m_path = m_data.path;
        m_activePathPoint = -1;
    }
}

void IslandLineElement::addCtrlPoint(std::pair<double, double> ps)
{
    m_data.path.push_back(ps);
    m_path = m_data.path;
    m_activePathPoint = -1;
}

QString IslandLineElement::color() const
{
    return m_data.warnColor;
}

void IslandLineElement::setColor(QString color)
{
    m_data.warnColor = color;
}

bool IslandLineElement::contains(int range, double x, double y) const
{
    return Element::contains(range, x, y);
}

void IslandLineElement::drawElement(QPainter *painter)
{
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_ISLAND)) return;

    std::vector<std::pair<double,double>> tmp_path = path();
    QPainterPath polygon;
    for(int i=0; i<tmp_path.size();++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = m_framework->LatLon2Pixel(ll.first, ll.second).toPointF();
        if(0 == i)
        {
            polygon.moveTo(pos);
        }
        else
        {
            polygon.lineTo(pos);
        }
        if(getIsActive())
        {
            PainterPair chk(painter);
            painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
            painter->setBrush(Qt::white);
            painter->drawEllipse(pos,5,5);
        }
    }
    polygon.closeSubpath(); //闭合路径
    PainterPair chk(painter);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(color()),3));
    painter->drawPath(polygon);
}
}

