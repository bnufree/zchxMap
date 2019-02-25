#ifndef ZCHXECDISDLGMGR_H
#define ZCHXECDISDLGMGR_H

#include <QObject>
#include "zchxutils.hpp"

namespace qt {
class zchxMapWidget;
class RodElement;
class AisElement;
class zchxCameraDatasMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxCameraDatasMgr(zchxMapWidget* w, QObject *parent = 0);
    //调用相机列表对话框
    void zchxOpenCameraListDlg(RodElement *item);
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

#endif // ZCHXECDISDLGMGR_H
