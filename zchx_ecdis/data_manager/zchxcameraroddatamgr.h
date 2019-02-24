#ifndef ZCHXCAMERARODDATAMGR_H
#define ZCHXCAMERARODDATAMGR_H

#include "zchxecdisdatamgr.h"
namespace qt {
class zchxCameraRodDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxCameraRodDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter *painter);
    bool updateActiveItem(const QPoint &pt);

    //摄像杆的信息
    void setCameraRodData(const QList<ZCHX::Data::ITF_CameraRod> &list);
    void updateCameraRodStatus(const QString& id, ZCHX::Data::CAMERAROD_STATUS sts);
    CameraRodElement *getRod(const QString& id);
    //更新相机
    void updateCamera(QList<ZCHX::Data::ITF_CameraDev>& list);
    QList<ZCHX::Data::ITF_CameraDev> getCameraOfRod(const QString& id);
    void updateCameraStatus(const QString& rod, const QString& cam, int status);

    //号角设备
    ZCHX::Data::IPCastDevice getIPCastDevice(const QString& id);  //通过杆ID查询对应的号角设备
    void updateIPCastDeviceList(const QString& id, QList<ZCHX::Data::IPCastDevice>& list);

private:
    void updateAllCameraRodStatus();


signals:
    void signalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机点击
    void cameraDoubleClicked(const ZCHX::Data::ITF_CameraDev &info);//相机双击

public slots:
    void slotSelectCameraFromDlg(const ZCHX::Data::ITF_CameraDev &camera);

private:
    QMap<QString, std::shared_ptr<CameraRodElement>> m_CameraRod;            //摄像杆数据
};
}

#endif // ZCHXCAMERARODDATAMGR_H
