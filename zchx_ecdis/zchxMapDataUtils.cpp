#include "zchxMapDataUtils.h"
#include <math.h>
#include <QDebug>

namespace qt {
zchxMapDataUtils::zchxMapDataUtils()
{

}

LatLon zchxMapDataUtils::mercatorToWgs84LonLat(const Mercator& mercator)
{
    double x = mercator.mX/EARTH_HALF_CIRCUL_LENGTH * 180.0;
    double y = mercator.mY/EARTH_HALF_CIRCUL_LENGTH * 180.0;
    y= 180/GLOB_PI*(2*atan(exp(y*GLOB_PI/180.0))-GLOB_PI/2.0);
    return LatLon(y, x);
}

Mercator zchxMapDataUtils::wgs84LonlatToMercator(const LatLon& wgs84 )
{
    //qDebug()<<"wgs:"<<wgs84.mLon<<wgs84.mLat;
    double x = wgs84.lon * EARTH_HALF_CIRCUL_LENGTH / 180;
    double y = log(tan((90 + wgs84.lat) * GLOB_PI / 360)) / (GLOB_PI / 180);
    y = y * EARTH_HALF_CIRCUL_LENGTH / 180;

    return Mercator(x, y);
}

//每像素对应的墨卡托坐标的长度
double zchxMapDataUtils::calResolution(int zoom)
{
    return EARTH_HALF_CIRCUL_LENGTH * 2 / MAP_IMG_SIZE / pow(2, zoom);
}

double zchxMapDataUtils::DegToRad(double deg)
{
    return deg * GLOB_PI / 180.0;
}


double zchxMapDataUtils::DistanceOnSphere(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg)
{
  double const lat1 = DegToRad(lat1Deg);
  double const lat2 = DegToRad(lat2Deg);
  double const dlat = sin((lat2 - lat1) * 0.5);
  double const dlon = sin((DegToRad(lon2Deg) - DegToRad(lon1Deg)) * 0.5);
  double const y = dlat * dlat + dlon * dlon * cos(lat1) * cos(lat2);
  return 2.0 * atan2(sqrt(y), sqrt(fmax(0.0, 1.0 - y)));
}

double zchxMapDataUtils::AreaOnSphere(LatLon const & ll1, LatLon const & ll2, LatLon const & ll3)
{
  // Todo: proper area on sphere (not needed for now)
  double const avgLat = DegToRad((ll1.lat + ll2.lat + ll3.lat) / 3);
  return cos(avgLat) * 0.5 * fabs((ll2.lon - ll1.lon)*(ll3.lat - ll1.lat) - (ll3.lon - ll1.lon)*(ll2.lat - ll1.lat));
}

double zchxMapDataUtils::AreaOnSphere(std::vector<LatLon> vectorPoints)
{
    int iCycle = 0;
    double iArea = 0;
    double avgLat = 0;
    int iCount = vectorPoints.size();

    for (iCycle = 0; iCycle < iCount; iCycle++)
    {
        LatLon one = vectorPoints[iCycle];
        LatLon second = vectorPoints[(iCycle + 1) % iCount];
        iArea += (one.lon * second.lat - second.lon * one.lat);
        avgLat += one.lat;
    }
    avgLat = DegToRad(avgLat / double(iCount));

    return (double)avgLat * fabs(0.5 * iArea);
}


double zchxMapDataUtils::getTotalDistance(const std::vector<std::pair<double, double> > &pointList)
{
    double totalDistance = 0;
    for (int i = 0; i < pointList.size(); i++)
    {
        if (i > 0)
        {
            LatLon first(pointList[i - 1].first, pointList[i - 1].second);
            LatLon second(pointList[i].first, pointList[i].second);

            double distance = zchxMapDataUtils::DistanceOnEarth(first, second) / 1852.000;
            totalDistance += distance;
        }
    }

    return totalDistance;
}

double zchxMapDataUtils::getTotalArea(const std::vector<std::pair<double, double> > &pointList)
{
    double areaNum = 0;
    if(pointList.size() >= 3)
    {
        LatLon start(pointList[0].first, pointList[0].second);
        for(int i = 1; i < pointList.size(); i++)
        {
            LatLon cur(pointList[i].first, pointList[i].second);
            if(i + 1 < pointList.size())
            {
                LatLon next(pointList[i + 1].first, pointList[i + 1].second);
                areaNum += zchxMapDataUtils::AreaOnEarth(start, cur, next) / 342990.400;//单位平方海里
            }
        }
    }

    return areaNum;
}

LatLon zchxMapDataUtils::getSmPoint(const LatLon &pt, double lonMetresR, double latMetresR)
{
    double const lat = pt.lat;
    double const lon = pt.lon;

    double const latDegreeOffset = latMetresR * degreeInMetres;
    double const newLat = fmin(90.0, fmax(-90.0, lat + latDegreeOffset));

    double const cosL = fmax(cos(DegToRad(newLat)), 0.00001);
    double const lonDegreeOffset = lonMetresR * degreeInMetres / cosL;
    double const newLon = fmin(180.0, fmax(-180.0, lon + lonDegreeOffset));

    return LatLon(newLat, newLon);
}
}

