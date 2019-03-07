#include "coastelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
CoastElement::CoastElement(const ZCHX::Data::ITF_CoastData &ele, zchxMapFrameWork* f)
    :MoveElement(f, ZCHX::Data::ELEMENT_COAST)
{
    setData(ele);
    setIsUpdate(true);
}

void CoastElement::setData(const ZCHX::Data::ITF_CoastData &ele)
{
    m_path = ele.path;
    //m_type = (RADARTYPE)ele.type;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > CoastElement::path() const
{
    return m_data.path;
}

void CoastElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

std::vector<std::pair<double, double> >& CoastElement::getPath()
{
    return m_data.path;
}

void CoastElement::updateOldPath()
{
    m_path = m_data.path;
}

int CoastElement::id() const
{
    return uuid;
}

void CoastElement::setId(int id)
{
    uuid = id;
}

std::string CoastElement::name() const
{
    return m_name;
}

void CoastElement::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_CoastData CoastElement::data() const
{
    return m_data;
}


//bool CoastElement::contains(int range, double x, double y) const
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

void CoastElement::drawElement(QPainter *painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_COASTDATA)) return;
    PainterPair chk(painter);
    std::vector<std::pair<double,double>> tmp_path = path();
    QPainterPath polygon;
    QString name = QString::fromStdString(this->name());

    painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
    painter->setBrush(Qt::white);

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
            painter->drawEllipse(pos,5,5);
        }
    }

    MapStyle colorType = m_framework->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::gray,2,Qt::DashLine));
    }
    else
    {
        painter->setPen(QPen(QColor(Qt::darkGray), 2, Qt::SolidLine));
    }

    painter->setBrush(Qt::NoBrush);
    painter->drawPath(polygon);
    painter->drawText(polygon.boundingRect().center(),name);
}
}

