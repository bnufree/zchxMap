#include "areanetelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
AreaNetElement::AreaNetElement(const ZCHX::Data::ITF_AreaNet &ele, zchxMapWidget* f)
    :MoveElement(f, ZCHX::Data::ELE_AREA_NET)
{
    setData(ele);
}

void AreaNetElement::setData(const ZCHX::Data::ITF_AreaNet &ele)
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

std::vector<std::pair<double, double> > AreaNetElement::path() const
{
    return m_data.path;
}

std::vector<std::pair<double, double> >& AreaNetElement::getPath()
{
    return m_data.path;
}

void AreaNetElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

void AreaNetElement::updateOldPath()
{
    m_path = m_data.path;
}

//int AreaNetElement::id() const
//{
//    return uuid;
//}

//void AreaNetElement::setId(int id)
//{
//    uuid = id;
//}

std::string AreaNetElement::name() const
{
    return m_name;
}

void AreaNetElement::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_AreaNet AreaNetElement::data() const
{
    return m_data;
}

//bool AreaNetElement::contains(int range, double x, double y) const
//{
//    if(!framework()) return false;
//    std::vector<std::pair<double,double>> tmp_path = path();
//    for(int i=0; i<tmp_path.size()-1;++i)
//    {
//        std::pair<double, double> p1 = tmp_path[i];
//        std::pair<double, double> p2 = tmp_path[i+1];
//        ZCHX::Data::Point2D start = framework()->LatLon2Pixel(p1.first, p1.second);
//        ZCHX::Data::Point2D end = framework()->LatLon2Pixel(p2.first, p2.second);

//        //检查3点是否共线
//        int p1x = start.x, p1y = start.y;
//        int p2x = end.x, p2y = end.y;
//        int check = (p1x - x) *(p2y - y) - (p2x - x) * (p1y - y);
//        //qDebug()<<"start:"<<p1x<<" "<<p1y<<" end:"<<p2x<<" "<<p2y<<" cur:"<<x<<" "<<y<<" area:"<<check;
//        if(abs(check) < range)
//        {
//            return true;
//        }
//    }

//    return false;
//}

void AreaNetElement::drawElement(QPainter *painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_AREANET)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString name = QString::fromStdString(this->name());
    QPolygonF polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = mView->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
        polygon.append(pos);
        if(getIsActive())
        {
            PainterPair chk(painter);
            painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
            painter->setBrush(Qt::white);
            painter->drawEllipse(pos,5,5);
        }
    }

    polygon.append(polygon.first());
    PainterPair chk(painter);
    MapStyle colorType = mView->framework()->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::gray, 2, Qt::DashLine));
    }
    else
    {
        painter->setPen(QPen(Qt::darkGray, 2, Qt::DashLine));
    }
    painter->setBrush(QBrush(Qt::blue, Qt::Dense7Pattern));
    painter->drawPolygon(polygon);
    painter->drawText(polygon.boundingRect().center(),name);
}
}

