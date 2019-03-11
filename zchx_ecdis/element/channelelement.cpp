#include "channelelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
ChannelElement::ChannelElement(const ZCHX::Data::ITF_Channel &ele, zchxMapWidget* f)
    :MoveElement(f, ZCHX::Data::ELE_CHANNEL)
{
    setData(ele);
    setIsUpdate(true);
}

void ChannelElement::setData(const ZCHX::Data::ITF_Channel &ele)
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

std::vector<std::pair<double, double> > ChannelElement::path() const
{
    return m_data.path;
}

void ChannelElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

//int ChannelElement::id() const
//{
//    return uuid;
//}

//void ChannelElement::setId(int id)
//{
//    uuid = id;
//}

std::string ChannelElement::name() const
{
    return m_name;
}

void ChannelElement::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_Channel ChannelElement::data() const
{
    return m_data;
}

void ChannelElement::setLineSelected(int i, bool selectStatus)
{
    if (i >= 0 && i < m_data.lineList.size())
    {
        m_data.lineList[i].isSelected = selectStatus;
    }
}

std::vector<std::pair<double, double> > & ChannelElement::getPath()
{
    return m_data.path;
}

void ChannelElement::updateOldPath()
{
    m_path = m_data.path;
}


void ChannelElement::drawElement(QPainter *painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CHANNEL)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString color = data().fillColor;
    int courseType = data().courseType;
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
            painter->drawEllipse(pos, 5, 5);
        }
    }

    polygon.append(polygon.first());
    PainterPair chk(painter);
    MapStyle colorType = mView->framework()->GetMapStyle();

    Qt::PenStyle style = Qt::SolidLine;
    if (courseType == ZCHX::Data::COURSE_TYPE_SIGNAL)
    {
        style = Qt::DashLine;
    }
    else if (courseType == ZCHX::Data::COURSE_TYPE_DOUBLE)
    {
        style = Qt::SolidLine;
    }
    else if (courseType == ZCHX::Data::COURSE_TYPE_RESTRICT)
    {
        style = Qt::DashDotLine;
    }
    else if (courseType == ZCHX::Data::COURSE_TYPE_OTHER)
    {
        style = Qt::DotLine;
    }

    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::white, 4, style));
    }
    else
    {
        painter->setPen(QPen(Qt::white, 4, style));
    }
    painter->setBrush(QBrush(QColor(color), Qt::SolidPattern));
    painter->setOpacity(0.3);
    painter->drawPolygon(polygon);
    painter->drawText(polygon.boundingRect().center(), name);

    // 航道线选中设置
    for (int i = 0; i < data().lineList.size(); i++)
    {
        ZCHX::Data::ITF_ChannelLine lineObj = data().lineList.at(i);
        if (lineObj.isSelected)
        {
            PainterPair chk(painter);
            painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
            painter->setBrush(Qt::white);
            QPointF pos = mView->framework()->LatLon2Pixel(lineObj.startLat, lineObj.startLon).toPointF();
            painter->drawEllipse(pos, 5, 5);
            QPointF pos2 = mView->framework()->LatLon2Pixel(lineObj.endLat, lineObj.endLon).toPointF();
            painter->drawEllipse(pos2, 5, 5);
            {
                PainterPair chk(painter);
                painter->setPen(QPen(Qt::white, 6, Qt::SolidLine));
                painter->drawLine(pos, pos2);
            }
            break;
        }
    }
}
}

