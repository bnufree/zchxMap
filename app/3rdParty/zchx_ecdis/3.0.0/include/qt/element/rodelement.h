#ifndef RODELEMENT_H
#define RODELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt
{

class  RodElement : public Element
{
public:
    explicit RodElement(const ZCHX::Data::ITF_CameraRod &data, zchxMapFrameWork* f);
    ZCHX::Data::ITF_CameraRod getData() const;
    void setData(const ZCHX::Data::ITF_CameraRod &data);

    QList<ZCHX::Data::ITF_CameraDev> getCameraList() const;
    QList<ZCHX::Data::IPCastDevice> getIPCList() const;

    void setStatus(ZCHX::Data::CAMERAROD_STATUS st);
    ZCHX::Data::CAMERAROD_STATUS status() const;
    virtual void drawElement(QPainter *painter);
    void updateGeometry(QPointF, int){}
    void updateElementStatus(qint64 ele, bool sts);
private:
    ZCHX::Data::ITF_CameraRod m_data;
    //保存当前各个子项的状态.子项全部OK则杆位OK,否则就是异常
    QList<qint64>           mOKEleList;
    QList<qint64>           mErrEleList;
};
}

#endif // RODELEMENT_H
