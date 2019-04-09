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
    virtual QList<QAction*> getRightMenuActions(const QPoint &pt);

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
public slots:
};

class zchxDrawPickPointsTool: public zchxDrawZoneTool
{
    Q_OBJECT
public:
    explicit zchxDrawPickPointsTool(zchxMapWidget* w, QObject *parent = 0):
        zchxDrawZoneTool(w, qt::eTool::PICKPOINTS, 3, parent) {}
    void endDraw();

signals:
public slots:
};

class zchxDrawChannelZoneTool: public zchxDrawZoneTool
{
    Q_OBJECT
public:
    explicit zchxDrawChannelZoneTool(zchxMapWidget* w, QObject *parent = 0):
        zchxDrawZoneTool(w, qt::eTool::CHANNELMANAGER, 3, parent) {}
    void endDraw();

signals:
public slots:
};

class zchxDrawMooringZoneTool: public zchxDrawZoneTool
{
    Q_OBJECT
public:
    explicit zchxDrawMooringZoneTool(zchxMapWidget* w, QObject *parent = 0):
        zchxDrawZoneTool(w, qt::eTool::MOORINGMANAGER, 3, parent) {}
    void endDraw();

signals:
public slots:
};

class zchxDrawCardMouthTool: public zchxDrawZoneTool
{
    Q_OBJECT
public:
    explicit zchxDrawCardMouthTool(zchxMapWidget* w, QObject *parent = 0):
        zchxDrawZoneTool(w, qt::eTool::CARDMOUTHMANAGER, 3, parent) {}
    void show(QPainter *painter);
    void endDraw();

signals:
public slots:
};

}

#endif // ZCHXDRAWRADARFEATUREZONETOOL_H
