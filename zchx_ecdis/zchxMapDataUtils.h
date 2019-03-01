#ifndef ZCHXECDISUTILS_H
#define ZCHXECDISUTILS_H

#include <math.h>
#include <QString>
#include <QDateTime>
#include <QPixmap>

#define         GLOB_PI                                 (3.14159265358979323846)
#define         DOUBLE_EPS                              0.000001
#define         EARTH_HALF_CIRCUL_LENGTH                20037508.3427892
#define         MAP_IMG_SIZE                            256

#define         DATETIME_STRING(datetime)       (datetime.toString("yyyy-MM-dd hh:mm:ss"))
#define         FLOAT_STRING(val ,n)   (QString::number(val, 'f', n))
#define         INT_STRING(val)      (QString("").sprintf("%d", val))
#define         INT_COMMA_STRING(val) (QString("%L1").arg(val))
#define         TIMESTAMP16_STRING(val) ( DATETIME_STRING(QDateTime::fromMSecsSinceEpoch(val)))
#define         TIMESTAMP10_STRING(val) ( DATETIME_STRING(QDateTime::fromTime_t(val)))
#define         KPH2KTS(val)            ((val) / 1.852)
#define         MPS2KTS(val)            (KPH2KTS((val) * 3.6))
#define         TIMEOFDAY2UTC(val)      (QDateTime(QDate::currentDate()).toMSecsSinceEpoch() + (val*1000))

namespace qt {
struct Mercator{
public:
    Mercator() {mX = 0.0; mY = 0.0;}
    Mercator(double x, double y){mX = x; mY= y;}
    bool operator ==(const Mercator& other)
    {
        return fabs(this->mX- other.mX) <= DOUBLE_EPS  && \
               fabs(this->mY - other.mY) <= DOUBLE_EPS ;
    }
    double mX;
    double mY;
};

struct LatLon{
public:
    LatLon() {lon = 0.0; lat = 0.0;}
    LatLon(double y, double x){lon = x; lat= y;}
    bool operator ==(const LatLon& other) const
    {
        return fabs(this->lon - other.lon) <= DOUBLE_EPS  && \
               fabs(this->lat - other.lat) <= DOUBLE_EPS ;
    }

    bool isNull() const
    {
        return fabs(lat) < 0.000001 && fabs(lon) < 0.000001;
    }

    double lat;
    double lon;
};

struct Point2D{
    double x;
    double y;

    Point2D(double px, double py) {
        x = px;
        y = py;
    }

    Point2D()
    {
        x = 0;
        y = 0;
    }

    Point2D(const QPoint& p)
    {
        x = p.x();
        y = p.y();
    }

    Point2D(const QPointF& p)
    {
        x = p.x();
        y = p.y();
    }

    QPointF toPointF() const
    {
        return QPointF(x, y);
    }

    QPoint toPoint() const
    {
        return QPoint(qRound(x), qRound(y));
    }


};

//struct strLatLon{
//    double lat;
//    double lon;
//};

struct MapRangeData{
    Mercator    mLowerLeft;         //左下
    Mercator    mTopRight;          //右上
};

enum    TILE_ORIGIN_POS{
    TILE_ORIGIN_TOPLEFT = 0,        //左上
    TILE_ORIGIN_BOTTEMLEFT,         //左下
};

enum    TILE_SOURCE{
    TILE_TMS = 0,
    TILE_GOOGLE = 1,

};

//每次加载瓦片地图的参数设定,主要是视窗的墨卡托范围,视窗屏幕坐标大小
struct MapLoadSetting{
    MapRangeData    mMapRange;
    double          mResolution;
    int             mZoom;
    int             mSource; //0:本地1:服务器地址
    int             mTilePos;
    LatLon          mCenter;
};

//瓦片图片信息
struct TileImage {
    QPixmap    mImg;
    int         mPosX;
    int         mPosY;
    QString     mName;

    TileImage(){}

    TileImage(const QPixmap& img, int x, int y, const QString& name = QString())
    {
        mImg = img;
        mPosX = x;
        mPosY = y;
        mName = name;
    }
};

class TileImageList:public QList<TileImage>
{
public:
    TileImageList():QList<TileImage>() {}
};

static double constexpr degreeInMetres = 360.0 / 40008245;

class zchxMapDataUtils
{
public:
    zchxMapDataUtils();
    //墨卡托和wgs84互转
    static LatLon mercatorToWgs84LonLat(const Mercator& mercator);
    static Mercator wgs84LonlatToMercator(const LatLon& wgs84 );
    static Mercator wgs84LatLonToMercator(double lat, double lon ) {return wgs84LonlatToMercator(LatLon(lat, lon));}
    static double calResolution(int zoom);
    //角度弧度换算
    static double DegToRad(double deg);
    //地球空间计算
    // 地球半径 单位米.
    static inline double EarthRadiusMeters() { return 6378000; }
    // Length of one degree square at the equator in meters.
    static inline double OneDegreeEquatorLengthMeters() { return 111319.49079; }
    // Distance on unit sphere between (lat1, lon1) and (lat2, lon2).
    // lat1, lat2, lon1, lon2 - in degrees.
    static double DistanceOnSphere(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg);
    // Area on unit sphere for a triangle (ll1, ll2, ll3).
    static double AreaOnSphere(LatLon const & ll1, LatLon const & ll2, LatLon const & ll3);
    static double AreaOnSphere(std::vector<LatLon> vectorPoints);

    // Distance in meteres on Earth between (lat1, lon1) and (lat2, lon2).
    // lat1, lat2, lon1, lon2 - in degrees.
    static double DistanceOnEarth(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg)
    {
      return EarthRadiusMeters() * DistanceOnSphere(lat1Deg, lon1Deg, lat2Deg, lon2Deg);
    }

    static double DistanceOnEarth(LatLon const & ll1, LatLon const & ll2)
    {
      return DistanceOnEarth(ll1.lat, ll1.lon, ll2.lat, ll2.lon);
    }

    static double AreaOnEarth(LatLon const & ll1, LatLon const & ll2, LatLon const & ll3)
    {
      return OneDegreeEquatorLengthMeters() * OneDegreeEquatorLengthMeters() * AreaOnSphere(ll1, ll2, ll3);
    }

    static double AreaOnEarth(std::vector<LatLon> vectorPoints)
    {
      return OneDegreeEquatorLengthMeters() * OneDegreeEquatorLengthMeters() * AreaOnSphere(vectorPoints);
    }

    static double getTotalDistance(const std::vector<std::pair<double, double> > &pointList);
    static double getTotalArea(const std::vector<std::pair<double, double> > &pointList);
    LatLon static getSmPoint(const LatLon & pt, double lonMetresR, double latMetresR);
    static double getDistancePixel(const Point2D& p1, const Point2D & p2);


    //计算向量的角度[P1,P2]
    static inline double AngleTo(const Point2D& p1, const Point2D & p2)
    {
        return atan2(p2.y - p1.y, p2.x - p1.x);
    }

    //计算两个向量的夹角[p, p1] 到[p, p2]. 逆时针旋转. [0, 2pi]
    static inline double TwoVectorsAngle(const Point2D& p0, const Point2D& p1, const Point2D & p2)
    {
        double a = AngleTo(p0, p2) - AngleTo(p0, p1);
        while (a < 0) a += GLOB_PI;
        return a;
    }
    //角度校正
    static double AngleIn2PI(double a);

};



typedef struct tagStartEndPoint{
    Point2D start;
    Point2D end;
}StartEndPoint;

typedef struct tagMultiBeamImg {
    double dMinLon;//多波束数据的最小经度
    double dMinLat;//多波束数据的最小纬度
    double dMaxLon;//多波束数据的最大经度
    double dMaxLat;//多波束数据的最大纬度
    QPixmap multibeamImg;//多波束数据图片
}MultiBeamImg;

enum MapStyle
{
    MapStyleBase = 0x0001,
    MapStyleStandard = 0x0002,
    MapStyleAll = 0x0004,
    MapStyleEcdisDayBright = 0x0008,       //晴天模式
    MapStyleEcdisNight = 0x0010,              //夜晚模式
    MapStyleEcdisDayDUSK = 0x0020,           //黄昏模式
    MapStyleEcdisDayBlackBack = 0x0040, //阴天模式
    MapStyleEcdisDayWhiteBack = 0x0080,  //白昼模式
    // Specifies number of MapStyle enum values, must be last
    MapStyleCount
};

enum MapUnit{
    MapUnitMeter = 1,   //米
    MapUnitFoot,    //英尺
    MapUnitKm,  //千米
    MapUnitNmi, //海里
};
}

Q_DECLARE_METATYPE(qt::TileImage)
Q_DECLARE_METATYPE(qt::TileImageList)

//地图配置文件相关的设定
#define             MAP_INDEX                       "Map_0"
#define             MAX_LINE_LENGTH                 "MaxLineLength"
#define             WARN_FLAH_COLOR_ALPHA           "FlashColorAlpha"
#define             OPEN_MEET                       "OpenMeet"
#define             MAP_UNIT                        "MapUnit"
#define             MAP_STYLE_AUTO_CHANGE           "StyleAutoChange"
#define             MAP_DAY_TIME                    "DayTime"
#define             MAP_NIGHT_TIME                    "NightTime"
#define             MAP_DUSK_TIME                    "DuskTime"
#define             MAP_DEFAULT_LAT                      "Lat"
#define             MAP_DEFAULT_LON                      "Lon"
#define             MAP_DEFAULT_ZOOM                    "Zoom"
#define             MAP_DEFAULT_TARGET_ZOOM             "TargetZoom"            //目标居中放大时的倍数
#define             MAP_UPDATE_INTERVAL             "UpdateInterval"            //刷新时间间隔毫秒
#define             MAP_SOURCE                      "MapSource"                 //地图数据来源
#define             MAP_DISPLAY_MENU                "DisplayRightMouseMenu"     //是狗显示右键菜单
#define             MAP_FILE_DIR                    "MapFileDir"

//Ais显示配置
#define             AIS_DISPLAY_SETTING                         "AIS"
#define             AIS_FILL_COLOR                              "FillColor"
#define             AIS_CONCERN_COLOR                           "ConcernColor"
#define             AIS_TEXT_COLOR                              "TextColor"
#define             AIS_BORDER_COLOR                            "BorderColor"
#define             AIS_FORCED_IMAGE                            "ForcedImageDisplay"
#define             AIS_CONCERN_NUM                             "ConcernNum"
#define             AIS_REPLACE_CONCERN                         "AutoReplaceConcern"
#define             AIS_TAIL_TRACK_NUM                          "TailTrackNum"
#define             AIS_REPLACE_TAIL_TRACK                      "AutoReplaceTailTrack"
#define             AIS_HISTORY_TRACK_NUM                       "HistoryTrackNum"
#define             AIS_REPLACE_HISTORY_TRACK                   "AutoReplaceHistoryTrack"
#define             AIS_EXTRAPOLATE_NUM                         "ExtrapolateNum"
#define             AIS_REPLACE_EXTRAPOLATE                       "AutoReplaceExtrapolate"

//雷达显示配置
#define             RADAR_DISPLAY_SETTING                       "Radar"
#define             RADAR_FILL_COLOR                            "FillColor"
#define             RADAR_CONCERN_COLOR                         "ConcernColor"
#define             RADAR_TEXT_COLOR                            "TextColor"
#define             RADAR_BORDER_COLOR                          "BorderColor"
#define             RADAR_SHAPE_RECT                            "ShapeAsRect"
#define             RADAR_FORCED_AIS                            "DrawAsAis"
#define             RADAR_CONCERN_NUM                             "ConcernNum"
#define             RADAR_REPLACE_CONCERN                         "AutoReplaceConcern"
#define             RADAR_TAIL_TRACK_NUM                          "TailTrackNum"
#define             RADAR_REPLACE_TAIL_TRACK                      "AutoReplaceTailTrack"
#define             RADAR_HISTORY_TRACK_NUM                       "HistoryTrackNum"
#define             RADAR_REPLACE_HISTORY_TRACK                   "AutoReplaceHistoryTrack"

#endif // ZCHXECDISUTILS_H
