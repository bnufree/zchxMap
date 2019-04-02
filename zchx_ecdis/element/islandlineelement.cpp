#include "islandlineelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
std::string IslandLineElement::content() const
{
    return m_content;
}

void IslandLineElement::copyDataFromOther(std::shared_ptr<Element> other)
{
    if(!other) return;
    IslandLineElement *src = dynamic_cast<IslandLineElement*>(other.get());
    if(src)
    {
        this->setData(src->data());
    }
}

void IslandLineElement::setContent(const std::string &content)
{
    m_content = content;
}



QString IslandLineElement::color() const
{
    return m_data.warnColor;
}

void IslandLineElement::setColor(QString color)
{
    m_data.warnColor = color;
}


void IslandLineElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;

    std::vector<std::pair<double,double>> tmp_path = path();
    QPainterPath polygon;
    for(int i=0; i<tmp_path.size();++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = mView->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
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

