#ifndef RODELE_H
#define RODELE_H

#include "fixelement.h"

namespace qt
{

class  RodElement : public FixElement<ZCHX::Data::ITF_CameraRod>
{
public:
    explicit RodElement(const ZCHX::Data::ITF_CameraRod &data, zchxMapWidget* f);
    QList<ZCHX::Data::ITF_CameraDev> getCameraList() const;
    QList<ZCHX::Data::IPCastDevice> getIPCList() const;

    void updateCamera(const ZCHX::Data::ITF_CameraDev& data);
    void updateIPC(const ZCHX::Data::IPCastDevice& data);

    void setStatus(ZCHX::Data::CAMERAROD_STATUS st);
    ZCHX::Data::CAMERAROD_STATUS status() const;
    virtual void drawElement(QPainter *painter);
    void updateElementStatus(qint64 ele, bool sts);
    void copyDataFromOther(std::shared_ptr<Element> other);

private:
    //保存当前各个子项的状态.子项全部OK则杆位OK,否则就是异常
    QList<ZCHX::Data::ITF_CameraDev>        mCameraList;
    QList<ZCHX::Data::IPCastDevice>         mIpcList;
    QList<qint64>           mOKEleList;
    QList<qint64>           mErrEleList;
};
}

#endif // RODELE_H
