#include "zchxuserdefinesdatamgr.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
#include "coastdatainfodialog.h"
#include "cardmouthinfodialog.h"
#include "channelinfodialog.h"
#include "mooringinfodialog.h"

zchxUserDefinesDataMgr::zchxUserDefinesDataMgr(zchxMapWidget* w, QObject *parent) : QObject(parent),mDisplayWidget(w)
{
}

bool zchxUserDefinesDataMgr::zchxWarringZoneData4id(int uuid, ZCHX::Data::ITF_WarringZone &info)
{
    std::vector<DrawElement::WarringZONE>::iterator it;
    for(it = m_WarringZone.begin(); it != m_WarringZone.end(); ++it)
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

bool zchxUserDefinesDataMgr::zchxWarringZoneDataByName(const QString &name, ZCHX::Data::ITF_WarringZone &info)
{
    return zchxWarringZoneDataByName(name.toStdString(), info);
}

bool zchxUserDefinesDataMgr::zchxWarringZoneDataByName(const std::string &name, ZCHX::Data::ITF_WarringZone &info)
{
    std::vector<DrawElement::WarringZONE>::iterator it;
    for(it=m_WarringZone.begin(); it != m_WarringZone.end(); ++it)
    {
        const DrawElement::WarringZONE &zone = (*it);
        if(zone.name().compare(name) == 0)
        {
            info = zone.data();
            return true;
        }
    }
    return false;
}


DrawElement::WarringZONE* zchxUserDefinesDataMgr::zchxWarringZoneItem(const std::string &name)
{
    for(int i = 0; i < m_WarringZone.size(); ++i)
    {
        if(m_WarringZone[i].name().compare(name) == 0)
        {
            return &m_WarringZone[i];
        }
    }
    return NULL;
}

void zchxUserDefinesDataMgr::updateWarrningZone(const QList<ZCHX::Data::ITF_WarringZone> &zonelist)
{
    for(const ZCHX::Data::ITF_WarringZone &zone : zonelist)
    {
        updateWarrningZone(zone);
    }
}

void zchxUserDefinesDataMgr::updateWarrningZone(const ZCHX::Data::ITF_WarringZone &zone)
{
    DrawElement::WarringZONE *zoneItem = zchxWarringZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::WarringZONE newitem(zone);
        {
            m_WarringZone.push_back(newitem);
        }
    }
}

void zchxUserDefinesDataMgr::setWarringZONEData(const std::vector<DrawElement::WarringZONE> &data)
{
    m_WarringZone = data;
}

void zchxUserDefinesDataMgr::removeWarrningZone(const ZCHX::Data::ITF_WarringZone &zone)
{
    std::vector<DrawElement::WarringZONE>::iterator it;
    {
        for(it = m_WarringZone.begin(); it != m_WarringZone.end(); ++it)
        {
            const DrawElement::WarringZONE &one = (*it);
            if(zone.name == QString::fromStdString(one.name())
                    || zone.id == one.id())
            {
                m_WarringZone.erase(it);
                break;
            }
        }
    }
}

QList<ZCHX::Data::ITF_WarringZone> zchxUserDefinesDataMgr::getAllWarrningZone() const
{
    QList<ZCHX::Data::ITF_WarringZone> list;
    for(const DrawElement::WarringZONE &zone : m_WarringZone)
        list.push_back(zone.data());
    return list;
}


void zchxUserDefinesDataMgr::updateCoastDataZone(const QList<ZCHX::Data::ITF_CoastData> &zonelist)
{
    for(const ZCHX::Data::ITF_CoastData &zone : zonelist)
    {
        updateCoastDataZone(zone);
    }
}





DrawElement::CoastData* zchxUserDefinesDataMgr::zchxCoastDataZoneItem(const std::string &name)
{
    for(int i = 0; i < m_coastDataLine.size(); ++i)
    {
        if(m_coastDataLine[i].name().compare(name) == 0)
        {
            return &m_coastDataLine[i];
        }
    }

    return NULL;
}

DrawElement::SeabedPipeLine *zchxUserDefinesDataMgr::zchxSeabedPipeLineZoneItem(const std::string &name)
{
    for(int i = 0; i < m_seabedPipeLineLine.size(); ++i)
    {
        if(m_seabedPipeLineLine[i].name().compare(name) == 0)
        {
            return &m_seabedPipeLineLine[i];
        }
    }

    return NULL;
}

DrawElement::Structure *zchxUserDefinesDataMgr::zchxStructureZoneItem(const std::string &name)
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

DrawElement::AreaNet *zchxUserDefinesDataMgr::zchxAreaNetZoneItem(const std::string &name)
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


void zchxUserDefinesDataMgr::updateCoastDataZone(const ZCHX::Data::ITF_CoastData &zone)
{
    DrawElement::CoastData *zoneItem = zchxCoastDataZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::CoastData newitem(zone);
        //if(zone.defenceShape == 2) //防线
            m_coastDataLine.push_back(newitem);
//        else
//            m_CoastDataZone.push_back(newitem);
    }
}

void zchxUserDefinesDataMgr::updateSeabedPipeLineZone(const QList<ZCHX::Data::ITF_SeabedPipeLine> &zonelist)
{
    for(const ZCHX::Data::ITF_SeabedPipeLine &zone : zonelist)
    {
        updateSeabedPipeLineZone(zone);
    }
}

void zchxUserDefinesDataMgr::updateSeabedPipeLineZone(const ZCHX::Data::ITF_SeabedPipeLine &zone)
{
    DrawElement::SeabedPipeLine *zoneItem = zchxSeabedPipeLineZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::SeabedPipeLine newitem(zone);
        m_seabedPipeLineLine.push_back(newitem);
    }
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
    DrawElement::Structure *zoneItem = zchxStructureZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::Structure newitem(zone);
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
    DrawElement::AreaNet *zoneItem = zchxAreaNetZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::AreaNet newitem(zone);
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

DrawElement::Channel *zchxUserDefinesDataMgr::zchxChannelZoneItem(const std::string &name)
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
    std::vector<DrawElement::Channel>::iterator it;
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
    DrawElement::Channel *zoneItem = zchxChannelZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::Channel newitem(zone);
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
    std::vector<DrawElement::Mooring>::iterator it;
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

DrawElement::Mooring *zchxUserDefinesDataMgr::zchxMooringZoneItem(const std::string &name)
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
    DrawElement::Mooring *zoneItem = zchxMooringZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::Mooring newitem(zone);
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
    DrawElement::CardMouth *zoneItem = zchxCardMouthZoneItem(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        //没有就添加
        DrawElement::CardMouth newitem(zone);
        m_cardMouthZone.push_back(newitem);
    }
}

void zchxUserDefinesDataMgr::setIslandLineData(const std::vector<DrawElement::IslandLine> &data)
{
    m_IslandLine = data;
}

bool zchxUserDefinesDataMgr::zchxIslandData4id(int uuid, ZCHX::Data::ITF_IslandLine &info)
{
    std::vector<DrawElement::IslandLine>::iterator it;
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

void zchxUserDefinesDataMgr::setLocalMarkData(const std::vector<DrawElement::LocalMark> &data)
{
    m_LocalMark = data;
}

void zchxUserDefinesDataMgr::setCoastData(const std::vector<DrawElement::CoastData> &data)
{
    m_coastDataLine = data;
}

void zchxUserDefinesDataMgr::importCoastData(const std::vector<std::pair<double, double> > & path)
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
            updateCoastDataZone(zone);
            emit mDisplayWidget->signalCreateCoastDataLINE(zone);
        }
    }
}

void zchxUserDefinesDataMgr::setSeabedPipeLine(const std::vector<DrawElement::SeabedPipeLine> &data)
{
    m_seabedPipeLineLine = data;
}

void zchxUserDefinesDataMgr::importSeabedPipeLine(const std::vector<std::pair<double, double> > &path)
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
            updateSeabedPipeLineZone(zone);
            emit mDisplayWidget->signalCreateSeabedPipeLineLINE(zone);
        }
    }
}

void zchxUserDefinesDataMgr::setStructure(const std::vector<DrawElement::Structure> & data)
{
    m_structurePoint = data;
}

void zchxUserDefinesDataMgr::setAreaNet(const std::vector<DrawElement::AreaNet> &data)
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

void zchxUserDefinesDataMgr::setChannel(const std::vector<DrawElement::Channel> &data)
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

void zchxUserDefinesDataMgr::setMooring(const std::vector<DrawElement::Mooring> &data)
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

void zchxUserDefinesDataMgr::setCardMouth(const std::vector<DrawElement::CardMouth> &data)
{
    m_cardMouthZone = data;
}

bool zchxUserDefinesDataMgr::zchxCardMouthZoneData4id(int uuid, ZCHX::Data::tagITF_CardMouth &info)
{
    std::vector<DrawElement::CardMouth>::iterator it;
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


DrawElement::CardMouth *zchxUserDefinesDataMgr::zchxCardMouthZoneItem(const std::string &name)
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


void zchxUserDefinesDataMgr::setDangerousCircleData(const std::vector<DrawElement::DangerousCircle> &data)
{
    m_DangerousCircle = data;
}

void zchxUserDefinesDataMgr::setPastrolStation(const std::vector<DrawElement::PastrolStation> &data)
{
    m_PastrolStation = data;
}
