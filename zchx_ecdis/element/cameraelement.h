#ifndef CAMERAELEMENT_H
#define CAMERAELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
class CameraElement : public Element
{
public:
    explicit CameraElement(const ZCHX::Data::ITF_CameraDev & data, zchxMapFrameWork* frame);

    const ZCHX::Data::ITF_CameraDev &getData() const;
    void setData(const ZCHX::Data::ITF_CameraDev& dev);
    int getParentID() const;

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
    void setParent(Element* ele);
    Element* getParent();

private:
    ZCHX::Data::ITF_CameraDev m_data;
    Element     *mParent;               //相机悬挂的目标
};

}

#endif // CAMERAELEMENT_H
