#ifndef ZCHXMAPVIEW_H
#define ZCHXMAPVIEW_H

#include <QObject>
#include <QPointF>
#include "zchxecdisutils.h"

class zchxMapView : public QObject
{
    Q_OBJECT
public:
    explicit zchxMapView(double center_lat, double center_lon, int zoom, int width, int height, QObject *parent = 0);
    Wgs84LonLat   pix2Lonlat(const Point2D& pos);
    Point2D     lonlat2pix(const Wgs84LonLat& ll);
    int         zoom() const;
    void        setZoom(int zoom) ;
    void        setViewSize(int width, int height);
    void        updateCenter(double lon, double lat);
    void        updateCenter(const Point2D& point);
    void        updateCenter(const Wgs84LonLat& ll) {updateCenter(ll.mLon, ll.mLat);}
    Wgs84LonLat center() const {return mCenter;}
    void        updateDisplayRange();
    Point2D     mercator2pixel(const Mercator& mct);
    void        zoomIn();
    void        zoomOut();
    void        update();
    void        drag(int x, int y);

signals:
    void        updateMap(const MapLoadSetting& set);
public slots:
private:
    MapRangeData        mMapRange;      //墨卡托坐标范围  左下最小  右上最大
    int                 mCurZoom;
    int                 mViewWidth;
    int                 mViewHeight;
    double              mUnitMercatorLength;
    Wgs84LonLat           mCenter;
};

#endif // ZCHXMAPVIEW_H
