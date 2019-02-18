#include "zchxaisdatamgr.h"
#include "zchxmapframe.h"

zchxAisDataMgr::zchxAisDataMgr(zchxMapWidget* w, QObject *parent) : zchxEcdisDataMgr(w, ZCHX_DATA_MGR_AIS, parent)
{
    mMaxConcernNum = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_CONCERN_NUM, 10).toInt();
    mMaxTailTrackNum = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_TAILTRACK_NUM, 10).toInt();
    mReplaceTrackWherOver = zchxEcdis::Profiles::instance()->value(AIS_DISPLAY_SETTING, AIS_REPLACE_TRACK, true).toBool();
    mShipTagDisplayMode = DrawElement::SHIP_ITEM_DEFAULT;
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

void zchxAisDataMgr::show(QPainter *painter)
{
    if(!mDisplayWidget || !mDisplayWidget->getMapLayerMgr()) return;
    if(!mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_AIS)) return;
    if(m_aisMap.size() == 0) return;

    PainterPair chk(painter);
    int curScale = mDisplayWidget->zoom() < 7 ? 5 : 10;
    int sideLen = 10;

    QHash<QString, std::shared_ptr<DrawElement::AisElement>>::iterator it = m_aisMap.begin();
    for(; it != m_aisMap.end(); ++it)
    {
        std::shared_ptr<DrawElement::AisElement> item = it.value();
        if(item->isEmpty()) continue;
        QPointF pos = item->getCurrentPos();
        if(!mDisplayWidget->rect().contains(pos.toPoint())) continue;
        item->updateGeometry(pos, curScale);
        item->setHistoryTrackStyle(m_sHistoryTrackStyle, m_iHistoryTrackWidth);

        //一般船舶显示
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_AIS_CURRENT) && item->getData().cargoType != 55)
        {
            item->drawFlashRegion(painter, pos, item->getData().warn_status, item->getData().warnStatusColor);
            if(item->getType() == DrawElement::RADARPLAN)
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
            else if(item->getType() == DrawElement::RADARSHIP || item->getType() == 3)//3为融合数据
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
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_AIS_LAW) && item->getData().cargoType == 55)
        {
            item->drawFlashRegion(painter, pos, item->getData().warn_status, item->getData().warnStatusColor);
            item->drawElement(painter);
            item->drawTargetInformation(mShipTagDisplayMode,painter);
            item->drawActive(painter);
            item->drawFocus(painter);
        }
        //绘制船舶轨迹点  横琴项目
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_AIS_TRACK))
        {
            std::vector<QPointF> pts = item->getTrack();
            PainterPair chk2(painter);
            painter->setPen(QPen(Qt::black,3,Qt::DashLine));
            painter->drawPolyline(&pts[0],pts.size());
        }
        //显示海缆的触地点
        if(mDisplayWidget->getMapLayerMgr()->isLayerVisible(ZCHX::LAYER_AIS_CABLE_TOUCHDOWN))
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

bool zchxAisDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}


bool zchxAisDataMgr::setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    if(!isTrack(id)) return false;
    std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(id, NULL);
    if(item)
    {
        item->setTrackList(data);
        return true;
    }
    return false;
}

void zchxAisDataMgr::removeAisHistoryData(QString id)
{
    removeTrack(id);
}

void zchxAisDataMgr::removeTrack(const QString &id)
{
    if(isTrack(id))
    {
        std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(id, 0);
        if(item) item->clearTrackList();
        zchxEcdisDataMgr::removeTrack(id);
        emit mDisplayWidget->signalSendHistoryTrail(id, false);
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
    QHash<QString, std::shared_ptr<DrawElement::AisElement>>::const_iterator it = m_aisMap.begin();
    for(; it != m_aisMap.end(); ++it)
    {
        it.value()->setIsActive(false);
    }
    //将当前id的船舶设定为active状态
    if(id.isEmpty()) return;
    std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(id, NULL);
    if(!item) return;
    item->setIsActive(true);
    //将当前的船舶居中放大显示
    std::pair<double, double> ll = item->getLatLon();
    mDisplayWidget->setCenterAtTargetLL(ll.first, ll.second);
    //更新当前选择的图元目标
    mDisplayWidget->setCurrentSelectedItem(item);
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
                removeTrack(dataShipId);
                std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(dataShipId, 0);
                if(item) item.reset();
                //删除对应的实时数据
                m_aisMap.remove(dataShipId);
            }
        }
    }

    //更新实时AIS数据
    for (const ZCHX::Data::ITF_AIS &aisdata : data)
    {
        //更新item对应的数据
        std::shared_ptr<DrawElement::AisElement> item = m_aisMap.value(aisdata.id, 0);
        if(!item) {
            item = std::shared_ptr<DrawElement::AisElement>(new DrawElement::AisElement(aisdata));
            item->setFrameWork(mDisplayWidget->framework());
        } else {
            item->setData(aisdata);
        }
        if(isConcern(aisdata.id)){
            item->setIsConcern(true);
        } else {
            item->setIsConcern(false);
        }
        if(isTrack(aisdata.id)){
            item->setIsTailTrack(true);
        } else {
            item->setIsTailTrack(false);
        }
        if(item == getCurrentAis()) {
            item->setIsActive(true);
        } else {
            item->setIsActive(false);
        }
        if(aisdata.is_construction_ship)
        {
            item->setForceImage(true);
            item->setDrawTargetInfo(false);
        }
        item->setUpdateUTC(QDateTime::currentMSecsSinceEpoch());
    }
#if 0
    if (m_bCameraTargerTrack &&m_cameraTrackTarget.type == 1)
    {
        DrawElement::AisElement *item = m_aisMap.value(m_cameraTrackTarget.id, NULL);
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


std::shared_ptr<DrawElement::AisElement> zchxAisDataMgr::getCurrentAis()
{
    if(!mDisplayWidget || !mDisplayWidget->getCurrentSelectedElement()) return NULL;
    std::shared_ptr<DrawElement::Element> item = mDisplayWidget->getCurrentSelectedElement();
    if(item->getElementType() != ZCHX::Data::ELEMENT_AIS) return NULL;
    return std::shared_ptr<DrawElement::AisElement>(static_cast<DrawElement::AisElement*>(item.get()));
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
        while (mTrackList.size() > 0) {
            removeTrack(mTrackList.first());
        }
    }
}

