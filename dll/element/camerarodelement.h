#ifndef CAMERARODELEMENT_H
#define CAMERARODELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace DrawElement
{

class  CameraRodElement : public Element
{
public:
    explicit CameraRodElement(const ZCHX::Data::ITF_CameraRod &data);
    ZCHX::Data::ITF_CameraRod data() const;

    void setStatus(ZCHX::Data::CAMERAROD_STATUS st);
    ZCHX::Data::CAMERAROD_STATUS status() const;
    virtual void drawElement(QPainter *painter);
    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_CameraRod m_data;
};
}

#endif // CAMERARODELEMENT_H
