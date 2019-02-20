#include "zchxmapframe.h"
#include <QDebug>

namespace qt {
zchxMapFrameWork::zchxMapFrameWork(double center_lat, double center_lon, int zoom, int width, int height, int source, QObject *parent) : QObject(parent),
  mViewWidth(0),
  mViewHeight(0),
  mSource(source),
  mStyle(MapStyleEcdisDayBright),
  mMinZoom(0),
  mMaxZoom(22)
{
    SetZoom(zoom);
    UpdateCenter(center_lon, center_lat);
    SetViewSize(width, height);
}

void zchxMapFrameWork::SetViewSize(int width, int height)
{
    mViewHeight = height;
    mViewWidth = width;
    UpdateDisplayRange();
}

void zchxMapFrameWork::SetSource(int source)
{
    if(mSource != source)
    {
        mSource = source;
        UpdateDisplayRange();
    }
}

void zchxMapFrameWork::SetZoom(int zoom)
{
    if(zoom < mMinZoom || zoom > mMaxZoom) return;
    mCurZoom = zoom;
    mUnitMercatorLength = zchxMapDataUtils::calResolution(zoom);
    UpdateDisplayRange();
}

void zchxMapFrameWork::SetMinZoom(int zoom)
{
    mMinZoom = zoom;
}

void zchxMapFrameWork::SetMaxZoom(int zoom)
{
    mMaxZoom = zoom;
}

int zchxMapFrameWork::Zoom() const
{
    return mCurZoom;
}

void zchxMapFrameWork::UpdateCenter(double lon, double lat)
{
    mCenter.lat = lat;
    mCenter.lon = lon;
    //重新计算当前视窗的显示范围
    UpdateDisplayRange();
}

void zchxMapFrameWork::UpdateCenterAndZoom(const LatLon &ll, int zoom)
{
    mCenter = ll;
    SetZoom(zoom);
}

void zchxMapFrameWork::Drag(int x, int y)
{
    UpdateCenter(Point2D(0.5*mViewWidth + x, 0.5*mViewHeight + y));
}

void zchxMapFrameWork::UpdateCenter(const Point2D &point)
{
    UpdateCenter(Pixel2LatLon(point));
}

Point2D zchxMapFrameWork::Mercator2Pixel(const Mercator &mct)
{
    double x = mct.mX - mMapRange.mLowerLeft.mX;
    double y = mct.mY - mMapRange.mTopRight.mY;
    Point2D res;
    res.x = x / mUnitMercatorLength;
    res.y = 0 - y / mUnitMercatorLength;
    return res;
}

Point2D zchxMapFrameWork::LatLon2Pixel(const LatLon &ll)
{
    //获取当前经纬度对应的墨卡托坐标
    Mercator mct = zchxMapDataUtils::wgs84LonlatToMercator(ll);
    //通过墨卡托坐标换算屏幕坐标
    return Mercator2Pixel(mct);
}

Point2D zchxMapFrameWork::LatLon2Pixel(double lat, double lon)
{
    return LatLon2Pixel(LatLon(lat, lon));
}



LatLon zchxMapFrameWork::Pixel2LatLon(const Point2D& pos)
{
    double x = pos.x;
    double y = mViewHeight - pos.y;
    //获取当前指定位置对应的墨卡托坐标
    Mercator target;
    target.mX = mMapRange.mLowerLeft.mX + mUnitMercatorLength * x;
    target.mY = mMapRange.mLowerLeft.mY + mUnitMercatorLength * y;

    return zchxMapDataUtils::mercatorToWgs84LonLat(target);
}

void zchxMapFrameWork::UpdateDisplayRange()
{
    if(mViewWidth == 0 || mViewHeight == 0) return;
    //计算当前中心经纬度对应的墨卡托坐标
    Mercator center_mct = zchxMapDataUtils::wgs84LonlatToMercator(mCenter);
    //计算当前视窗对应的墨卡托坐标的显示范围
    mMapRange.mLowerLeft.mX = center_mct.mX - mUnitMercatorLength * mViewWidth / 2.0;
    mMapRange.mLowerLeft.mY = center_mct.mY - mUnitMercatorLength * mViewHeight / 2.0;
    mMapRange.mTopRight.mX = center_mct.mX + mUnitMercatorLength * mViewWidth / 2.0;
    mMapRange.mTopRight.mY = center_mct.mY + mUnitMercatorLength * mViewHeight / 2.0;

    MapLoadSetting setting;
    setting.mMapRange = mMapRange;
    setting.mSource = mSource;
    if(setting.mSource == TILE_GOOGLE)
    {
        setting.mTilePos = TILE_ORIGIN_TOPLEFT;
    } else
    {
        setting.mTilePos = TILE_ORIGIN_BOTTEMLEFT;
    }
    setting.mResolution = mUnitMercatorLength;
    setting.mZoom = Zoom();
    setting.mCenter = mCenter;
    emit UpdateMap(setting);
}

void zchxMapFrameWork::ZoomIn()
{
    int zoom = mCurZoom;
    SetZoom(++zoom);
}

void zchxMapFrameWork::ZoomOut()
{
    int zoom = mCurZoom;
    SetZoom(--zoom);
}

void zchxMapFrameWork::Update()
{

}
}
