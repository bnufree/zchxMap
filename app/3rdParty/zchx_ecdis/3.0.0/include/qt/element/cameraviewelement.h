#ifndef CAMERAVIEWELE_H
#define CAMERAVIEWELE_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
class  CameraViewElement: public Element
{
public:
    explicit CameraViewElement(const ZCHX::Data::ITF_CameraView &data, zchxMapWidget* f);
    ZCHX::Data::ITF_CameraView getData() const;
    void setData(const ZCHX::Data::ITF_CameraView &data);
    virtual void drawElement(QPainter *painter);
    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_CameraView m_data;
};
}

#endif // CAMERAVIEWELE_H
