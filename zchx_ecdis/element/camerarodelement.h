#ifndef CAMERARODELEMENT_H
#define CAMERARODELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt
{

class  CameraRodElement : public Element
{
public:
    explicit CameraRodElement(const ZCHX::Data::ITF_CameraRod &data);
    ZCHX::Data::ITF_CameraRod getData() const;
    void setData(const ZCHX::Data::ITF_CameraRod &data);

    QList<ZCHX::Data::ITF_CameraDev> getCameraList() const;
    void setCameraData(const QList<ZCHX::Data::ITF_CameraDev> &data);

    QList<ZCHX::Data::IPCastDevice> getIPCList() const;
    void setIPCData(const QList<ZCHX::Data::IPCastDevice> &data);

    void setStatus(ZCHX::Data::CAMERAROD_STATUS st);
    ZCHX::Data::CAMERAROD_STATUS status() const;
    virtual void drawElement(QPainter *painter);
    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_CameraRod m_data;
    QList<ZCHX::Data::ITF_CameraDev> m_camera_list;
    QList<ZCHX::Data::IPCastDevice> m_ipc_list;
};
}

#endif // CAMERARODELEMENT_H
