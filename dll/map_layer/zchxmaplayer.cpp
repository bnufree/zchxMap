#include "zchxMapLayer.h"
#include "zchxmaplayer_p.h"
#include "zchxmapframe.h"
#include <QGeoCoordinate>

zchxMapLayer::zchxMapLayer(const QString &type, const QString &displayName, bool visible, QObject *parent)
    : QObject(*new zchxMapLayerPrivate(type, displayName, visible), parent)
{

}

zchxMapLayer::zchxMapLayer(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible, QObject *parent)
    : QObject(*new zchxMapLayerPrivate(drawWidget, type, displayName, visible), parent)
{

}

zchxMapLayer::zchxMapLayer(QObject *parent)
    : QObject(*new zchxMapLayerPrivate, parent)
{

}

QString zchxMapLayer::type() const
{
    Q_D(const zchxMapLayer);
    return d->m_type;
}

void zchxMapLayer::setType(const QString &type)
{
    Q_D(zchxMapLayer);
    d->m_type = type;
}

QString zchxMapLayer::displayName() const
{
    Q_D(const zchxMapLayer);
    return d->m_displayName;
}

void zchxMapLayer::setDisplayName(const QString &displayName)
{
    Q_D(zchxMapLayer);
    if(displayName == d->m_displayName)
        return;

    d->m_displayName = displayName;
    emit displayNameChanged(d->m_displayName);
}

bool zchxMapLayer::visible() const
{
    Q_D(const zchxMapLayer);
    return d->m_visible;
}

void zchxMapLayer::setVisible(bool visible)
{
    Q_D(zchxMapLayer);
    if(visible == d->m_visible)
        return;

    d->m_visible = visible;
    emit visibleChanged(d->m_visible);
}

bool zchxMapLayer::hasChildLayer() const
{
    Q_D(const zchxMapLayer);
    return !d->m_children.empty();
}

const std::list<std::shared_ptr<zchxMapLayer> > &zchxMapLayer::getChildLayers()
{
    Q_D(const zchxMapLayer);
    return d->m_children;
}

void zchxMapLayer::addChildLayer(std::shared_ptr<zchxMapLayer> layer)
{
    Q_D(zchxMapLayer);
    bool added = (std::find(d->m_children.begin(), d->m_children.end(), layer) != d->m_children.end());
    if(!added)
        d->m_children.push_back(layer);
}

void zchxMapLayer::removeChildLayer(std::shared_ptr<zchxMapLayer> layer)
{
    Q_D(zchxMapLayer);
    d->m_children.remove(layer);
}

std::shared_ptr<zchxMapLayer> zchxMapLayer::getChildLayer(const QString &type)
{
    Q_D(zchxMapLayer);
    for(std::shared_ptr<zchxMapLayer> layer : d->m_children)
    {
        if(layer->type() == type)
            return layer;
    }
    return NULL;
}

void zchxMapLayer::addElement(std::shared_ptr<DrawElement::Element> element)
{
    if(!element)
        return;

    Q_D(zchxMapLayer);
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

void zchxMapLayer::removeElement(std::shared_ptr<DrawElement::Element> element)
{
    if(!element)
        return;

    Q_D(zchxMapLayer);
    element->layer().reset();

    bool contained = (std::find(d->m_elements.begin(), d->m_elements.end(), element) != d->m_elements.end());
    if(contained)
        return;

    d->m_elements.remove(element);

//    update();
}

std::list<std::shared_ptr<DrawElement::Element> > zchxMapLayer::getElements()
{
    Q_D(zchxMapLayer);
    return d->m_elements;
}

void zchxMapLayer::update()
{
    Q_D(zchxMapLayer);
    if(!d->m_enableUpdate)
        return;

    if(d->m_drawWidget)
    {
        d->m_drawWidget->update();
    }
}

void zchxMapLayer::drawLayer(QPainter *painter)
{
    Q_D(zchxMapLayer);
    for(std::shared_ptr<DrawElement::Element> element : d->m_elements)
    {
        if(element.get())
            element->drawElement(painter);
    }
}

std::shared_ptr<DrawElement::Element> zchxMapLayer::pickUpElement(QPointF pos, const QGeoCoordinate &geoPos)
{
    Q_D(zchxMapLayer);
    for(std::shared_ptr<DrawElement::Element> element : d->m_elements)
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
            if(element->contains(pos))
                return element;
        }
    }
    return NULL;
}

std::shared_ptr<DrawElement::Element> zchxMapLayer::pickUpElement(const QString &id)
{
    Q_D(zchxMapLayer);
    for(std::shared_ptr<DrawElement::Element> element : d->m_elements)
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

std::shared_ptr<DrawElement::Element> zchxMapLayer::pickUpElement(QPointF pos)
{
    Q_D(zchxMapLayer);
    if(d->m_drawWidget->framework())
    {
        for(std::shared_ptr<DrawElement::Element> element : d->m_elements)
        {
            if(!element)
                return element;

            if(element->contains(d->m_drawWidget->framework(), 10, pos.x(), pos.y()) )
            {
                return element;
            }
        }
    }
    return NULL;
}

QPointF zchxMapLayer::convertToView(double lon, double lat)
{
    Q_D(zchxMapLayer);
    QPointF pos;
    if(d->m_drawWidget->framework())
    {
        Point2D posD = d->m_drawWidget->framework()->LatLon2Pixel(lat, lon);
        pos = QPointF(posD.x, posD.y);
    }
    return pos;
}

bool zchxMapLayer::clearElementState()
{
    bool cleared = false;
    Q_D(zchxMapLayer);
    for(std::shared_ptr<DrawElement::Element> e : d->m_elements)
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

bool zchxMapLayer::clearActiveState()
{
    bool cleared = false;
    Q_D(zchxMapLayer);
    for(std::shared_ptr<DrawElement::Element> element : d->m_elements)
    {
        if(!element)
            continue;
        if(element->getIsActive())
            cleared = true;
        element->setIsActive(false);
    }
    return cleared;
}

bool zchxMapLayer::clearFocusState()
{
    bool cleared = false;
    Q_D(zchxMapLayer);
    for(std::shared_ptr<DrawElement::Element> element : d->m_elements)
    {
        if(!element)
            continue;
        if(element->getIsFocus())
            cleared = true;
        element->setIsFocus(false);
    }
    return cleared;
}

bool zchxMapLayer::clearHoverState()
{
    bool cleared = false;
    Q_D(zchxMapLayer);
    for(std::shared_ptr<DrawElement::Element> element : d->m_elements)
    {
        if(!element)
            continue;
        if(element->getIsHover())
            cleared = true;
        element->setIsHover(false);
    }
    return cleared;
}

bool zchxMapLayer::getEnableUpdate() const
{
    Q_D(const zchxMapLayer);
    return d->m_enableUpdate;
}

void zchxMapLayer::setEnableUpdate(bool enableUpdate)
{
    Q_D(zchxMapLayer);
    d->m_enableUpdate = enableUpdate;
}

ZCHX::Data::ECDIS_PLUGIN_USE_MODELs zchxMapLayer::mode() const
{
    Q_D(const zchxMapLayer);
    return d->m_mode;
}

void zchxMapLayer::setMode(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL &mode, bool addOrRemove)
{
    Q_D(zchxMapLayer);
    if(addOrRemove)
        d->m_mode |= mode;
    else
        d->m_mode &= ~mode;
}

zchxMapLayerPrivate::zchxMapLayerPrivate()
    : m_visible(true)
    , m_enableUpdate(true)
    , m_drawWidget(0)
{

}

zchxMapLayerPrivate::zchxMapLayerPrivate(const QString &type, const QString &displayName, bool visible)
    : m_type(type)
    , m_displayName(displayName)
    , m_visible(visible)
    , m_enableUpdate(true)
    , m_drawWidget(0)
{

}

zchxMapLayerPrivate::zchxMapLayerPrivate(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible)
    : m_type(type)
    , m_displayName(displayName)
    , m_visible(visible)
    , m_enableUpdate(true)
    , m_drawWidget(drawWidget)
{

}
