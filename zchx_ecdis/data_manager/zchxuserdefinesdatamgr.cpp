#include "zchxuserdefinesdatamgr.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
#include "coastdatainfodialog.h"
#include "cardmouthinfodialog.h"
#include "channelinfodialog.h"
#include "mooringinfodialog.h"

namespace qt {

template<typename K, typename T>
zchxUserDefinesDataMgr::zchxUserDefinesDataMgr(zchxMapWidget* w, int type, const QString& layer, QObject *parent) :
    zchxEcdisDataMgr(w, type, parent), mLayerName(layer)
{
}

template<typename K, typename T>
void zchxUserDefinesDataMgr::show(QPainter *painter)
{
    if( !painter || !MapLayerMgr::instance()->isLayerVisible(layer) || mData.empty()) return;
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

template<typename K, typename T>
bool zchxUserDefinesDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}

template<typename K, typename T>
bool zchxUserDefinesDataMgr::data4id(int uuid, T &info)
{
    for(std::shared_ptr<K> ele : mData)
    {
        if(ele->data().id == uuid)
        {
            info = ele->data();
            return true;
        }
    }
    return false;
}

template<typename K, typename T>
bool zchxUserDefinesDataMgr::dataByName(const QString &name, T &info)
{
    return dataByName(name.toStdString(), info);
}

template<typename K, typename T>
bool zchxUserDefinesDataMgr::dataByName(const std::string &name, T &info)
{
    for(std::shared_ptr<K> ele : mData)
    {
        if(ele->name().compare(name) == 0)
        {
            info = ele->data();
            return true;
        }
    }
    return false;
}

template<typename K, typename T>
K* zchxUserDefinesDataMgr::item(const std::string &name)
{
    for(std::shared_ptr<K> ele : mData)
    {
        if(ele->name().compare(name) == 0)
        {
            return ele.get();
        }
    }
    return NULL;
}

template<typename K, typename T>
K* zchxUserDefinesDataMgr::item(int id)
{
    for(std::shared_ptr<K> ele : mData)
    {
        if(ele->data().id == id)
        {
            return ele.get();
        }
    }
    return NULL;
}

template<typename K, typename T>
void zchxUserDefinesDataMgr::updateData(const QList<T> &list)
{
    for(const T &zone : list)
    {
        updateData(zone);
    }
}

template<typename K, typename T>
void zchxUserDefinesDataMgr::updateData(const T &zone)
{
    K *zoneItem = item(zone.name.toStdString());
    //更新已有的
    if(zoneItem)
    {
        zoneItem->setData(zone);
    }
    else
    {
        mData.push_back(std::shared_ptr<K>(new K(zone, mDisplayWidget->framework())));
    }
}

template<typename K, typename T>
void zchxUserDefinesDataMgr::setData(const QList<T> &list)
{
    //不能直接clear,主要是防止当前图元正处于选择状态
    for(std::shared_ptr<K> ele : mData){
        ele->setIsUpdate(false);
    }
    updateData(list);
    //删除已经过时的图元
    for(std::shared_ptr<K> ele : mData){
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

template<typename K, typename T>
void zchxUserDefinesDataMgr::removeData(const T &zone)
{
    QList<std::shared_ptr<K>>::iterator it = mData.begin();
    for(; it != mData.end(); ++it)
    {
        if(zone.name == QString::fromStdString(it->name()) || zone.id == it->id())
        {
            mData.erase(it);
            break;
        }
    }
}

template<typename K, typename T>
QList<T> zchxUserDefinesDataMgr::getData() const
{
    QList<T> list;
    for(std::shared_ptr<K> ele : mData){
        list.push_back(ele->data());
    }
    return list;
}

void zchxCoastDataMgr::importData(const std::vector<std::pair<double, double> > & path)
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

void zchxSeabedPipLineDataMgr::importData(const std::vector<std::pair<double, double> > &path)
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

void zchxChannelDataMgr::importData(const std::vector<std::pair<double, double> > &path)
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
        updateData(zone);
        emit mDisplayWidget->signalCreateChannelZone(zone);
    }
}

void zchxChannelDataMgr::selectChannelLine(int channelId, const ZCHX::Data::ITF_ChannelLine & line)
{
    ChannelElement *ele = item(channelId);
    if(!ele) return;
    ZCHX::Data::ITF_Channel item = ele->data();
    for (int j = 0; j < item.lineList.size(); j++)
    {
        if (item.lineList[j] == line)
        {
            ele->setLineSelected(j, true);
        }
        else
        {
            ele->setLineSelected(j, false);
        }
    }

}

void zchxChannelDataMgr::cancelChannelLine(int channelId)
{
    ChannelElement *ele = item(channelId);
    if(!ele) return;

    ZCHX::Data::ITF_Channel item = ele->data();
    for (int j = 0; j < item.lineList.size(); j++)
    {
        ele->setLineSelected(j, false);
    }
}

void zchxMooringDataMgr::importData(const std::vector<std::pair<double, double> > &path)
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
        updateData(zone);
        emit mDisplayWidget->signalCreateAreaNetZone(zone);
    }
}

void zchxMooringDataMgr::importData(const std::vector<std::pair<double, double> > &path)
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
        updateData(zone);
        emit mDisplayWidget->signalCreateMooringZone(zone);
    }
}

void zchxCardMouthDataMgr::importData(const std::vector<std::pair<double, double> > &path)
{
    ZCHX::Data::ITF_CardMouth zone;
    zone.path = path;
    zone.id = 0;

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
        updateData(zone);
        emit mDisplayWidget->signalCreateCardMouthZone(zone);
    }
}
}
