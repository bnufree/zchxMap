#ifndef ZCHXMAPLOADTHREAD_H
#define ZCHXMAPLOADTHREAD_H

#include <QThread>
#include <QMutex>
#include "zchxecdisutils.h"

enum    TILE_ORIGIN_POS{
    TILE_ORIGIN_TOPLEFT = 0,        //左上
    TILE_ORIGIN_BOTTEMLEFT,         //左下
};

class zchxMapLoadThread : public QThread
{
    Q_OBJECT
public:
    explicit zchxMapLoadThread(TILE_ORIGIN_POS pos = TILE_ORIGIN_TOPLEFT, QObject *parent = 0);
    void     run();
private:
    bool     taskNow(MapLoadSetting& task);

signals:
    void     signalSendCurPixmap(const QPixmap& v, int x, int y);
    void     signalSendNewMap(double lon, double lat, int zoom);
    void     signalSendImgList(const TileImageList& list);
public slots:    
    void     appendTask(const MapLoadSetting& task);
    void     appendTileImg(const QPixmap& img, int x, int y);
private:
    QList<MapLoadSetting>       mTaskList;
    QMutex                      mMutex;
    QList<QThread*>             mWorkThreadList;
    QString                     mLocalUrl;
    TileImageList               mTileImgList;
    QMutex                      mImgMutex;
    int                         mOriginPos;
};

#endif // ZCHXMAPLOADTHREAD_H
