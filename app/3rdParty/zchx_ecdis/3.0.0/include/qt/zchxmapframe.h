#ifndef ZCHXMAPFRAMEWORK_H
#define ZCHXMAPFRAMEWORK_H

#include <QObject>
#include <QPointF>
#include "zchxMapDataUtils.h"

namespace qt {
class zchxMapLoadThread;
class zchxMapFrameWork : public QObject
{
    Q_OBJECT
public:
    explicit zchxMapFrameWork(double center_lat, double center_lon, int zoom, int width, int height, int source, int min_zoom, int max_zoom, QObject *parent = 0);
    ZCHX::Data::LatLon      Pixel2LatLon(const ZCHX::Data::Point2D& pos);
    ZCHX::Data::Point2D     LatLon2Pixel(const ZCHX::Data::LatLon& ll);
    ZCHX::Data::Point2D     LatLon2Pixel(double lat, double lon);
    void        setOffSet(int offset_x, int offset_y);
    int         Zoom() const;
    int         GetDrawScale() const {return Zoom();}
    void        SetZoom(int zoom) ;
    void        SetMinZoom(int zoom);
    void        SetMaxZoom(int zoom);
    int         GetMinZoom() const {return mMinZoom;}
    void        Zoom2Min() {SetZoom(mMinZoom);}
    void        SetViewSize(int width, int height);
    void        UpdateCenterAndZoom(const ZCHX::Data::LatLon& ll, int zoom);
    void        UpdateCenter(double lon, double lat);
    void        UpdateCenter(const ZCHX::Data::Point2D& point);
    void        UpdateCenter(const ZCHX::Data::LatLon& ll) {UpdateCenter(ll.lon, ll.lat);}
    ZCHX::Data::LatLon      Center() const {return mCenter;}
    void        UpdateDisplayRange();
    ZCHX::Data::Point2D     Mercator2Pixel(const ZCHX::Data::Mercator& mct);
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
    //
    PPATH convert2QtPonitList(const GPATH &path);
    //          画地图
    void        updateEcdis(QPainter* painter, QRect rect,  bool image_num = false);

signals:
    void        signalSendCurMapinfo(double lat, double lon, int zoom);
public slots:
    void append(const QPixmap& img, int x, int y);
    void append(const TileImageList& list);
    void clear() {mDataList.clear(); /*update();*/}
    void slotRecvNewMap(double lon, double lat, int zoom, bool sync);

private:
    MapRangeData        mMapRange;      //墨卡托坐标范围  左下最小  右上最大
    int                 mCurZoom;
    int                 mMinZoom;
    int                 mMaxZoom;
    int                 mViewWidth;
    int                 mViewHeight;
    double              mUnitMercatorLength;
    ZCHX::Data::LatLon              mCenter;
    int                 mSource;
    MapStyle            mStyle;
    double              mRotateAngle;
    QSize               mOffset;
    //地图图片更新
    zchxMapLoadThread*          mMapThread;
    TileImageList               mDataList;
};
}

#endif // ZCHXMAPFRAMEWORK_H
