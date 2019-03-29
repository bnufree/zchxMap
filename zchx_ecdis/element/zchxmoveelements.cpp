#include "zchxmoveelements.h"
#include "zchxMapDataUtils.h"
#include "zchxmapframe.h"

using namespace qt;
void WarningZoneElement::drawElement(QPainter *painter)
{
    //禁止显示防区信息
    if(!this->isDrawAvailable(painter)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QPolygonF polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        ZCHX::Data::Point2D  curPos = this->framework()->LatLon2Pixel(ll.first, ll.second);
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
    if(m_data.shape != ZCHX::Data::ITF_WarringZone::ZONE_LINE){
        if(polygon.first() != polygon.last())
        {
            polygon.append(polygon.first());
        }
    }
    PainterPair chk(painter);
    MapStyle colorType = this->framework()->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::gray, 2 ,Qt::DashLine));
    }
    else
    {
        painter->setPen(QPen(QColor(getDefenceColor()),2,Qt::DashLine));
    }
    painter->setBrush(QBrush(Qt::blue, Qt::Dense7Pattern));
    if(m_data.shape != ZCHX::Data::ITF_WarringZone::ZONE_LINE){
        painter->drawPolygon(polygon);
        painter->drawText(polygon.boundingRect().center(), m_data.getName());
    } else {
        painter->drawPolyline(polygon);
        painter->drawText(polygon.last(), m_data.getName());
    }
}

void CoastElement::drawElement(QPainter *painter)
{
    if(!this->isDrawAvailable(painter)) return;
    PainterPair chk(painter);
    std::vector<std::pair<double,double>> tmp_path = path();
    QPainterPath polygon;
    QString name = QString::fromStdString(this->name());

    painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
    painter->setBrush(Qt::white);

    for(int i=0; i<tmp_path.size();++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = this->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
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

    MapStyle colorType = this->framework()->GetMapStyle();
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

void SeabedPipeLineElement::drawElement(QPainter *painter)
{
    if(!this->isDrawAvailable(painter)) return;
    PainterPair chk(painter);
    std::vector<std::pair<double,double>> tmp_path = path();
    QPainterPath polygon;
    QString name = QString::fromStdString(this->name());

    painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
    painter->setBrush(Qt::white);

    for(int i=0; i<tmp_path.size();++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = this->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
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
    MapStyle colorType = this->framework()->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::gray,2,Qt::DashLine));
    }
    else
    {
        painter->setPen(QPen(QColor(Qt::black), 4, Qt::DashLine));
    }

    painter->setBrush(Qt::NoBrush);
    painter->drawPath(polygon);
    painter->drawText(polygon.boundingRect().center(),name);
}


void AreaNetElement::drawElement(QPainter *painter)
{
    if(!this->isDrawAvailable(painter)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString name = QString::fromStdString(this->name());
    QPolygonF polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = this->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
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
    MapStyle colorType = this->framework()->GetMapStyle();
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

void CardMouthElement::drawElement(QPainter *painter)
{
    if(!this->isDrawAvailable(painter)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString name = QString::fromStdString(this->name());
    QString color = data().fillColor;
    QPainterPath polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = this->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
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
    MapStyle colorType = this->framework()->GetMapStyle();
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

void ChannelElement::setLineSelected(int i, bool selectStatus)
{
    if (i >= 0 && i < m_data.lineList.size())
    {
        m_data.lineList[i].isSelected = selectStatus;
    }
}

void ChannelElement::drawElement(QPainter *painter)
{
    if(!this->isDrawAvailable(painter)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString color = data().fillColor;
    int courseType = data().courseType;
    QString name = QString::fromStdString(this->name());
    QPolygonF polygon;

    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = this->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
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
    MapStyle colorType = this->framework()->GetMapStyle();

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
            QPointF pos = this->framework()->LatLon2Pixel(lineObj.startLat, lineObj.startLon).toPointF();
            painter->drawEllipse(pos, 5, 5);
            QPointF pos2 = this->framework()->LatLon2Pixel(lineObj.endLat, lineObj.endLon).toPointF();
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

void MooringElement::drawElement(QPainter *painter)
{
    if(!this->isDrawAvailable(painter)) return;
    std::vector<std::pair<double,double>> tmp_path = path();
    QString name = QString::fromStdString(this->name());
    QString color = data().fillColor;
    QPolygonF polygon;
    for(int i = 0; i < tmp_path.size(); ++i)
    {
        std::pair<double, double> ll = tmp_path[i];
        QPointF pos = this->framework()->LatLon2Pixel(ll.first, ll.second).toPointF();
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
    MapStyle colorType = this->framework()->GetMapStyle();
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

