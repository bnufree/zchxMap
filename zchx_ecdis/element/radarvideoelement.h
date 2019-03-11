#ifndef RADARVIDEOELE_H
#define RADARVIDEOELE_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
class RadarVideoGlowElement : public Element
{
public:
    explicit RadarVideoGlowElement(const ZCHX::Data::ITF_RadarVideoGLow& data, zchxMapWidget* frame);

    const ZCHX::Data::ITF_RadarVideoGLow &data() const;
    void setData(const ZCHX::Data::ITF_RadarVideoGLow& dev);
    void drawElement(QPainter *painter);
    void drawOutline(QPainter *painter, const QPointF& center, double in, double out);
    std::string name () const {return m_data.name.toStdString();}

private:
    ZCHX::Data::ITF_RadarVideoGLow  m_data;
};

}

#endif // RADARVIDEOELE_H
