#ifndef ZCHXMAPFRAMEWORK_H
#define ZCHXMAPFRAMEWORK_H

#include <QObject>
#include <QPointF>
#include "zchxMapDataUtils.h"

namespace qt {
class zchxMapFrameWork : public QObject
{
    Q_OBJECT
public:
    explicit zchxMapFrameWork(double center_lat, double center_lon, int zoom, int width, int height, int source, QObject *parent = 0);
    LatLon      Pixel2LatLon(const Point2D& pos);
    Point2D     LatLon2Pixel(const LatLon& ll);
    Point2D     LatLon2Pixel(double lat, double lon);
    int         Zoom() const;
    int         GetDrawScale() const {return Zoom();}
    void        SetZoom(int zoom) ;
    void        SetMinZoom(int zoom);
    void        SetMaxZoom(int zoom);
    void        SetViewSize(int width, int height);
    void        UpdateCenterAndZoom(const LatLon& ll, int zoom);
    void        UpdateCenter(double lon, double lat);
    void        UpdateCenter(const Point2D& point);
    void        UpdateCenter(const LatLon& ll) {UpdateCenter(ll.lon, ll.lat);}
    LatLon      Center() const {return mCenter;}
    void        UpdateDisplayRange();
    Point2D     Mercator2Pixel(const Mercator& mct);
    void        ZoomIn();
    void        ZoomOut();
    void        Update();
    void        Drag(int x, int y);
    void        SetSource(int source);

    //mapStyle
    void        SetMapStyle(MapStyle style) {mStyle = style;}
    MapStyle    GetMapStyle() const {return mStyle;}
    //地图旋转不支持,旋转角度为正北方向夹角
    double      GetRotateAngle() {return 0.0;}
    void        SetRotateAngle(double ang) {mRotateAngle = ang;}

signals:
    void        UpdateMap(const MapLoadSetting& set);
public slots:
private:
    MapRangeData        mMapRange;      //墨卡托坐标范围  左下最小  右上最大
    int                 mCurZoom;
    int                 mMinZoom;
    int                 mMaxZoom;
    int                 mViewWidth;
    int                 mViewHeight;
    double              mUnitMercatorLength;
    LatLon              mCenter;
    int                 mSource;
    MapStyle            mStyle;
    double              mRotateAngle;
};
}

#endif // ZCHXMAPFRAMEWORK_H
