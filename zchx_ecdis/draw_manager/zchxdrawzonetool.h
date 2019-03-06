#ifndef ZCHXDRAWZONETOOL_H
#define ZCHXDRAWZONETOOL_H

#include "zchxdrawtool.h"

namespace qt {
class zchxDrawZoneTool : public zchxDrawTool
{
    Q_OBJECT
public:
    explicit zchxDrawZoneTool(zchxMapWidget* w, int type, int num, QObject *parent = 0);
    virtual void show(QPainter *painter);
    virtual bool checkPnts();

signals:

public slots:
private:
    int     mMinPntsNum;
};

class zchxDrawRadarFeatureZoneTool: public zchxDrawZoneTool
{
    Q_OBJECT
public:
    explicit zchxDrawRadarFeatureZoneTool(zchxMapWidget* w, QObject *parent = 0):
        zchxDrawZoneTool(w, qt::eTool::ZONEDRAWRADAR, 4, parent) {}
    void endDraw();

signals:
    void signalCreateRadarFeatureZONE(const ZCHX::Data::ITF_RadarFeaturesZone& zone);
public slots:
};

class zchxDrawWarningZoneTool: public zchxDrawZoneTool
{
    Q_OBJECT
public:
    explicit zchxDrawWarningZoneTool(zchxMapWidget* w, QObject *parent = 0):
        zchxDrawZoneTool(w, qt::eTool::ZONEDRAW, 3, parent) {}
    void endDraw();

signals:
    void signalCreateWarringZONE(const ZCHX::Data::ITF_WarringZone& zone);
public slots:
};

}

#endif // ZCHXDRAWRADARFEATUREZONETOOL_H
