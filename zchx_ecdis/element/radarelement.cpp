#include "radarelement.h"
#include "zchxmapframe.h"
#include "profiles.h"


namespace qt
{
RadarAreaElement::RadarAreaElement(double radarY, double radarX, int centerLineAngel, int radius, int maxScanRangeANgle, int numberofChannele, int maxWakePointsNumber)
    :Element(radarY,radarX, 0, ZCHX::Data::ELEMENT_RADAR_AREA)
{
    setRadarX(radarX);
    setRadarY(radarY);
    setCenterLineAngel(centerLineAngel);
    setRadius(radius);
    setMaxScanRangeANgle(maxScanRangeANgle);
    setNumberofChannele(numberofChannele);
    setMaxWakePointsNumber(maxWakePointsNumber);


    m_data.radarX = radarX;
    m_data.radarY = radarY;
    m_data.centerLineAngel = centerLineAngel;
    m_data.radius = radius;
    m_data.maxScanRangeANgle = maxScanRangeANgle;
    m_data.numberofChannele = numberofChannele;
    m_data.maxWakePointsNumber = maxWakePointsNumber;
}

RadarAreaElement::RadarAreaElement(const ZCHX::Data::ITF_RadarArea &ele)
    :Element(ele.radarY,ele.radarX,0, ZCHX::Data::ELEMENT_RADAR_AREA)
{
    setRadarX(ele.radarY);
    setRadarY(ele.radarX);
    setCenterLineAngel(ele.centerLineAngel);
    setRadius(ele.radius);
    setMaxScanRangeANgle(ele.maxScanRangeANgle);
    setNumberofChannele(ele.numberofChannele);
    setMaxWakePointsNumber(ele.maxWakePointsNumber);
    m_data = ele;

    uuid = ele.uuid;
}

double RadarAreaElement::radarX() const
{
    return m_radarX;
}

void RadarAreaElement::setRadarX(double radarX)
{
    m_radarX = radarX;
}

double RadarAreaElement::radarY() const
{
    return m_radarY;
}

void RadarAreaElement::setRadarY(double radarY)
{
    m_radarY = radarY;
}

int RadarAreaElement::centerLineAngel() const
{
    return m_centerLineAngel;
}

void RadarAreaElement::setCenterLineAngel(int centerLineAngel)
{
    m_centerLineAngel = centerLineAngel;
}

int RadarAreaElement::radius() const
{
    return m_radius;
}

void RadarAreaElement::setRadius(int radius)
{
    m_radius = radius;
}

int RadarAreaElement::maxScanRangeANgle() const
{
    return m_maxScanRangeANgle;
}

void RadarAreaElement::setMaxScanRangeANgle(int maxScanRangeANgle)
{
    m_maxScanRangeANgle = maxScanRangeANgle;
}

int RadarAreaElement::numberofChannele() const
{
    return m_numberofChannele;
}

void RadarAreaElement::setNumberofChannele(int numberofChannele)
{
    m_numberofChannele = numberofChannele;
}

int RadarAreaElement::maxWakePointsNumber() const
{
    return m_maxWakePointsNumber;
}

void RadarAreaElement::setMaxWakePointsNumber(int maxWakePointsNumber)
{
    m_maxWakePointsNumber = maxWakePointsNumber;
}

ZCHX::Data::ITF_RadarArea RadarAreaElement::data() const
{
    return m_data;
}

QPolygonF RadarAreaElement::getShapePnts(zchxMapFrameWork *framework, double angleFromNorth)
{
    QPolygonF polygon;
    if(framework)
    {
        double startAng = this->centerLineAngel() - this->maxScanRangeANgle() / 2;
        double lenAng =   this->maxScanRangeANgle();
        ZCHX::Data::Point2D centerPos = framework->LatLon2Pixel(this->radarY(),this->radarX());

        //通过推算经纬度计算半径
        ZCHX::Data::LatLon drll(0, 0);
        ZCHX::Utils::distbear_to_latlon(this->radarY(),this->radarX(),this->radius() * 1000,-90 + startAng, drll.lat, drll.lon);
        ZCHX::Data::Point2D drPos = framework->LatLon2Pixel(drll);
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
        //qDebug()<<"polygon:"<<polygon;
    }

    return polygon;
}

bool RadarAreaElement::contains(zchxMapFrameWork *framework, double angleFromNorth, double x, double y) const
{
    if(!framework)
        return false;
    QPolygonF polygon;
    double startAng = this->centerLineAngel() - this->maxScanRangeANgle() / 2;
    double lenAng =   this->maxScanRangeANgle();
    ZCHX::Data::Point2D centerPos = framework->LatLon2Pixel(this->radarY(),this->radarX());

    //通过推算经纬度计算半径
    ZCHX::Data::LatLon drll(0, 0);
    ZCHX::Utils::distbear_to_latlon(this->radarY(),this->radarX(),this->radius() * 1000,-90 + startAng, drll.lat, drll.lon);
    ZCHX::Data::Point2D drPos = framework->LatLon2Pixel(drll);
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
        polygon.append(QPointF(centerPos.x + dr * cos(alpha), centerPos.y - dr*sin(alpha)));
    }
    polygon.append(QPointF(centerPos.x, centerPos.y));
    return polygon.containsPoint(QPointF(x, y), Qt::OddEvenFill);
}

//雷达点
RadarPointElement::RadarPointElement(const double &lat, const double &lon, zchxMapFrameWork* f)
    :Element(lat,lon, f, ZCHX::Data::ELEMENT_RADAR_POINT),m_radar_type(RADARSHIP)
{
        initFromSettings();
}

RadarPointElement::RadarPointElement(const ZCHX::Data::ITF_RadarPoint &ele, zchxMapFrameWork* f)
    :Element(ele.lat,ele.lon, f, ZCHX::Data::ELEMENT_RADAR_POINT, ele.warnStatusColor),m_path(ele.path),m_radar_type((RADARSHIP))
{
    if(!ele.path.empty())
    {
        elelat = ele.path.back().first;
        elelon = ele.path.back().second;
        //这里的displaylat, displaylon暂且一致,防止调用item.contains()方法时出现错误
        displayLat = elelat;
        displayLon = elelon;
    }
    m_data = ele;
    uuid = ele.uuid;
    m_shan = true;
    initFromSettings();
}

RadarPointElement::RadarPointElement(const RadarPointElement &pt)
    : Element(pt)
    , m_path(pt.m_path)
    , m_radar_type(pt.m_radar_type)
    , m_data(pt.m_data)
    , m_shan(pt.m_shan)
    , m_needDrawBox(pt.m_needDrawBox)
    , m_status(pt.m_status)
    , mDrawAsAis(pt.mDrawAsAis)
    , mRadarShapeAsRect(pt.mRadarShapeAsRect)
{

}

std::pair<double, double> RadarPointElement::getPoint()
{
    return std::pair<double, double>(elelat,elelon);
}

const ZCHX::Data::ITF_RadarPoint &RadarPointElement::getData() const
{
    return m_data;
}

void RadarPointElement::setData(const ZCHX::Data::ITF_RadarPoint &data)
{
    m_data = data;
}

const std::vector<std::pair<double, double> >& RadarPointElement::getPath() const
{
    return m_path;
}

void RadarPointElement::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_path = path;
}

RADARTYPE RadarPointElement::getRadarType() const
{
    return m_radar_type;
}

void RadarPointElement::setRadarType(const RADARTYPE &type)
{
    m_radar_type = type;
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
        m_data.trackby = "-1";
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
    if(!painter) return;
    //qDebug()<<"radar property:"<<getData().trackNumber<<getDrawAsAis()<<getDrawShape()<<getFillingColor().name()<<getBorderColor().name();

    Element::drawElement(painter);
    //检查轨迹点是否已经赋值,没赋值有可能有异常,暂时就不处理
    if(getPath().size() == 0) return;
    //取得当前图元对应的屏幕坐标位置
    QPointF pos = getCurrentPos();
    int curScale = m_framework->Zoom() < 7 ? 5 : 10;
    int sideLen = 10;
    double angleFromNorth = m_framework->GetRotateAngle(); //计算当前正北方向的方向角
    QColor fillColor = getIsConcern() ? getConcernColor() : getFillingColor();
    //地图缩放比例小于10级的情况只画点
    if(m_framework->Zoom() < 10)
    {
        PainterPair chk(painter);
        painter->setPen(Qt::NoPen);
        painter->setBrush(fillColor);
        if(mRadarShapeAsRect)
        {
            painter->drawRect(QRect(pos.x() - 2, pos.y() - 2, 4, 4));
        } else
        {
            painter->drawEllipse(pos.x() - 2, pos.y() - 2, 4, 4);
        }
        return;
    }
    //开始处理雷达点具体画法
    QRect elementRect; //目标盒子
    //这个RADARPLAN是什么类型,暂时未知
    if(getRadarType() == RADARPLAN)
    {
        PainterPair chk(painter);
        if(getIsActive())
        {
            //画选中的外框
            QRect planeRect(pos.x()-curScale-2,pos.y()-curScale-2,curScale+4,curScale+4);
            elementRect = planeRect.adjusted(-2, -2, 2, 2);
            painter->setPen(Qt::black);
            painter->drawRect(planeRect);
        }
        //目标填充(黄色)
        painter->setBrush(Qt::yellow);
        QRectF rect(pos.x() - curScale, pos.y() - curScale, curScale, curScale);
        painter->drawRect(rect);
    }
    else if(getRadarType() == RADARSHIP)
    {
        QRect shipRect(0,0,sideLen,sideLen);
        shipRect.moveCenter(QPoint(pos.x(), pos.y()) );
        //qDebug()<<"draw as ais:"<<getDrawAsAis();
        if(getDrawAsAis()) //ship
        {
            QPixmap shipPic = ZCHX::Utils::getImage(":/element/ship.png", Qt::yellow, m_framework->Zoom());
            shipRect.setSize(shipPic.size());
            shipRect.moveCenter(QPoint(pos.x(), pos.y()) );
            painter->drawPixmap(shipRect, shipPic);
        } else
        {
            PainterPair chk(painter);
            painter->setPen(mBorderColor);
            painter->setBrush(fillColor);
            painter->translate(pos.x(),pos.y());
            painter->rotate((int)(getData().cog + angleFromNorth) % 360);
            painter->translate(-pos.x(), -pos.y());
            if(mRadarShapeAsRect)
            {
                painter->drawRect(pos.x()-3,pos.y()-5,6,10);
            } else
            {
                painter->drawEllipse(pos.x() - sideLen / 2, pos.y() - sideLen / 2, sideLen, sideLen);
            }
        }

        elementRect = shipRect.adjusted(-2, -2, 2, 2);

        if(getIsActive())
        {
            //画目标选择的情况
            PainterPair chk(painter);
            QPen pen(Qt::red,2,Qt::DashDotLine); //选中使用点划线
            painter->setBrush(Qt::NoBrush);
            painter->setPen(pen);
            painter->drawRect(pos.x() -sideLen * 1.5, pos.y() - sideLen * 1.5,sideLen * 3, sideLen * 3);
        }

        //绘制交汇
        if( getIsOpenMeet() && getData().RadarMeetVec.size()>0)
        {
            //qDebug()<<"!!!!!!!!!!";
            PainterPair chk(painter);
            QPen pen(Qt::red, 2, Qt::DashLine);
            painter->setPen(pen);
            for(int j = 0;j<getData().RadarMeetVec.size();j++)
            {
                ZCHX::Data::RadarMeet meetItem = getData().RadarMeetVec.at(j);
                ZCHX::Data::Point2D meetPos = m_framework->LatLon2Pixel(meetItem.lat,meetItem.lon);
                uint time_hour = meetItem.UTC / 3600;
                uint time_minute = meetItem.UTC / 60 - time_hour * 60;
                uint time_second = meetItem.UTC % 60;
                QString darwText = QObject::tr("Time: %1H %2M %3S; Diatance: %L4m")
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
    }
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
    if(!m_framework) return;
    double angleFromNorth = m_framework->GetRotateAngle(); //计算当前正北方向的方向角
    PainterPair chk(painter);
    painter->setPen(mTextColor);
    QString radarName = QObject::tr("T%1").arg(QString::number(getData().trackNumber).right(4));
    QFont font("新宋体", 10, QFont::Normal, false);
    painter->setFont(font);
    painter->drawText(pos.x()+2+sideLen / 2,pos.y() + sideLen / 2,radarName);
    painter->setPen(mBorderColor);
    painter->translate(pos.x(),pos.y());
    painter->rotate((int)(getData().cog + angleFromNorth) % 360);
    painter->translate(-pos.x(), -pos.y());
    painter->drawLine(pos.x(),pos.y(),pos.x(),pos.y() -  sideLen * (getData().sog/3.6*1.852  + 0.5));
}

void RadarPointElement::drawTrack(QPainter *painter)
{
    const std::vector<std::pair<double, double>>& path = getPath();
    if(path.empty()) return;

    std::vector<QPointF> pts = convert2QtPonitList(path);
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::black,1,Qt::DashLine));
    painter->drawPolyline(&pts[0],pts.size());
    return;
}
}
