#ifndef ZCHXRODDATAMGR_H
#define ZCHXRODDATAMGR_H

#include "zchxecdisdatamgr.h"
namespace qt {
class zchxRodDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxRodDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter* painter, double offset_x, double offset_y);
    bool updateActiveItem(const QPoint &pt);

    //摄像杆的信息
    void setRodData(const QList<ZCHX::Data::ITF_CameraRod> &list);
    void updateRodStatus(const QString& id, ZCHX::Data::CAMERAROD_STATUS sts);
    RodElement *getRod(const QString& id);
    //更新相机
    void updateCamera(const QList<ZCHX::Data::ITF_CameraDev>& list);
    QList<ZCHX::Data::ITF_CameraDev> getCameraOfRod(const QString& id);
    void updateCameraStatus(const QString& rod, const QString& cam, int status);

    //号角设备
    QList<ZCHX::Data::IPCastDevice> getIPCastDeviceOfRod(const QString& id);  //通过杆ID查询对应的号角设备
    void updateIPCastDeviceList(const QList<ZCHX::Data::IPCastDevice>& list);
    void updateIpcStatus(const QString& rod, const QString& ipc, int status);

private:
    void updateAllCameraRodStatus();


signals:
    void signalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机点击
    void cameraDoubleClicked(const ZCHX::Data::ITF_CameraDev &info);//相机双击

public slots:

private:
    QMap<QString, std::shared_ptr<RodElement>> m_CameraRod;            //摄像杆数据
};
}

#endif // ZCHXRODDATAMGR_H
