#include "IDrawElement.hpp"
#include "zchxmapwidget.h"
#include "map_layer/zchxMapLayer.h"
#include "zchxmapframe.h"
#include "profiles.h"
#include "zchxMapDatautils.h"
#include <QGeoCoordinate>

Q_LOGGING_CATEGORY(ecdis, "zchx.Ecdis")

using namespace DrawElement;
int Element::g_maxLineLength = 100;

Element::Element(const double &lat, const double &lon, ZCHX::Data::ELETYPE type,const QColor& flashColor)
    : elelat(lat)
    , elelon(lon)
    , displayLat(lat)
    , displayLon(lon)
    , isActive(false)
    , isHover(false)
    , isFocus(false)
    , isConcern(false)
    , isTailTrack(false)
    , isOpenMeet(false)
    , uuid(-1)
    , m_strID("")
    , m_element_type(type)
    , m_pos(QPointF(-1, -1))
    , m_forceImage(false)
    , m_layer(0)
    , m_framework(0)
    , m_uuid(QUuid::createUuid())
    , m_updateUTC(QDateTime::currentMSecsSinceEpoch())
    , m_geometryChanged(false)
	, mFlashColor(flashColor)
    , mConcernColor(QColor())
    , mTextColor(QColor())
    , mBorderColor(QColor())
    , mFillingColor(QColor())
{

    Element::g_maxLineLength = zchxEcdis::Profiles::instance()->value(MAP_INDEX, MAX_LINE_LENGTH).toInt();
}

Element::Element(const Element &element)
    : elelat(element.elelat)
    , elelon(element.elelon)
    , displayLat(element.displayLat)
    , displayLon(element.displayLon)
    , isActive(element.isActive)
    , isHover(element.isHover)
    , isFocus(element.isFocus)
    , isConcern(element.isConcern)
    , isTailTrack(element.isTailTrack)
    , isOpenMeet(element.isOpenMeet)
    , uuid(element.uuid)
    , m_strID(element.m_strID)
    , m_element_type(element.m_element_type)
    , m_pos(element.m_pos)
    , m_forceImage(element.m_forceImage)
    , mFlashColor(element.mFlashColor)
    , m_layer(element.m_layer)
    , m_framework(element.m_framework)
    , m_uuid(element.m_uuid)
    , m_updateUTC(element.m_updateUTC)
    , m_boundingRectSmall(element.m_boundingRectSmall)
    , m_boundingRect(element.m_boundingRect)
    , m_activeRect(element.m_activeRect)
    , m_focusRect(element.m_focusRect)
    , m_geometryChanged(element.m_geometryChanged)
    , mConcernColor(element.mConcernColor)
    , mTextColor(element.mTextColor)
    , mBorderColor(element.mBorderColor)
    , mFillingColor(element.mFillingColor)
{

}

Element::~Element()
{
}

std::shared_ptr<zchxMapLayer> Element::getLayer()
{
    return m_layer;
}

std::pair<double, double> Element::getLatLon() const
{
    return std::pair<double,double>(elelat,elelon);
}

double Element::lat() const
{
    return elelat;
}

double Element::lon() const
{
    return elelon;
}

void Element::setLatLon(const std::pair<double, double> &ll)
{
    elelat = ll.first;
    elelon = ll.second;
}

void Element::setLatLon(double lat, double lon)
{
    elelat = lat;
    elelon = lon;
}

bool Element::getIsActive() const
{
    return isActive;
}

void Element::setIsActive(bool value)
{
    isActive = value;
}

ZCHX::Data::ELETYPE Element::getElementType() const
{
    return m_element_type;
}

void Element::setElementType(const ZCHX::Data::ELETYPE &type)
{
    m_element_type = type;
}

int Element::getUuid() const
{
    return uuid;
}

bool Element::getIsHover() const
{
    return isHover;
}

void Element::setIsHover(bool value)
{
    isHover = value;
}

bool Element::getIsFocus() const
{
    return isFocus;
}

void Element::setIsFocus(bool value)
{
    isFocus = value;
}

bool Element::getIsConcern() const
{
    return isConcern;
}

void Element::setIsConcern(bool value)
{
    isConcern = value;
}

bool Element::getIsTailTrack() const
{
    return isTailTrack;
}

void Element::setIsTailTrack(bool value)
{
    isTailTrack = value;
}

double Element::getDisplayLat() const
{
    return displayLat;
}

void Element::setDisplayLat(double value)
{
    displayLat = value;
}

double Element::getDisplayLon() const
{
    return displayLon;
}

void Element::setDisplayLon(double value)
{
    displayLon = value;
}

bool Element::isUseDisplayLatLon() const
{
    return useDisplayLatLon;
}

void Element::setUseDisplayLatLon(bool value)
{
    useDisplayLatLon = value;
}

bool Element::contains(zchxMapFrameWork *framework, int range, double x, double y) const
{
    if(!framework || range <= 0)
        return false;
    Point2D gpos = framework->LatLon2Pixel(displayLat, displayLon);
    QRectF rect(0, 0, 2*range, 2*range);
    rect.moveCenter(QPointF(gpos.x, gpos.y));
    return rect.contains(QPointF(x, y));
}

bool Element::contains(QPointF pos) const
{
    return false;
}

bool Element::contains(const QGeoCoordinate &geoPos) const
{
    return false;
}

bool Element::isEmpty() const
{
    return false;
}

QPointF Element::getViewPos(zchxMapFrameWork *f)
{
    if(!f)
        f = m_framework;
    if(f)
    {
        Point2D gpos = f->LatLon2Pixel(displayLat, displayLon);
        m_pos = QPointF(gpos.x, gpos.y);
    }
    return m_pos;
}

void Element::drawElement(QPainter *painter)
{
    for(std::shared_ptr<Element> child : m_children)
    {
        child->drawElement(painter);
    }
}

void Element::drawActive(QPainter *painter)
{
    if(painter && getIsActive())
    {
        PainterPair chk(painter);
        QPen pen(Qt::red,2,Qt::DotLine);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(pen);
        painter->drawRect(m_activeRect);
    }
}

void Element::drawFocus(QPainter *painter)
{
    if(painter && getIsFocus())
    {
        PainterPair chk(painter);
        QPen pen(Qt::red,2,Qt::DotLine);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(pen);
        painter->drawRect(m_focusRect);
    }
}

void Element::updateGeometry(QPointF pos, qreal size)
{
//    qreal maxVal = qMax(m_boundingRect.width(), m_boundingRect.height());
    qreal half = size / 2.0;
    size += 1;
    m_activeRect = m_boundingRect.adjusted(-size,-size,size,size);
    m_focusRect = m_activeRect.adjusted(-half,-half,half,half);
    m_boundingRectSmall.setCoords(pos.x() - 2, pos.y() - 2, pos.x() + 2, pos.y() + 2);
}

void Element::addChild(std::shared_ptr<Element> child)
{
    m_children.push_back(child);
    if(m_layer)
    {
        //暂不加入
//        d->layer->addElement(child);
    }
}

void Element::removeChild(std::shared_ptr<Element> child)
{
    m_children.remove(child);
}

std::list<std::shared_ptr<Element> > &Element::getChildren()
{
    return m_children;
}

void Element::setParent(std::shared_ptr<Element> ele)
{
    m_parent.reset(ele.get());
}

std::shared_ptr<Element> Element::parent()
{
    return m_parent;
}

void Element::drawSpeedDirectionLine(QPainter *painter, QPointF pos, qreal sog, qreal cog, qreal rot)
{
    if(!painter || !m_framework)
        return;

    double angleFromNorth = m_framework->GetRotateAngle(); //计算当前正北方向的方向角
    qreal sideLen = getDrawScaleSize();

    PainterPair chk(painter);
    painter->setPen(Qt::black);
    //执行转向
    painter->translate(pos.x(), pos.y());
    painter->rotate((int)(cog + angleFromNorth) % 360);
    painter->translate(-pos.x(), -pos.y());

    //速度等比线
    QLineF speedline(pos.x(),pos.y(),pos.x(),pos.y() - sideLen * (sog + 0.5));
    if(speedline.length() < Element::g_maxLineLength)
        painter->drawLine(speedline);

    //方向角度线
    if(rot != 0)//等于0 不用绘制
    {
        qreal y = pos.y() - sideLen * (sog/3.6*1.852 + 0.5);
        QLineF sogline(pos.x(), y, pos.x() - 10, y);//rot < 0
        if (rot > 0)
        {
            sogline = QLineF(pos.x(), y, pos.x() + 10, y);
        }
        if(sogline.length() < Element::g_maxLineLength)
            painter->drawLine(sogline);
    }
}

void Element::drawFlashRegion(QPainter *painter, QPointF pos, int status, QColor color, qreal radius)
{
    if(!painter || status == 0)
        return;
    if(radius < 0.1)
        radius = getDrawScaleSize();

//    if(color.isValid())
//        m_mapFlashColor.insert(status, color);

    QColor flashcolor = /*m_mapFlashColor.value(status)*/color;
    if(!flashcolor.isValid() )
    {
        return;
    }

    // 航运预警不闪烁
    if (status < ZCHX::Data::WARN_CHANNEL_YAW || status > ZCHX::Data::WARN_CHANNEL_MOORING)
    {
        flashcolor.setAlpha(zchxEcdis::Profiles::instance()->value(MAP_INDEX, WARN_FLAH_COLOR_ALPHA, 100).toInt());
    }
    PainterPair chk(painter);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(flashcolor));
    painter->drawEllipse(pos, radius, radius);
}

std::vector<QPointF> Element::convert2QtPonitList(const std::vector<std::pair<double, double> > &path)
{
    std::vector<QPointF> pts;
    if(!m_framework)
        return pts;

    for(int i = 0; i < path.size(); ++i)
    {
        std::pair<double, double> ll = path[i];
        Point2D pos = m_framework->LatLon2Pixel(ll.first,ll.second);
        pts.push_back(QPointF(pos.x,pos.y));
    }
    return pts;
}

QPointF Element::convertToView(double lon, double lat)
{
    QPointF pos;
    if(m_framework)
    {
        Point2D posD = m_framework->LatLon2Pixel(lat, lon);
        pos = QPointF(posD.x, posD.y);
    }
    return pos;
}

qint64 Element::getUpdateUTC() const
{
    return m_updateUTC;
}

void Element::setUpdateUTC(const qint64 &updateUTC)
{
    m_updateUTC = updateUTC;
}

bool Element::getForceImage() const
{
    return m_forceImage;
}

void Element::setForceImage(bool forceImage)
{
    m_forceImage = forceImage;
}

bool Element::getGeometryChanged() const
{
    return m_geometryChanged;
}

void Element::setGeometryChanged(bool geometryChanged)
{
    m_geometryChanged = geometryChanged;
}

int Element::getDrawScaleSize() const
{
    if(!m_framework)
        return 10;
    int curScale = m_framework->Zoom() < 7 ? 5 : 10;
    return curScale;
}

QString Element::getStrID() const
{
    return m_strID;
}

void Element::setStrID(const QString &strID)
{
    m_strID = strID;
}

void Element::setUuid(const QUuid &uuid)
{
    m_uuid = uuid;
}

QUuid Element::getUuid2() const
{
    return m_uuid;
}

void Element::updateFlashRegionColor(const QColor& color)
{
    mFlashColor = color;
}

QColor Element::getBorderColor() const
{
    return mBorderColor;
}
void   Element::setBorderColor(const QColor& color)
{
    mBorderColor = color;
}

QColor Element::getFillingColor() const
{
    return mFillingColor;
}
void   Element::setFillingColor(const QColor& color)
{
    mFillingColor = color;
}

QColor Element::getTextColor() const
{
    return mTextColor;
}
void   Element::setTextColor(const QColor& color)
{
    mTextColor = color;
}

QColor Element::getConcernColor() const
{
    return mConcernColor;
}
void   Element::setConcernColor(const QColor& color)
{
    mConcernColor = color;
}

void  Element::initFromSettings()
{
    return;
}

QPointF Element::getCurrentPos()
{
    Point2D pos = m_framework->LatLon2Pixel(lat(), lon());
    return QPointF(pos.x, pos.y);
}

bool Element::isLayervisible()
{
    if(!m_layer) return false;
    return m_layer->visible();
}
