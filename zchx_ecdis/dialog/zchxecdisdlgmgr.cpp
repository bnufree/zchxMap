#include "zchxecdisdlgmgr.h"
#include "zchxcameralistdlg.h"
#include "zchxmapwidget.h"
#include "zchxmapframe.h"
#include "element/aiselement.hpp"
#include "element/rodelement.h"
#include <QDebug>

namespace qt {
zchxEcdisDlgMgr::zchxEcdisDlgMgr(zchxMapWidget* w,  QObject *parent) : QObject(parent),
    mDisplayWidget(w)
{

}

void zchxEcdisDlgMgr::zchxOpenCameraListDlg(RodElement *item)
{
    if(!item) return;
    QList<ZCHX::Data::ITF_CameraDev> list = item->getCameraList();
    QList<ZCHX::Data::IPCastDevice> device = item->getIPCList();
    if(device.size())
    {
        ZCHX::Data::ITF_CameraDev fakecam;
        fakecam.nType = ZCHX::Data::CAMERA_FAKE_IPCAST;
        fakecam.mIpcastDevice = device[0];
        fakecam.szCamName = device[0].name;
        fakecam.nStatus = device[0].status == -1 ? 512 : 1;//状态转换
        list.append(fakecam);
    }
    if(list.empty())
    {
        qDebug()<<"No camera deivce on this Camera rod!"<<item->getData().szName;
        return;
    }
    zchxOpenCameraListDlg(list, ZCHX::Data::LatLon(item->lat(), item->lon()));
}

void zchxEcdisDlgMgr::zchxOpenCameraListDlg(AisElement *item)
{
    if(!item && !item->hasCamera())
        return;


    QList<ZCHX::Data::ITF_CameraDev> list = QList<ZCHX::Data::ITF_CameraDev>::fromVector(QVector<ZCHX::Data::ITF_CameraDev>::fromStdVector(item->getCameraData()));
    if(list.empty())
    {
        qDebug()<<"No camera deivce on this ais :"<<item->data().mmsi;
        return;
    }
    zchxOpenCameraListDlg(list, ZCHX::Data::LatLon(item->lat(), item->lon()));
}

//在指定的经纬度位置弹出对话框
void zchxEcdisDlgMgr::zchxOpenCameraListDlg(QList<ZCHX::Data::ITF_CameraDev>& list, const ZCHX::Data::LatLon& ll)
{
    if((!mDisplayWidget) || list.empty()) return;

    ZCHXCameraListDlg d(list, mDisplayWidget->parentWidget());
    d.setWindowFlags( Qt::SubWindow | Qt::Dialog | Qt::FramelessWindowHint);
    d.setWindowModality(Qt::ApplicationModal);
    connect(&d, &ZCHXCameraListDlg::cameraDevSelected,  this,   &zchxEcdisDlgMgr::signalIsSelected4CameraDev); //发送单击的相机信息
    connect(&d, &ZCHXCameraListDlg::cameraDevDoubleClicked, this, &zchxEcdisDlgMgr::signalCameraDoubleClicked);
    ZCHX::Data::Point2D pos;
    if(ll.isNull())
    {
        pos = mDisplayWidget->framework()->LatLon2Pixel(list[0].getLat(), list[0].getLon());
    } else
    {
        pos = mDisplayWidget->framework()->LatLon2Pixel(ll);
    }
    d.resize(list.size() * 87,132);
    QPoint screenPos = mDisplayWidget->mapToGlobal(QPoint(pos.x, pos.y));
    d.move(screenPos);
    d.exec();
}
}


