#include "zchxmapframe.h"
#include <QDebug>
#include "zchxmaploadthread.h"
#include <QPainter>

using namespace qt;

zchxMapFrameWork::zchxMapFrameWork(double center_lat, double center_lon, int zoom, int width, int height, int source,  int min_zoom, int max_zoom, QObject *parent) : QObject(parent),
  mViewWidth(0),
  mViewHeight(0),
  mSource(source),
  mStyle(MapStyleEcdisDayBright),
  mMinZoom(min_zoom),
  mMaxZoom(max_zoom),
  mMapThread(0),
  mOffset(0, 0)
{
    SetZoom(zoom);
    UpdateCenter(center_lon, center_lat);
    SetViewSize(width, height);

    mMapThread = new zchxMapLoadThread;
    connect(mMapThread, SIGNAL(signalSendCurPixmap(QPixmap,int,int)), this, SLOT(append(QPixmap,int,int)));
    connect(mMapThread, SIGNAL(signalSendNewMap(double, double, int, bool)), this, SLOT(slotRecvNewMap(double,double,int, bool)));
    connect(mMapThread, SIGNAL(signalSendImgList(TileImageList)), this, SLOT(append(TileImageList)));
    mMapThread->start();
}

void zchxMapFrameWork::setOffSet(int offset_x, int offset_y)
{
    mOffset.setWidth(offset_x);
    mOffset.setHeight(offset_y);
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

void zchxMapFrameWork::UpdateCenterAndZoom(const ZCHX::Data::LatLon &ll, int zoom)
{
    mCenter = ll;
    SetZoom(zoom);
}

void zchxMapFrameWork::Drag(int x, int y)
{
    UpdateCenter(ZCHX::Data::Point2D(0.5*mViewWidth + x, 0.5*mViewHeight + y));
}

void zchxMapFrameWork::UpdateCenter(const ZCHX::Data::Point2D &point)
{
    UpdateCenter(Pixel2LatLon(point));
}

ZCHX::Data::Point2D zchxMapFrameWork::Mercator2Pixel(const ZCHX::Data::Mercator &mct)
{
    double x = mct.mX - mMapRange.mLowerLeft.mX;
    double y = mct.mY - mMapRange.mTopRight.mY;
    ZCHX::Data::Point2D res;
    res.x = x / mUnitMercatorLength + mOffset.width();
    res.y = 0 - y / mUnitMercatorLength + mOffset.height();
    return res;
}

ZCHX::Data::Point2D zchxMapFrameWork::LatLon2Pixel(const ZCHX::Data::LatLon &ll)
{
    //获取当前经纬度对应的墨卡托坐标
    ZCHX::Data::Mercator mct = zchxMapDataUtils::wgs84LonlatToMercator(ll);
    //通过墨卡托坐标换算屏幕坐标
    return Mercator2Pixel(mct);
}

ZCHX::Data::Point2D zchxMapFrameWork::LatLon2Pixel(double lat, double lon)
{
    return LatLon2Pixel(ZCHX::Data::LatLon(lat, lon));
}



ZCHX::Data::LatLon zchxMapFrameWork::Pixel2LatLon(const ZCHX::Data::Point2D& pos)
{
    double x = pos.x;
    double y = mViewHeight - pos.y;
    //获取当前指定位置对应的墨卡托坐标
    ZCHX::Data::Mercator target;
    target.mX = mMapRange.mLowerLeft.mX + mUnitMercatorLength * x;
    target.mY = mMapRange.mLowerLeft.mY + mUnitMercatorLength * y;

    return zchxMapDataUtils::mercatorToWgs84LonLat(target);
}

void zchxMapFrameWork::UpdateDisplayRange()
{
    if(mViewWidth == 0 || mViewHeight == 0) return;
    //计算当前中心经纬度对应的墨卡托坐标
    ZCHX::Data::Mercator center_mct = zchxMapDataUtils::wgs84LonlatToMercator(mCenter);
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
    if(mMapThread) mMapThread->appendTask(setting);
}

void zchxMapFrameWork::ZoomIn(double lon, double lat)
{
    int zoom = mCurZoom;
    if(fabs(lon) > 0.00001)
    {
        mCenter.lon = lon;
    }
    if(fabs(lat) > 0.00001)
    {
        mCenter.lat = lat;
    }
    SetZoom(++zoom);
}

void zchxMapFrameWork::ZoomOut(double lon, double lat)
{
    int zoom = mCurZoom;
    if(fabs(lon) > 0.00001)
    {
        mCenter.lon = lon;
    }
    if(fabs(lat) > 0.00001)
    {
        mCenter.lat = lat;
    }
    SetZoom(--zoom);
}

void zchxMapFrameWork::Update()
{

}

PPATH zchxMapFrameWork::convert2QtPonitList(const GPATH& path)
{
    PPATH pts;
    for(int i = 0; i < path.size(); ++i)
    {
        GPNT ll = path[i];
        pts.push_back(LatLon2Pixel(ll.first,ll.second).toPointF());
    }
    return pts;
}

void zchxMapFrameWork::updateEcdis(QPainter *painter, QRect rect, bool img_num)
{
    if(!painter) return;
    foreach(TileImage data, mDataList)
    {
        int x = data.mPosX + mOffset.width();
        int y = data.mPosY + mOffset.height();
        painter->drawPixmap(x, y, data.mImg);
        if(img_num)painter->drawText(x, y, data.mName);
    }
    switch (mStyle) {
    case MapStyleEcdisDayBright:
        painter->fillRect(rect, Qt::transparent);
        break;
    case MapStyleEcdisNight:
        painter->fillRect(rect, QColor(100,100,100,100));
        break;
    case MapStyleEcdisDayDUSK:
        painter->fillRect(rect, QColor(0,100,100,100));
        break;
    default:
        break;
    }
}

void zchxMapFrameWork::slotRecvNewMap(double lon, double lat, int zoom, bool sync)
{
    emit signalSendCurMapinfo(lat, lon, zoom);
    if(!sync) clear();
}

void zchxMapFrameWork::append(const QPixmap &img, int x, int y)
{
    mDataList.append(TileImage(img, x, y));
}

void zchxMapFrameWork::append(const TileImageList &list)
{
    mDataList = list;
    setOffSet(0, 0);
}
