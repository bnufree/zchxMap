#include "radarelement.h"
#include "zchxmapframe.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
#include "map_layer/zchxMapLayer.h"


namespace qt
{

RadarAreaElement::RadarAreaElement(const ZCHX::Data::ITF_RadarArea &ele, zchxMapWidget* v)
    :FixElement<ZCHX::Data::ITF_RadarArea>(ele, ZCHX::Data::ELE_RADAR_AREA, ZCHX::LAYER_RADAR_AREA, v)
{
}

double RadarAreaElement::radarX() const
{
    return m_data.radarX;
}

void RadarAreaElement::setRadarX(double radarX)
{
    m_data.radarX = radarX;
}

double RadarAreaElement::radarY() const
{
    return m_data.radarY;
}

void RadarAreaElement::setRadarY(double radarY)
{
    m_data.radarY = radarY;
}

int RadarAreaElement::centerLineAngel() const
{
    return m_data.centerLineAngel;
}

void RadarAreaElement::setCenterLineAngel(int centerLineAngel)
{
    m_data.centerLineAngel = centerLineAngel;
}

int RadarAreaElement::radius() const
{
    return m_data.radius;
}

void RadarAreaElement::setRadius(int radius)
{
    m_data.radius = radius;
}

int RadarAreaElement::maxScanRangeANgle() const
{
    return m_data.maxScanRangeANgle;
}

void RadarAreaElement::setMaxScanRangeANgle(int maxScanRangeANgle)
{
    m_data.maxScanRangeANgle = maxScanRangeANgle;
}

int RadarAreaElement::numberofChannele() const
{
    return m_data.numberofChannele;
}

void RadarAreaElement::setNumberofChannele(int numberofChannele)
{
    m_data.numberofChannele = numberofChannele;
}

int RadarAreaElement::maxWakePointsNumber() const
{
    return m_data.maxWakePointsNumber;
}

void RadarAreaElement::setMaxWakePointsNumber(int maxWakePointsNumber)
{
    m_data.maxWakePointsNumber = maxWakePointsNumber;
}

void RadarAreaElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    PainterPair chk(painter);

    int scale = getDrawScaleSize();
    painter->setBrush(QBrush(QColor(0,255,0,125)));
    double angleFromNorth = framework()->GetRotateAngle();
    double startAng = centerLineAngel() - maxScanRangeANgle() / 2;
    double lenAng =   maxScanRangeANgle();

    QPointF centerPos = getCurrentPos();

    //通过推算经纬度计算半径
    ZCHX::Data::LatLon drll(0, 0);
    ZCHX::Utils::distbear_to_latlon(radarY(), radarX(), radius() * 1000, -90 + startAng, drll.lat, drll.lon);
    QPointF drPos = framework()->LatLon2Pixel(drll).toPointF();
    double dr = sqrt(abs(drPos.y() - centerPos.y()) * abs(drPos.y()-centerPos.y()) + abs(drPos.x() - centerPos.x()) * abs(drPos.x() - centerPos.x()));

    QRectF rectangle(centerPos.x()-dr,centerPos.y()-dr, dr*2, dr*2);
    int startAngle = (90 - startAng - angleFromNorth)* 16;
    int spanAngle = (-lenAng) * 16;
    painter->drawPie(rectangle, startAngle, spanAngle);
    //绘制刻度线
    int ruleAngle = maxScanRangeANgle()  / numberofChannele();
    for(int i=0; i<= numberofChannele(); ++i)
    {
        int angle = ruleAngle * i;
        QLineF line = QLineF::fromPolar(dr,startAngle / 16 - angle);
        line.translate(centerPos.x(),centerPos.y());
        painter->drawLine(line);
        if(scale > 5)
        {
            painter->drawText(line.p2(),QString("%1°").arg(angle - maxScanRangeANgle() / 2));
        }
    }

    //draw rule arc
    double arc_ratio =  dr / radius();
    int  stepNumber = radius() / maxWakePointsNumber();
    for(int i=0; i<stepNumber; ++i)
    {
        if(i==0)
        {
            continue;
        }
        int dis = arc_ratio * i *maxWakePointsNumber();
        QRectF rect(centerPos.x()-dis, centerPos.y()-dis,dis*2,dis*2);
        painter->drawArc(rect,startAngle ,spanAngle);
        QLineF line = QLineF::fromPolar(dis,(startAngle + spanAngle / 2) / 16);
        line.translate(centerPos.x(),centerPos.y());
        if(scale > 5)
        {
            painter->drawText(line.p2(),QString("%1km").arg(i * maxWakePointsNumber()));
        }
    }
    mShapePnts = getShapePnts(angleFromNorth);
    //是否选择
    if(getIsActive())
    {
        PainterPair chk(painter);
        QPen pen(Qt::red,2,Qt::DashDotLine); //选中使用点划线
        painter->setBrush(Qt::NoBrush);
        painter->setPen(pen);
        painter->drawPolygon(mShapePnts);
    }
}

QPolygonF RadarAreaElement::getShapePnts(double angleFromNorth) const
{
    QPolygonF polygon;
    if(framework())
    {
        double startAng = this->centerLineAngel() - this->maxScanRangeANgle() / 2;
        double lenAng =   this->maxScanRangeANgle();
        ZCHX::Data::Point2D centerPos = framework()->LatLon2Pixel(this->radarY(),this->radarX());

        //通过推算经纬度计算半径
        ZCHX::Data::LatLon drll(0, 0);
        ZCHX::Utils::distbear_to_latlon(this->radarY(),this->radarX(),this->radius() * 1000,-90 + startAng, drll.lat, drll.lon);
        ZCHX::Data::Point2D drPos = framework()->LatLon2Pixel(drll);
        double dr = sqrt(abs(drPos.y - centerPos.y) * abs(drPos.y-centerPos.y) + abs(drPos.x - centerPos.x) * abs(drPos.x - centerPos.x));

        QRectF rectangle(centerPos.x-dr,centerPos.y-dr, dr*2, dr*2);
        int startAngle = (90 - startAng - angleFromNorth);
        int spanAngle = (-lenAng);
        //计算弧线上的点，添加到多边形
        polygon.append(QPointF(centerPos.x, centerPos.y));
        double pntsize = 100;//假定弧线上取100个点
        for(int i=0; i<=pntsize; i++)
        {
            double degree = startAngle + i/pntsize * spanAngle;
            double alpha = degree / 180.0 * GLOB_PI;
            //qDebug()<<"degree:"<<degree<<" alpha:"<<alpha;
            polygon.append(QPointF(centerPos.x + dr * cos(alpha), centerPos.y - dr* sin(alpha)));
        }
        polygon.append(QPointF(centerPos.x, centerPos.y));
    }

    return polygon;
}

//雷达点
RadarPointElement::RadarPointElement(const ZCHX::Data::ITF_RadarPoint &ele, zchxMapWidget* f)
    :FixElement<ZCHX::Data::ITF_RadarPoint>(ele, ZCHX::Data::ELE_RADAR_POINT, ZCHX::LAYER_RADAR, f)
    ,mDrawAsAis(false)
    ,m_needDrawBox(false)
    ,mRadarShapeAsRect(false)
    ,m_status(0)
    ,m_shan(true)
{
    setFlashColor(ele.warnStatusColor);
    initFromSettings();
}

std::pair<double, double> RadarPointElement::getPoint()
{
    return std::pair<double, double>(elelat,elelon);
}

const std::vector<std::pair<double, double> >& RadarPointElement::getPath() const
{
    return m_data.path;
}

void RadarPointElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

bool RadarPointElement::getShan() const
{
    return m_shan;
}

void RadarPointElement::setShan(bool shan)
{
    m_shan = shan;
}

uint RadarPointElement::getStatus() const
{
    return m_data.status;
}

void RadarPointElement::setStatus(const uint &value)
{
    m_data.status = value;
}

bool RadarPointElement::getNeedDrawBox() const
{
    return (m_data.trackby == "1");
}

void RadarPointElement::setNeedDrawBox(bool needDrawBox)
{
    if(needDrawBox)
    {
        m_data.trackby = "1";
    }
    else
    {
        m_data.trackby.clear();
    }
}

void RadarPointElement::initFromSettings()
{
    //初始化雷达颜色设定
    mFillingColor.setNamedColor(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_FILL_COLOR, QColor(Qt::green).name()).toString());
    mTextColor.setNamedColor(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_TEXT_COLOR, QColor(Qt::black).name()).toString());
    mConcernColor.setNamedColor(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_CONCERN_COLOR, QColor(Qt::red).name()).toString());;
    mBorderColor.setNamedColor(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_BORDER_COLOR, QColor(Qt::black).name()).toString());
    mRadarShapeAsRect = Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_SHAPE_RECT, true).toBool();
    mDrawAsAis = Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_FORCED_AIS, false).toBool();

    Element::initFromSettings();
    //qDebug()<<"radar ini seetings."<<mDrawAsAis<<mRadarShape<<mFillingColor.name()<<mTextColor.name()<<mConcernColor.name()<<mBorderColor.name();
}

void RadarPointElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    Element::drawElement(painter);

    //开始画尾迹点
    if(getIsRealtimeTailTrack()){
        drawTrack(painter);
    }
    //预警
    if(getStatus() > 0)
    {
        drawFlashRegion(painter, getCurrentPos(), getStatus(), m_data.warnStatusColor);
    }
    //开始画雷达目标
    //取得当前图元对应的屏幕坐标位置
    QPointF pos = getCurrentPos();
    int sideLen = getDrawScaleSize();
    updateGeometry(pos, sideLen);
    double angleFromNorth = mView->framework()->GetRotateAngle(); //计算当前正北方向的方向角
    QColor fillColor = getIsConcern() ? getConcernColor() : getFillingColor();
    //地图缩放比例小于10级的情况只画点
    if(mView->framework()->Zoom() < 10)
    {
        PainterPair chk(painter);
        painter->setPen(Qt::NoPen);
        painter->setBrush(fillColor);
        QRect rect(0, 0, sideLen, sideLen);
        rect.moveCenter(pos.toPoint());
        if(mRadarShapeAsRect)
        {
            painter->drawRect(rect);
        } else
        {
            painter->drawEllipse(rect.center(), rect.width() / 2, rect.height() / 2);
        }
        return;
    }
    //开始处理雷达点具体画法
    QRect elementRect; //目标盒子
    //这个RADARPLAN是什么类型,暂时未知
    //    if(getRadarType() == RADARPLAN)
    //    {
    //        PainterPair chk(painter);
    //        if(getIsActive())
    //        {
    //            //画选中的外框
    //            QRect planeRect(pos.x()-curScale-2,pos.y()-curScale-2,curScale+4,curScale+4);
    //            elementRect = planeRect.adjusted(-2, -2, 2, 2);
    //            painter->setPen(Qt::black);
    //            painter->drawRect(planeRect);
    //        }
    //        //目标填充(黄色)
    //        painter->setBrush(Qt::yellow);
    //        QRectF rect(pos.x() - curScale, pos.y() - curScale, curScale, curScale);
    //        painter->drawRect(rect);
    //    }
    //    else if(getRadarType() == RADARSHIP)
    //    {
    QRect shipRect(0,0,sideLen,sideLen);
    shipRect.moveCenter(QPoint(pos.x(), pos.y()) );
    //qDebug()<<"draw as ais:"<<getDrawAsAis();
    if(getDrawAsAis()) //ship
    {
        QPixmap shipPic = ZCHX::Utils::getImage(":/element/ship.png", Qt::yellow, mView->framework()->Zoom());
        //shipRect.setSize(shipPic.size());
        shipRect.moveCenter(QPoint(pos.x(), pos.y()) );
        painter->drawPixmap(shipRect, shipPic);
    } else
    {
        PainterPair chk(painter);
        painter->setPen(mBorderColor);
        painter->setBrush(fillColor);
        painter->translate(pos.x(),pos.y());
        painter->rotate((int)(m_data.cog + angleFromNorth) % 360);
        painter->translate(-pos.x(), -pos.y());
        if(mRadarShapeAsRect)
        {
            painter->drawRect(shipRect);
        } else
        {
            painter->drawEllipse(pos.x() - sideLen / 2, pos.y() - sideLen / 2, sideLen, sideLen);
        }
    }
    elementRect = shipRect.adjusted(-2, -2, 2, 2);

    drawActive(painter);

    //绘制交汇
    if( mView->getIsOpenMeet() && m_data.RadarMeetVec.size()>0)
    {
        //qDebug()<<"!!!!!!!!!!";
        PainterPair chk(painter);
        QPen pen(Qt::red, 2, Qt::DashLine);
        painter->setPen(pen);
        for(int j = 0;j<m_data.RadarMeetVec.size();j++)
        {
            ZCHX::Data::RadarMeet meetItem = m_data.RadarMeetVec.at(j);
            ZCHX::Data::Point2D meetPos = mView->framework()->LatLon2Pixel(meetItem.lat,meetItem.lon);
            uint time_hour = meetItem.UTC / 3600;
            uint time_minute = meetItem.UTC / 60 - time_hour * 60;
            uint time_second = meetItem.UTC % 60;
            QString darwText = QObject::tr("时间: %1时 %2分 %3秒; 距离: %L4米")
                    .arg(time_hour)
                    .arg(time_minute, 2, 10, QLatin1Char('0'))
                    .arg(time_second, 2, 10, QLatin1Char('0'))
                    .arg(meetItem.disrance, 0, 'f', 0);
            painter->drawLine(meetPos.x, meetPos.y, pos.x(), pos.y());
            painter->drawText(pos.x() - 10, pos.y() - sideLen / 2, darwText);
        }
    }
    //绘制方向线和文本
    drawText(painter, pos, sideLen);
    //    }
    //是否需要套框
    if(getNeedDrawBox())
    {
        //套框使用虚线
        QPen penBox(Qt::red,2,Qt::DashLine);
        PainterPair chk(painter);
        painter->setPen(penBox);
        painter->drawRect(elementRect);
    }

}

void RadarPointElement::drawText(QPainter *painter, QPointF pos, int sideLen)
{
    if(!mView->framework()) return;
    double angleFromNorth = mView->framework()->GetRotateAngle(); //计算当前正北方向的方向角
    PainterPair chk(painter);
    painter->setPen(mTextColor);
    QString radarName = QObject::tr("T%1").arg(QString::number(m_data.trackNumber).right(4));
    QFont font("新宋体", 10, QFont::Normal, false);
    painter->setFont(font);
    painter->drawText(pos.x()+2+sideLen / 2,pos.y() + sideLen / 2,radarName);
    painter->setPen(mBorderColor);
    painter->translate(pos.x(),pos.y());
    painter->rotate((int)(m_data.cog + angleFromNorth) % 360);
    painter->translate(-pos.x(), -pos.y());
    painter->drawLine(pos.x(),pos.y(),pos.x(),pos.y() -  sideLen * (m_data.sog/3.6*1.852  + 0.5));
}

void RadarPointElement::drawTrack(QPainter *painter)
{
    const std::vector<std::pair<double, double>>& path = getPath();
    if(path.empty()) return;

    std::vector<QPointF> pts = mView->framework()->convert2QtPonitList(path);
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::black,1,Qt::DashLine));
    painter->drawPolyline(&pts[0],pts.size());
    return;
}



void RadarPointElement::clicked(bool isDouble)
{
    if(!mView) return;
    if(isDouble) {
        mView->signalIsDoubleClicked4RadarPoint(m_data);
    } else {
        mView->signalIsSelected4RadarPoint(m_data, false);
    }
}

void RadarPointElement::showToolTip(const QPoint &pos)
{
    ZCHX::Data::ITF_RadarPoint info = data();
    QString pos_text = QObject::tr("跟踪号: T%1\n").arg(info.trackNumber);
    pos_text += QObject::tr("经度: %1\n").arg(FLOAT_STRING(info.wgs84PosLon, 6));
    pos_text += QObject::tr("纬度: %1\n").arg(FLOAT_STRING(info.wgs84PosLat, 6));;
    pos_text += QObject::tr("更新时间: %1\n").arg(QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss"));
    pos_text += QObject::tr("方位角: %1\n").arg(FLOAT_STRING(info.cog, 0));
    pos_text += QObject::tr("速度: %1").arg(FLOAT_STRING(info.sog/3.6*1.852, 2));
    QToolTip::showText(pos, pos_text);
}

QList<QAction*> RadarPointElement::getRightMenuAction()
{
    QList<QAction*> list;
    //获取当前选择的目标对象
    if(this->getIsActive())
    {
        list.append(addAction(tr("实时轨迹"),this, SLOT(slotSetRealTimeTraces())));
        list.append(addAction(tr("关注"),this, SLOT(slotSetConcern())));
        list.append(addAction(tr("联动"),this, SLOT(slotInvokeLinkageSpot())));
    }
    return list;
}
}
