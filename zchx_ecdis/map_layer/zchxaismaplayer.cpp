#include "zchxaismaplayer.h"
#include "profiles.h"

using namespace qt;
zchxAisMapLayer::zchxAisMapLayer(zchxMapWidget *drawWidget, bool visible, QObject *parent)
    : MapLayer(drawWidget, ZCHX::LAYER_AIS, ZCHX::TR_LAYER_AIS, visible, parent)
    , mShipTagDisplayMode(SHIP_ITEM_DEFAULT)
{
    setMaxConcernNum(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_CONCERN_NUM, 10).toInt());
    setConcernReplace(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_CONCERN, true).toBool());
    setMaxRealtimeTrackNum(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_TAIL_TRACK_NUM, 10).toInt());
    setRealtimeTrackReplace(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_TAIL_TRACK, true).toBool());
    setMaxExtrapolationNum(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_EXTRAPOLATE_NUM, 10).toInt());
    setExtrapolationReplace(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_EXTRAPOLATE, true).toBool());
    setMaxHistoryTrackNum(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_HISTORY_TRACK_NUM, 10).toInt());
    setHistoryTrackReplace(Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_HISTORY_TRACK, true).toBool());
}

void zchxAisMapLayer::drawLayer(QPainter *painter)
{
    if(!visible()) return;
    //开始画当前图层的图元
    for(std::shared_ptr<Element> ele : getElements())
    {
        if(!ele) continue;
        AisElement *item = dynamic_cast<AisElement*> (ele.get());
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
        if(getDrawWidget() && item == getDrawWidget()->getCurrentSelectedElement()) {
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
        item->setPrepushTrackStyle(m_sPrepushTrackStyle,  m_iPrepushTrackWidth);
        item->setLabelDisplayMode(SHIP_ITEMs(mShipTagDisplayMode));
        item->setHistoryTrackStyle(m_sHistoryTrackStyle, m_iHistoryTrackWidth);
        //检查是否有碰撞的情况
        if(!item->data().objCollide.id.isEmpty())
        {
            AisElement* collide = dynamic_cast<AisElement*>(getElement(item->data().objCollide.id).get());
            if(collide) item->setCollideAis(collide->data());
        }
        if(item->isEmpty()) continue;
        item->drawElement(painter);
    }
}

void zchxAisMapLayer::setRealtimeTailTrack(const QString &id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    if(isRealtimeTrack(id))
    {
        AisElement* item = dynamic_cast<AisElement*>(getElement(id).get());
        if(item) item->setRealtimeTailTrackList(data);
    }
}

void zchxAisMapLayer::SetEnableShipTag(int val)
{
    mShipTagDisplayMode = val;
}

void zchxAisMapLayer::setHistoryTrackStyle(const QString &color, const int lineWidth)
{
    m_sHistoryTrackStyle = color;
    m_iHistoryTrackWidth = lineWidth;
}

void zchxAisMapLayer::setPrepushTrackStyle(const QString &color, const int lineWidth)
{
    m_sPrepushTrackStyle = color;
    m_iPrepushTrackWidth = lineWidth;
}


void zchxAisMapLayer::removeRealtimeTailTrack(const QString &id)
{
    if(isRealtimeTrack(id))
    {
        AisElement* item = dynamic_cast<AisElement*>(getElement(id).get());
        if(item) item->clearRealtimeTailTrackList();
        removeRealtimeTrack(id);
        //emit mDisplayWidget->signalSendHistoryTrail(id, false);
    }
}


void zchxAisMapLayer::setPickUpAisInfo(const QString& id)
{
    MapLayer::pickUpElement(id);
}

void zchxAisMapLayer::setAisData(const QList<ZCHX::Data::ITF_AIS> &list, bool check)
{
    QList<std::shared_ptr<Element>> res;
    //更新实时AIS数据
    for (const ZCHX::Data::ITF_AIS &data : list)
    {
        //更新item对应的数据
        std::shared_ptr<AisElement> ele(new AisElement(data, getDrawWidget()));
        ele->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
        res.push_back(ele);
    }
    addElements(res, check);
}

//将历史和实时的数据进行合并,切换显示模式的时候 清除对应的数据
void zchxAisMapLayer::setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    setAisData(data, true);
}

void zchxAisMapLayer::setConsAisData(const ZCHX::Data::ITF_AIS &aisdata)
{
    setAisData(QList<ZCHX::Data::ITF_AIS>() <<aisdata, false);
}

void zchxAisMapLayer::updateCamera(const QList<ZCHX::Data::ITF_CameraDev> &list)
{
    //更新item对应的数据
    foreach (ZCHX::Data::ITF_CameraDev data, list) {
        AisElement *ele = dynamic_cast<AisElement*>(getElement(data.mParentEleID).get());
        if(ele) {
            ele->appendCamera(data);
        }
    }
}

void zchxAisMapLayer::setSingleAisData(const QString& id, const QList<ZCHX::Data::ITF_AIS> &list)
{
    if(!isHistoryTrack(id)) return ;
    //将历史轨迹数据转化为子图元保存
    std::shared_ptr<Element> ele = getElement(id);
    if(!ele) return;
    ele->removeChildren(ZCHX::Data::ELE_AIS_HISTORY_TRACK);
    //更新图元的历史轨迹数据
    ele->addChild(std::shared_ptr<AisHistoryElement>(new AisHistoryElement(list, getDrawWidget())));
}

void zchxAisMapLayer::removeAisHistoryData(const QString& id)
{
   removeHistoryTrack(id);
}

void zchxAisMapLayer::removeHistoryTrack(const QString &id)
{
    if(isHistoryTrack(id))
    {
        //取得id对应的图元
        std::shared_ptr<Element> target = getElement(id);
        if(target){
            //删除对于的历史轨迹图元
            target->removeChildren(ZCHX::Data::ELE_AIS_HISTORY_TRACK);
        }
        MapLayer::removeHistoryTrack(id);
        //emit mDisplayWidget->signalSendHistoryTrail(id, false);
    }
}

