#include "zchxecdisutils.h"

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
    double x = wgs84.mLon * EARTH_HALF_CIRCUL_LENGTH / 180;
    double y = log(tan((90 + wgs84.mLat) * GLOB_PI / 360)) / (GLOB_PI / 180);
    y = y * 20037508.34 / 180;

    return Mercator(x, y);
}
