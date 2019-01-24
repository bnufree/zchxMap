#include "zchxmapview.h"
#include <QDebug>

zchxMapView::zchxMapView(double center_lat, double center_lon, int zoom, int width, int height, QObject *parent) : QObject(parent),
  mViewWidth(0),
  mViewHeight(0)
{
    setZoom(zoom);
    updateCenter(center_lon, center_lat);    
    setViewSize(width, height);
}

void zchxMapView::setViewSize(int width, int height)
{
    mViewHeight = height;
    mViewWidth = width;
    updateDisplayRange();
}

void zchxMapView::setZoom(int zoom)
{
    if(zoom < 0 || zoom > 22) return;
    mCurZoom = zoom;
    mUnitMercatorLength = zchxEcdisUtils::calResolution(zoom);
    updateDisplayRange();
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

void zchxMapView::drag(int x, int y)
{
    updateCenter(Point2D(0.5*mViewWidth + x, 0.5*mViewHeight + y));
}

void zchxMapView::updateCenter(const Point2D &point)
{
    updateCenter(pix2Lonlat(point));
}

Point2D zchxMapView::mercator2pixel(const Mercator &mct)
{
    double x = mct.mX - mMapRange.mLowerLeft.mX;
    double y = mct.mY - mMapRange.mTopRight.mY;
    Point2D res;
    res.x = x / mUnitMercatorLength;
    res.y = 0 - y / mUnitMercatorLength;
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
    double x = pos.x;
    double y = mViewHeight - pos.y;
    //获取当前指定位置对应的墨卡托坐标
    Mercator target;
    target.mX = mMapRange.mLowerLeft.mX + mUnitMercatorLength * x;
    target.mY = mMapRange.mLowerLeft.mY + mUnitMercatorLength * y;

    return zchxEcdisUtils::mercatorToWgs84LonLat(target);
}

void zchxMapView::updateDisplayRange()
{
    if(mViewWidth == 0 || mViewHeight == 0) return;
    //计算当前中心经纬度对应的墨卡托坐标
    Mercator center_mct = zchxEcdisUtils::wgs84LonlatToMercator(mCenter);
    //计算当前视窗对应的墨卡托坐标的显示范围
    mMapRange.mLowerLeft.mX = center_mct.mX - mUnitMercatorLength * mViewWidth / 2.0;
    mMapRange.mLowerLeft.mY = center_mct.mY - mUnitMercatorLength * mViewHeight / 2.0;
    mMapRange.mTopRight.mX = center_mct.mX + mUnitMercatorLength * mViewWidth / 2.0;
    mMapRange.mTopRight.mY = center_mct.mY + mUnitMercatorLength * mViewHeight / 2.0;

    MapLoadSetting setting;
    setting.mMapRange = mMapRange;
    setting.mMode = 0;
    setting.mResolution = mUnitMercatorLength;
    setting.mZoom = zoom();
    setting.mCenter = mCenter;
    emit updateMap(setting);
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

void zchxMapView::update()
{

}
