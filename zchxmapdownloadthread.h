#ifndef ZCHXMAPDOWNLOADTHREAD_H
#define ZCHXMAPDOWNLOADTHREAD_H

#include <QThread>

class zchxMapDownloadThread : public QThread
{
    Q_OBJECT
public:
    explicit zchxMapDownloadThread(QObject *parent = 0);
public:
    void run();

signals:
    void signalDownloadFinished(int zoom);
public slots:
};

#endif // ZCHXMAPDOWNLOADTHREAD_H
