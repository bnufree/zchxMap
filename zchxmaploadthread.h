#ifndef ZCHXMAPLOADTHREAD_H
#define ZCHXMAPLOADTHREAD_H

#include <QThread>
#include <QMutex>
#include "zchxecdisutils.h"


class zchxMapLoadThread : public QThread
{
    Q_OBJECT
public:
    explicit zchxMapLoadThread(QObject *parent = 0);
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
};

#endif // ZCHXMAPLOADTHREAD_H
