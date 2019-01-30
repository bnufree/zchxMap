#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include "zchxmapframe.h"

namespace ZCHX {
Utils *Utils::d = NULL;
QString Utils::gCurrProject;

const double radius =6378000.00;
/*-------------------------------------------
    *
    * 已知一点的坐标和目地点距离和方位角，求目标点地理坐标
    *
    ---------------------------------------------*/
Utils::Utils()
{

}

Utils *Utils::instance()
{
    if(!d)
    {
        d = new Utils;
    }
    return d;
}

void Utils::test()
{

}

void Utils::distbear_to_latlon(double lat1,double lon1,double dist,
                        double brng,double &lat_out,double &lon_out )
{
    brng=toRad(brng);
    lat1=toRad(lat1);
    lon1=toRad(lon1);
    double R=radius; //Earth radius 6371000
    double lat2 = asin( sin(lat1)*cos(dist/R) + cos(lat1)*sin(dist/R)*cos(brng) );
    double lon2 = lon1 + atan2(sin(brng)*sin(dist/R)*cos(lat1),cos(dist/R)-sin(lat1)*sin(lat2));
    lat_out=toDeg(lat2);
    lon_out=toDeg(lon2);
}

/*-------------------------------------------
    *
    * 已知两点坐标，求两点连线的方位角
    *
    ---------------------------------------------*/
double Utils::calcAzimuth(double lon1, double lat1, double lon2, double lat2)
{
    double arvLat,antPos;
    double pi= ZCHX::pi;
    arvLat=(lat1+lat2)/2;
    if((lat1-lat2)==0){
        antPos=90;
    }else{
        antPos=atan((lon1-lon2)*cos(ang2Cir(arvLat))/(lat1-lat2))*180/pi;
    }

    if(lat1>lat2){
        antPos+=180;
    }

    if(antPos<0){
        antPos+=360;
    }
    antPos*=10;
    antPos=int(antPos+0.5);
    antPos/=10;

    return antPos;
}

/*-------------------------------------------
    *
    * 求两点连线之间的距离
    *
    ---------------------------------------------*/
double Utils::getDis(double startX, double startY, double endX, double endY)
{
    double* course1 = NULL;
    double* course2 = NULL;

    QPointF p1(startX,startY);
    QPointF p2(endX,endY);
    if ( p1.x() == p2.x() && p1.y() == p2.y() )
        return 0;

    double a = 6378137.0;
    double b = 6356752.314245;
    double f = 1.0 / 298.257223563;

    double p1_lat = toRad( p1.y() ), p1_lon = toRad( p1.x() );
    double p2_lat = toRad( p2.y() ), p2_lon = toRad( p2.x() );

    double L = p2_lon - p1_lon;
    double U1 = atan(( 1 - f ) * tan( p1_lat ) );
    double U2 = atan(( 1 - f ) * tan( p2_lat ) );
    double sinU1 = sin( U1 ), cosU1 = cos( U1 );
    double sinU2 = sin( U2 ), cosU2 = cos( U2 );
    double lambda = L;
    double lambdaP = 2 * ZCHX::pi;

    double sinLambda = 0;
    double cosLambda = 0;
    double sinSigma = 0;
    double cosSigma = 0;
    double sigma = 0;
    double alpha = 0;
    double cosSqAlpha = 0;
    double cos2SigmaM = 0;
    double C = 0;
    double tu1 = 0;
    double tu2 = 0;

    int iterLimit = 20;
    while ( qAbs( lambda - lambdaP ) > 1e-12 && --iterLimit > 0 )
    {
        sinLambda = sin( lambda );
        cosLambda = cos( lambda );
        tu1 = ( cosU2 * sinLambda );
        tu2 = ( cosU1 * sinU2 - sinU1 * cosU2 * cosLambda );
        sinSigma = sqrt( tu1 * tu1 + tu2 * tu2 );
        cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
        sigma = atan2( sinSigma, cosSigma );
        alpha = asin( cosU1 * cosU2 * sinLambda / sinSigma );
        cosSqAlpha = cos( alpha ) * cos( alpha );
        cos2SigmaM = cosSigma - 2 * sinU1 * sinU2 / cosSqAlpha;
        C = f / 16 * cosSqAlpha * ( 4 + f * ( 4 - 3 * cosSqAlpha ) );
        lambdaP = lambda;
        lambda = L + ( 1 - C ) * f * sin( alpha ) *
                ( sigma + C * sinSigma * ( cos2SigmaM + C * cosSigma * ( -1 + 2 * cos2SigmaM * cos2SigmaM ) ) );
    }

    if ( iterLimit == 0 )
        return -1;  // formula failed to converge

    double uSq = cosSqAlpha * ( a * a - b * b ) / ( b * b );
    double A = 1 + uSq / 16384 * ( 4096 + uSq * ( -768 + uSq * ( 320 - 175 * uSq ) ) );
    double B = uSq / 1024 * ( 256 + uSq * ( -128 + uSq * ( 74 - 47 * uSq ) ) );
    double deltaSigma = B * sinSigma * ( cos2SigmaM + B / 4 * ( cosSigma * ( -1 + 2 * cos2SigmaM * cos2SigmaM ) -
                                                                B / 6 * cos2SigmaM * ( -3 + 4 * sinSigma * sinSigma ) * ( -3 + 4 * cos2SigmaM * cos2SigmaM ) ) );
    double s = b * A * ( sigma - deltaSigma );

    if ( course1 )
    {
        *course1 = atan2( tu1, tu2 );
    }
    if ( course2 )
    {
        // PI is added to return azimuth from P2 to P1
        *course2 = atan2( cosU1 * sinLambda, -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda ) + ZCHX::pi;
    }

    return s;
}

/*-------------------------------------------
    *
    * 求一系列点的中心点
    *
    ---------------------------------------------*/
void Utils::getCenterPosValueByPosList(const QList<QPointF> &posList,QPointF &centerPos)
{
    double lon = 0.0;
    double lat = 0.0;
    int posCnt = posList.count();
    foreach(const QPointF& pos, posList)
    {
        lon += pos.x();
        lat += pos.y();
    }

    centerPos.setX((lon/posCnt)*1.0);
    centerPos.setY((lat/posCnt)*1.0);
}

/*-------------------------------------------
    *
    * 角度转弧度
    *
    ---------------------------------------------*/
double Utils::toRad(double deg)
{
    double pi = ZCHX::pi;

    return deg*pi/180.0;
}

/*-------------------------------------------
    *
    * 弧度转角度
    *
    ---------------------------------------------*/
double Utils::toDeg(double rad)
{
    double pi = ZCHX::pi;

    return rad*180/pi;
}

double Utils::ang2Cir(double angle)
{
    double pi=ZCHX::pi;
    return (pi/180)*angle;
}

QString Utils::latLonToString(double coordinate, bool latflag)
{
    QString mark = "";
    int wkval = (int)(floor(coordinate));
    if(wkval < 0)
    {
        mark = latflag ? "S " : "W ";
    } else
    {
        mark = latflag ? "N " : "E ";
    }
    int deg = (int)coordinate;
    int min = (int)((coordinate-deg)*60);
    double sed = fabs(((coordinate - deg)*60 - min ) * 60);

    return QString("").sprintf("%s%d°%d'%.2f\"", mark.toStdString().data(), deg, min, sed);

}

QPixmap Utils::getAisImage(int imageID)
{
    return QPixmap(getAisImagePath(imageID) );
}

QString Utils::getAisImagePath(int imageID)
{
    return QString(":/custom/") % QString::number(imageID) % ".png";
}

QString Utils::getImagePath(const QString &path, const QString &project)
{
    QString strReplace = project;
    if(strReplace.isEmpty() )
    {
        strReplace = gCurrProject;
        if(strReplace.isEmpty())
            return path;
    }
    QString projImagePath = path;
    projImagePath.replace("element", strReplace);
//    qDebug()<<path<<strReplace<<projImagePath;
    return projImagePath;
}

QPixmap Utils::getImage(const QString &path, const QString &project)
{
    QPixmap pixmap(getImagePath(path, project));
    return pixmap;
}

QString Utils::getImagePath(Data::CAMERATYPE type, uint status)
{
    QString path;
    switch (type)
    {
    case Data::CAMERA_BALL:
        path = ":/element/camera3";
        break;
    case Data::CAMERA_GUN:
        path = ":/element/camera1";
        break;
    case Data::CAMERA_GAN_LEFT:
        path = ":/element/camera2";
        break;
    case Data::CAMERA_GAN_RIGHT:
        path = ":/element/camera1";
        break;
    case Data::CAMERA_GDY: //光电仪暂时不属于杆上
        path = ":/element/gdy";
        break;
    case Data::CAMERA_FAKE_IPCAST:
        path = ":/element/ipcast1";
        break;
    case Data::CAMERA_NOPEOLEPLNA:
        path = ":/element/plan";
        break;
    default:
        return path;
    }

    switch(status)
    {
    case 0:
    case 9:
        path.append("_normal.png");
        break;
    case 6:
    case 7:
    case 100:
        path.append("_bug.png");
        break;
    case 512:
        path.append("_error.png");
        break;
    default:
        path.append("_error.png");
        break;
    }

    return getImagePath(path);
}

QPixmap Utils::getImage(Data::CAMERATYPE type, uint status, int currScale)
{
    QString path = getImagePath(type, status);
    if(path.isEmpty())
        return QPixmap();
    path = getImagePath(path);

    QPixmap image(5,5);
    if(currScale < 10)
    {
        switch(status)
        {
        case 0:
        case 9:
            image.fill(Qt::green);
            break;
        case 6:
        case 7:
        case 100:
            image.fill(Qt::yellow);
            break;
        case 512:
        default:
            image.fill(Qt::red);
            break;
        }
    }
    else
    {
        image.load(path);
    }

    return image;
}

QPixmap Utils::getImage(const QString &path, QColor color, int currScale)
{
    QString projPath = getImagePath(path);

    QPixmap image(5,5);
    if(currScale < 10)
    {
        image.fill(color);
    }
    else
    {
        image.load(projPath);
    }

    return image;
}

QString Utils::getWarnName(int warnStatus)
{
    QString warnName = "";
    switch (warnStatus) {
    case ZCHX::Data::WARN_FOCUS_AREA:
        return "入侵预警";
    case ZCHX::Data::WARN_OVERSPEED:
        return "超速预警";
    case ZCHX::Data::WARN_DANGER_CIRCLE:
        return "危险圈预警";
    case ZCHX::Data::WARN_CHANNEL_YAW:
        return "偏航预警";
    case ZCHX::Data::WARN_CHANNEL_COLLISION:
        return "碰撞预警";
    case ZCHX::Data::WARN_SPEED_BORDER:
        return "越线预警";
    case ZCHX::Data::WARN_CHANNEL_MOORING:
        return "锚泊预警";
    case ZCHX::Data::WARN_OTHER_WARNING:
        return "其他预警";
    case ZCHX::Data::WARN_BLACKLIST:
        return "黑名单预警";
    case ZCHX::Data::WARN_AISUNOPEN:
        return "AIS未开启";
    case ZCHX::Data::WARN_OVERLOAD:
        return "超载航行";
    case ZCHX::Data::WARN_RETROGRADE:
        return "逆行报警";
    case ZCHX::Data::WARN_OVERLENGTH:
        return "超长报警";
    case ZCHX::Data::WARN_OVERTAKING:
        return "追越报警";
    case ZCHX::Data::WARN_ERROR_DROP_ANCHOR:
        return "违规抛锚预警";
    default:
        break;
    }

    return warnName;
}

QString Utils::getAlarmImg(int type)
{
    QString alarmImg = "";
    switch(type)
    {
    case 1:
        alarmImg = ":/eventLine/入侵预警.png";
        break;
    case 2:
        alarmImg = ":/eventLine/超速预警.png";
        break;
    case 3:
        alarmImg = ":/eventLine/危险圈预警.png";
        break;
    case 4:
        alarmImg = ":/eventLine/偏航预警.png";
        break;
    case 5:
        alarmImg = ":/eventLine/碰撞预警.png";
        break;
    case 6:
        alarmImg = ":/eventLine/越线预警.png";
        break;
    case 7:
        alarmImg = ":/eventLine/锚泊预警.png";
        break;
    case 8:
        alarmImg = ":/eventLine/其他预警.png";
        break;
    case 9:
        alarmImg = ":/eventLine/黑名单预警.png";
        break;
    case 10:
        alarmImg = ":/eventLine/AIS未启动.png";
        break;
    case 11:
        alarmImg = ":/eventLine/超载预警.png";
        break;
    case 12:
        alarmImg = ":/eventLine/逆行预警.png";
        break;
    case 13:
        alarmImg = ":/eventLine/超长预警.png";
        break;
    case 14:
        alarmImg = ":/eventLine/追越预警.png";
        break;
    case 15:
        alarmImg = ":/eventLine/违规抛锚.png";
        break;
    default:
        break;
    }
    return alarmImg;
}

int    Utils::rounds(double x)
{
    return (x > 0.0 ? int(x + 0.5) : int(x - 0.5));
}

double Utils::getDistanceByPixel(zchxMapFrameWork *f, double lat1, double lon1, double dist, double brng, QPointF pos)
{
    if(!f)
        return 0.0;
    //通过推算经纬度计算半径
    LatLon drll(0, 0);
    ZCHX::Utils::distbear_to_latlon(lat1, lon1, dist, brng, drll.lat, drll.lon);
    Point2D drPos = f->LatLon2Pixel(drll);
    QPointF pos2(drPos.x, drPos.y);
    QLineF line(pos, pos2);

    return line.length();
}

namespace Data{

CableAssembly& CableAssembly::operator=(const CableAssembly &other)
{
    if(this == &other)
        return *this;
    m_uID = other.m_uID;
    m_sAssemblyName = other.m_sAssemblyName;
    m_uCompenCable = other.m_uCompenCable;
    m_uRplIndex = other.m_uRplIndex;
    m_uRouteID = other.m_uRouteID;
    m_uAssemblyType = other.m_uAssemblyType;
    m_uAssemblyID = other.m_uAssemblyID;
    m_sCableTypeName = other.m_sCableTypeName;
    m_dStartLon = other.m_dStartLon;
    m_dStartLat = other.m_dStartLat;
    m_dEndLon = other.m_dEndLon;
    m_dEndLat = other.m_dEndLat;
    m_fAssemblyLength = other.m_fAssemblyLength;
    m_sAssemblyRemark =other.m_sAssemblyRemark;
    m_sCableColor = other.m_sCableColor;
    m_uCableType = other.m_uCableType;
    m_uCableWidth = other.m_uCableWidth;
    m_uCableCategory = other.m_uCableCategory;
    m_uDiagramSymbol = other.m_uDiagramSymbol;
    m_iProjectStage = other.m_iProjectStage;
    return *this;
}

CableAssembly::CableAssembly()
{
    m_uID = 0;
    m_sAssemblyName = "";
    m_uCompenCable = 0;
    m_uRplIndex = -1;
    m_uRouteID = 0;
    m_uAssemblyType = 0;
    m_uAssemblyID = 0;
    m_sCableTypeName = "";
    m_dStartLon = 0;
    m_dStartLat = 0;
    m_dEndLon = 0;
    m_dEndLat = 0;
    m_fAssemblyLength = 0;
    m_sAssemblyRemark ="";
    m_sCableColor = Qt::black;
    m_uCableWidth = 0;
    m_uCableType = 0;
    m_uCableCategory = 0;
    m_uDiagramSymbol = 0;
    m_iProjectStage = 0;
}


CableBaseData& CableBaseData::operator=(const CableBaseData &other)
{
    if(this != &other)
    {
        mName = other.mName;
        mColor = other.mColor;
        mStyle = other.mStyle;
        mWidth = other.mWidth;
    }
    return *this;
}

CableBaseData::CableBaseData()
{
    mName = "UnDef";
    mColor = QColor(42,84,139);
    mStyle = Qt::SolidLine;
    mWidth = 2;
}

CableInterfaceData& CableInterfaceData::operator=(const CableInterfaceData &other)
{
    if(this != &other)
    {
        mName = other.mName;
        mDiagramSymbol = other.mDiagramSymbol;
    }
    return *this;
}

CableInterfaceData::CableInterfaceData()
{
    mName = "UnDef";
    mDiagramSymbol = 0;
}


RoutePoint::RoutePoint()
{
    lon = 0;
    lat = 0;
    pointType = 0;//0-route;1-kp
    m_idbID = 0;                //编号
    m_sEvent = "";               // 事件
    m_fdirection = 0;           //走向
    m_fRoutePointDistance = 0;  //路由点间距离
    m_fRouteTotalDistance = 0;  //路由总距离
    m_fRemain = 0;              //余量
    m_fCablePointDistance = 0;   //海缆点间距离
    m_fCableTotalDistance = 0;   //海缆总距离
    m_fMoudleValue = 0;         //为模贵重
    //m_fCableLength = 0;         //海缆长度
    m_fWaterDepth = 0;          //水深
    m_fTemperature = 0;         //温度
    m_fRouteEmbBuryDepth = 0;     //海缆水深
    m_sRoutePath = "";             //路径
    m_sRouteIndex = "";            //索引
    m_sRouteTag = "";              //标签
    m_sRouteComment = "";          //注释
    m_fRouteDepth = 0;            //路由点深度
    m_fRouteKpValue = 0;          //Kp值
    m_fRouteBottomDis = 0;        //底部距离
    m_bRouteUseful = true;           //有效性
    m_uRoutePlough = 0;           //犁设备
    m_sRouteEmbTraverse = "";      //海缆横贯
    m_fRouteElseCost = 0;         //其他费用
    m_routeDelayTime = 0;         //延迟时间
    m_uRouteRouId = 0;            //路由路径编号
    m_uRoutePointType = 0;        //是否是连接点（0-否,1-是）备注：使用的以前的字段
    m_fRouteBottomMargin = 0;     //底部余量
    m_fRouteFaceMargin = 0;       //表面余量
    m_dRoundNumber = -1;           //序号
    m_sAutoTag = "";               //自动标签
    m_dSteer = 0;                 //转向角
    m_sMainType = "";              //主体类型 (接头)
    m_sRoudEmbType = "";           //海缆类型
    //m_uRoudEmbTyID = 0;
    m_sCableLenght = 0;            //海缆类型长度
    m_fRoudCableCompen = 0;        //海缆补偿
    m_uRoudSlackChange = 0;        //是否改变余量(0：不改变，1：改变)
    m_uRoudSlackType = 1;          //余量类型(1：表面余量，2：底部余量)
    m_uRoudRouteReference = 0;     //路由点是否兼具参考点类型(0:否，1：是)
    m_iRoudRouteReJoint = 0;
    mAcPoint = false;
}
RoutePoint & RoutePoint::operator = (const RoutePoint &other)
{
    if(this == &other)
        return *this;
    lon = other.lon;
    lat = other.lat;
    pointType = other.pointType;//0-route;1-kp
    m_idbID = other.m_idbID;                //编号
    m_sEvent = other.m_sEvent;               // 事件
    m_fdirection = other.m_fdirection;           //走向
    m_fRoutePointDistance = other.m_fRoutePointDistance;  //路由点间距离
    m_fRouteTotalDistance = other.m_fRouteTotalDistance;  //路由总距离
    m_fRemain = other.m_fRemain;              //余量
    m_fCablePointDistance = other.m_fCablePointDistance;   //海缆点间距离
    m_fCableTotalDistance = other.m_fCableTotalDistance;   //海缆总距离
    m_fMoudleValue = other.m_fMoudleValue;         //为模贵重
    //m_fCableLength = other.m_fCableLength;         //海缆长度
    m_fWaterDepth = other.m_fWaterDepth;          //水深
    m_fTemperature = other.m_fTemperature;         //温度
    m_fRouteEmbBuryDepth = other.m_fRouteEmbBuryDepth;     //海缆水深
    m_sRoutePath = other.m_sRoutePath;             //路径
    m_sRouteIndex = other.m_sRouteIndex;            //索引
    m_sRouteTag = other.m_sRouteTag;              //标签
    m_sRouteComment = other.m_sRouteComment;          //注释
    m_fRouteDepth = other.m_fRouteDepth;            //路由点深度
    m_fRouteKpValue = other.m_fRouteKpValue;          //Kp值
    m_fRouteBottomDis = other.m_fRouteBottomDis;        //底部距离
    m_bRouteUseful = other.m_bRouteUseful;           //有效性
    m_uRoutePlough = other.m_uRoutePlough;           //犁设备
    m_sRouteEmbTraverse = other.m_sRouteEmbTraverse;      //海缆横贯
    m_fRouteElseCost = other.m_fRouteElseCost;         //其他费用
    m_routeDelayTime = other.m_routeDelayTime;         //延迟时间
    m_uRouteRouId = other.m_uRouteRouId;            //路由路径编号
    m_uRoutePointType = other.m_uRoutePointType;        //是否是连接点（0-否,1-是）备注：使用的以前的字段
    m_fRouteBottomMargin = other.m_fRouteBottomMargin;     //底部余量
    m_fRouteFaceMargin = other.m_fRouteFaceMargin;       //表面余量
    m_dRoundNumber = other.m_dRoundNumber;           //序号
    m_sAutoTag = other.m_sAutoTag;               //自动标签
    m_dSteer = other.m_dSteer;                 //转向角
    m_sMainType = other.m_sMainType;              //主体类型 (接头)
    m_sRoudEmbType = other.m_sRoudEmbType;           //海缆类型
    //m_uRoudEmbTyID = other.m_uRoudEmbTyID;
    m_sCableLenght = other.m_sCableLenght;            //海缆类型长度
    m_fRoudCableCompen = other.m_fRoudCableCompen;        //海缆补偿
    m_uRoudSlackChange = other.m_uRoudSlackChange;        //是否改变余量(0：不改变，1：改变)
    m_uRoudSlackType = other.m_uRoudSlackType;          //余量类型(1：表面余量，2：底部余量)
    m_uRoudRouteReference = other.m_uRoudRouteReference;     //路由点是否兼具参考点类型(0:否，1：是)
    m_iRoudRouteReJoint = other.m_iRoudRouteReJoint;
    return *this;
}

bool RoutePoint::operator ==(const RoutePoint &other)
{
    if(lon != other.lon)
        return false;
    if(lat != other.lat)
        return false;
    if(pointType != other.pointType)
        return false;
    if(m_idbID != other.m_idbID)
        return false;
    if(m_sEvent != other.m_sEvent)
        return false;
    if(m_fdirection != other.m_fdirection)
        return false;
    if(m_fRoutePointDistance != other.m_fRoutePointDistance)
        return false;
    if(m_fRemain != other.m_fRemain)
        return false;
    if(m_fCablePointDistance != other.m_fCablePointDistance)
        return false;
    if(m_fCableTotalDistance != other.m_fCableTotalDistance)
        return false;
    if(m_fMoudleValue != other.m_fMoudleValue)
        return false;
//    if(m_fCableLength != other.m_fCableLength)
//        return false;
    if(m_fWaterDepth != other.m_fWaterDepth)
        return false;
    if(m_fTemperature != other.m_fTemperature)
        return false;
    if(m_fRouteEmbBuryDepth != other.m_fRouteEmbBuryDepth)
        return false;
    if(m_sRoutePath != other.m_sRoutePath)
        return false;
    if(m_sRouteIndex != other.m_sRouteIndex)
        return false;
    if(m_sRouteTag != other.m_sRouteTag)
        return false;
    if(m_sRouteComment != other.m_sRouteComment)
        return false;
    if(m_fRouteDepth != other.m_fRouteDepth)
        return false;
    if(m_fRouteKpValue != other.m_fRouteKpValue)
        return false;
    if(m_fRouteBottomDis != other.m_fRouteBottomDis)
        return false;
    if(m_bRouteUseful != other.m_bRouteUseful)
        return false;
    if(m_uRoutePlough != other.m_uRoutePlough)
        return false;
    if(m_sRouteEmbTraverse != other.m_sRouteEmbTraverse)
        return false;
    if(m_fRouteElseCost != other.m_fRouteElseCost)
        return false;
    if(m_routeDelayTime != other.m_routeDelayTime)
        return false;
    if(m_uRouteRouId != other.m_uRouteRouId)
        return false;
    if(m_uRoutePointType != other.m_uRoutePointType)
        return false;
    if(m_fRouteBottomMargin != other.m_fRouteBottomMargin)
        return false;
    if(m_fRouteFaceMargin != other.m_fRouteFaceMargin)
        return false;

    if(m_dRoundNumber != other.m_dRoundNumber)
        return false;
    if(m_sAutoTag != other.m_sAutoTag)
        return false;
    if(m_dSteer != other.m_dSteer)
        return false;
    if(m_sMainType != other.m_sMainType)
        return false;
    if(m_sRoudEmbType != other.m_sRoudEmbType)
        return false;
    //if(m_uRoudEmbTyID != other.m_uRoudEmbTyID)
    //    return false;
    if(m_sCableLenght != other.m_sCableLenght)
        return false;

    if(m_fRoudCableCompen != other.m_fRoudCableCompen)
        return false;
    if(m_uRoudSlackChange != other.m_uRoudSlackChange)
        return false;
    if(m_uRoudSlackType != other.m_uRoudSlackType)
        return false;
    if(m_uRoudRouteReference != other.m_uRoudRouteReference)
        return false;
    if(m_iRoudRouteReJoint != other.m_iRoudRouteReJoint)
        return false;
    return true;
}

QString RoutePoint::getTypeStr() const
{
    QString sType;
    if(pointType == 1)
    {
        sType = QObject::tr("Rigid Position");
    }
    else if(pointType == 2)
    {
        sType = QObject::tr("Clamped");
    }
    else if(pointType == 5)
    {
        sType = QObject::tr("Sliding");
    }

    return sType;
}

RouteLine::RouteLine()
{
    routeID = 0;
    projectID = 0;
    projectName = "";
    name = "";
    points.clear();
    cableAssemblyVec.clear();
    recondPoints.clear();
    recondCableAssemblyVec.clear();
    routeType = 0;
    routeAssessStatus = 0;
    routeProjectStage = 0;
    startLat = 0;
    startLon = 0;
    endLat = 0;
    endLon = 0;
    routeLenth = 0;
    updateTime = 0;
    m_iHistoryBatches = 0;
}

RouteLine & RouteLine::operator = (const RouteLine &other)
{
    if(this == &other)
        return *this;
    routeID = other.routeID;
    projectID = other.projectID;
    projectName = other.projectName;
    name = other.name;
    points = other.points;
    cableAssemblyVec = other.cableAssemblyVec;
    recondPoints = other.recondPoints;
    recondCableAssemblyVec = other.recondCableAssemblyVec;
    routeType = other.routeType;
    routeAssessStatus = other.routeAssessStatus;
    routeProjectStage = other.routeProjectStage;
    startLat = other.startLat;
    startLon = other.startLon;
    endLat = other.endLat;
    endLon = other.endLon;
    routeLenth = other.routeLenth;
    updateTime = other.updateTime;
    m_iHistoryBatches = other.m_iHistoryBatches;
    return *this;
}
bool RouteLine::operator ==(const RouteLine &other)
{
    if(routeID != other.routeID)
        return false;
    if(projectID != other.projectID)
        return false;
    if(projectName != other.projectName)
        return false;
    if(name != other.name)
        return false;

    if(points.size() != other.points.size())
        return false;
    for(int i = 0;i<points.size();i++)
    {
        if(!(points[i]==other.points[i]))
        {
            return false;
        }
    }
    if(routeType != other.routeType)
        return false;
    if(startLat != other.startLat)
        return false;
    if(startLon != other.startLon)
        return false;
    if(endLat != other.endLat)
        return false;
    if(endLon != other.endLon)
        return false;
    if(routeLenth != other.routeLenth)
        return false;
    if(m_iHistoryBatches != other.m_iHistoryBatches)
        return false;
    return true;
}


ITF_RouteCross::ITF_RouteCross()
{
    m_uKeyID = 0;
    m_sOriginalRouteName = "";    //原路由名称
    m_sCrossoverRouteName = "";   //交越路由名称
    m_fOriginalKP = 0;           //原路由KP
    m_fCrossoverKP = 0;          //交越KP
    m_fCrossLat = 0;             //交越经度
    m_fCrossLon = 0;             //交越纬度
    m_fCrossoverDepth = 0;       //交越深度
    m_sProtectedType = "";        //保护方式
}

ITF_RouteCross & ITF_RouteCross::operator =(const ITF_RouteCross &other)
{
    if(this == &other)
        return *this;
    m_uKeyID = other.m_uKeyID;
    m_sOriginalRouteName = other.m_sOriginalRouteName;    //原路由名称
    m_sCrossoverRouteName = other.m_sCrossoverRouteName;   //交越路由名称
    m_fOriginalKP = other.m_fOriginalKP;           //原路由KP
    m_fCrossoverKP = other.m_fCrossoverKP;          //交越KP
    m_fCrossLat = other.m_fCrossLat;             //交越经度
    m_fCrossLon = other.m_fCrossLon;             //交越纬度
    m_fCrossoverDepth = other.m_fCrossoverDepth;       //交越深度
    m_sProtectedType = other.m_sProtectedType;        //保护方式
    return *this;
}

ShipPlanPoint::ShipPlanPoint()
{
    m_dId = 0;
    m_dShipPlanIndex = 0;
    m_dShipPlanLabel = "";
    m_dShipLon = 0;
    m_dShipLat = 0;
    m_sDiatance = 0;
    m_dShipSpeed = 2.0;
    m_sShipSlack = "3%";
    m_sPayoutSpeed = 0;
    m_sCableDis = 0;
    m_lSpacingTime = "";
    m_sComment = "";
    m_iShipId = 0;
    m_sAutoLabel = "";
    m_dShipBearing = 0;
    m_dShipAlterCourse = 0;
    m_dShipSegment = 0;
    m_dCableSegment = 0;
    m_dDay = "1";
    m_sTime = 0;
    m_dPotType = 1;
    m_sCableId = 0;
    m_sCbaleTypeName = "";
    m_dWaterDepth = 0;
    m_pWaterAngle = 0;
    m_dTension = 0;
}

ShipPlanPoint & ShipPlanPoint::operator =(const ShipPlanPoint &other)
{
    if(this == &other)
        return *this;
    m_dId               = other.m_dId;
    m_dShipPlanIndex    = other.m_dShipPlanIndex;
    m_dShipPlanLabel    = other.m_dShipPlanLabel;
    m_dShipLon          = other.m_dShipLon;
    m_dShipLat          = other.m_dShipLat;
    m_sDiatance         = other.m_sDiatance;
    m_dShipSpeed        = other.m_dShipSpeed;
    m_sShipSlack        = other.m_sShipSlack;
    m_sPayoutSpeed      = other.m_sPayoutSpeed;
    m_sCableDis         = other.m_sCableDis;
    m_lSpacingTime      = other.m_lSpacingTime;
    m_sComment          = other.m_sComment;
    m_iShipId           = other.m_iShipId;
    m_sAutoLabel        = other.m_sAutoLabel;
    m_dShipBearing      = other.m_dShipBearing;
    m_dShipAlterCourse  = other.m_dShipAlterCourse;
    m_dShipSegment      = other.m_dShipSegment;
    m_dCableSegment     = other.m_dCableSegment;
    m_dDay              = other.m_dDay;
    m_sTime             = other.m_sTime;
    m_dPotType          = other.m_dPotType;
    m_sCableId          = other.m_sCableId;
    m_sCbaleTypeName    = other.m_sCbaleTypeName;
    m_dWaterDepth       = other.m_dWaterDepth;
    m_pWaterAngle       = other.m_pWaterAngle;
    m_dTension          = other.m_dTension;
    return *this;
}

bool ShipPlanPoint::operator ==(const ShipPlanPoint &other)
{
    if(m_dId != other.m_dId)
        return false;
    if(m_dShipLon != other.m_dShipLon)
        return false;
    if(m_dShipLat != other.m_dShipLat)
        return false;
    return true;
}

ShipPlanLine::ShipPlanLine()
{
    m_dShipPlanId = 0;
    m_iStartPosType = 0;        //路径开始方式  :  1路由 ,2KP , 3海缆
    m_sStartPosValue = 0;       //开始点位置的值(由值和单位组合的字符串) double
    m_iStartCondType = 0;       //船舶开始类型
    m_sStartCondValue = 0;      //船舶开始类型值                     double
    m_iEndPosType = 0;          //路径结束方式:路由 ,KP , 海缆
    m_sEndPosValue = 0;         //结束点的值                         double
    m_dSpeedModel = 0;          //速度模式
    m_dSlackModel = 0;          //拖缆模式  1.estimate ship slack, 2.constant ship slack
    m_dSlackModeValue = 0;      //拖缆模式的值
    m_iSlopeRangeOne = 0;       //底部斜率角度范围条件1(less than range_one)
    m_iClearAnceOne = 0;        //条件1间隙值
    m_iSlopeRangeTwo = 0;       //底部斜率角度范围条件2(less than range_one)
    m_iClearAnceTwo = 0;        //条件2间隙值
    m_iSlopeRangeThr = 0;       //底部斜率角度范围条件3(less than range_one)
    m_iClearAnceThr = 0;        //条件3间隙值
    m_iSlopeRangeFor = 0;       //底部斜率角度范围条件4(less than range_one)
    m_iClearAnceFor = 0;        //条件4间隙值
    m_iSlopeRangeFiv = 0;       //底部斜率角度范围条件5(less than range_one)
    m_iClearAnceFiv = 0;        //条件5间隙值
    m_dSlowDown = 0;            //斜率改变前的减速距离
    m_dSlowDownPlus = 0;        //减速距离余量
    m_dSpeedUp = 0;             //斜率改变后的加速距离
    m_dSpeedUpPlus = 0;         //加速距离余量
    m_dRouteID = 0;             //路由路径编号
    m_dProjectID = 0;           //项目编号
    //7.20 add by dug
    m_iEmbraceStatus = 0;       //缆型状态字段(收缆 布缆 埋设 检修,其他;
    m_dShipPlanKp = 0;          //船舶计划开始点对应路由的kp值
    m_dRemainderEmbraceLength = 0;   //剩余海缆长度(对应RPL中还未使用的海缆)
    m_dRemainderDistance = 0;        //剩余距离
    m_dDistanceDifference = 0;       //距离差
    m_dShipPlanPoints.clear();
    m_dShipRecondPoints.clear();
}

ShipPlanLine & ShipPlanLine::operator =(const ShipPlanLine &other)
{
    if(this == &other)
        return *this;
//    m_dRouteID          = other.m_dRouteID;
//    m_dProjectID        = other.m_dProjectID;
//    m_sShipPlanName     = other.m_sShipPlanName;
//    m_dShipPlanId       = other.m_dShipPlanId;
//    m_dShipPlanPoints   = other.m_dShipPlanPoints;

    m_dShipPlanId     = other.m_dShipPlanId;
    m_iStartPosType   = other.m_iStartPosType;
    m_sStartPosValue  = other.m_sStartPosValue;
    m_iStartCondType  = other.m_iStartCondType;
    m_sStartCondValue = other.m_sStartCondValue;
    m_iEndPosType     = other.m_iEndPosType;
    m_sEndPosValue    = other.m_sEndPosValue;
    m_dSpeedModel     = other.m_dSpeedModel;
    m_dSlackModel     = other.m_dSlackModel;
    m_dSlackModeValue = other.m_dSlackModeValue;
    m_iSlopeRangeOne  = other.m_iSlopeRangeOne;
    m_iClearAnceOne   = other.m_iClearAnceOne;
    m_iSlopeRangeTwo  = other.m_iSlopeRangeTwo;
    m_iClearAnceTwo   = other.m_iClearAnceTwo;
    m_iSlopeRangeThr  = other.m_iSlopeRangeThr;
    m_iClearAnceThr   = other.m_iClearAnceThr;
    m_iSlopeRangeFor  = other.m_iSlopeRangeFor;
    m_iClearAnceFor   = other.m_iClearAnceFor;
    m_iSlopeRangeFiv  = other.m_iSlopeRangeFiv;
    m_iClearAnceFiv   = other.m_iClearAnceFiv;
    m_dSlowDown       = other.m_dSlowDown;
    m_dSlowDownPlus   = other.m_dSlowDownPlus;
    m_dSpeedUp        = other.m_dSpeedUp;
    m_dSpeedUpPlus    = other.m_dSpeedUpPlus;
    m_dRouteID        = other.m_dRouteID;
    m_dProjectID      = other.m_dProjectID;
    m_sShipPlanName   = other.m_sShipPlanName;
    m_iEmbraceStatus  = other.m_iEmbraceStatus;
    m_dShipPlanKp     = other.m_dShipPlanKp;
    m_dRemainderEmbraceLength = other.m_dRemainderEmbraceLength;
    m_dRemainderDistance      = other.m_dRemainderDistance;
    m_dDistanceDifference     = other.m_dDistanceDifference;
    m_dShipPlanPoints         = other.m_dShipPlanPoints;
    m_dShipRecondPoints       = other.m_dShipRecondPoints;
    return *this;
}
bool ShipPlanLine::operator ==(const ShipPlanLine &other)
{
    if(m_dRouteID != other.m_dRouteID)
        return false;
    if(m_dProjectID != other.m_dProjectID)
        return false;
    if(m_sShipPlanName != other.m_sShipPlanName)
        return false;
    if(m_iEmbraceStatus !=other.m_iEmbraceStatus)
        return false;
    if(m_dShipPlanKp != other.m_dShipPlanKp)
        return false;
    if(m_dRemainderEmbraceLength != other.m_dRemainderEmbraceLength)
        return false;
    if(m_dRemainderDistance != other.m_dRemainderDistance)
        return false;
    if(m_dDistanceDifference != other.m_dDistanceDifference)
        return false;
    for(int i = 0; i < other.m_dShipPlanPoints.size(); ++i)
    {
        if(!(m_dShipPlanPoints[i] == other.m_dShipPlanPoints[i]))
        {
            return false;
        }
    }
    return true;
}

std::vector<std::pair<double, double> > ITF_AIS::getPath() const
{
    return _path;
}

void ITF_AIS::setPath(const std::vector<std::pair<double, double> > &value)
{
    _path = value;

    if(value.empty())
        return;

    if(lon == -999 && lat == -999)
    {
        std::pair<double, double> pos = value.back();
        lat = pos.first;
        lon = pos.second;
    }
}

std::vector<std::pair<double, double> > ITF_AIS::getTouchdown() const
{
    return _touchdown;
}

void ITF_AIS::setTouchdown(const std::vector<std::pair<double, double> > &value)
{
    _touchdown = value;
}

ITF_AIS::ITF_AIS()
    : type(0)
    , mmsi(0)
    , lon(-999)
    , lat(-999)
    , icon_number(-1)
    , is_fleet(false)
{
    _path.clear();
    _touchdown.clear();
}

ITF_Multibeam::ITF_Multibeam()
{
    m_uRouteID = 0;
    m_dLon = 0;
    m_dLat = 0;
    m_dHeight = 0;
    m_dX = 0;
    m_dY = 0;
}

ITF_Multibeam &ITF_Multibeam::operator =(const ITF_Multibeam &other)
{
    m_uRouteID = other.m_uRouteID;
    m_dLon = other.m_dLon;
    m_dLat = other.m_dLat;
    m_dHeight = other.m_dHeight;
    m_dX = other.m_dX;
    m_dY = other.m_dY;
    return *this;
}

SpecialRouteLine::SpecialRouteLine()
{
    m_iId = 0;
    m_iProjectId = 0;
    m_dMainSeaCable = 0;
    points.clear();
}

SpecialRouteLine &SpecialRouteLine::operator=(const SpecialRouteLine &other)
{
    m_iId = other.m_iId;
    m_iProjectId = other.m_iProjectId;
    m_sCreateTime = other.m_sCreateTime;
    m_sFilename = other.m_sFilename;
    m_sSeaCableType = other.m_sSeaCableType;
    m_sOperator = other.m_sOperator;
    m_dMainSeaCable = other.m_dMainSeaCable;
    points = other.points;
}

SpecialRoutePoint::SpecialRoutePoint()
{
    m_iFileId = 0;
    m_iNumber = 0;      //序号
    m_dX = 0;           //经度
    m_dY = 0;           //纬度
    m_dKp = 0;
    m_dActualDepth = 0; //实际埋深（M）
    m_dTargetDepth = 0; //目标埋深（M）
    m_dDepthDifference = 0;     //埋深差值（M）
    m_dResistSnchorWeight = 0;  //抗击锚重（kg）
}

SpecialRoutePoint &SpecialRoutePoint::operator=(const SpecialRoutePoint &other)
{
    m_iFileId = other.m_iFileId;
    m_iNumber = other.m_iNumber;      //序号
    m_sCableType = other.m_sCableType;
    m_dX = other.m_dX;                //经度
    m_dY = other.m_dY;                //纬度
    m_dKp = other.m_dKp;
    m_dActualDepth = other.m_dActualDepth; //实际埋深（M）
    m_dTargetDepth = other.m_dTargetDepth; //目标埋深（M）
    m_dDepthDifference = other.m_dDepthDifference;        //埋深差值（M）
    m_dResistSnchorWeight = other.m_dResistSnchorWeight;  //抗击锚重（kg）
    m_sRiprapProtection = other.m_sRiprapProtection;
    m_sRemarks =  other.m_sRemarks;
}

bool tagITF_ChannelLine::operator ==(const tagITF_ChannelLine &other)
{
    if (fabs(startLon - other.startLon) < FLOAT_ZERO &&
            fabs(startLat - other.startLat) < FLOAT_ZERO &&
            fabs(endLon - other.endLon) < FLOAT_ZERO &&
            fabs(endLat - other.endLat) < FLOAT_ZERO)
    {
        return true;
    }
    else
    {
        return false;
    }
}

}
} //ZCHX
