#ifndef ZCHXECDISUTILS_H
#define ZCHXECDISUTILS_H

#include <math.h>

#define     GLOB_PI                                 (3.14159265358979323846)
#define     DOUBLE_EPS                              0.000001
#define     EARTH_HALF_CIRCUL_LENGTH                20037508.3427892
#define     MAP_IMG_SIZE                            256

struct Mercator{
public:
    Mercator(double x, double y){mX = x; mY= y;}
    bool operator ==(const Mercator& other)
    {
        return fabs(this->mX- other.mX) <= DOUBLE_EPS  && \
               fabs(this->mY - other.mY) <= DOUBLE_EPS ;
    }
    double mX;
    double mY;
};

struct Wgs84LonLat{
public:
    Wgs84LonLat() {mLon = 0.0; mLat = 0.0;}
    Wgs84LonLat(double x, double y){mLon = x; mLat= y;}
    bool operator ==(const Wgs84LonLat& other) const
    {
        return fabs(this->mLon - other.mLon) <= DOUBLE_EPS  && \
               fabs(this->mLat - other.mLat) <= DOUBLE_EPS ;
    }

    double mLon;
    double mLat;
};


class zchxEcdisUtils
{
public:
    zchxEcdisUtils();
    //墨卡托和wgs84互转
    static Wgs84LonLat mercatorToWgs84LonLat(const Mercator& mercator);
    static Mercator wgs84LonlatToMercator(const Wgs84LonLat& wgs84 );
};

#endif // ZCHXECDISUTILS_H
