#ifndef ZCHXMAPLOADTHREAD_H
#define ZCHXMAPLOADTHREAD_H

#include <QObject>
#include <QMutex>
#include "zchxecdisutils.h"

//每次加载瓦片地图的参数设定,主要是视窗的墨卡托范围,视窗屏幕坐标大小
struct MapLoadSetting{
    MapPointData    mTopLeft;
    MapPointData    mBottomRight;
    double          mResolution;
};


class zchxMapLoadThread : public QObject
{
    Q_OBJECT
public:
    explicit zchxMapLoadThread(QObject *parent = 0);
    void     appendTask(const MapLoadSetting& task);
    void     run();
private:
    bool     taskNow(MapLoadSetting& task);

signals:

public slots:
private:
    QList<MapLoadSetting>       mTaskList;
    QMutex                      mMutex;
    QList<QThread*>             mWorkThreadList;
};

#endif // ZCHXMAPLOADTHREAD_H
