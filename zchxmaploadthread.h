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
public slots:    
    void     appendTask(const MapLoadSetting& task);
private:
    QList<MapLoadSetting>       mTaskList;
    QMutex                      mMutex;
    QList<QThread*>             mWorkThreadList;
    QString                     mLocalUrl;
};

#endif // ZCHXMAPLOADTHREAD_H
