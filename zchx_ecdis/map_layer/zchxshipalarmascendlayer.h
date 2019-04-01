#ifndef ZCHXSHIPALARMASCENDLAYER_H
#define ZCHXSHIPALARMASCENDLAYER_H

#include "zchxMapLayer.h"

namespace qt {
class ZCHX_ECDIS_EXPORT zchxShipAlarmAscendLayer : public MapLayer
{
    Q_OBJECT
public:
    explicit zchxShipAlarmAscendLayer(zchxMapWidget *drawWidget, bool visible, QObject *parent = 0);
    void drawLayer(QPainter *painter);
signals:




public slots:


private:
};
}

#endif // ZCHXSHIPALARMASCENDLAYER_H
