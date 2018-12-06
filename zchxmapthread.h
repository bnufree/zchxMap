#ifndef ZCHXMAPTHREAD_H
#define ZCHXMAPTHREAD_H

#include <QThread>
#include <QList>
#include <QPixmap>
#include <QMutex>

struct MapBounds{
    double     min_x;
    double     min_y;
    double     max_x;
    double     max_y;
};

struct zchxMapTask{
    double  lon;
    double  lat;
    int     view_x;
    int     view_y;
    int     zoom;

    zchxMapTask() {}
    zchxMapTask(int x, int y, int zoom)
    {
        //63.822037,41.449826
        this->lat = 30.123456;
        this->lon = 104.123456;
        this->zoom = zoom;
        this->view_x = x;
        this->view_y = y;
    }

    zchxMapTask(double lon, double lat, int x, int y, int zoom)
    {
        this->lon = lon;
        this->lat = lat;
        this->view_x = x;
        this->view_y = y;
        this->zoom = zoom;
    }

    bool operator ==(const zchxMapTask& other)
    {
        return  this->lat == other.lat &&
                this->lon == other.lon &&
                this->view_x == other.view_x &&
                this->view_y == other.view_y &&
                this->zoom == other.zoom;
    }
};

class zchxMapThread : public QThread
{
    Q_OBJECT
public:
    explicit zchxMapThread(QObject *parent = 0);
    void     appendTask(const zchxMapTask& task);
    void     run();
private:
    bool     taskNow(zchxMapTask& task);
    double          calResolution(int zoom);
    MapBounds calViewBounds(double mct_x, double mct_y, double size_x, double size_y, double resolution);

signals:
    void     signalSendCurSize(int size_x, int size_y);
    void     signalSendCurPixmap(const QPixmap& v, int x, int y);

public slots:
private:
    QList<zchxMapTask>      mTaskList;
    QMutex                  mMutex;
};

#endif // ZCHXMAPTHREAD_H
