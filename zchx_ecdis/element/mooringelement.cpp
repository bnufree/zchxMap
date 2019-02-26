#include "mooringelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
MooringElement::MooringElement(const ZCHX::Data::ITF_Mooring &ele, zchxMapFrameWork* f)
    :MoveElement(f, ZCHX::Data::ELEMENT_MOOR)
{
    setData(ele);
    setIsUpdate(true);
}

void MooringElement::setData(const ZCHX::Data::ITF_Mooring &ele)
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

std::vector<std::pair<double, double> > MooringElement::path() const
{
    return m_data.path;
}

void MooringElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int MooringElement::id() const
{
    return uuid;
}

void MooringElement::setId(int id)
{
    uuid = id;
}

std::string MooringElement::name() const
{
    return m_name;
}

void MooringElement::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_Mooring MooringElement::data() const
{
    return m_data;
}

std::vector<std::pair<double, double> > & MooringElement::getPath()
{
    return m_data.path;
}

void MooringElement::updateOldPath()
{
    m_path = m_data.path;
}
void MooringElement::drawElement(QPainter *painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_MOORING)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString name = QString::fromStdString(name());
    QString color = data().fillColor;
    QPolygonF polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = m_framework->LatLon2Pixel(ll.first, ll.second).toPointF();
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
    MapStyle colorType = m_framework->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::white, 4, Qt::SolidLine));
    }
    else
    {
        painter->setPen(QPen(Qt::white, 4, Qt::SolidLine));
    }
    painter->setBrush(QBrush(QColor(color), Qt::SolidPattern));
    painter->setOpacity(0.3);
    painter->drawPolygon(polygon);
    painter->drawText(polygon.boundingRect().center(),name);
}
}

