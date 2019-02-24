#include "zchxcameradatasmgr.h"
#include "zchxcameralistdlg.h"
#include "zchxmapwidget.h"
#include "zchxmapframe.h"
#include <QDebug>

namespace qt {
zchxCameraDatasMgr::zchxCameraDatasMgr(zchxMapWidget* w,  QObject *parent) : QObject(parent),
    mDisplayWidget(w)
{

}

void zchxCameraDatasMgr::setCameraRodData(const std::vector<CameraRodElement> &data)
{
    m_CameraRod = data;

    updateAllCameraRodStatus();
}

void zchxCameraDatasMgr::updateAllCameraRodStatus()
{
    for(uint i = 0; i < m_CameraRod.size(); ++i)
    {
        CameraRodElement *rod = &m_CameraRod[i];
        checkCameraRodStatus(rod);
    }
}

void zchxCameraDatasMgr::zchxUpdateCameraRodStatus(uint uuid)
{
    CameraRodElement *rod = getCameraRod(uuid);
    if(!rod)
        return;

    checkCameraRodStatus(rod);
    //    update();
}

CameraRodElement *zchxCameraDatasMgr::getCameraRod(uint uuid)
{
    for(int i= 0; i< m_CameraRod.size();++i)
    {
        CameraRodElement *item = &m_CameraRod[i];
        if(item->getUuid() == uuid)
        {
            return item;
        }
    }
    return NULL;
}

void zchxCameraDatasMgr::showCameraRod(QPainter *painter)
{
    std::vector<CameraRodElement>::iterator it;
    for(it = m_CameraRod.begin(); it != m_CameraRod.end(); ++it)
    {
        CameraRodElement item = (*it);
        item.drawElement(painter);
    }
}

ZCHX::Data::CAMERAROD_STATUS zchxCameraDatasMgr::checkCameraRodStatus(CameraRodElement *item)
{
    if(!item)
        return ZCHX::Data::CAMERAROD_ERROR;

    int errNum = 0;
    int bugCount = 0;
    int carmeDevNum = 0;

    //检查相机状态
    for(const CameraElement &camera : m_CameraDev)
    {
        if( ZCHX::Data::CAMERA_GDY  ==  camera.getType())
            continue;

        if(item->getUuid() == camera.getGanPid() )
        {
            ++carmeDevNum;
            if(camera.isError())
                ++errNum;
            if(camera.isBug())
                ++bugCount;
        }
    }

    //检查电线杆上的号角设备状态
    std::shared_ptr<ZCHX::Data::IPCastDevice> ipcastDevice = getIPCastDevice(item->data().szID);
    if(ipcastDevice)
    {
        ++carmeDevNum;
        if(ipcastDevice->status == -1)
        {
            ++errNum;
        }
    }

    qDebug()<<item->data().szName<<"errNum: "<<errNum<<"; bugCount: "<<bugCount<<"; carmeDevNum: "<<carmeDevNum;
    //规则如下:
    //杆上所有相机为 Error 状态, 则杆为 Error 状态;
    //杆上有相机为 Error 或者 Bug 状态, 则杆为 Warning 状态;
    //赶上所有相机都是 Normal 状态, 则杆为 OK 状态;
    ZCHX::Data::CAMERAROD_STATUS status = ZCHX::Data::CAMERAROD_ERROR;
    if(errNum == carmeDevNum)
    {
        status = ZCHX::Data::CAMERAROD_ERROR;
    }
    else if(errNum > 0 || bugCount > 0)
    {
        status = ZCHX::Data::CAMERAROD_WARRING;
    }
    else
    {
        status = ZCHX::Data::CAMERAROD_OK;
    }
    item->setStatus(status);

    return status;
}

std::shared_ptr<ZCHX::Data::IPCastDevice> zchxCameraDatasMgr::getIPCastDevice(const QString &rodID)
{
    for(std::shared_ptr<ZCHX::Data::IPCastDevice> device : m_ipcastDevices)
    {
        if(device->rodID == rodID)
            return device;
    }
    return NULL;

}

void zchxCameraDatasMgr::updateIPCastDeviceList(std::list<std::shared_ptr<ZCHX::Data::IPCastDevice> > list)
{
    m_ipcastDevices = list;

    updateAllCameraRodStatus();
}

void zchxCameraDatasMgr::setCameraDevData(const std::vector<CameraElement> &data)
{
    m_CameraDev = data;

    updateAllCameraRodStatus();
}

void zchxCameraDatasMgr::showCamera(QPainter *painter)
{
    std::vector<CameraElement>::iterator it;
    for(it = m_CameraDev.begin(); it != m_CameraDev.end(); ++it)
    {
        CameraElement item = (*it);
        item.drawElement(painter);
    }
}

void zchxCameraDatasMgr::slotSelectCameraFromDlg(const ZCHX::Data::ITF_CameraDev &camera)
{
    for(int i=0; i<m_CameraDev.size(); i++)
    {
        CameraElement &item = m_CameraDev[i];
        item.setIsActive(false);
        if(item.data().nDBID == camera.nDBID)
        {
            item.setIsActive(true);
            emit signalIsSelected4CameraDev(camera);
        }
    }
}

void zchxCameraDatasMgr::zchxOpenCameraListDlg(CameraRodElement *item)
{
    if(!item)
        return;

    //    qDebug()<<"openCameraList"<<item;
    ZCHX::Data::ITF_CameraRod rod = item->data();
    int uuid = rod.nUUID;
    QList<ZCHX::Data::ITF_CameraDev> list;
    for(int i=0; i< m_CameraDev.size(); ++i )
    {
        ZCHX::Data::ITF_CameraDev cam = m_CameraDev[i].data();
        if(cam.mRod && cam.mRod->nUUID == uuid)
        {
            list.append(cam);
        }
    }
    std::shared_ptr<ZCHX::Data::IPCastDevice> device = getIPCastDevice(item->data().szID);
    if(device)
    {
        ZCHX::Data::ITF_CameraDev fakecam;
        fakecam.nType = ZCHX::Data::CAMERA_FAKE_IPCAST;
        fakecam.mIpcastDevice = device.get();
        fakecam.szCamName = device->name;
        fakecam.nStatus = device->status == -1 ? 512 : 0;//状态转换
        list.append(fakecam);
    }
    if(list.empty())
    {
        qDebug()<<"No camera deivce on this Camera rod!"<<uuid;
        return;
    }
    zchxOpenCameraListDlg(list, LatLon(item->lat(), item->lon()));
}

void zchxCameraDatasMgr::zchxOpenCameraListDlg(AisElement *item)
{
    if(!item && !item->hasCamera())
        return;


    QList<ZCHX::Data::ITF_CameraDev> list = QList<ZCHX::Data::ITF_CameraDev>::fromVector(QVector<ZCHX::Data::ITF_CameraDev>::fromStdVector(item->getCameraData()));
    if(list.empty())
    {
        qDebug()<<"No camera deivce on this ais :"<<item->getData().mmsi;
        return;
    }
    zchxOpenCameraListDlg(list, LatLon(item->lat(), item->lon()));
}

//在指定的经纬度位置弹出对话框
void zchxCameraDatasMgr::zchxOpenCameraListDlg(QList<ZCHX::Data::ITF_CameraDev>& list, const LatLon& ll)
{
    if((!mDisplayWidget) || list.empty()) return;

    ZCHXCameraListDlg d(list, mDisplayWidget->parentWidget());
    d.setWindowFlags( Qt::SubWindow | Qt::Dialog | Qt::FramelessWindowHint);
    d.setWindowModality(Qt::ApplicationModal);
    connect(&d, &ZCHXCameraListDlg::cameraDevSelected,  this,   &zchxCameraDatasMgr::slotSelectCameraFromDlg); //发送单击的相机信息
    connect(&d, &ZCHXCameraListDlg::cameraDevDoubleClicked, this, &zchxCameraDatasMgr::cameraDoubleClicked);
    Point2D pos;
    if(ll.isNull())
    {
        pos = mDisplayWidget->framework()->LatLon2Pixel(list[0].nLatLon.lat, list[0].nLatLon.lon);
    } else
    {
        pos = mDisplayWidget->framework()->LatLon2Pixel(ll);
    }
    d.resize(list.size() * 87,132);
    QPoint screenPos = mDisplayWidget->mapToGlobal(QPoint(pos.x, pos.y));
    d.move(screenPos);
    d.exec();
}

void zchxCameraDatasMgr::setCameraObservationZoneData(const std::vector<CameraObservationZone> &data)
{
    m_cameraObservationZone = data;
}

void zchxCameraDatasMgr::setCameraVideoWarnData(const std::vector<CameraVideoWarn> &data)
{
    //qDebug()<<__FILE__<<__LINE__<<data.size();
    //为了让视频分析的目标被选择，这里进行比对更新,java后台端对目标进行了缓存，如果目标在
    //m_CameraVideoWarn中存在，但是在data中不存在，则认为目标已经消失
    //取得当前已经选择的视频分析目标
    QString selObjId = "";
    for(int i=0; i<m_CameraVideoWarn.size(); i++)
    {
        CameraVideoWarn &item = m_CameraVideoWarn[i];
        if(item.getIsActive())
        {
            selObjId = item.getObjId();
            break;
        }
    }

    m_CameraVideoWarn = data;
    if(selObjId.length() > 0)
    {
        for(int i=0; i<m_CameraVideoWarn.size(); i++)
        {
            CameraVideoWarn &item = m_CameraVideoWarn[i];
            if(item.getObjId() == selObjId)
            {
                item.setIsActive(true);
                break;
            }
        }
    }
}
}


