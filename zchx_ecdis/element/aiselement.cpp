#include "aiselement.hpp"
#include "zchxmapframe.h"
#include "zchxmapwidget.h"
#include "zchxMapDatautils.h"
#include "profiles.h"
#include "zchxutils.hpp"
#include "map_layer/zchxmaplayermgr.h"
#include <QDebug>

#define     SHIP_INTERVAL_TIME  60 * 1000 // 1分钟

using namespace qt;

AisHistoryElement::AisHistoryElement(const ZCHX::Data::ITF_AIS_TRACK &ele, zchxMapWidget *w)
    :FixElement<ZCHX::Data::ITF_AIS_TRACK>(ele, ZCHX::Data::ELE_AIS_HISTORY_TRACK, ZCHX::LAYER_HISTORY_AIS, w)
{
    mBigDisplayHistoryIndex = -1;
    m_sHistoryTrackStyle = QColor(Qt::yellow).name();
    m_iHistoryTrackWidth = 1;

}

void AisHistoryElement::setHistoryTrackStyle(const QString &color, const int lineWidth)
{
    m_sHistoryTrackStyle = color;
    m_iHistoryTrackWidth = lineWidth;
}

void AisHistoryElement::drawHistoryTrackPolyLine(std::vector<QPointF>& pts, QPainter* painter)
{
    if(pts.size() == 0 || !painter) return;
    //画尾迹线段
    PainterPair chk(painter);
    painter->setPen(QPen(QColor(m_sHistoryTrackStyle), m_iHistoryTrackWidth, Qt::SolidLine));
    painter->drawPolyline(&pts[0], pts.size());
}

void AisHistoryElement::drawHistoryTrackPoint(QPainter *painter)
{
    int size = m_data.size();
    if(size == 0 || !painter) return;
    //画尾迹点
    PainterPair chk(painter);
    QColor color(230, 230, 230);
    painter->setPen(QPen(color, 3, Qt::SolidLine));
    qint64 PreviousTime = 0;
    for(int i = 0; i < size; i++)
    {
        ZCHX::Data::ITF_AIS AisData = m_data[i];
        ZCHX::Data::Point2D meetPos = mView->framework()->LatLon2Pixel(AisData.getLat(),AisData.getLon());
        if(i == 0)
        {
            PreviousTime = AisData.UTC;
            painter->setPen(QPen(color, 3, Qt::SolidLine));
        }
        else if(i == size - 1 && ((i + 1) % 5 > 0))
        {
            PreviousTime = m_data[i].UTC;
            painter->drawEllipse(meetPos.x - 4, meetPos.y - 4, 8, 8);
        }
        else
        {
            if(AisData.UTC >= PreviousTime + SHIP_INTERVAL_TIME)
            {
                if((i + 1) % 5 > 0)
                {
                    PreviousTime = AisData.UTC;
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

void AisHistoryElement::copyDataFromOther(std::shared_ptr<Element> other)
{
    if(!other) return;
    AisHistoryElement *src = dynamic_cast<AisHistoryElement*>(other.get());
    if(src)
    {
        this->setData(src->data());
    }
}

bool AisHistoryElement::contains(const QPoint &pt)
{
    //检查轨迹是否选中
    int size = m_data.size();
    for(int i = 0; i < size; ++i)
    {
        ZCHX::Data::ITF_AIS ais = m_data[i];
        ZCHX::Data::Point2D pos =  mView->framework()->LatLon2Pixel(ais.getLat(),ais.getLon());
        QRect rect(0, 0, 16, 16);
        rect.moveCenter(pos.toPoint());
        if(rect.contains(pt))
        {
            mBigDisplayHistoryIndex = i;
            return true;
        }
    }
    return false;
}

void AisHistoryElement::drawActive(QPainter *painter)
{
    if(!getIsActive() || !painter) return;
    if( mBigDisplayHistoryIndex >= 0 &&  mBigDisplayHistoryIndex < m_data.size())
    {
        ZCHX::Data::ITF_AIS data = m_data[mBigDisplayHistoryIndex];
        QPointF PickPos = framework()->LatLon2Pixel(data.getLat(), data.getLon()).toPointF();
        {
            QRectF RectPos(0, 0, 20, 20);
            RectPos.moveCenter(PickPos);
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
            QDateTime ArriveTime;
            ArriveTime.setMSecsSinceEpoch(data.UTC);
            QString TimeStr = ArriveTime.toString("yyyy-MM-dd,hh:mm:ss");
            QFont objFont = painter->font();

            objFont = QFont("微软雅黑",9,QFont::Normal,true);
            painter->setFont(objFont);
            painter->drawText(PickPos.x()+29,PickPos.y()-50,120,20,0,TimeStr);
        }
    }
}

void AisHistoryElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    int size = m_data.size();
    if(size == 0) return;
    std::vector<QPointF> pts;


    //转换所有的点 转换为屏幕坐标点 绘制线段
    for(int i = 0; i < size; i++)
    {
        ZCHX::Data::ITF_AIS AisData = m_data.at(i);
        ZCHX::Data::Point2D pos =  mView->framework()->LatLon2Pixel(AisData.lat,AisData.lon);
        pts.push_back(QPointF(pos.x, pos.y));
    }
    //画尾迹线段
    drawHistoryTrackPolyLine(pts, painter);
    drawHistoryTrackPoint(painter);
    //绘制拾取的点
    drawActive(painter);
}

AisElement::AisElement(const ZCHX::Data::ITF_AIS &ele, zchxMapWidget* w)
    : FixElement<ZCHX::Data::ITF_AIS>(ele, ZCHX::Data::ELE_AIS, ZCHX::LAYER_AIS, w)
{
    mLabelDisplayMode = SHIP_ITEM::SHIP_ITEM_DEFAULT;
    mRealtimeTailTrackList.clear();
    m_drawTargetInfo = true;
    updateFlashRegionColor(ele.warnStatusColor);
    initFromSettings();
    setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
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

void AisElement::setData(const ZCHX::Data::ITF_AIS &data)
{
    FixElement::setData(data);
    setID(m_data.id);
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

void AisElement::appendCamera(const ZCHX::Data::ITF_CameraDev &data)
{
    m_cameraDev.push_back(data);
}

void AisElement::drawCPA(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    if(m_data.cpaAttr.isEmpty()) return;
    QPoint pos = getCurrentPos().toPoint();

    QStringList list = m_data.cpaAttr.split(",");
    if(list.size() != 3) return;

    double lon = list.at(0).toDouble();
    double lat = list.at(1).toDouble();
    double distance = list.at(2).toDouble();

    ZCHX::Data::Point2D CPApos = this->framework()->LatLon2Pixel(lat, lon);
    PainterPair chk(painter);
    //划线
    painter->setPen(QPen(QColor(210,210,90),2));
    painter->drawLine(QPointF(CPApos.x, CPApos.y), pos);
    painter->drawEllipse(CPApos.x-4, CPApos.y-4,8,8);
    //添加标注
    int timer = qRound(distance/m_data.sog*0.514); //节转米每秒
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
    if(!isDrawAvailable(painter) || needDrawImage()) return;
    QPointF pos = getCurrentPos();
    double angleFromNorth = this->framework()->GetRotateAngle(); //计算当前正北方向的方向角
    //绘制船舶的时候黑白船名单优先
    int mark_type = m_data.markType;
    //绘制船和方向线
    PainterPair chk(painter);
    MapStyle colorType = this->framework()->GetMapStyle();
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
    if(m_data.type == 3)
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


void AisElement::setPrepushTrackStyle(const QString &color, const int lineWidth)
{
    m_sPrepushTrackStyle = color;
    m_iPrepushTrackWidth = lineWidth;
}

void AisElement::setHistoryTrackStyle(const QString& style,  int lineWidth)
{
    //取得历史轨迹子图元
    std::list<std::shared_ptr<Element>> children = getChildren(ZCHX::Data::ELE_AIS_HISTORY_TRACK);
    foreach (std::shared_ptr<Element> ele, children) {
        AisHistoryElement *item = dynamic_cast<AisHistoryElement*>(ele.get());
        if(item) item->setHistoryTrackStyle(style, lineWidth);
    }
}

void AisElement::drawRealtimeTailTrack(QPainter *painter)
{
    if(!painter || !getIsRealtimeTailTrack()) return;
    //绘制船舶尾迹
    if(mRealtimeTailTrackList.size() == 0) return;
    qint64 TimeValue=0;
    int scale = mView->framework()->GetDrawScale();
    QPainterPath path;
    for(int j =0; j <mRealtimeTailTrackList.size(); ++j)
    {
        ZCHX::Data::ITF_AIS ItemData = mRealtimeTailTrackList.at(j);
        ZCHX::Data::Point2D curPos = mView->framework()->LatLon2Pixel(ZCHX::Data::LatLon(ItemData.lat, ItemData.lon));
        if(j == 0)
        {
            path.moveTo(curPos.x,curPos.y);
            //第一个点
            {
                TimeValue = ItemData.UTC;
                {
                    PainterPair chk(painter);
                    painter->setPen(QPen(Qt::red,2,Qt::DotLine));
                    painter->drawPoint(curPos.x, curPos.y);
                }
                if(scale >13)
                {
                    PainterPair chk(painter);;
                    painter->setPen(QPen(QColor(150,47,54),1,Qt::SolidLine));
                    painter->drawRect(curPos.x+28, curPos.y-5,124,20);
                    painter->drawLine(curPos.x, curPos.y, curPos.x+28, curPos.y+9);
                    QDateTime DateTime;
                    DateTime.setMSecsSinceEpoch(ItemData.UTC);
                    QString TimeStr = DateTime.toString("yyyy-MM-dd hh:mm:ss");
                    QFont objFont = painter->font();
                    objFont = QFont("微软雅黑",9,QFont::Normal,true);
                    painter->setFont(objFont);
                    painter->drawText(curPos.x+30,curPos.y-3,120,20,0,TimeStr);
                }
            }
        } else {
            path.lineTo(curPos.x,curPos.y);
            if(ItemData.UTC - TimeValue >= 60000)
            {
                TimeValue = ItemData.UTC;
                {
                    PainterPair chk(painter);
                    painter->setPen(QPen(Qt::red,4,Qt::DotLine));
                    painter->drawPoint(curPos.x, curPos.y);
                }

                if(scale >13)
                {
                    PainterPair chk(painter);
                    painter->setPen(QPen(QColor(150,47,54),1,Qt::SolidLine));
                    painter->drawRect(curPos.x+28, curPos.y-5,124,20);
                    painter->drawLine(curPos.x, curPos.y, curPos.x+28, curPos.y+9);
                    QDateTime DateTime;
                    DateTime.setMSecsSinceEpoch(ItemData.UTC);
                    QString TimeStr = DateTime.toString("yyyy-MM-dd hh:mm:ss");
                    QFont objFont = painter->font();
                    objFont = QFont("微软雅黑",9,QFont::Normal,true);
                    painter->setFont(objFont);
                    painter->drawText(curPos.x+30,curPos.y-3,120,20,0,TimeStr);
                }
            }
        }

    }
    PainterPair chk(painter);
    QPen objPen;
    if(getIsActive())
    {
        objPen.setColor(QColor(255,0,0,100));
    }
    else
    {
        objPen.setColor(QColor(0,0,0,100));
    }

    objPen.setWidth(1);
    painter->setPen(objPen);
    painter->drawPath(path);
}

void AisElement::drawExtrapolation(QPainter *painter)
{
    if(!isDrawAvailable(painter) || !getIsExtrapolate() || getExtrapolateTime() == 0) return;
    //绘制外推点
    ZCHX::Data::Point2D  curPos = framework()->LatLon2Pixel(m_data.getLat(),m_data.getLon());

    //第二点
    double lat = 0.0, lon = 0.0;
    double Speed = data().sog * 1852 /3600.0;
    double Distance = getExtrapolateTime() * 60 * Speed;
    ZCHX::Utils::distbear_to_latlon(data().lat,data().lon,Distance,data().cog, lat, lon);
    ZCHX::Data::Point2D nextPos = mView->framework()->LatLon2Pixel(lat, lon);
    PainterPair chk(painter);
    //画点
    painter->setPen(QPen(QColor(m_sPrepushTrackStyle),m_iPrepushTrackWidth,Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(nextPos.x-3,nextPos.y-3,6,6);

    //线
    painter->setPen(QPen(QColor(m_sPrepushTrackStyle),m_iPrepushTrackWidth,Qt::DotLine));
    painter->drawLine(curPos.x, curPos.y, nextPos.x, nextPos.y);
}

void AisElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter))  return;
    Element::drawElement(painter);
    QPointF pos = getCurrentPos();
    if(!mView->rect().contains(pos.toPoint())) return;
    updateGeometry(pos, getDrawScaleSize());
    //根据船舶的类型进行不同的显示
    ZCHX::Data::ITF_AIS::TargetType target_type = m_data.type;
    if(target_type != ZCHX::Data::ITF_AIS::Target_AIS) return;
    int cargo_type = m_data.cargoType;
    if(cargo_type == ZCHX::Data::ITF_AIS::Cargo_LAW || cargo_type == ZCHX::Data::ITF_AIS::Cargo_Construction)
    {
        //执法船或者施工船
        setForceImage(true);
    }
    //预警输出
    drawFlashRegion(painter, pos, m_data.warn_status, m_data.warnStatusColor);
    //目标本身
    int scale = framework()->Zoom();
    //填充颜色设定.默认是地图配置的颜色
    QColor fillColor = getFillingColor();
    //如果用户进行了外部配置,则使用户用户的颜色
    if(m_data.onlineStatusColor.isValid())  fillColor = m_data.onlineStatusColor;
    //如果用户进行了关注,则关注颜色优先
    if(getIsConcern()) fillColor = getConcernColor();
    if(scale < 10)
    {
        //直接画小矩形
        PainterPair chk(painter);
        painter->setPen(Qt::NoPen);
        painter->setBrush(fillColor);
        painter->drawRect(m_boundingRectSmall);
        return;
    }
    //开始具体化图形
    //CPA
    drawCPA(painter);
    //画船舶三角形
    if(!needDrawImage()){
        drawShipTriangle(painter, fillColor);
    } else {
        drawShipImage(painter);
    }
    //画实时尾迹
    drawRealtimeTailTrack(painter);
    //画预推点
    drawExtrapolation(painter);
    //显示框选
    drawActive(painter);
    //显示聚焦
    drawFocus(painter);
    //显示文本
    drawTargetInformation(painter);
    //显示碰撞信息
    drawCollide(painter);

    //绘制交汇
    drawMeet(painter);
}

void AisElement::drawMeet(QPainter *painter)
{
    if((!isDrawAvailable(painter)) || m_data.RadarMeetVec.size() == 0 || !mView->getIsOpenMeet()) return;

    PainterPair chk2(painter);
    QPen pen(Qt::red,2,Qt::DashLine);
    painter->setPen(pen);
    uint time_hour = 0;
    uint time_minute = 0;
    uint time_second = 0;
    QPoint pos = getViewPos().toPoint();
    for(int j = 0; j < m_data.RadarMeetVec.size(); j++)
    {
        ZCHX::Data::RadarMeet meetItem = m_data.RadarMeetVec.at(j);
        ZCHX::Data::Point2D meetPos = framework()->LatLon2Pixel(meetItem.lat, meetItem.lon);
        time_hour = meetItem.UTC / 3600;
        time_minute = meetItem.UTC / 60 - time_hour * 60;
        time_second = meetItem.UTC % 60;
        QString str = tr("Time ")+QString::number(time_hour)+tr("H ") + QString::number(time_minute)+ tr("M ")+ \
                QString::number(time_second)+tr("S; Distance: ")+QString::number(meetItem.disrance,'f',3)+"m";

        painter->drawLine(pos.x(),pos.y(),meetPos.x,meetPos.y);
        painter->drawText(pos.x()-10,pos.y()-5, str);
    }
}

void AisElement::drawShipImage(QPainter *painter)
{
    if(!painter)  return;
    if(!needDrawImage()) return;

    double angleFromNorth = mView->framework()->GetRotateAngle(); //计算当前正北方向的方向角
    QPoint pos = getCurrentPos().toPoint();
    PainterPair chk(painter);
    painter->translate(pos.x(), pos.y());
    int rotateAngle =((int)(data().cog + angleFromNorth) % 360);
    painter->rotate(rotateAngle);
    painter->translate(-pos.x() , -pos.y());
    painter->drawPixmap(m_boundingRect.toRect(), getShipImage());
    painter->rotate(-rotateAngle);
}



void AisElement::drawTargetInformation(QPainter *painter)
{
    if(!painter || !m_drawTargetInfo) return;
    int mode = mLabelDisplayMode;
    double angleFromNorth = mView->framework()->GetRotateAngle(); //计算当前正北方向的方向角
    QPointF pos = getCurrentPos();
    int scale = mView->framework()->Zoom();

    if(scale < 10)
    {
        return;
    }
    //绘制船和方向线
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QColor(255,127,39));//橙色
    QString Information ;
    ZCHX::Data::ITF_AIS AisData = data();
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

void AisElement::drawCollide(QPainter *painter)
{
    if(mCollideAis.id.isEmpty()) return;
    QString warnName = ZCHX::Utils::getWarnName(m_data.warn_status);

    // 航道碰撞预警
    if(!warnName.isEmpty() && m_data.warn_status == ZCHX::Data::WARN_CHANNEL_COLLISION)
    {
        QPointF pos = getCurrentPos();
        PainterPair chk(painter);
        QPointF objCollidePos = framework()->LatLon2Pixel(m_data.objCollide.lat, m_data.objCollide.lon).toPointF();
        QPointF targetPos = framework()->LatLon2Pixel(mCollideAis.getLat(), mCollideAis.getLon()).toPointF();

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
        painter->drawText(20, -3, QString("%1米 %2分钟").arg(m_data.objCollide.disrance).arg(m_data.objCollide.collideTime));
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

    m_shipImage = ZCHX::Utils::getImage(path, Qt::green, mView->framework()->Zoom());
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
    m_cameraImage = ZCHX::Utils::getImage(path, Qt::green, mView->framework()->Zoom());
    return m_cameraImage;
}

std::vector<QPointF> AisElement::getTrack()
{
    return mView->framework()->convert2QtPonitList(m_data.getPath());
}

std::vector<QPointF> AisElement::getTouchdown()
{
    return mView->framework()->convert2QtPonitList(m_data.getTouchdown());
}

void AisElement::updateGeometry(QPointF pos, qreal size)
{
    m_polygon = QPolygonF();
    if(needDrawImage() )
    {
        getShipImage();
        getCameraImage();
        m_cameraPos = QPointF(pos.x() - m_cameraImage.width()/2., pos.y() + m_cameraImage.height()/2.);
        updateBouningRect(pos, m_shipImage.width(), m_shipImage.height());
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
    return (hasCamera() || (m_data.cargoType == 55) || isForceImage || m_data.is_fleet);
}

std::vector<ZCHX::Data::ITF_CameraDev> AisElement::getCameraData() const
{
    return m_cameraDev;
}

void AisElement::initFromSettings()
{
    //初始化Ais颜色设定,默认设定添加
    mTextColor.setNamedColor(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_TEXT_COLOR, QColor(Qt::black).name()).toString());
    mBorderColor.setNamedColor(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_BORDER_COLOR, QColor(Qt::green).name()).toString());
    mFillingColor.setNamedColor(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_FILL_COLOR, QColor(Qt::green).name()).toString());
    mConcernColor.setNamedColor(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_CONCERN_COLOR, QColor(Qt::red).name()).toString());
    setForceImage(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_FORCED_IMAGE, false).toBool());
    Element::initFromSettings();

    //qDebug()<<"ais ini seetings."<<getForceImage()<<mFillingColor.name()<<mTextColor.name()<<mConcernColor.name()<<mBorderColor.name();
}

void AisElement::clicked(bool isDouble)
{
    if(!mView) return;
    if(isDouble)
    {
        mView->signalIsDoubleClicked4Ais(data());
    } else
    {
        mView->signalIsSelected4Ais(data(), false, false);
    }
}

void AisElement::showToolTip(const QPoint& pos)
{
    ZCHX::Data::ITF_AIS info = data();

    QString base_text = QObject::tr("船名: ")+info.shipName+"\n";
    base_text += QObject::tr("呼号: ")+info.callSign+"\n";
    base_text += QObject::tr("MMSI: ")+QString::number(info.mmsi)+"\n";
    base_text += QObject::tr("IMO: ")+QString::number(info.imo)+"\n";
    base_text += QObject::tr("船长: ")+QString::number(info.shipLength)+"\n";
    base_text += QObject::tr("船宽: ")+QString::number(info.shipWidth)+"\n";
    base_text += QObject::tr("吃水: ")+QString::number(info.draught)+"\n";
    base_text += QObject::tr("经度: ")+FLOAT_STRING(info.getLon(), 6)+"\n";
    base_text += QObject::tr("纬度: ")+FLOAT_STRING(info.getLat(), 6)+"\n";
    base_text += QObject::tr("船首向: ")+QString::number(info.heading)+"\n";
    base_text += QObject::tr("航向: ")+QString::number(info.cog)+"\n";
    base_text += QObject::tr("船速: ")+QString::number(info.sog)+"\n";
    base_text += QObject::tr("目的地: ")+info.dest+"\n";
    base_text += QObject::tr("到达时间: ")+info.eta+"\n";
    base_text += QObject::tr("最新时间: ")+QDateTime::fromTime_t(info.UTC / 1000).toString("MM/dd/yyyy HH:mm:ss");
    QToolTip::showText(pos,base_text);
}


QList<QAction*> AisElement::getRightMenuAction()
{
    QList<QAction*> list;
    //获取当前选择的目标对象
    if(this->getIsActive())
    {
        if(hasCamera())
        {
            list.append(addAction(tr("相机列表"),this, SLOT(slotOpenCameraList())));
        }
        //list.append(addAction(tr("画中画"),this, SLOT(setPictureInPicture())));
        //list.append(addAction(tr("船队"),this, SLOT(setFleet())));
        list.append(addAction(tr("模拟外推"),this, SLOT(slotSetSimulationExtrapolation())));
        list.append(addAction(tr("历史轨迹"),this, SLOT(slotSetHistoryTraces())));
        list.append(addAction(tr("实时轨迹"),this, SLOT(slotSetRealTimeTraces())));
        list.append(addAction(tr("关注"),this, SLOT(slotSetConcern())));
        list.append(addAction(tr("黑名单"),this, SLOT(slotSetBlackList())));
        list.append(addAction(tr("白名单"),this, SLOT(slotSetWhiteList())));
        //list.append(addAction(tr("CPA跟踪"),this, SLOT(setCPATrack())));
        list.append(addAction(tr("联动"),this, SLOT(slotInvokeLinkageSpot())));
    }
    return list;
}

void AisElement::slotOpenCameraList()
{

}
void AisElement::slotSetFleet()
{
    if(mView)
    {
        emit mView->signalAddFleet(data());
    }
}

void AisElement::slotSetBlackList()
{
    if(mView) mView->signalCreateBlackOrWhiteList(getID(), ZCHX::Data::SHIP_BW_BLACK);
}

void AisElement::slotSetWhiteList()
{
    if(mView) mView->signalCreateBlackOrWhiteList(getID(), ZCHX::Data::SHIP_BW_WHITE);
}

void AisElement::slotSetCPATrack()
{
    if(mView) mView->signalCreateCPATrack(getID());
}

void AisElement::copyDataFromOther(std::shared_ptr<Element> other)
{
    if(!other) return;
    AisElement* src = dynamic_cast<AisElement*>(other.get());
    if(src)
    {
        this->setData(src->data());
    }
}

