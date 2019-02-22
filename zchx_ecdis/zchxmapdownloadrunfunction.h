#ifndef ZCHXMAPDOWNLOADRUNFUNCTION_H
#define ZCHXMAPDOWNLOADRUNFUNCTION_H

#include <QRunnable>
#include <QObject>

namespace qt {
struct zchxMapRange{
    double bottemLat;
    double bottemLon;    
    double topLat;
    double topLon;
};

struct zchxMapBounds{
    double     min_x;
    double     min_y;
    double     max_x;
    double     max_y;
};


class zchxMapDownloadRunFunction: public QObject, public QRunnable
{
    Q_OBJECT
public:
    zchxMapDownloadRunFunction(const zchxMapRange& range, int zoom);

    void run();
signals:
    void signalDownloadFinished(int zoom);
private:
    int mZoom;
    zchxMapRange mRange;
};
}

#endif // ZCHXMAPDOWNLOADRUNFUNCTION_H
