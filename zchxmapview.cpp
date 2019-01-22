#include "zchxmapview.h"

zchxMapView::zchxMapView(double center_lat, double center_lon, int zoom, int width, int height, QObject *parent) : QObject(parent)
{
    setViewSize(width, height);
    setZoom(zoom);
    updateCenter(center_lon, center_lat);
}

void zchxMapView::setViewSize(int width, int height)
{
    mViewHeight = height;
    mViewWidth = width;
    mTopLeft.mPixelPos.x = 0;
    mTopLeft.mPixelPos.y = 0;
    mRightBottom.mPixelPos.x = width;
    mRightBottom.mPixelPos.y = height;
}

void zchxMapView::setZoom(int zoom)
{
    if(zoom < 0 || zoom > 18) return;
    mCurZoom = zoom;
    mUnitMercatorLength = zchxEcdisUtils::calResolution(zoom);
}

int zchxMapView::zoom() const
{
    return mCurZoom;
}

void zchxMapView::updateCenter(double lon, double lat)
{
    mCenter.mLat = lat;
    mCenter.mLon = lon;
    //重新计算当前视窗的显示范围
    updateDisplayRange();
}

void zchxMapView::updateCenter(const Point2D &point)
{
    updateCenter(pix2Lonlat(point));
}

Point2D zchxMapView::mercator2pixel(const Mercator &mct)
{
    double x = mct.mX - mTopLeft.mMercatot.mX;
    double y = mct.mY - mTopLeft.mMercatot.mY;
    Point2D res;
    res.x = mTopLeft.mPixelPos.x + x / mUnitMercatorLength;
    res.y = mTopLeft.mPixelPos.y + y / mUnitMercatorLength;
    return res;
}

Point2D zchxMapView::lonlat2pix(const Wgs84LonLat &ll)
{
    //获取当前经纬度对应的墨卡托坐标
    Mercator mct = zchxEcdisUtils::wgs84LonlatToMercator(ll);
    //通过墨卡托坐标换算屏幕坐标
    return mercator2pixel(mct);
}



Wgs84LonLat zchxMapView::pix2Lonlat(const Point2D& pos)
{
    double x = pos.x - mTopLeft.mPixelPos.x;
    double y = pos.y - mTopLeft.mPixelPos.y;
    //获取当前指定位置对应的墨卡托坐标
    Mercator target;
    target.mX = mTopLeft.mMercatot.mX + mUnitMercatorLength * x;
    target.mY = mTopLeft.mMercatot.mY + mUnitMercatorLength * y;

    return zchxEcdisUtils::mercatorToWgs84LonLat(target);
}

void zchxMapView::updateDisplayRange()
{
    if(mViewWidth == 0 || mViewHeight == 0) return;
    //计算当前中心经纬度对应的墨卡托坐标
    Mercator center_mct = zchxEcdisUtils::wgs84LonlatToMercator(mCenter);
    //计算当前视窗对应的墨卡托坐标的显示范围
    mTopLeft.mMercatot.mX = center_mct.mX - mUnitMercatorLength * mViewWidth / 2.0;
    mTopLeft.mMercatot.mY = center_mct.mY - mUnitMercatorLength * mViewHeight / 2.0;
    mRightBottom.mMercatot.mX = center_mct.mX + mUnitMercatorLength * mViewWidth / 2.0;
    mRightBottom.mMercatot.mY = center_mct.mY + mUnitMercatorLength * mViewHeight / 2.0;
}

void zchxMapView::zoomIn()
{
    int zoom = mCurZoom;
    setZoom(++zoom);
}

void zchxMapView::zoomOut()
{
    int zoom = mCurZoom;
    setZoom(--zoom);
}
