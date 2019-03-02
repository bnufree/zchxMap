#include "zchxaisdatamgr.h"
#include "zchxmapframe.h"

namespace qt {
zchxAisDataMgr::zchxAisDataMgr(zchxMapWidget* w, QObject *parent) : zchxEcdisDataMgr(w, DATA_MGR_AIS, parent)
  , mSelHistoryPointIndex(-1)
  , mSelHistoryTrackID("")
{
    mMaxConcernNum = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_CONCERN_NUM, 10).toInt();
    mReplaceConcernWhenOver = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_CONCERN, true).toBool();
    mMaxRealtimeTailTrackNum = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_TAIL_TRACK_NUM, 10).toInt();
    mReplaceRealtimeTailTrackWhenOver = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_TAIL_TRACK, true).toBool();
    mMaxHistoryTrackNum = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_HISTORY_TRACK_NUM, 10).toInt();
    mReplaceHistoryTrackWhenOver = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_HISTORY_TRACK, true).toBool();
    mMaxExtrapolationNum = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_EXTRAPOLATE_NUM, 10).toInt();
    mReplaceExtrapolationWhenOver = Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_EXTRAPOLATE, true).toBool();
    mShipTagDisplayMode = SHIP_ITEM_DEFAULT;
}

void zchxAisDataMgr::SetEnableShipTag(int val)
{
    mShipTagDisplayMode = val;
}

void zchxAisDataMgr::setHistoryTrackStyle(const QString &color, const int lineWidth)
{
    m_sHistoryTrackStyle = color;
    m_iHistoryTrackWidth = lineWidth;
}

void zchxAisDataMgr::setPrepushTrackStyle(const QString &color, const int lineWidth)
{
    m_sPrepushTrackStyle = color;
    m_iPrepushTrackWidth = lineWidth;
}

void zchxAisDataMgr::show(QPainter* painter, double offset_x, double offset_y)
{
    if(!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_AIS)) return;
    if(m_aisMap.size() == 0) return;

    PainterPair chk(painter);
    int curScale = mDisplayWidget->zoom() < 7 ? 5 : 10;
    int sideLen = 10;

    QHash<QString, std::shared_ptr<AisElement>>::iterator it = m_aisMap.begin();
    for(; it != m_aisMap.end(); ++it)
    {
        std::shared_ptr<AisElement> item = it.value();
        //更新状态
        ZCHX::Data::ITF_AIS aisdata = item->getData();
        if(isConcern(aisdata.id)){
            item->setIsConcern(true);
        } else {
            item->setIsConcern(false);
        }
        if(isRealtimeTailTrack(aisdata.id)){
            item->setIsRealtimeTailTrack(true);
        } else {
            item->setIsRealtimeTailTrack(false);
        }
        if(isHistoryTrack(aisdata.id)){
            item->setIsHistoryTrack(true);
        } else {
            item->setIsHistoryTrack(false);
        }
        if(item.get() == mDisplayWidget->getCurrentSelectedElement()) {
            item->setIsActive(true);
        } else {
            item->setIsActive(false);
        }
        if(isExtrapolation(aisdata.id)){
            item->setIsExtrapolate(true);
            item->setExtrapolateTime(getExtrapolationTime(aisdata.id));
        } else {
            item->setIsExtrapolate(false);
        }
        if(aisdata.is_construction_ship)
        {
            item->setForceImage(true);
            item->setDrawTargetInfo(false);
        }

        if(item->isEmpty()) continue;
        QPointF pos = item->getCurrentPos();
        if(!mDisplayWidget->rect().contains(pos.toPoint())) continue;
        item->updateGeometry(pos, curScale);
        item->setHistoryTrackStyle(m_sHistoryTrackStyle, m_iHistoryTrackWidth);

        //一般船舶显示
        if(MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_AIS_CURRENT) && item->getData().cargoType != 55)
        {
            item->drawFlashRegion(painter, pos, item->getData().warn_status, item->getData().warnStatusColor);
            if(item->getType() == RADARPLAN)
            {
                if(item->getIsActive())
                {
                    PainterPair chk1(painter);
                    painter->setPen(Qt::red);
                    painter->drawRect(pos.x()-curScale-2,pos.y()-curScale-2,curScale+6,curScale+6);
                }
                PainterPair chk2(painter);
                painter->setBrush(Qt::yellow);
                QRectF rect(pos.x()-curScale,pos.y()-curScale,curScale,curScale);
                painter->drawRect(rect);
            }
            //船
            else if(item->getType() == RADARSHIP || item->getType() == 3)//3为融合数据
            {
                item->drawElement(painter);
                item->drawActive(painter);
                item->drawTargetInformation(mShipTagDisplayMode, painter);

                QString targetId = item->getData().objCollide.id;
                if (m_aisMap.contains(targetId))
                {
                    item->drawCollide(m_aisMap.value(targetId)->getData(), painter);
                }
                item->drawFocus(painter);

                //绘制交汇
                if(item->getData().RadarMeetVec.size() > 0)
                {
                    if(mDisplayWidget->getIsOpenMeet())
                    {
                        PainterPair chk2(painter);
                        QPen pen(Qt::red,2,Qt::DashLine);
                        painter->setPen(pen);
                        uint time_hour = 0;
                        uint time_minute = 0;
                        uint time_second = 0;
                        for(int j = 0; j < item->getData().RadarMeetVec.size(); j++)
                        {
                            ZCHX::Data::RadarMeet meetItem = item->getData().RadarMeetVec.at(j);
                            Point2D meetPos = item->framework()->LatLon2Pixel(meetItem.lat, meetItem.lon);
                            time_hour = meetItem.UTC / 3600;
                            time_minute = meetItem.UTC / 60 - time_hour * 60;
                            time_second = meetItem.UTC % 60;
                            QString str = tr("Time ")+QString::number(time_hour)+tr("H ") + QString::number(time_minute)+ tr("M ")+ \
                                    QString::number(time_second)+tr("S; Distance: ")+QString::number(meetItem.disrance,'f',3)+"m";

                            painter->drawLine(pos.x(),pos.y(),meetPos.x,meetPos.y);
                            painter->drawText(pos.x()-10,pos.y()-sideLen/2, str);
                        }
                    }
                }
            }
        }
        //执法船显示
        if(MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_AIS_LAW) && item->getData().cargoType == 55)
        {
            item->drawFlashRegion(painter, pos, item->getData().warn_status, item->getData().warnStatusColor);
            item->drawElement(painter);
            item->drawTargetInformation(mShipTagDisplayMode,painter);
            item->drawActive(painter);
            item->drawFocus(painter);
        }
        //绘制船舶轨迹点  横琴项目
        if(MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_AIS_TRACK))
        {
            std::vector<QPointF> pts = item->getTrack();
            PainterPair chk2(painter);
            painter->setPen(QPen(Qt::black,3,Qt::DashLine));
            painter->drawPolyline(&pts[0],pts.size());
        }
        //显示海缆的触地点
        if(MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_AIS_CABLE_TOUCHDOWN))
        {
            std::vector<QPointF> pts = item->getTouchdown();
            if(pts.size() > 0)
            {
                PainterPair chk2(painter);
                painter->setPen(QPen(Qt::green,3,Qt::DashLine));
                painter->drawPolyline(&pts[0],pts.size());
            }
        }
    }

}

void zchxAisDataMgr::clearHistoryTrackSel()
{
    mSelHistoryPointIndex = -1;
    mSelHistoryTrackID = "";
}

bool zchxAisDataMgr::updateActiveItem(const QPoint &pt)
{
    if(!MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_AIS)) return false;
    int type = mDisplayWidget->getCurPickupType();
    if(type != ZCHX::Data::ECDIS_PICKUP_AIS && type != ZCHX::Data::ECDIS_PICKUP_ALL ) return false;

    //重置历史轨迹点的显示
    clearHistoryTrackSel();
    //检查AIS图元本身是否选中
    Element* ele = selectItem(pt);
    if(ele)
    {
        mDisplayWidget->setCurrentSelectedItem(ele);
        return true;
    }
    //检查轨迹是否选中
    foreach(std::shared_ptr<AisElement> item, m_aisMap)
    {
        if(isHistoryTrack(item->getData().id))
        {
            int size = item->getHistoryTrackList().size();
            for(int i = 0; i < size; ++i)
            {
                ZCHX::Data::ITF_AIS ais = item->getHistoryTrackList().at(i);
                std::shared_ptr<AisElement> ele(new AisElement(ais, mDisplayWidget->framework()));
                if(ele->contains(pt)){
                    mSelHistoryPointIndex = i;
                    mSelHistoryTrackID = item->getData().id;
                    qDebug()<<"id:"<<item->getData().id<<" history contains:"<<true;
                    break;
                }
            }
        }
        if(mSelHistoryPointIndex >= 0) return true;
    }
    return false;
}

Element* zchxAisDataMgr::selectItem(const QPoint &pt)
{
    foreach(std::shared_ptr<AisElement> item, m_aisMap)
    {
        //检查AIS图元本身是否选中
        if(item->contains(10, pt.x() * 1.0, pt.y() * 1.0))
        {
            return item.get();
        }
    }
    return 0;
}

bool zchxAisDataMgr::setRealtimeTailTrack(const QString &id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    if(!isRealtimeTailTrack(id)) return false;
    //更新图元的历史轨迹数据
    std::shared_ptr<AisElement> item = m_aisMap.value(id, NULL);
    if(item)
    {
        item->setRealtimeTailTrackList(data);
        return true;
    }
    return false;
}


bool zchxAisDataMgr::setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    if(!isHistoryTrack(id)) return false;
    //更新图元的历史轨迹数据
    std::shared_ptr<AisElement> item = m_aisMap.value(id, NULL);
    if(item)
    {
        item->setHistoryTrackList(data);
        return true;
    }
    return false;
}

void zchxAisDataMgr::removeAisHistoryData(QString id)
{
    removeHistoryTrack(id);
}

void zchxAisDataMgr::removeRealtimeTailTrack(const QString &id)
{
    if(isRealtimeTailTrack(id))
    {
        std::shared_ptr<AisElement> item = m_aisMap.value(id, 0);
        if(item) item->clearRealtimeTailTrackList();
        zchxEcdisDataMgr::removeRealtimeTailTrack(id);
        //emit mDisplayWidget->signalSendHistoryTrail(id, false);
    }
}

void zchxAisDataMgr::removeHistoryTrack(const QString &id)
{
    if(isHistoryTrack(id))
    {
        std::shared_ptr<AisElement> item = m_aisMap.value(id, 0);
        if(item) item->clearHistoryTrackList();
        zchxEcdisDataMgr::removeHistoryTrack(id);
        //emit mDisplayWidget->signalSendHistoryTrail(id, false);
    }
}

void zchxAisDataMgr::removeConcern(const QString &id)
{
    if(isConcern(id))
    {
        zchxEcdisDataMgr::removeConcern(id);
    }
}


void zchxAisDataMgr::SetPickUpAisInfo(QString id)
{
    mDisplayWidget->setCurrentSelectedItem(0);
    mDisplayWidget->setCurPickupType(ZCHX::Data::ECDIS_PICKUP_AIS);
    //将当前所有的船舶选择状态清空
    QHash<QString, std::shared_ptr<AisElement>>::const_iterator it = m_aisMap.begin();
    for(; it != m_aisMap.end(); ++it)
    {
        it.value()->setIsActive(false);
    }
    //将当前id的船舶设定为active状态
    if(id.isEmpty()) return;
    std::shared_ptr<AisElement> item = m_aisMap.value(id, NULL);
    if(!item) return;
    item->setIsActive(true);
    //将当前的船舶居中放大显示
    std::pair<double, double> ll = item->getLatLon();
    mDisplayWidget->setCenterAtTargetLL(ll.first, ll.second);
    //更新当前选择的图元目标
    mDisplayWidget->setCurrentSelectedItem(item.get());
}

void zchxAisDataMgr::setAisData(const QList<ZCHX::Data::ITF_AIS> &data, bool check )
{
    if(check)
    {
        //删除当前实时数据中不存在的旧数据
        QList<QString> newDataId;
        for(const ZCHX::Data::ITF_AIS &aisdatas : data)
        {
            newDataId.append(aisdatas.id);
        }
        QList<QString> oldDataId = m_aisMap.uniqueKeys();
        for(int number = 0; number < oldDataId.size(); ++number)
        {
            QString dataShipId = oldDataId.at(number);
            if(!newDataId.contains(dataShipId))
            {
                //取消原来数据的关注和尾迹
                removeConcern(dataShipId);
                removeRealtimeTailTrack(dataShipId);
                removeHistoryTrack(dataShipId);
                removeExtrapolation(dataShipId);
                std::shared_ptr<AisElement> item = m_aisMap.value(dataShipId, 0);
                if(item){
                    //如果是当前选择的目标,则将目标清空
                    if(item.get() == mDisplayWidget->getCurrentSelectedElement())
                    {
                        mDisplayWidget->setCurrentSelectedItem(0);
                    }
                    item.reset();
                }
                //删除对应的实时数据
                m_aisMap.remove(dataShipId);
            }
        }
    }

    //更新实时AIS数据
    for (const ZCHX::Data::ITF_AIS &aisdata : data)
    {
        //更新item对应的数据
        std::shared_ptr<AisElement> item = m_aisMap.value(aisdata.id, 0);
        if(!item) {
            item = std::shared_ptr<AisElement>(new AisElement(aisdata, mDisplayWidget->framework()));
            item->setFrameWork(mDisplayWidget->framework());
            m_aisMap[aisdata.id] = item;
        } else {
            item->setData(aisdata);
        }
        item->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
    }
#if 0
    if (m_bCameraTargerTrack &&m_cameraTrackTarget.type == 1)
    {
        AisElement *item = m_aisMap.value(m_cameraTrackTarget.id, NULL);
        if(item)
        {
                m_cameraTrackTarget.id = item->getStrID(); //设置当前选中的船舶的
                m_cameraTrackTarget.lon = item->lon();
                m_cameraTrackTarget.lat = item->lat();
                m_cameraTrackTarget.type = 1;
                emit signalIsEcdisCameraTrackTarget(m_cameraTrackTarget);
                //releaseDrawStatus();
        }
        else // 目标丢失(千米目标丢失的给移除了)
        {
            m_cameraTrackTarget.type = 0;
            //m_bCameraTargerTrack = false;
        }
    }
#endif
    //TODO: 判断是否需要移除过长时间没有数据更新的对象
}

//将历史和实时的数据进行合并,切换显示模式的时候 清除对应的数据
void zchxAisDataMgr::setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    setAisData(data);
}

void zchxAisDataMgr::setConsAisData(const ZCHX::Data::ITF_AIS &aisdata)
{
    setAisData(QList<ZCHX::Data::ITF_AIS>() <<aisdata, false);
}

void zchxAisDataMgr::setClearHistoryData(bool states)
{
    if(states)
    {
        //清空历史数据
        m_aisMap.clear();
        //m_HistoryRadarPoint.clear();
    }
    else
    {
        //清空历史尾迹
        while (mHistoryTrackList.size() > 0) {
            removeHistoryTrack(mHistoryTrackList.first());
        }
    }
}

QList<QAction*> zchxAisDataMgr::getRightMenuActions(const QPoint &pt)
{
    QList<QAction*> list;
    //获取当前选择的目标对象
    if(mDisplayWidget)
    {
        Element* item = mDisplayWidget->getCurrentSelectedElement();
        if(item && item->getElementType() == ZCHX::Data::ELEMENT_AIS)
        {
            //目标确定为AIS,弹出对应的右键菜单
            AisElement* ele = static_cast<AisElement*>(item);
            if(ele){
                if(ele->hasCamera())
                {
                    list.append(addAction(tr("相机列表"),this, SLOT(slotOpenCameraList()), (void*) ele));
                }
                list.append(addAction(tr("画中画"),this, SLOT(setPictureInPicture()), (void*) ele));
                list.append(addAction(tr("船队"),this, SLOT(setFleet()), (void*) ele));
                list.append(addAction(tr("模拟外推"),this, SLOT(setSimulationExtrapolation()), (void*) ele));
                list.append(addAction(tr("历史轨迹"),this, SLOT(setHistoryTraces()), (void*) ele));
                list.append(addAction(tr("实时轨迹"),this, SLOT(setRealTimeTraces()), (void*) ele));
                list.append(addAction(tr("黑名单"),this, SLOT(setBlackList()), (void*) ele));
                list.append(addAction(tr("白名单"),this, SLOT(setWhiteList()), (void*) ele));
                list.append(addAction(tr("CPA跟踪"),this, SLOT(setCPATrack()), (void*) ele));
                list.append(addAction(tr("关注"),this, SLOT(setConcern()), (void*) ele));
                list.append(addAction(tr("联动"),this, SLOT(invokeLinkageSpot()), (void*) ele));
            }
        }
    }
    return list;
}

void zchxAisDataMgr::slotOpenCameraList()
{

}

void zchxAisDataMgr::setPictureInPicture()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    if(mDisplayWidget)
    {
        mDisplayWidget->signalSendPictureInPictureTarget(ele->getElementType(), ele->getData().id);
    }
}

void zchxAisDataMgr::setFleet()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    emit signalFleet(ele->getData());
}

void zchxAisDataMgr::setSimulationExtrapolation()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    QString id = ele->getData().id;
    if(isExtrapolation(id))
    {
        removeExtrapolation(id);
    } else
    {
        appendExtrapolationList(QStringList()<<id, true);
    }
    emit signalShipExtrapolation(id, isExtrapolation(id));
}

void zchxAisDataMgr::setHistoryTraces()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    QString id = ele->getData().id;
    if(isHistoryTrack(id))
    {
        removeHistoryTrack(id);
    } else
    {
        appendHistoryTrackList(QStringList()<<id, true);
    }
    if(mDisplayWidget) mDisplayWidget->signalSendHistoryTrail(id, isHistoryTrack(id));
}

void zchxAisDataMgr::setRealTimeTraces()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    QString id = ele->getData().id;
    if(isRealtimeTailTrack(id))
    {
        removeRealtimeTailTrack(id);
    } else
    {
        appendRealtimeTailTrackList(QStringList()<<id, true);
    }
    if(mDisplayWidget) mDisplayWidget->signalSendRealTimeTrail(id, isRealtimeTailTrack(id));
}

void zchxAisDataMgr::setBlackList()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    QString id = ele->getData().id;
    emit signalCreateBlackOrWhiteList(id, ZCHX::Data::SHIP_BW_BLACK);
}

void zchxAisDataMgr::setWhiteList()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    QString id = ele->getData().id;
    emit signalCreateBlackOrWhiteList(id, ZCHX::Data::SHIP_BW_WHITE);
}

void zchxAisDataMgr::setCPATrack()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    QString id = ele->getData().id;
    emit signalCreateCPATrack(id);
}

void zchxAisDataMgr::invokeLinkageSpot()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    ZCHX::Data::ITF_CloudHotSpot data;
    data.fllow = ZCHX::Data::ITF_CloudHotSpot::FLLOW_TYPE_LINKAGE_TRACKING;
    data.mode = ZCHX::Data::ITF_CloudHotSpot::MODE_HANDLE;
    data.targetNumber = ele->getData().id;
    data.targetType = 1;
    data.targetLon = ele->getData().lon;
    data.targetLat = ele->getData().lat;
    if(mDisplayWidget) mDisplayWidget->signalInvokeHotSpot(data);
}

void zchxAisDataMgr::setConcern()
{
    AisElement* ele = static_cast<AisElement*>(getElementOfSender());
    if(!ele) return;
    QString id = ele->getData().id;
    appendConcernList(QStringList()<<id, true);
}

void zchxAisDataMgr::updateCamera(const QList<ZCHX::Data::ITF_CameraDev> &list)
{
    //更新item对应的数据
    foreach (ZCHX::Data::ITF_CameraDev data, list) {
        std::shared_ptr<AisElement> item = m_aisMap.value(data.mParentEleID, 0);
        if(item) {
            item->appendCamera(data);
        }
    }
}


}

