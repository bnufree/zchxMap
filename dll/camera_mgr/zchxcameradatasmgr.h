#ifndef ZCHXCAMERADATASMGR_H
#define ZCHXCAMERADATASMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>

class zchxMapWidget;
class zchxCameraDatasMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxCameraDatasMgr(zchxMapWidget* w, QObject *parent = 0);

    //摄像杆信息
    //更新摄像杆的信息
    void setCameraRodData(const std::vector<DrawElement::CameraRodElement> &data);
    //检查指定摄像杆的状态
    ZCHX::Data::CAMERAROD_STATUS checkCameraRodStatus(DrawElement::CameraRodElement *item);
    void zchxUpdateCameraRodStatus(uint uuid);
    //获取指定的摄像杆
    DrawElement::CameraRodElement *getCameraRod(uint uuid);
    void    showCameraRod(QPainter* painter);

    //号角设备
    std::shared_ptr<ZCHX::Data::IPCastDevice> getIPCastDevice(const QString &rodID);  //通过杆ID查询对应的号角设备
    void updateIPCastDeviceList(std::list<std::shared_ptr<ZCHX::Data::IPCastDevice> > list);

    //所有相机（枪机球机光电仪全景相机无人机船载相机）
    void setCameraDevData(const std::vector<DrawElement::CameraElement> &data);
    void showCamera(QPainter* painter);

    //调用相机列表对话框
    void zchxOpenCameraListDlg(DrawElement::CameraRodElement *item);
    void zchxOpenCameraListDlg(DrawElement::AisElement *item);
    void zchxOpenCameraListDlg(QList<ZCHX::Data::ITF_CameraDev>& list, const LatLon& ll = LatLon());

private:
    void updateAllCameraRodStatus();


signals:
    void signalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机点击
    void cameraDoubleClicked(const ZCHX::Data::ITF_CameraDev &info);//相机双击

public slots:
    void slotSelectCameraFromDlg(const ZCHX::Data::ITF_CameraDev &camera);

private:
    std::vector<DrawElement::CameraRodElement> m_CameraRod;            //摄像杆数据
    std::vector<DrawElement::CameraElement> m_CameraDev;            //相机设备
    std::list<std::shared_ptr<ZCHX::Data::IPCastDevice>> m_ipcastDevices; //号角设备

    zchxMapWidget       *mDisplayWidget;
};

#endif // ZCHXCAMERADATASMGR_H
