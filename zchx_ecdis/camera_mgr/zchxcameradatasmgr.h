#ifndef ZCHXCAMERADATASMGR_H
#define ZCHXCAMERADATASMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "../zchxutils.hpp"
#include "../zchxMapDatautils.h"
#include <QPainter>

namespace qt {
class zchxMapWidget;
class zchxCameraDatasMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxCameraDatasMgr(zchxMapWidget* w, QObject *parent = 0);

    //摄像杆信息
    //更新摄像杆的信息
    void setCameraRodData(const std::vector<CameraRodElement> &data);
    //检查指定摄像杆的状态
    ZCHX::Data::CAMERAROD_STATUS checkCameraRodStatus(CameraRodElement *item);
    void zchxUpdateCameraRodStatus(uint uuid);
    //获取指定的摄像杆
    CameraRodElement *getCameraRod(uint uuid);
    void    showCameraRod(QPainter* painter);

    //号角设备
    std::shared_ptr<ZCHX::Data::IPCastDevice> getIPCastDevice(const QString &rodID);  //通过杆ID查询对应的号角设备
    void updateIPCastDeviceList(std::list<std::shared_ptr<ZCHX::Data::IPCastDevice> > list);

    //所有相机（枪机球机光电仪全景相机无人机船载相机）
    void setCameraDevData(const std::vector<CameraElement> &data);
    void showCamera(QPainter* painter);

    //调用相机列表对话框
    void zchxOpenCameraListDlg(CameraRodElement *item);
    void zchxOpenCameraListDlg(AisElement *item);
    void zchxOpenCameraListDlg(QList<ZCHX::Data::ITF_CameraDev>& list, const LatLon& ll = LatLon());

    //相机视场
    void setCameraObservationZoneData(const std::vector<CameraObservationZone> &data);
    //人车船视频分析目标更新
    void setCameraVideoWarnData(const std::vector<CameraVideoWarn> &data);

private:
    void updateAllCameraRodStatus();


signals:
    void signalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机点击
    void cameraDoubleClicked(const ZCHX::Data::ITF_CameraDev &info);//相机双击

public slots:
    void slotSelectCameraFromDlg(const ZCHX::Data::ITF_CameraDev &camera);

private:
    std::vector<CameraRodElement> m_CameraRod;            //摄像杆数据
    std::vector<CameraElement> m_CameraDev;            //相机设备
    std::list<std::shared_ptr<ZCHX::Data::IPCastDevice>> m_ipcastDevices; //号角设备
    std::vector<CameraObservationZone> m_cameraObservationZone;    //相机视场
    std::vector<CameraVideoWarn> m_CameraVideoWarn;//人车船

    zchxMapWidget       *mDisplayWidget;
};
}

#endif // ZCHXCAMERADATASMGR_H
