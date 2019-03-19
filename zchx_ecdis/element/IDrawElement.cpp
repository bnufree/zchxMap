#include "IDrawElement.hpp"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
#include "zchxmapframe.h"
#include "profiles.h"
#include "zchxMapDatautils.h"
#include <QGeoCoordinate>

Q_LOGGING_CATEGORY(ecdis, "zchx.Ecdis")

namespace qt {
int Element::g_maxLineLength = 100;
int Element::gSetFlashAlphaStep = 100;

Element::Element(const double &lat, const double &lon, zchxMapWidget* view, ZCHX::Data::ELETYPE type, const QColor& flashColor)
    : elelat(lat)
    , elelon(lon)
    , displayLat(lat)
    , displayLon(lon)
    , isActive(false)
    , isHover(false)
    , isFocus(false)
    , isConcern(false)
    , isRealtimeTailTrack(false)
    , isHistroyTrack(false)
    , isOpenMeet(false)
    , isUpdate(false)
    , mID("")
    , m_element_type(type)
    , isForceImage(false)
    , m_layer(0)
    , mView(view)
    , m_updateUTC(QDateTime::currentMSecsSinceEpoch())
    , m_geometryChanged(false)
	, mFlashColor(flashColor)
    , mConcernColor(QColor())
    , mTextColor(QColor())
    , mBorderColor(QColor())
    , mFillingColor(QColor())
    , m_layerName("")
{

    Element::g_maxLineLength = Profiles::instance()->value(MAP_INDEX, MAX_LINE_LENGTH).toInt();
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
    , isRealtimeTailTrack(element.isRealtimeTailTrack)
    , isHistroyTrack(element.isHistroyTrack)
    , isOpenMeet(element.isOpenMeet)
    , isUpdate(element.isUpdate)
    , mID(element.mID)
    , m_element_type(element.m_element_type)
    , isForceImage(element.isForceImage)
    , mFlashColor(element.mFlashColor)
    , m_layer(element.m_layer)
    , mView(element.mView)
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
    , m_layerName(element.m_layerName)
{

}

Element::~Element()
{
}

std::shared_ptr<MapLayer> Element::getLayer()
{
    return m_layer;
}

void Element::setLayer(const QString& layer)
{
    //多次调用要导致崩溃
    //m_layer.reset(MapLayerMgr::instance()->getLayer(layer).get());
    m_layerName = layer;
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

QString Element::getID() const
{
    return mID;
}

void Element::setID(const QString &id)
{
    mID = id;
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

bool Element::getIsRealtimeTailTrack() const
{
    return isRealtimeTailTrack;
}

void Element::setIsRealtimeTailTrack(bool value)
{
    isRealtimeTailTrack = value;
}

bool Element::getIsHistoryTrack() const
{
    return isHistroyTrack;
}

void Element::setIsHistoryTrack(bool value)
{
    isHistroyTrack = value;
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

bool Element::contains(int range, double x, double y) const
{
    if(!mView || !mView->framework() || range <= 0) return false;
    ZCHX::Data::Point2D gpos = mView->framework()->LatLon2Pixel(displayLat, displayLon);
    QRectF rect(0, 0, 2*range, 2*range);
    rect.moveCenter(QPointF(gpos.x, gpos.y));
    return rect.contains(QPointF(x, y));
}

bool Element::contains(const QPoint& pos) const
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

QPointF Element::getViewPos()
{
    QPointF pos;
    if(mView && mView->framework())
    {
        pos = mView->framework()->LatLon2Pixel(elelat, elelon).toPointF();
    }
    return pos;
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

std::list<std::shared_ptr<Element> > Element::getChildren(ZCHX::Data::ELETYPE type) const
{
    if(type == ZCHX::Data::ELE_NONE) return m_children;
    std::list<std::shared_ptr<Element> > list;
    foreach (std::shared_ptr<Element> ele, m_children) {
        if(ele->getElementType() == type){
            list.push_back(ele);
        }
    }
    return list;
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
    if(!painter || !mView || !mView->framework()) return;
    double angleFromNorth = mView->framework()->GetRotateAngle(); //计算当前正北方向的方向角
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
        flashcolor.setAlpha(Profiles::instance()->value(MAP_INDEX, WARN_FLAH_COLOR_ALPHA, 100).toInt());
    }
    PainterPair chk(painter);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(flashcolor));
    painter->drawEllipse(pos, radius, radius);
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
    return isForceImage;
}

void Element::setForceImage(bool forceImage)
{
    isForceImage = forceImage;
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
    if(!mView || !mView->framework()) return 10;
    int curScale = mView->framework()->Zoom() < 7 ? 5 : 10;
    return curScale;
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
    return getViewPos();
}

bool Element::isViewAvailable() const
{
    if(!mView || !mView->framework()) return false;
    return true;
}

zchxMapFrameWork* Element::framework() const
{
    if(mView) return mView->framework();
    return 0;
}

bool Element::isLayervisible()
{
    if(!m_layer) return false;
    return m_layer->visible();
}

bool Element::isDrawAvailable(QPainter* painter)
{
    if(!painter ||!MapLayerMgr::instance()->isLayerVisible(m_layerName) || !isViewAvailable()) return false;
    return true;
}

void Element::setFlashColor(const QColor &color)
{
    mFlashColor = color;
}

}
