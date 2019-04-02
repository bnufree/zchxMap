#ifndef RADARVIDEOELE_H
#define RADARVIDEOELE_H

#include "fixelement.h"

namespace qt {
class ZCHX_ECDIS_EXPORT RadarVideoGlowElement : public FixElement<ZCHX::Data::ITF_RadarVideoGLow>
{
public:
    explicit RadarVideoGlowElement(const ZCHX::Data::ITF_RadarVideoGLow& data, zchxMapWidget* frame)
        :FixElement<ZCHX::Data::ITF_RadarVideoGLow>(data, ZCHX::Data::ELE_RADAR_VIDEOGLOW, ZCHX::LAYER_RADARVIDEO, frame) {}
    void drawElement(QPainter *painter);
    void drawOutline(QPainter *painter, const QPointF& center, double in, double out);
    void copyDataFromOther(std::shared_ptr<Element> other);

private:
};

}

#endif // RADARVIDEOELE_H
