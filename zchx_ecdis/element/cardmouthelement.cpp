#include "cardmouthelement.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"

namespace qt {
CardMouthElement::CardMouthElement(const ZCHX::Data::ITF_CardMouth &ele, zchxMapFrameWork* f)
    :MoveElement(f, ZCHX::Data::ELEMENT_CARD_MOUTH)
{
    setData(ele);
}

void CardMouthElement::setData(const ZCHX::Data::ITF_CardMouth &ele)
{
    m_path = ele.path;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > CardMouthElement::path() const
{
    return m_data.path;
}

void CardMouthElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int CardMouthElement::id() const
{
    return uuid;
}

void CardMouthElement::setId(int id)
{
    uuid = id;
}

std::string CardMouthElement::name() const
{
    return m_name;
}

void CardMouthElement::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_CardMouth CardMouthElement::data() const
{
    return m_data;
}

std::vector<std::pair<double, double> > & CardMouthElement::getPath()
{
    return m_data.path;
}

void CardMouthElement::updateOldPath()
{
    m_path = m_data.path;
}


void CardMouthElement::drawElement(QPainter *painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CARDMOUTH)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString name = QString::fromStdString(this->name());
    QString color = data().fillColor;
    QPainterPath polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
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

    PainterPair chk(painter);
    MapStyle colorType = m_framework->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(QColor(color), 4, Qt::SolidLine));
    }
    else
    {
        painter->setPen(QPen(QColor(color), 4, Qt::SolidLine));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(polygon);
    painter->drawText(polygon.boundingRect().center(),name);
}
}

