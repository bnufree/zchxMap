#ifndef ZCHXRADARVIDEOLAYER_H
#define ZCHXRADARVIDEOLAYER_H

#include "zchxMapLayer.h"

namespace qt {
class zchxRadarVideoLayer : public MapLayer
{
    Q_OBJECT
public:
    explicit zchxRadarVideoLayer(zchxMapWidget *drawWidget, bool visible, QObject *parent = 0);
    void    setIsDisplay(bool sts);
    void    setRadarVideoData(double dCentreLon, double dCentreLat, double dDistance, int uDisplayType,int uLoopNum);
    void    setRadarVideoPixmap(int uIndex, const QPixmap &objPixmap, const QPixmap &prePixmap);
    void    setCurrentRadarVideoPixmap(const QPixmap &objPixmap);
    void    update();
signals:




public slots:


private:
    ZCHX::Data::ITF_RadarVideoGLow      mRadarVideoData;
};
}

#endif // ZCHXRADARVIDEOLAYER_H
