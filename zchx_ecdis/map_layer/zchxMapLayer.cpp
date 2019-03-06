#include "zchxMapLayer.h"
#include "zchxmaplayer_p.h"
#include "zchxmapframe.h"
#include <QGeoCoordinate>

namespace qt {
MapLayer::MapLayer(const QString &type, const QString &displayName, bool visible, QObject *parent)
    : QObject(*new MapLayerPrivate(type, displayName, visible), parent)
{

}

MapLayer::MapLayer(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible, QObject *parent)
    : QObject(*new MapLayerPrivate(drawWidget, type, displayName, visible), parent)
{

}

MapLayer::MapLayer(QObject *parent)
    : QObject(*new MapLayerPrivate, parent)
{

}

QString MapLayer::type() const
{
    Q_D(const MapLayer);
    return d->m_type;
}

void MapLayer::setType(const QString &type)
{
    Q_D(MapLayer);
    d->m_type = type;
}

QString MapLayer::displayName() const
{
    Q_D(const MapLayer);
    return d->m_displayName;
}

void MapLayer::setDisplayName(const QString &displayName)
{
    Q_D(MapLayer);
    if(displayName == d->m_displayName)
        return;

    d->m_displayName = displayName;
    emit displayNameChanged(d->m_displayName);
}

bool MapLayer::visible() const
{
    Q_D(const MapLayer);
    return d->m_visible;
}

void MapLayer::setVisible(bool visible)
{
    Q_D(MapLayer);
    if(visible == d->m_visible)
        return;

    d->m_visible = visible;
    emit visibleChanged(d->m_visible);
}

bool MapLayer::hasChildLayer() const
{
    Q_D(const MapLayer);
    return !d->m_children.empty();
}

const std::list<std::shared_ptr<MapLayer> > &MapLayer::getChildLayers()
{
    Q_D(const MapLayer);
    return d->m_children;
}

void MapLayer::addChildLayer(std::shared_ptr<MapLayer> layer)
{
    Q_D(MapLayer);
    bool added = (std::find(d->m_children.begin(), d->m_children.end(), layer) != d->m_children.end());
    if(!added)
        d->m_children.push_back(layer);
}

void MapLayer::removeChildLayer(std::shared_ptr<MapLayer> layer)
{
    Q_D(MapLayer);
    d->m_children.remove(layer);
}

std::shared_ptr<MapLayer> MapLayer::getChildLayer(const QString &type)
{
    Q_D(MapLayer);
    for(std::shared_ptr<MapLayer> layer : d->m_children)
    {
        if(layer->type() == type)
            return layer;
    }
    return NULL;
}

void MapLayer::addElement(std::shared_ptr<Element> element)
{
    if(!element)
        return;

    Q_D(MapLayer);
    if(element->layer())
    {
        element->layer()->removeElement(element);
    }
    element->layer().reset(this);
//    element->m_layer = this;
    element->setFrameWork(d->m_drawWidget->framework());

    bool contained = (std::find(d->m_elements.begin(), d->m_elements.end(), element) != d->m_elements.end());
    if(contained)
        return;

    d->m_elements.push_back(element);

//    update();
}

void MapLayer::removeElement(std::shared_ptr<Element> element)
{
    if(!element)
        return;

    Q_D(MapLayer);
    element->layer().reset();

    bool contained = (std::find(d->m_elements.begin(), d->m_elements.end(), element) != d->m_elements.end());
    if(contained)
        return;

    d->m_elements.remove(element);

//    update();
}

std::list<std::shared_ptr<Element> > MapLayer::getElements()
{
    Q_D(MapLayer);
    return d->m_elements;
}

void MapLayer::update()
{
    Q_D(MapLayer);
    if(!d->m_enableUpdate)
        return;

    if(d->m_drawWidget)
    {
        d->m_drawWidget->update();
    }
}

void MapLayer::drawLayer(QPainter *painter)
{
    Q_D(MapLayer);
    for(std::shared_ptr<Element> element : d->m_elements)
    {
        if(element.get())
            element->drawElement(painter);
    }
}

std::shared_ptr<Element> MapLayer::pickUpElement(QPointF pos, const QGeoCoordinate &geoPos)
{
    Q_D(MapLayer);
    for(std::shared_ptr<Element> element : d->m_elements)
    {
        if(!element)
            continue;

        if(geoPos.isValid() )
        {
            if(element->contains(geoPos))
                return element;
        }
        else
        {
            if(element->contains(QPoint(pos.x(), pos.y())))
                return element;
        }
    }
    return NULL;
}

std::shared_ptr<Element> MapLayer::pickUpElement(const QString &id)
{
    Q_D(MapLayer);
    for(std::shared_ptr<Element> element : d->m_elements)
    {
        if(!element)
            continue;

        if(element->getStrID() == id)
        {
            return element;
        }
    }
    return NULL;
}

std::shared_ptr<Element> MapLayer::pickUpElement(QPointF pos)
{
    Q_D(MapLayer);
    if(d->m_drawWidget->framework())
    {
        for(std::shared_ptr<Element> element : d->m_elements)
        {
            if(!element)
                return element;

            if(element->contains(10, pos.x(), pos.y()) )
            {
                return element;
            }
        }
    }
    return NULL;
}

QPointF MapLayer::convertToView(double lon, double lat)
{
    Q_D(MapLayer);
    QPointF pos;
    if(d->m_drawWidget->framework())
    {
        ZCHX::Data::Point2D posD = d->m_drawWidget->framework()->LatLon2Pixel(lat, lon);
        pos = QPointF(posD.x, posD.y);
    }
    return pos;
}

bool MapLayer::clearElementState()
{
    bool cleared = false;
    Q_D(MapLayer);
    for(std::shared_ptr<Element> e : d->m_elements)
    {
        if(!e)
            continue;
        if(!cleared)
            cleared = e->getIsActive()||e->getIsFocus()||e->getIsHover();
        e->setIsActive(false);
        e->setIsFocus(false);
        e->setIsHover(false);
    }
    return cleared;
}

bool MapLayer::clearActiveState()
{
    bool cleared = false;
    Q_D(MapLayer);
    for(std::shared_ptr<Element> element : d->m_elements)
    {
        if(!element)
            continue;
        if(element->getIsActive())
            cleared = true;
        element->setIsActive(false);
    }
    return cleared;
}

bool MapLayer::clearFocusState()
{
    bool cleared = false;
    Q_D(MapLayer);
    for(std::shared_ptr<Element> element : d->m_elements)
    {
        if(!element)
            continue;
        if(element->getIsFocus())
            cleared = true;
        element->setIsFocus(false);
    }
    return cleared;
}

bool MapLayer::clearHoverState()
{
    bool cleared = false;
    Q_D(MapLayer);
    for(std::shared_ptr<Element> element : d->m_elements)
    {
        if(!element)
            continue;
        if(element->getIsHover())
            cleared = true;
        element->setIsHover(false);
    }
    return cleared;
}

bool MapLayer::getEnableUpdate() const
{
    Q_D(const MapLayer);
    return d->m_enableUpdate;
}

void MapLayer::setEnableUpdate(bool enableUpdate)
{
    Q_D(MapLayer);
    d->m_enableUpdate = enableUpdate;
}

ZCHX::Data::ECDIS_PLUGIN_USE_MODELs MapLayer::mode() const
{
    Q_D(const MapLayer);
    return d->m_mode;
}

void MapLayer::setMode(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL &mode, bool addOrRemove)
{
    Q_D(MapLayer);
    if(addOrRemove)
        d->m_mode |= mode;
    else
        d->m_mode &= ~mode;
}

MapLayerPrivate::MapLayerPrivate()
    : m_visible(true)
    , m_enableUpdate(true)
    , m_drawWidget(0)
{

}

MapLayerPrivate::MapLayerPrivate(const QString &type, const QString &displayName, bool visible)
    : m_type(type)
    , m_displayName(displayName)
    , m_visible(visible)
    , m_enableUpdate(true)
    , m_drawWidget(0)
{

}

MapLayerPrivate::MapLayerPrivate(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible)
    : m_type(type)
    , m_displayName(displayName)
    , m_visible(visible)
    , m_enableUpdate(true)
    , m_drawWidget(drawWidget)
{

}
}
