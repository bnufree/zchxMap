#ifndef ZCHXCAMERADATAMGR_H
#define ZCHXCAMERADATAMGR_H

#include "zchxecdisdatamgr.h"

namespace qt {
//杆子相机，船载相机不包含在这里。这里只是相机直接显示的情况
class zchxCameraDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxCameraDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void    show(QPainter* painter, double offset_x, double offset_y);
    bool    updateActiveItem(const QPoint& pt);
    void    updateCameraStatus(const QString &id, int sts);

    //所有相机（枪机球机光电仪全景相机无人机）
    void setCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data);

private:


signals:
    void signalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机点击
    void cameraDoubleClicked(const ZCHX::Data::ITF_CameraDev &info);//相机双击

private:
    QMap<QString, std::shared_ptr<CameraElement>> m_CameraDev;            //相机设备
};
}

#endif // ZCHXCAMERADATAMGR_H
