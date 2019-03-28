#include "zchxMapLayer.h"
#include "zchxmaplayer_p.h"
#include "zchxmapframe.h"
#include <QGeoCoordinate>
#include "element/fixelement.h"
#include "element/moveelement.h"

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
    //检查父层的状态
    if(d->m_parent && !(d->m_parent->visible())) return;
    //先改变子层的状态,如果父层不可见
    if(d->m_children.size() > 0 && !visible){
        for(std::shared_ptr<MapLayer> child : d->m_children)
        {
            child->setVisible(visible);
        }
    }
    //改变当前层的状态
    if(visible == d->m_visible) return;
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
    if(!element) return;

    Q_D(MapLayer);
    //确保图元对应element
    if(d->m_drawWidget && d->m_drawWidget != element->view())
    {
        element->setView(d->m_drawWidget);
    }
    //检查图元是否添加过
    element->setLayer(d->m_type);
    if(d->m_elements.contains(element->getID()) && d->m_drawWidget){
        std::shared_ptr<Element> old = d->m_elements[element->getID()];
        if(old.get() == d->m_drawWidget->getCurrentSelectedElement())
        {
            d->m_drawWidget->setCurrentSelectedItem(element.get());
        }
    }

    d->m_elements[element->getID()] = element;
}

void MapLayer::addElements(QList<std::shared_ptr<Element> > &list, bool check)
{
//    qDebug()<<"total element size:"<<list.size()<<QDateTime::currentDateTime().toString("hh:mm:ss zzz");
//    QTime t;
//    t.start();
    Q_D(MapLayer);
    if(check)
    {
        //先将旧的图元设定为不刷新
        for(std::shared_ptr<Element> ele : d->m_elements)
        {
            ele->setIsUpdate(false);
        }
    }
    for(std::shared_ptr<Element> ele : list) {
        addElement(ele);
    }
    //删除没有更新的图元
    for(std::shared_ptr<Element> ele : d->m_elements)
    {
        if(!ele->getIsUpdate())
        {
            //qDebug()<<"remove element:"<<ele->getElementType()<<ele->getID();
            //删除数据之前，检查数据是否在其他地方是否被使用
            removeConcern(ele->getID());
            removeRealtimeTrack(ele->getID());
            removeHistoryTrack(ele->getID());
            removeExtrapolation(ele->getID());
            removeElement(ele);
        }
    }

//    qDebug()<<"total elements size:"<<d->m_elements.size();

//    for(std::shared_ptr<Element> ele : d->m_elements)
//    {
//        qDebug()<<"element:"<<ele->getID()<<FLOAT_STRING(ele->elelat, 6)<<FLOAT_STRING(ele->elelon, 6);
//    }
//    qDebug()<<"total element size:"<<list.size()<<QDateTime::currentDateTime().toString("hh:mm:ss zzz")<<t.elapsed();
}

void MapLayer::removeElement(std::shared_ptr<Element> element)
{
    if(!element)
        return;

    Q_D(MapLayer);
    //检查是否是当前选择的图元
    if( d->m_drawWidget && element.get() == d->m_drawWidget->getCurrentSelectedElement())
    {
        d->m_drawWidget->setCurrentSelectedItem(0);
    }
    d->m_elements.remove(element->getID());

//    bool contained = (std::find(d->m_elements.begin(), d->m_elements.end(), element) != d->m_elements.end());
//    if(!contained) return;



//    d->m_elements.remove(element);
}

void MapLayer::removeElement(const QString &id)
{
    Q_D(MapLayer);
    removeElement(getElement(id));
}

void MapLayer::removeAllElement()
{
    Q_D(MapLayer);
    d->m_elements.clear();
}

std::list<std::shared_ptr<Element> > MapLayer::getElements()
{
    Q_D(MapLayer);
    return d->m_elements.values().toStdList();
}

std::shared_ptr<Element> MapLayer::getElement(const QString &name)
{
    Q_D(MapLayer);
    return d->m_elements[name];
//    for(std::shared_ptr<Element> element : d->m_elements)
//    {
//        if(!element) continue;

//        if(element->getID() == name)
//        {
//            return element;
//        }
//    }

//    return 0;
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
    if(!visible()) return;
    //开始画当前图层的图元
    for(std::shared_ptr<Element> item : d->m_elements)
    {
        if(!item) continue;
        //更新各个element的状态
        QString id = item->getID();
        if(isConcern(id)){
            item->setIsConcern(true);
        } else {
            item->setIsConcern(false);
        }
        if(isRealtimeTrack(id)){
            item->setIsRealtimeTailTrack(true);
        } else {
            item->setIsRealtimeTailTrack(false);
        }
        if(isHistoryTrack(id)){
            item->setIsHistoryTrack(true);
        } else {
            item->setIsHistoryTrack(false);
        }
        if(d->m_drawWidget && item.get() == d->m_drawWidget->getCurrentSelectedElement()) {
            item->setIsActive(true);
        } else {
            item->setIsActive(false);
        }
        if(isExtrapolation(id)){
            item->setIsExtrapolate(true);
            item->setExtrapolateTime(getExtrapolationTime(id));
        } else {
            item->setIsExtrapolate(false);
        }

        if(item->isEmpty()) continue;
        item->drawElement(painter);
    }
}

std::shared_ptr<Element> MapLayer::pickUpElement(const QPoint& pos, const QGeoCoordinate &geoPos)
{
    Q_D(MapLayer);
    if(visible() && d->m_drawWidget && d->m_drawWidget->framework())
    {
        d->m_drawWidget->setCurrentSelectedItem(0);
        for(std::shared_ptr<Element> element : d->m_elements)
        {
            if(!element) continue;

            if(element->contains(pos) || element->contains(d->m_drawWidget->framework()->LatLon2Pixel(geoPos.latitude(), geoPos.longitude()).toPoint()))
            {
                d->m_drawWidget->setCurrentSelectedItem(element.get());
                return element;
            }
        }
    }
    return NULL;
}

std::shared_ptr<Element> MapLayer::pickUpElement(const QString &id)
{
    Q_D(MapLayer);
    if(visible() && d->m_drawWidget && d->m_drawWidget->framework())
    {
        d->m_drawWidget->setCurrentSelectedItem(0);
        for(std::shared_ptr<Element> element : d->m_elements)
        {
            if(!element) continue;

            if(element->getID() == id)
            {
                d->m_drawWidget->setCurrentSelectedItem(element.get());
                std::pair<double, double> ll = element->getLatLon();
                d->m_drawWidget->setCenterAtTargetLL(ll.first, ll.second);
                return element;
            }
        }
    }
    return NULL;
}

std::shared_ptr<Element> MapLayer::pickUpElement(const QPoint& pos)
{
    Q_D(MapLayer);
    if(visible() && d->m_drawWidget && d->m_drawWidget->framework())
    {
        d->m_drawWidget->setCurrentSelectedItem(0);
        std::shared_ptr<Element> ele = hoverElement(pos);
        if(ele) {
            d->m_drawWidget->setCurrentSelectedItem(ele.get());
        }
        return ele;
    }
    return NULL;
}

std::shared_ptr<Element> MapLayer::hoverElement(const QPoint& pos)
{
    Q_D(MapLayer);
    if(visible() && d->m_drawWidget && d->m_drawWidget->framework())
    {
        for(std::shared_ptr<Element> element : d->m_elements)
        {
            if(!element) continue;
            if(element->contains(pos))
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

void MapLayer::setParentLayer(std::shared_ptr<MapLayer> parent)
{
    Q_D(MapLayer);
    if(d->m_parent != parent)
        d->m_parent.reset(parent.get());
}

std::shared_ptr<MapLayer> MapLayer::getParentLayer() const
{
    Q_D(const MapLayer);
    return d->m_parent;
}

bool MapLayer::hasParentLayer() const
{
    Q_D(const MapLayer);
    return d->m_parent != NULL;
}

void MapLayer::setPickupAvailable(bool sts)
{
    Q_D(MapLayer);
    d->m_pickupAvailable = sts;
}

bool MapLayer::getPickupAvailable() const
{
    Q_D(const MapLayer);
    if(!d->m_drawWidget) return false;
    if(d->m_drawWidget->getCurPickupType() == ZCHX::Data::ECDIS_PICKUP_NONE) return false;
    return d->m_pickupAvailable;
}

int MapLayer::getMaxConcernNum() const
{
    Q_D(const MapLayer);
    return d->mConcernSetting.getMaxOptNum();
}

void MapLayer::setMaxConcernNum(int number)
{
    Q_D(MapLayer);
    d->mConcernSetting.setMaxOptNum(number);
}

void MapLayer::removeConcern(const QString& id)
{
    Q_D(MapLayer);
    if(d->mConcernSetting.contains(id)) d->mConcernSetting.remove(id);
}

bool MapLayer::isConcern(const QString& id) const
{
    Q_D(const MapLayer);
    return d->mConcernSetting.contains(id);
}

QStringList MapLayer::getConcernList()const
{
    Q_D(const MapLayer);
    return d->mConcernSetting.getList();
}

void MapLayer::setConcernReplace(bool replace)
{
    Q_D(MapLayer);
    d->mConcernSetting.setAutoReplace(replace);
}


bool MapLayer::appendConcern(const QString &id)
{
    Q_D(MapLayer);
    if(!d->mConcernSetting.append(id)){
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1个关注目标.\n%2未添加").arg(d->mConcernSetting.getMaxOptNum()).arg(id));
        return false;
    }
    return true;
}

int MapLayer::getMaxRealtimeTrackNum() const
{
    Q_D(const MapLayer);
    return d->mRealtimeTailTrackSetting.getMaxOptNum();
}

void MapLayer::setMaxRealtimeTrackNum(int number)
{
    Q_D(MapLayer);
    d->mRealtimeTailTrackSetting.setMaxOptNum(number);
}

void MapLayer::removeRealtimeTrack(const QString& id)
{
    Q_D(MapLayer);
    if(d->mRealtimeTailTrackSetting.contains(id)) d->mRealtimeTailTrackSetting.remove(id);
}

bool MapLayer::isRealtimeTrack(const QString& id) const
{
    Q_D(const MapLayer);
    return d->mRealtimeTailTrackSetting.contains(id);
}

QStringList MapLayer::getRealtimeTrackList()const
{
    Q_D(const MapLayer);
    return d->mRealtimeTailTrackSetting.getList();
}

void MapLayer::setRealtimeTrackReplace(bool replace)
{
    Q_D(MapLayer);
    d->mRealtimeTailTrackSetting.setAutoReplace(replace);
}


bool MapLayer::appendRealtimeTrack(const QString &id)
{
    Q_D(MapLayer);
    if(!d->mRealtimeTailTrackSetting.append(id)){
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1个实时轨迹.\n%2未添加").arg(d->mRealtimeTailTrackSetting.getMaxOptNum()).arg(id));
        return false;
    }
    return true;
}

int MapLayer::getMaxHistoryTrackNum() const
{
    Q_D(const MapLayer);
    return d->mHistoryTrackSetting.getMaxOptNum();
}

void MapLayer::setMaxHistoryTrackNum(int number)
{
    Q_D(MapLayer);
    d->mHistoryTrackSetting.setMaxOptNum(number);
}

void MapLayer::removeHistoryTrack(const QString& id)
{
    Q_D(MapLayer);
    if(d->mHistoryTrackSetting.contains(id)) d->mHistoryTrackSetting.remove(id);
}

bool MapLayer::isHistoryTrack(const QString& id) const
{
    Q_D(const MapLayer);
    return d->mHistoryTrackSetting.contains(id);
}

QStringList MapLayer::getHistoryTrackList()const
{
    Q_D(const MapLayer);
    return d->mHistoryTrackSetting.getList();
}

void MapLayer::setHistoryTrackReplace(bool replace)
{
    Q_D(MapLayer);
    d->mHistoryTrackSetting.setAutoReplace(replace);
}


bool MapLayer::appendHistoryTrack(const QString &id)
{
    Q_D(MapLayer);
    if(!d->mHistoryTrackSetting.append(id)) {
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1条历史轨迹.\n%2未添加").arg(d->mHistoryTrackSetting.mMaxNum).arg(id));
        return false;
    }
    return true;
}

int MapLayer::getMaxExtrapolationNum() const
{
    Q_D(const MapLayer);
    return d->mExtrapolationSetting.getMaxOptNum();
}

void MapLayer::setMaxExtrapolationNum(int number)
{
    Q_D(MapLayer);
    d->mExtrapolationSetting.setMaxOptNum(number);
}


bool MapLayer::isExtrapolation(const QString& id) const
{
    Q_D(const MapLayer);
    return d->mExtrapolationSetting.contains(ZCHX::Data::ExtrapolateParam(id));
}

ZCHX::Data::ExtrapolateList MapLayer::getExtrapolationList() const
{
    Q_D(const MapLayer);
    return d->mExtrapolationSetting.getList();
}

void MapLayer::setExtrapolationReplace(bool replace)
{
    Q_D(MapLayer);
    d->mExtrapolationSetting.setAutoReplace(replace);
}

void MapLayer::removeExtrapolation(const QString& id)
{
    Q_D(MapLayer);
    if(d->mExtrapolationSetting.contains(ZCHX::Data::ExtrapolateParam(id)))
    {
        d->mExtrapolationSetting.remove(ZCHX::Data::ExtrapolateParam(id));
    }
}

bool MapLayer::appendExtrapolation(const QString &id, double val)
{
    Q_D(MapLayer);\
    if(!(d->mExtrapolationSetting.append(ZCHX::Data::ExtrapolateParam(id, val))))
    {
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1个预推目标.\n%2未添加").arg(d->mExtrapolationSetting.getMaxOptNum()).arg(id));
        return false;
    }
    return true;
}


double MapLayer::getExtrapolationTime(const QString& id) const
{
    Q_D(const MapLayer);
    int index = d->mExtrapolationSetting.indexOf(ZCHX::Data::ExtrapolateParam(id));
    if(index < 0) return 0;
    ZCHX::Data::ExtrapolateParam param = d->mExtrapolationSetting.valueAt(index);
    return param.mVal;
}

void MapLayer::updateExtrapolationTime(const QString& id, double val)
{
    Q_D(MapLayer);
    int index = d->mExtrapolationSetting.indexOf(ZCHX::Data::ExtrapolateParam(id));
    if(index < 0) return;
    ZCHX::Data::ExtrapolateParam& param = d->mExtrapolationSetting.valueAt(index);
    param.mVal = val;
}

zchxMapWidget* MapLayer::getDrawWidget() const
{
    Q_D(const MapLayer);
    return d->m_drawWidget;
}



MapLayerPrivate::MapLayerPrivate()
    : m_visible(true)
    , m_enableUpdate(true)
    , m_drawWidget(0)
    , m_parent(0)
    , m_pickupAvailable(true)
{

}

MapLayerPrivate::MapLayerPrivate(const QString &type, const QString &displayName, bool visible)
    : m_type(type)
    , m_displayName(displayName)
    , m_visible(visible)
    , m_enableUpdate(true)
    , m_drawWidget(0)
    , m_parent(0)
    , m_pickupAvailable(true)
{

}

MapLayerPrivate::MapLayerPrivate(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible)
    : m_type(type)
    , m_displayName(displayName)
    , m_visible(visible)
    , m_enableUpdate(true)
    , m_drawWidget(drawWidget)
    , m_parent(0)
    , m_pickupAvailable(true)
{

}
}
