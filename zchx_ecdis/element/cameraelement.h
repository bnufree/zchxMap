#ifndef CAMERAELEMENT_H
#define CAMERAELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
class CameraElement : public Element
{
public:
    explicit CameraElement(const ZCHX::Data::ITF_CameraDev & data, Element* depends = 0);

    const ZCHX::Data::ITF_CameraDev &getData() const;
    void setData(const ZCHX::Data::ITF_CameraDev& dev);
    int getGanPid() const;
    ZCHX::Data::CAMERATYPE getType() const;

    uint getStatus() const;
    void setStatus(uint status);

    bool isNormal() const;
    bool isBug() const;
    bool isError() const;

    QPixmap getImage(zchxMapFrameWork *f = 0) const;
    double getMaxTrackRange() const;

    void updateGeometry(QPointF, int){}
    void drawElement(QPainter *painter);

private:
    ZCHX::Data::ITF_CameraDev m_data;
    Element*        mDependElement;             //相机所依附的第三方图元　　比如杆　　船等
};

}

#endif // CAMERAELEMENT_H
