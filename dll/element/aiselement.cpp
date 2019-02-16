#include "aiselement.hpp"
#include "zchxmapframe.h"
#include "zchxmapwidget.h"
#include "zchxMapDatautils.h"
#include "profiles.h"
#include "zchxutils.hpp"
#include <QDebug>

#define     SHIP_INTERVAL_TIME  60 * 1000 // 1分钟

namespace DrawElement{

AisElement::AisElement()
    : Element(0,0,ZCHX::Data::ELEMENT_AIS)
//    , m_isFleet(false)
{
    m_drawTargetInfo = true;
    mTrackList.clear();
    mBigDisplayTrackIndex = -1;
    initFromSettings();
}

AisElement::AisElement(const ZCHX::Data::ITF_AIS &ele)
    : Element(ele.lat,ele.lon, ZCHX::Data::ELEMENT_AIS, ele.warnStatusColor)
    , m_data(ele)
//    , m_isFleet(false)
{
    setStrID(m_data.id);
    mTrackList.clear();
    mBigDisplayTrackIndex = -1;
    m_drawTargetInfo = true;
    initFromSettings();
}

const std::vector<std::pair<double, double> >& AisElement::getPath() const
{
    return m_data.getPath();
}

void AisElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.setPath(path);
}

RADARTYPE AisElement::getType() const
{
    return (RADARTYPE)m_data.type;
}

const ZCHX::Data::ITF_AIS& AisElement::getData() const
{
    return m_data;
}

void AisElement::setData(const ZCHX::Data::ITF_AIS &data)
{
    m_data = data;
    setStrID(m_data.id);
    Element::setLatLon(data.lat, data.lon);
    Element::setDisplayLat(data.lat);
    Element::setDisplayLon(data.lon);
    Element::updateFlashRegionColor(data.warnStatusColor);
}

qint64 AisElement::getUtc() const
{
    return m_data.UTC;
}

uint AisElement::getStatus() const
{
    return m_data.status;
}

void AisElement::setStatus(const uint &value)
{
    m_data.status = value;
}

bool AisElement::hasCamera() const
{
    return m_cameraDev.size() != 0;
}

void AisElement::setCameraData(const std::vector<ZCHX::Data::ITF_CameraDev> &camera)
{
    m_cameraDev = camera;
}

void AisElement::drawCPA(QPainter *painter)
{
    if(!painter)  return;
    if(getData().cpaAttr.isEmpty()) return;
    QPoint pos = getCurrentPos().toPoint();

    QStringList list = getData().cpaAttr.split(",");
    if(list.size() != 3) return;

    double lon = list.at(0).toDouble();
    double lat = list.at(1).toDouble();
    double distance = list.at(2).toDouble();

    Point2D CPApos = m_framework->LatLon2Pixel(lat, lon);
    PainterPair chk(painter);
    //划线
    painter->setPen(QPen(QColor(210,210,90),2));
    painter->drawLine(QPointF(CPApos.x, CPApos.y), pos);
    painter->drawEllipse(CPApos.x-4, CPApos.y-4,8,8);
    //添加标注
    int timer = qRound(distance/getData().sog*0.514); //节转米每秒
    int timerM = timer/60;
    int timerS = timer%60;

    QString InfoStr = QObject::tr("距离: %1米 时间: %2分%3秒")
            .arg(distance, 0, 'f', 0)
            .arg(timerM, 2, 10, QLatin1Char('0'))
            .arg(timerS, 2, 10, QLatin1Char('0'));
    painter->setFont(QFont("新宋体",8,QFont::Normal,true));
    painter->setPen(QPen(QColor(255,10,10),1));
    painter->drawText(pos.x()+10,pos.y()+10,200,20,0,InfoStr);
    painter->drawLine(pos.x()+10, pos.y()+30,pos.x()+210, pos.y()+30);
}

void AisElement::drawShipTriangle(QPainter *painter, const QColor& fillColor)
{
    if(!painter || needDrawImage()) return;
    QPointF pos = getCurrentPos();
    double angleFromNorth = m_framework->GetRotateAngle(); //计算当前正北方向的方向角
    //绘制船舶的时候黑白船名单优先
    int mark_type = getData().markType;
    //绘制船和方向线
    PainterPair chk(painter);
    MapStyle colorType = m_framework->GetMapStyle();
    if(colorType == MapStyleEcdisNight || colorType == MapStyleEcdisDayDUSK)
    {
        painter->setPen(QPen(Qt::gray,1));
    }
    else
    {
        painter->setPen(QPen(Qt::black,1));
    }

    if(mark_type > 0)
    {
        if(mark_type == 1)
        {
            painter->setBrush(QColor(0,0,0));
        }
        else if(mark_type == 2)
        {
            painter->setBrush(QColor(255,255,255));
        }
    }
    else
    {
        painter->setBrush(fillColor);
    }
    if(getIsConcern())
    {
        painter->setBrush(getConcernColor());
    }
    painter->translate(pos.x(), pos.y());
    painter->rotate((int)(m_data.cog + angleFromNorth) % 360);
    painter->translate(-pos.x(), -pos.y());
    painter->drawLine(pos.x(), pos.y(), pos.x(), pos.y() - 1 * (m_data.sog + 15));//速度等比线
    if (m_data.rot > 0)
    {
        painter->drawLine(pos.x(), pos.y() -  1 * (m_data.sog + 15), pos.x() + 10, pos.y() -  1 * (m_data.sog + 15));//方向角度线
    }
    else if (m_data.rot < 0)
    {
        painter->drawLine(pos.x(),pos.y() -  1 * (m_data.sog + 15), pos.x() - 10, pos.y() -  1 * (m_data.sog + 15));//方向角度线
    }

    //船舶没有相机，直接画三角形图标
    painter->drawPolygon(m_polygon);
    if(getData().type == 3)
    {
        double size = 0;
        double shipSpace = 0;
        if(m_polygon.size() == 3)
        {
            QPointF pos1 = m_polygon.at(1);
            QPointF pos2 = m_polygon.at(2);
            size = pos1.x()-pos2.x();
        }
        if(size >0)
        {
            //size +=1;
            shipSpace = size*2/5.0;
        }
        else
        {
            size = 2;
        }
        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(pos.x() - shipSpace, pos.y() - shipSpace, shipSpace * 2, size + shipSpace);
    }
}

void AisElement::setHistoryTrackStyle(const QString &color, const int lineWidth)
{
    m_sHistoryTrackStyle = color;
    m_iHistoryTrackWidth = lineWidth;
}

void AisElement::setPrepushTrackStyle(const QString &color, const int lineWidth)
{
    m_sPrepushTrackStyle = color;
    m_iPrepushTrackWidth = lineWidth;
}

void AisElement::drawTailTrackPolyLine(std::vector<QPointF>& pts, QPainter* painter)
{
    if(pts.size() == 0 || !painter) return;
    //画尾迹线段
    PainterPair chk(painter);
    painter->setPen(QPen(QColor(m_sHistoryTrackStyle), m_iHistoryTrackWidth, Qt::SolidLine));
    painter->drawPolyline(&pts[0], pts.size());
}

void AisElement::drawTailTrackPoint(QPainter *painter)
{
    int size = getTrackList().size();
    if(size == 0 || !painter) return;
    //画尾迹点
    PainterPair chk(painter);
    QColor color(230, 230, 230);
    painter->setPen(QPen(color, 3, Qt::SolidLine));
    qint64 PreviousTime = 0;
    for(int i = 0; i < size; i++)
    {
        ZCHX::Data::ITF_AIS AisData = mTrackList[i];
        Point2D meetPos = m_framework->LatLon2Pixel(AisData.lat,AisData.lon);
        if(i == 0)
        {
            PreviousTime = AisData.UTC;
            painter->setPen(QPen(color, 3, Qt::SolidLine));
        }
        else if(i == size - 1 && ((i + 1) % 5 > 0))
        {
            PreviousTime = mTrackList[i].UTC;
            painter->drawEllipse(meetPos.x - 4, meetPos.y - 4, 8, 8);
        }
        else
        {
            if(AisData.UTC >= PreviousTime + SHIP_INTERVAL_TIME)
            {
                if((i + 1) % 5 > 0)
                {
                    PreviousTime = AisData.UTC;
                    //painter->setPen(QPen(QColor(150,47,54),2,Qt::SolidLine));
                    painter->drawEllipse(meetPos.x - 4, meetPos.y - 4, 8, 8);
                }
                else
                {
                    PreviousTime = AisData.UTC;
                    PainterPair chk1(painter);
                    painter->setBrush(color);
                    painter->setPen(QPen(color, 3, Qt::SolidLine));
                    painter->drawEllipse(meetPos.x - 8, meetPos.y - 8, 16, 16);
                }
            }
        }
    }
}

void AisElement::drawTailTrack(QPainter *painter)
{
    if(!painter) return;
    //绘制船舶尾迹
    int numberSize = getTrackList().size();
    if(numberSize == 0) return;

    int aisIndex = getBigDisplayTrackIndex();
    std::vector<QPointF> pts;


    //转换所有的点 转换为屏幕坐标点 绘制线段
    for(int i = 0; i < numberSize; i++)
    {
        ZCHX::Data::ITF_AIS AisData = getTrackList().at(i);
        Point2D pos =  m_framework->LatLon2Pixel(AisData.lat,AisData.lon);
        pts.push_back(QPointF(pos.x, pos.y));
    }
    //画尾迹线段
    drawTailTrackPolyLine(pts, painter);
    drawTailTrackPoint(painter);
    //绘制拾取的点
    if(pts.size() > aisIndex && aisIndex >= 0)
    {
       QPointF PickPos = pts.at(aisIndex);
       QRectF RectPos;
       RectPos.setX(PickPos.x() - 10);
       RectPos.setY(PickPos.y() - 10);
       RectPos.setWidth(20);
       RectPos.setHeight(20);

       {
           PainterPair chk(painter);
           QPen pen(Qt::red,2,Qt::DotLine);
           painter->setBrush(Qt::NoBrush);
           painter->setPen(pen);
           painter->drawRect(RectPos);
       }

       {
           PainterPair chk(painter);
           QColor color(230, 230, 230);
           painter->setPen(QPen(color, 1, Qt::SolidLine));
           painter->drawRect(PickPos.x() + 30, PickPos.y() - 50, 124, 20);
           painter->drawLine(PickPos.x(), PickPos.y(), PickPos.x()+30, PickPos.y()-50);
       }

       {
           PainterPair chk(painter);
           painter->setPen(QPen(Qt::black,1,Qt::SolidLine));
           ZCHX::Data::ITF_AIS AisData1 = mTrackList.at(aisIndex);
           QDateTime ArriveTime;
           ArriveTime.setMSecsSinceEpoch(AisData1.UTC);
           QString TimeStr = ArriveTime.toString("yyyy-MM-dd,hh:mm:ss");
           QFont objFont = painter->font();

           objFont = QFont("微软雅黑",9,QFont::Normal,true);
           painter->setFont(objFont);
           painter->drawText(PickPos.x()+29,PickPos.y()-50,120,20,0,TimeStr);
       }
    }
}

void AisElement::drawElement(QPainter *painter)
{
    if(!painter)  return;

    Element::drawElement(painter);
    int scale = m_framework->Zoom();
    //填充颜色设定.默认是地图配置的颜色
    QColor fillColor = getFillingColor();
    if(getData().onlineStatusColor.isValid())
    {
        //如果用户进行了外部配置,则使用户用户的颜色
        fillColor = getData().onlineStatusColor;
    }
    //如果用户进行了关注,则关注颜色优先
    if(getIsConcern())
    {
        fillColor = getConcernColor();
    }

    if(scale < 10)
    {
        PainterPair chk(painter);
        painter->setPen(Qt::NoPen);
        painter->setBrush(fillColor);
        painter->drawRect(m_boundingRectSmall);
        return;
    }
    //CPA
    drawCPA(painter);
    //画船舶三角形
    drawShipTriangle(painter, fillColor);
    //画船舶图片显示
    drawShipImage(painter);
    //画尾迹
    drawTailTrack(painter);
}

void AisElement::drawShipImage(QPainter *painter)
{
    if(!painter)  return;
    if(!needDrawImage()) return;

    double angleFromNorth = m_framework->GetRotateAngle(); //计算当前正北方向的方向角
    QPoint pos = getCurrentPos().toPoint();
    PainterPair chk(painter);
    painter->translate(pos.x(), pos.y());
    int rotateAngle =((int)(getData().cog + angleFromNorth) % 360);
    painter->rotate(rotateAngle);
    painter->translate(-pos.x() , -pos.y());
    painter->drawPixmap(m_boundingRect.toRect(), getShipImage());
    painter->rotate(-rotateAngle);
}



void AisElement::drawTargetInformation(int mode, QPainter *painter)
{
    if(!painter || !m_drawTargetInfo)
        return;

    Element::drawElement(painter);
    double angleFromNorth = m_framework->GetRotateAngle(); //计算当前正北方向的方向角
    QPointF pos = getCurrentPos();
    int scale = m_framework->Zoom();

    if(scale < 10)
    {
        return;
    }
    //绘制船和方向线
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QColor(255,127,39));//橙色
    QString Information ;
    ZCHX::Data::ITF_AIS AisData = getData();
    int sizeNumer = 0;
    if(!(mode & SHIP_ITEM_LABEL)) return;

    QString warnName = ZCHX::Utils::getWarnName(AisData.warn_status);
    QStringList InfoList;
    if(mode & SHIP_ITEM_NAME) InfoList.append(AisData.shipName);
    if(mode & SHIP_ITEM_MMSI) InfoList.append(QString::number(AisData.mmsi));
    if(mode & SHIP_ITEM_LATLON) {
        QString geostr = QString("%1, %2")
                .arg(ZCHX::Utils::instance()->latLonToString(AisData.lon, false))
                .arg(ZCHX::Utils::instance()->latLonToString(AisData.lat, true));
        InfoList.append(geostr);
    }
    if(mode & SHIP_ITEM::SHIP_ITEM_SOG) InfoList.append(QString("%1 Kn").arg(AisData.sog));
    if(mode & SHIP_ITEM::SHIP_ITEM_COG) InfoList.append(QString("%1 °").arg(AisData.cog));
    sizeNumer = InfoList.size();

    if(mode & SHIP_ITEM::SHIP_ITEM_MULTILINE) //多行
    {
        Information = InfoList.join("\n");
        QFont font("微软雅黑", 9, QFont::Normal, false);//微软雅黑 新宋体
        painter->setFont(font);
        QRect rectangle = QRect(pos.x() - 150, pos.y() - 25 * sizeNumer, 300, 25 * sizeNumer);
        painter->drawText(rectangle, Qt::AlignCenter,Information);

        if(!warnName.isEmpty() && AisData.warn_status != ZCHX::Data::WARN_CHANNEL_COLLISION)
        {
            QPixmap devicePix;
            devicePix.load(":/navig64/warning.png");
            painter->drawPixmap(pos.x() - 50 - devicePix.width() / 2, pos.y() - 25 * sizeNumer + 5 - devicePix.height() / 2,
                                devicePix.width(), devicePix.height(), devicePix);

            painter->setPen(QPen(Qt::red, 1));
            QRect rectangle = QRect(pos.x() - 150, pos.y() - 25 * sizeNumer - 5, 300, 25);
            painter->drawText(rectangle, Qt::AlignCenter, warnName);
        }
    } else {
        Information = InfoList.join(" ");
        QFont font("微软雅黑", 9, QFont::Normal, false);////微软雅黑 新宋体
        painter->setFont(font);
        QRect rectangle = QRect(pos.x()-(150*sizeNumer), pos.y()-30, 300*sizeNumer, 30);
        painter->drawText(rectangle, Qt::AlignCenter, Information);

        if(!warnName.isEmpty() && AisData.warn_status != ZCHX::Data::WARN_CHANNEL_COLLISION)
        {
            QPixmap devicePix;
            devicePix.load(":/navig64/warning.png");
            painter->drawPixmap(pos.x() - 50 - devicePix.width() / 2, pos.y() - 40 - devicePix.height() / 2,
                                devicePix.width(), devicePix.height(), devicePix);

            painter->setPen(QPen(Qt::red, 1));
            QRect rectangle = QRect(pos.x() - 150, pos.y() - 50, 300, 30);
            painter->drawText(rectangle, Qt::AlignCenter, warnName);
        }
    }

}

void AisElement::drawCollide(const ZCHX::Data::ITF_AIS &targetAis, QPainter *painter)
{
    ZCHX::Data::ITF_AIS AisData = getData();
    QString warnName = ZCHX::Utils::getWarnName(AisData.warn_status);

    // 航道碰撞预警
    if(!warnName.isEmpty() && AisData.warn_status == ZCHX::Data::WARN_CHANNEL_COLLISION)
    {
        QPointF pos = getCurrentPos();

        PainterPair chk(painter);
        Point2D objCollideD = m_framework->LatLon2Pixel(AisData.objCollide.lat, AisData.objCollide.lon);
        QPointF objCollidePos = QPointF(objCollideD.x, objCollideD.y);

        Point2D targetD = m_framework->LatLon2Pixel(targetAis.lat, targetAis.lon);
        QPointF targetPos = QPointF(targetD.x, targetD.y);

        painter->setPen(QPen(Qt::red, 1));
        painter->drawLine(pos, objCollidePos);
        painter->drawLine(targetPos, objCollidePos);
        painter->drawText(objCollidePos.x(), objCollidePos.y(), warnName);

        QPixmap devicePix;
        devicePix.load(":/navig64/warning.png");
        painter->drawPixmap(objCollidePos.x() - 20 - devicePix.width() / 2, objCollidePos.y() - 10 - devicePix.height() / 2,
                            devicePix.width(), devicePix.height(), devicePix);

        float rotateAngle = atan2(objCollidePos.y() - pos.y(), objCollidePos.x() - pos.x()) / 3.1415926 * 180 + 180;
        painter->translate(objCollidePos.x(), objCollidePos.y());
        painter->rotate(rotateAngle);
        painter->drawText(20, -3, QString("%1米 %2分钟").arg(AisData.objCollide.disrance).arg(AisData.objCollide.collideTime));
        painter->rotate(-rotateAngle);
        painter->translate(-objCollidePos.x(), -objCollidePos.y());
    }
}

bool AisElement::isEmpty() const
{
    return m_data.getPath().empty();
}

void AisElement::setDrawTargetInfo(bool val)
{
    m_drawTargetInfo = val;
}

//bool AisElement::getFleet()
//{
//    return m_isFleet;
//}

//void AisElement::setFleet(bool val)
//{
//    m_isFleet = val;
//}

QPixmap AisElement::getShipImage()
{
    QString path;

    //首先使用设置的ais的图标
    if(m_data.icon_number > 0)
    {
        m_shipImage = ZCHX::Utils::getAisImage(m_data.icon_number);
        return m_shipImage;
    }

    //执法船
    if (m_data.cargoType == 55)
    {
        path = ":/element/lawship";
        if(m_data.status == 1)
            path.append("_error.png");
        else if(m_data.warn_status > 0)
            path.append("_bug.png");
        else if(m_data.status == 0)
            path.append("_normal.png");
    }
    else if(m_data.is_fleet) //船队
    {
        path = ":/element/boat1.png";
    }
    else //常规船只
    {
        path = ":/element/boat.png";
    }    

    m_shipImage = ZCHX::Utils::getImage(path, Qt::green, m_framework->Zoom());
    return m_shipImage;
}

QPixmap AisElement::getCameraImage()
{
    QString path = ":/element/camera";
    if(m_data.status == 1)
        path.append("_error.png");
    else if(m_data.warn_status > 0)
        path.append("_bug.png");
    else if(m_data.status == 0)
        path.append("_normal.png");
    m_cameraImage = ZCHX::Utils::getImage(path, Qt::green, m_framework->Zoom());
    return m_cameraImage;
}

std::vector<QPointF> AisElement::getTrack()
{
    return Element::convert2QtPonitList(m_data.getPath());
}

std::vector<QPointF> AisElement::getTouchdown()
{
    return Element::convert2QtPonitList(m_data.getTouchdown());
}

void AisElement::updateGeometry(QPointF pos, int size)
{
    m_polygon = QPolygonF();

    if(needDrawImage() )
    {
        getShipImage();
        getCameraImage();
        m_cameraPos = QPointF(pos.x() - m_cameraImage.width()/2., pos.y() + m_cameraImage.height()/2.);
        m_boundingRect = QRectF(0,0,m_shipImage.width(),m_shipImage.height());
        m_boundingRect.moveCenter(pos);
    }
    else
    {
        qreal half = size / 2.0;
        m_polygon << QPointF(pos.x() - half ,pos.y() + half)
             << QPointF(pos.x() + half, pos.y() + half )
             << QPointF(pos.x(), pos.y() - (half + 10));

        m_boundingRect =  m_polygon.boundingRect().normalized();
    }

    Element::updateGeometry(pos, size);
}

bool AisElement::needDrawImage() const
{
    return (hasCamera() || (m_data.cargoType == 55) || m_forceImage || getData().is_fleet);
}

std::vector<ZCHX::Data::ITF_CameraDev> AisElement::getCameraData() const
{
    return m_cameraDev;
}

void AisElement::initFromSettings()
{
    //初始化Ais颜色设定,默认设定添加
    mTextColor.setNamedColor(zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_TEXT_COLOR, QColor(Qt::black).name()).toString());
    mBorderColor.setNamedColor(zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_BORDER_COLOR, QColor(Qt::green).name()).toString());
    mFillingColor.setNamedColor(zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_FILL_COLOR, QColor(Qt::green).name()).toString());
    mConcernColor.setNamedColor(zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_CONCERN_COLOR, QColor(Qt::red).name()).toString());
    setForceImage(zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_FORCED_IMAGE, false).toBool());
    Element::initFromSettings();

    //qDebug()<<"ais ini seetings."<<getForceImage()<<mFillingColor.name()<<mTextColor.name()<<mConcernColor.name()<<mBorderColor.name();
}

//namespace end
}
