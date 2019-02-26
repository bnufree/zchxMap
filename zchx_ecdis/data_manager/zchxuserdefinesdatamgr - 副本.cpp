#include "zchxuserdefinesdatamgr.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
#include "coastdatainfodialog.h"
#include "cardmouthinfodialog.h"
#include "channelinfodialog.h"
#include "mooringinfodialog.h"

namespace qt {
zchxWarningZoneDataMgr::zchxWarningZoneDataMgr(zchxMapWidget* w, QObject *parent) :
    zchxEcdisDataMgr(w, ZCHX_DATA_MGR_WARNING_ZONE, parent)
{
}

void zchxWarningZoneDataMgr::show(QPainter *painter)
{
    if( !painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_DEFENCE) || mData.empty()) return;
    for(std::shared_ptr<WarningZoneElement> ele : mData)
    {
        if(ele.get() == mDisplayWidget->getCurrentSelectedElement()){
            ele->setIsActive(true);
        } else {
            ele->setIsActive(false);
        }
        ele->drawElement(painter);
    }
}

bool zchxWarningZoneDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}

bool zchxWarningZoneDataMgr::data4id(int uuid, ZCHX::Data::ITF_WarringZone &info)
{
    for(std::shared_ptr<WarningZoneElement> ele : mData)
    {
        if(ele->data().id == myid)
        {
            info = ele->data();
            return true;
        }
    }
    return false;
}

bool zchxWarningZoneDataMgr::dataByName(const QString &name, ZCHX::Data::ITF_WarringZone &info)
{
    return dataByName(name.toStdString(), info);
}

bool zchxWarningZoneDataMgr::dataByName(const std::string &name, ZCHX::Data::ITF_WarringZone &info)
{
    for(std::shared_ptr<WarningZoneElement> ele : mData)
    {
        if(ele->name().compare(name) == 0)
        {
            info = ele->data();
            return true;
        }
    }
    return false;
}


WarningZoneElement* zchxWarningZoneDataMgr::item(const std::string &name)
{
    for(std::shared_ptr<WarningZoneElement> ele : mData)
    {
        if(ele->name().compare(name) == 0)
        {
            return ele.get();
        }
    }
    return NULL;
}

void zchxWarningZoneDataMgr::updateData(const QList<ZCHX::Data::ITF_WarringZone> &zonelist)
{
    for(const ZCHX::Data::ITF_WarringZone &zone : zonelist)
    {
        updateData(zone);
    }
}

void zchxWarningZoneDataMgr::updateData(const ZCHX::Data::ITF_WarringZone &zone)
{
    WarningZoneElement *zoneItem = item(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        mData.push_back(std::shared_ptr<WarningZoneElement>(new WarningZoneElement(zone, mDisplayWidget->framework())));
    }
}

void zchxWarningZoneDataMgr::setData(const QList<ZCHX::Data::ITF_WarringZone> &list)
{
    //不能直接clear,主要是防止当前图元正处于选择状态
    for(std::shared_ptr<WarningZoneElement> ele : mData){
        ele->setIsUpdate(false);
    }
    updateData(list);
    //删除已经过时的图元
    for(std::shared_ptr<WarningZoneElement> ele : mData){
        if(!ele->getIsUpdate())
        {
            if(ele.get() == mDisplayWidget->getCurrentSelectedElement())
            {
                mDisplayWidget->setCurrentSelectedItem(0);
            }
            mData.removeOne(ele);
        }
    }
}

void zchxWarningZoneDataMgr::removeData(const ZCHX::Data::ITF_WarringZone &zone)
{
    QList<std::shared_ptr<WarningZoneElement>>::iterator it = mData.begin();
    for(; it != mData.end(); ++it)
    {
        if(zone.name == QString::fromStdString(it->name()) || zone.id == it->id())
        {
            mData.erase(it);
            break;
        }
    }
}

QList<ZCHX::Data::ITF_WarringZone> zchxWarningZoneDataMgr::getData() const
{
    QList<ZCHX::Data::ITF_WarringZone> list;
    for(const WarningZoneElement &zone : mData){
        list.push_back(zone.data());
    }
    return list;
}

//岸线数据管理

zchxCoastDataMgr::zchxCoastDataMgr(zchxMapWidget *w, QObject *parent):zchxEcdisDataMgr(w, ZCHX_DATA_MGR_COAST, parent)
{

}

void zchxCoastDataMgr::show(QPainter *painter)
{
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_COASTDATA) || mData.size() == 0) return;
    for(std::shared_ptr<CoastElement> ele : mData){
        if(ele.get() == mDisplayWidget->getCurrentSelectedElement())
        {
            ele->setIsActive(true);
        } else {
            ele->setIsActive(false);
        }
        ele->drawElement(painter);
    }
}

bool zchxCoastDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}

void zchxCoastDataMgr::setData(const QList<ZCHX::Data::ITF_CoastData>& list)
{
    //不能直接clear,主要是防止当前图元正处于选择状态
    for(std::shared_ptr<CoastElement> ele : mData){
        ele->setIsUpdate(false);
    }
    updateData(list);
    //删除已经过时的图元
    for(std::shared_ptr<CoastElement> ele : mData){
        if(!ele->getIsUpdate())
        {
            if(ele.get() == mDisplayWidget->getCurrentSelectedElement())
            {
                mDisplayWidget->setCurrentSelectedItem(0);
            }
            mData.removeOne(ele);
        }
    }

}

void zchxCoastDataMgr::updateData(const QList<ZCHX::Data::ITF_CoastData> &zonelist)
{
    for(const ZCHX::Data::ITF_CoastData &zone : zonelist)
    {
        updateData(zone);
    }
}

void zchxCoastDataMgr::updateData(const ZCHX::Data::ITF_CoastData &zone)
{
    CoastElement *zoneItem = item(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
        zoneItem->setIsUpdate(true);
    }
    else
    {
        mData.append(std::shared_ptr<CoastElement>(new CoastElement(zone, mDisplayWidget->framework())));
    }
}

CoastElement* zchxCoastDataMgr::item(const std::string &name)
{
    for(std::shared_ptr<CoastElement> ele : mData){
        if(ele->name().compare(name) == 0)
        {
            return ele.get();
        }
    }

    return NULL;
}

void zchxCoastDataMgr::importCoastData(const std::vector<std::pair<double, double> > & path)
{
    ZCHX::Data::ITF_CoastData zone;
    zone.path = path;
    zone.id = 0;
    zone.beachLength = zchxMapDataUtils::getTotalDistance(path);

    CoastDataInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.manageOrganization = dlg.getOrg();
        zone.responsibilityAccount = dlg.getAccount();
        zone.height = dlg.getHeight();

        if(!zone.name.isEmpty())
        {
            //先添加进去, 确保显示
            updateData(zone);
            emit mDisplayWidget->signalCreateCoastDataLINE(zone);
        }
    }
}

//海底管线数据管理
zchxSeabedPipeLineMgr::zchxSeabedPipeLineMgr(zchxMapWidget *w, QObject *parent):zchxEcdisDataMgr(w, ZCHX_DATA_MGR_SEABEDIPLINE, parent)
{

}

void zchxSeabedPipeLineMgr::show(QPainter *painter)
{
    if(!painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_SEABEDPIPELINE) || mData.size() == 0) return;
    for(std::shared_ptr<SeabedPipeLineElement> ele : mData){
        if(ele.get() == mDisplayWidget->getCurrentSelectedElement())
        {
            ele->setIsActive(true);
        } else {
            ele->setIsActive(false);
        }
        ele->drawElement(painter);
    }
}

bool zchxSeabedPipeLineMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}

void zchxSeabedPipeLineMgr::setData(const QList<ZCHX::Data::ITF_SeabedPipeLine>& list)
{
    //不能直接clear,主要是防止当前图元正处于选择状态
    for(std::shared_ptr<SeabedPipeLineElement> ele : mData){
        ele->setIsUpdate(false);
    }
    updateData(list);
    //删除已经过时的图元
    for(std::shared_ptr<SeabedPipeLineElement> ele : mData){
        if(!ele->getIsUpdate())
        {
            if(ele.get() == mDisplayWidget->getCurrentSelectedElement())
            {
                mDisplayWidget->setCurrentSelectedItem(0);
            }
            mData.removeOne(ele);
        }
    }

}

void zchxSeabedPipeLineMgr::importData(const std::vector<std::pair<double, double> > &path)
{
    ZCHX::Data::ITF_SeabedPipeLine zone;
    zone.path = path;
    zone.id = 0;
    zone.lineLength = zchxMapDataUtils::getTotalDistance(path);

    CoastDataInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.manageOrganization = dlg.getOrg();
        zone.responsibilityAccount = dlg.getAccount();
        zone.depth = dlg.getHeight();

        if(!zone.name.isEmpty())
        {
            //先添加进去, 确保显示
            updateData(zone);
            emit mDisplayWidget->signalCreateSeabedPipeLineLINE(zone);
        }
    }
}

void zchxSeabedPipeLineMgr::updateData(const QList<ZCHX::Data::ITF_SeabedPipeLine> &zonelist)
{
    for(const ZCHX::Data::ITF_SeabedPipeLine &zone : zonelist)
    {
        updateData(zone);
    }
}

void zchxSeabedPipeLineMgr::updateData(const ZCHX::Data::ITF_SeabedPipeLine &zone)
{
    SeabedPipeLineElement *zoneItem = item(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        mData.append(std::shared_ptr<SeabedPipeLineElement>(new SeabedPipeLineElement(zone, mDisplayWidget->framework())));
    }
}


SeabedPipeLineElement *zchxSeabedPipeLineMgr::item(const std::string &name)
{
    for(std::shared_ptr<SeabedPipeLineElement> ele : mData){
    {
        if(ele->name().compare(name) == 0)
        {
            return ele.get();
        }
    }

    return NULL;
}

Structure *zchxUserDefinesDataMgr::zchxStructureZoneItem(const std::string &name)
{
    for(int i = 0; i < m_structurePoint.size(); ++i)
    {
        if(m_structurePoint[i].name().compare(name) == 0)
        {
            return &m_structurePoint[i];
        }
    }

    return NULL;
}

AreaNet *zchxUserDefinesDataMgr::zchxAreaNetZoneItem(const std::string &name)
{
    for(int i = 0; i < m_areaNetZone.size(); ++i)
    {
        if(m_areaNetZone[i].name().compare(name) == 0)
        {
            return &m_areaNetZone[i];
        }
    }

    return NULL;
}






void zchxUserDefinesDataMgr::updateStructureZone(const QList<ZCHX::Data::ITF_Structure> &zonelist)
{
    for(const ZCHX::Data::ITF_Structure &zone : zonelist)
    {
        updateStructureZone(zone);
    }
}

void zchxUserDefinesDataMgr::updateStructureZone(const ZCHX::Data::ITF_Structure &zone)
{
    Structure *zoneItem = zchxStructureZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        Structure newitem(zone);
        m_structurePoint.push_back(newitem);
    }
}

void zchxUserDefinesDataMgr::updateAreaNetZone(const QList<ZCHX::Data::ITF_AreaNet> &zonelist)
{
    for(const ZCHX::Data::ITF_AreaNet &zone : zonelist)
    {
        updateAreaNetZone(zone);
    }
}

void zchxUserDefinesDataMgr::updateAreaNetZone(const ZCHX::Data::ITF_AreaNet &zone)
{
    AreaNet *zoneItem = zchxAreaNetZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        AreaNet newitem(zone);
        m_areaNetZone.push_back(newitem);
    }
}

void zchxUserDefinesDataMgr::updateChannelZone(const QList<ZCHX::Data::ITF_Channel> &zonelist)
{
    for(const ZCHX::Data::ITF_Channel &zone : zonelist)
    {
        updateChannelZone(zone);
    }
}

Channel *zchxUserDefinesDataMgr::zchxChannelZoneItem(const std::string &name)
{
    for(int i = 0; i < m_channelZone.size(); ++i)
    {
        if(m_channelZone[i].name().compare(name) == 0)
        {
            return &m_channelZone[i];
        }
    }

    return NULL;
}

bool zchxUserDefinesDataMgr::zchxChannelZoneData4id(int uuid, ZCHX::Data::tagITF_Channel &info)
{
    std::vector<Channel>::iterator it;
    for(it = m_channelZone.begin(); it != m_channelZone.end(); ++it)
    {
        int myid = (*it).data().id;
        if(uuid == myid)
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

void zchxUserDefinesDataMgr::updateChannelZone(const ZCHX::Data::ITF_Channel &zone)
{
    Channel *zoneItem = zchxChannelZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        Channel newitem(zone);
        m_channelZone.push_back(newitem);
    }
}

void zchxUserDefinesDataMgr::updateMooringZone(const QList<ZCHX::Data::ITF_Mooring> &zonelist)
{
    for(const ZCHX::Data::ITF_Mooring &zone : zonelist)
    {
        updateMooringZone(zone);
    }
}

bool zchxUserDefinesDataMgr::zchxMooringZoneData4id(int uuid, ZCHX::Data::tagITF_Mooring &info)
{
    std::vector<Mooring>::iterator it;
    for(it = m_mooringZone.begin(); it != m_mooringZone.end(); ++it)
    {
        int myid = (*it).data().id;
        if(uuid == myid)
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

Mooring *zchxUserDefinesDataMgr::zchxMooringZoneItem(const std::string &name)
{
    for(int i = 0; i < m_mooringZone.size(); ++i)
    {
        if(m_mooringZone[i].name().compare(name) == 0)
        {
            return &m_mooringZone[i];
        }
    }

    return NULL;
}

void zchxUserDefinesDataMgr::updateMooringZone(const ZCHX::Data::ITF_Mooring &zone)
{
    Mooring *zoneItem = zchxMooringZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        Mooring newitem(zone);
        m_mooringZone.push_back(newitem);
    }
}

void zchxUserDefinesDataMgr::updateCardMouthZone(const QList<ZCHX::Data::ITF_CardMouth> &zonelist)
{
    for(const ZCHX::Data::ITF_CardMouth &zone : zonelist)
    {
        updateCardMouthZone(zone);
    }
}

void zchxUserDefinesDataMgr::updateCardMouthZone(const ZCHX::Data::ITF_CardMouth &zone)
{
    CardMouth *zoneItem = zchxCardMouthZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        CardMouth newitem(zone);
        m_cardMouthZone.push_back(newitem);
    }
}

void zchxUserDefinesDataMgr::setIslandLineData(const std::vector<IslandLine> &data)
{
    m_IslandLine = data;
}

bool zchxUserDefinesDataMgr::zchxIslandData4id(int uuid, ZCHX::Data::ITF_IslandLine &info)
{
    std::vector<IslandLine>::iterator it;
    for(it =m_IslandLine.begin(); it != m_IslandLine.end(); ++it)
    {
        if(uuid == (*it).id())
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

void zchxUserDefinesDataMgr::setLocalMarkData(const std::vector<LocalMark> &data)
{
    m_LocalMark = data;
}






void zchxUserDefinesDataMgr::setStructure(const std::vector<Structure> & data)
{
    m_structurePoint = data;
}

void zchxUserDefinesDataMgr::setAreaNet(const std::vector<AreaNet> &data)
{
    m_areaNetZone = data;
}

void zchxUserDefinesDataMgr::importAreaNet(const std::vector<std::pair<double, double> > &path)
{
    ZCHX::Data::ITF_AreaNet zone;
    zone.path = path;
    zone.id = 0;
    zone.area = zchxMapDataUtils::getTotalArea(path);

    QInputDialog input_dialog;
    input_dialog.setCancelButtonText(tr("Cancel"));
    input_dialog.setOkButtonText(tr("OK"));
    input_dialog.setWindowTitle(tr("WARRING ZONE"));
    input_dialog.setLabelText(tr("(Default 5 minute warning)Name:"));
    if (input_dialog.exec())
    {
        zone.name = input_dialog.textValue();
    }

    if(!zone.name.isEmpty())
    {
        //先添加进去, 确保显示
        updateAreaNetZone(zone);
        emit mDisplayWidget->signalCreateAreaNetZone(zone);
    }
}

void zchxUserDefinesDataMgr::setChannel(const std::vector<Channel> &data)
{
    m_channelZone = data;
}

void zchxUserDefinesDataMgr::importChannel(const std::vector<std::pair<double, double> > &path)
{
    ZCHX::Data::ITF_Channel zone;
    zone.path = path;
    zone.id = 0;
//    zone.area = getArea(path);

    ChannelInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.courseType = dlg.getChannelType();
        zone.isWarn = dlg.getChannelStatus();
        zone.speedLimit = dlg.getMaxSpeed();
        zone.collisionThreshold = dlg.getCollisionMaxDis();
        zone.yawThreshold = dlg.getYawMinDis();
        zone.overtakThreshold = dlg.getOvertak();
        zone.dropAnchorThreshold = dlg.getDropAnchor();
        zone.overtakShortDis = dlg.getOvertakShortDis();
        zone.overtakVerticalDis = dlg.getOvertakVerticalDis();
        zone.acceleraThreshold = dlg.getAcceleraThreshold();
        zone.acceleraDfference = dlg.getAcceleraDfference();
        zone.isConverseWarn = dlg.isConverseWarn();
        zone.isOverloadWarn = dlg.isOverloadWarn();

        zone.fillColor = dlg.getColor();
    }

    if(!zone.name.isEmpty())
    {
        //先添加进去, 确保显示
        updateChannelZone(zone);
        emit mDisplayWidget->signalCreateChannelZone(zone);
    }
}

void zchxUserDefinesDataMgr::selectChannelLine(int channelId, const ZCHX::Data::ITF_ChannelLine & line)
{
    for (int i = 0; i < m_channelZone.size(); i++)
    {
        ZCHX::Data::ITF_Channel item = m_channelZone[i].data();

        if (item.id == channelId)
        {
            for (int j = 0; j < item.lineList.size(); j++)
            {
                if (item.lineList[j] == line)
                {
                    m_channelZone[i].setLineSelected(j, true);
                }
                else
                {
                    m_channelZone[i].setLineSelected(j, false);
                }
            }

            break;
        }
    }
}

void zchxUserDefinesDataMgr::cancelChannelLine(int channelId)
{
    for (int i = 0; i < m_channelZone.size(); i++)
    {
        ZCHX::Data::ITF_Channel item = m_channelZone[i].data();

        if (item.id == channelId)
        {
            for (int j = 0; j < item.lineList.size(); j++)
            {
                m_channelZone[i].setLineSelected(j, false);
            }

            break;
        }
    }
}

void zchxUserDefinesDataMgr::setMooring(const std::vector<Mooring> &data)
{
    m_mooringZone = data;
}

void zchxUserDefinesDataMgr::importMooring(const std::vector<std::pair<double, double> > &path)
{
    ZCHX::Data::ITF_Mooring zone;
    zone.path = path;
    zone.id = 0;
//    zone.area = getArea(path);

    MooringInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.anchorType = dlg.getMooringType();
        zone.isWarn = dlg.getMooringStatus();
        zone.fillColor = dlg.getColor();
        zone.displaceDis = dlg.getDisplaceDis();
        zone.displaceCycle = dlg.getDisplaceCycle();
    }

    if(!zone.name.isEmpty())
    {
        //先添加进去, 确保显示
        updateMooringZone(zone);
        emit mDisplayWidget->signalCreateMooringZone(zone);
    }
}

void zchxUserDefinesDataMgr::setCardMouth(const std::vector<CardMouth> &data)
{
    m_cardMouthZone = data;
}

bool zchxUserDefinesDataMgr::zchxCardMouthZoneData4id(int uuid, ZCHX::Data::tagITF_CardMouth &info)
{
    std::vector<CardMouth>::iterator it;
    for(it = m_cardMouthZone.begin(); it != m_cardMouthZone.end(); ++it)
    {
        int myid = (*it).data().id;
        if(uuid == myid)
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}


CardMouth *zchxUserDefinesDataMgr::zchxCardMouthZoneItem(const std::string &name)
{
    for(int i = 0; i < m_cardMouthZone.size(); ++i)
    {
        if(m_cardMouthZone[i].name().compare(name) == 0)
        {
            return &m_cardMouthZone[i];
        }
    }

    return NULL;
}

void zchxUserDefinesDataMgr::importCardMouth(const std::vector<std::pair<double, double> > &path)
{
    ZCHX::Data::ITF_CardMouth zone;
    zone.path = path;
    zone.id = 0;
//    zone.area = getArea(path);

    CardMouthInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.caMouCapConType = dlg.getType();
        zone.isAisUnopened = dlg.isAisUnopened();
        zone.isWarn = dlg.getWarning();
        zone.fillColor = dlg.getColor();
        zone.overlength = dlg.getOverlength();
    }

    if(!zone.name.isEmpty())
    {
        //先添加进去, 确保显示
        updateCardMouthZone(zone);
        emit mDisplayWidget->signalCreateCardMouthZone(zone);
    }
}


void zchxUserDefinesDataMgr::setDangerousCircleData(const std::vector<DangerousCircle> &data)
{
    m_DangerousCircle = data;
}

void zchxUserDefinesDataMgr::setPastrolStation(const std::vector<PastrolStation> &data)
{
    m_PastrolStation = data;
}
}
