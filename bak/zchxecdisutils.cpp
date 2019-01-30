#include "zchxecdisutils.h"
#include <QDebug>

zchxEcdisUtils::zchxEcdisUtils()
{

}

Wgs84LonLat zchxEcdisUtils::mercatorToWgs84LonLat(const Mercator& mercator)
{
    double x = mercator.mX/EARTH_HALF_CIRCUL_LENGTH * 180.0;
    double y = mercator.mY/EARTH_HALF_CIRCUL_LENGTH * 180.0;
    y= 180/GLOB_PI*(2*atan(exp(y*GLOB_PI/180.0))-GLOB_PI/2.0);
    return Wgs84LonLat(x, y);
}

Mercator zchxEcdisUtils::wgs84LonlatToMercator(const Wgs84LonLat& wgs84 )
{
    //qDebug()<<"wgs:"<<wgs84.mLon<<wgs84.mLat;
    double x = wgs84.mLon * EARTH_HALF_CIRCUL_LENGTH / 180;
    double y = log(tan((90 + wgs84.mLat) * GLOB_PI / 360)) / (GLOB_PI / 180);
    y = y * EARTH_HALF_CIRCUL_LENGTH / 180;

    return Mercator(x, y);
}

//每像素对应的墨卡托坐标的长度
double zchxEcdisUtils::calResolution(int zoom)
{
    return EARTH_HALF_CIRCUL_LENGTH * 2 / MAP_IMG_SIZE / pow(2, zoom);
}
