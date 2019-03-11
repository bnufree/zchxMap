#include "moveelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
MoveElement::MoveElement(zchxMapWidget* f, ZCHX::Data::ELETYPE type)
    :Element(0,0,f,type)
{
    setData();
}

void MoveElement::setData()
{
    m_activePathPoint = -1;
}

void MoveElement::changePathPoint(double lat, double lon)
{
    std::vector<std::pair<double, double> > & path = getPath();
    if(0 <= m_activePathPoint && m_activePathPoint < path.size())
    {
        path[m_activePathPoint].first = lat;
        path[m_activePathPoint].second = lon;
    }
}

void MoveElement::moveTo(double lat, double lon)
{
    std::vector<std::pair<double, double> > & path = getPath();
    for(int i= 0; i< path.size(); ++i)
    {
        path[i].first  = path[i].first  + lat;
        path[i].second = path[i].second + lon;
    }
}

int MoveElement::activePathPoint() const
{
    return m_activePathPoint;
}

void MoveElement::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void MoveElement::delPathPoint(int idx)
{
    std::vector<std::pair<double, double> > & path = getPath();
    if(path.size() > idx)
    {
        path.erase(path.begin() + idx);
        m_activePathPoint = -1;
    }
}

void MoveElement::addCtrlPoint(std::pair<double, double> ps)
{
    std::vector<std::pair<double, double> > & path = getPath();
    path.push_back(ps);
    m_activePathPoint = -1;
}

bool MoveElement::contains(int range, double x, double y) const
{
    if(!mView->framework()) return false;

    std::vector<std::pair<double,double>> tmp_path = path();
    for(int i = 0; i < tmp_path.size() - 1; ++i)
    {
        std::pair<double, double> p1 = tmp_path[i];
        std::pair<double, double> p2 = tmp_path[i+1];
        ZCHX::Data::Point2D start = mView->framework()->LatLon2Pixel(p1.first, p1.second);
        ZCHX::Data::Point2D end = mView->framework()->LatLon2Pixel(p2.first, p2.second);

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

bool MoveElement::contains(const QPoint &pt) const
{
    if(!mView->framework()) return false;
    std::vector<std::pair<double,double>> tmp_path = path();
    QPainterPath paint_path;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        ZCHX::Data::Point2D  curPos = mView->framework()->LatLon2Pixel(ZCHX::Data::LatLon(ll.first,ll.second));
        if(i == 0)
        {
            paint_path.moveTo(QPointF(curPos.x,curPos.y));
        }
        else
        {
            paint_path.lineTo(QPointF(curPos.x,curPos.y));
        }
    }
    return paint_path.contains(QPointF(pt.x(), pt.y()));
}
}

