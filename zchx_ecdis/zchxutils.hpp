#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <cmath>

#include <QtGui>

#include "zchx_ecdis_global.h"

#define FLOAT_ZERO 0.000001

namespace qt {
class zchxMapFrameWork;

class PainterPair{
public:
    PainterPair(QPainter *painter):mPainter(painter)
    {
        if(mPainter)
        {
            painter->save();
        }
    }

    ~PainterPair()
    {
        if(mPainter)
        {
            mPainter->restore();
        }
    }

private:
    QPainter *mPainter;
};

namespace ZCHX {
  double constexpr pi = 3.14159265358979323846;
  double constexpr pi2 = pi / 2.;
  double constexpr pi4 = pi / 4.;
  double constexpr twicePi = 2. * pi;

  template <class T> T sqr(T t) { return (t*t); }

namespace Data{
//zxl add 定义海图插件使用模式{编辑模式与显示模式是互斥的}
enum ECDIS_PLUGIN_USE_MODEL{
    NO_MODEL = 0x00,
    ECDIS_PLUGIN_USE_DISPLAY_MODEL = 0x01,        //当前海图处理显示模式
    ECDIS_PLUGIN_USE_EDIT_MODEL = 0x02, //当前海图处于编辑模式
};
Q_DECLARE_FLAGS(ECDIS_PLUGIN_USE_MODELs, ECDIS_PLUGIN_USE_MODEL);
Q_DECLARE_OPERATORS_FOR_FLAGS(ECDIS_PLUGIN_USE_MODELs);

enum ELETYPE{
    ELE_NONE,
    ELEELLIPSE,  //圆元素
    ELETRIANGLE, //三角形元素
    ELELINE,     //直线元素
    ELERECT,      //矩形元素
    ELEMENT_AIS,
    ELEMENT_RADAR_POINT,
    ELEMENT_RADAR_AREA,
    ELEMENT_ROD,//杆
    ELEMENT_IPC,
    ELEMENT_CAMERA,
    ELEMENT_VIDEO_TARGET,
    ELEMENT_CAMERA_VIEW,
    ELEMENT_WARNING_ZONE,


};
enum ECDIS_DISPLAY_MODEL{//海图显示模式
    ECDIS_MODEL_BASE = 0, //海图基本模式
    ECDIS_MODEL_STANDARD, //海图标准模式
    ECDIS_MODEL_ALL       //海图全部模式
};
enum ECDISCOLOR{//海图天气颜色显示模式
    ECDIS_COLOR_DAYBLACKBACK = 0,
    ECDIS_COLOR_NIGHT,
    ECDIS_COLOR_DAYDUSK,
    ECDIS_COLOR_DAYWHITEBACK,
    ECDIS_COLOR_DAYBRIGHT
};
enum CAMERATYPE{//相机类型
    CAMERA_UNDEF = 0,
    CAMERA_BALL = 1,            //球机
    CAMERA_GUN = 2,             //枪机
    CAMERA_GDY = 3,             //光电仪
    CAMERA_PANORAMA = 4,         //全景相机panorama
    CAMERA_GAN_LEFT ,        //向左的枪机
    CAMERA_GAN_RIGHT,           //向右的枪机-PC
    CAMERA_FAKE_IPCAST,         //假的摄像头: 号角设备
    CAMERA_NOPEOLEPLNA,    //无人机
};
enum ECDIS_STATION{
    ECDIS_STATION_RADAR=0, //雷达站
    ECDIS_STATION_PATROL //巡逻站
};

//设置可拾取的类型
enum ECDIS_PICKUP_TYPE{
    ECDIS_PICKUP_AIS=0,
    ECDIS_PICKUP_RADARORPOINT,
    ECDIS_PICKUP_RADARAREA,
    ECDIS_PICKUP_CAMERAROD,
    ECDIS_PICKUP_CAMERADEV,
    ECDIS_PICKUP_PASTROLSTATION,
    ECDIS_PICKUP_RADARVIDEO,
    ECDIS_PICKUP_ISLANDLINE,
    ECDIS_PICKUP_WARRINGZONE,
    ECDIS_PICKUP_LOCALMAR,
    ECDIS_PICKUP_CAMERAVIDEOWARN,
    ECDIS_PICKUP_ROUTEANDCROSS,
    ECDIS_PICKUP_CHANNELZONE,
    ECDIS_PICKUP_MOORINGZONE,
    ECDIS_PICKUP_CARDMOUTHZONE,
    ECDIS_PICKUP_COMMONZONE,
    ECDIS_PICKUP_ALL,
    ECDIS_PICKUP_NONE //不拾取

};

enum COURSE_TYPE
{
    COURSE_TYPE_SIGNAL = 0,
    COURSE_TYPE_DOUBLE,
    COURSE_TYPE_RESTRICT,
    COURSE_TYPE_OTHER
};
#if 0
//操作权限列表
enum EOperatePrivilege
{
    EOP_PROJECT_MANAGE = 0,//项目管理
        EOP_PROJECT_VIEW,  //1打开项目1
        EOP_PROJECT_NEW,   //2新建
        EOP_PROJECT_EDIT,  //3
        EOP_PROJECT_DELETE,//4
    EOP_PROJECT_OVERVIEW ,               //5项目总览0
        EOP_PROJECT_OVERVIEW_VIEW,      //6
        EOP_PROJECT_OVERVIEW_FILTER,    //7
        EOP_PROJECT_OVERVIEW_STATISTIC, //8
        EOP_PROJECT_OVERVIEW_EXPORT,    //9

//    EOP_CONSTRUCTION_NAVIGATION,

    //项目管理
    EOP_PROJECT_MANAGER,
        EOP_MAINTAIN_ROUTE_PATH,                    //维护路由路径
      //      EOP_MAINTAIN_ROUTE_PATH_VIEW,               //维护路由路径信息查看
     //       EOP_MAINTAIN_ROUTE_PATH_NEW,                //维护路由路径信息更新
            EOP_MAINTAIN_ROUTE_PATH_DELETE,             //维护路由路径信息删除
            EOP_MAINTAIN_ROUTE_PATH_IMPORT,             //维护路由路径信息导入
            EOP_MAINTAIN_ROUTE_PATH_EXPORT,             //维护路由路径信息导出
     //       EOP_MAINTAIN_ROUTE_PATH_HISTORY,            //维护路由路径信息历史
            EOP_MAINTAIN_ROUTE_PATH_EVALUATE,           //维护路由路径信息评审

//        EOP_MAINTAIN_ROUTE_DETAIL,                  //维护路由明细
//            EOP_MAINTAIN_ROUTE_DETAIL_VIEW,             //维护路由明细信息查看
//            EOP_MAINTAIN_ROUTE_DETAIL_EDIT,             //维护路由明细信息
//            EOP_MAINTAIN_ROUTE_DETAIL_INSERT,           //维护路由明细信息
//            EOP_MAINTAIN_ROUTE_DETAIL_ADD,              //维护路由明细信息
//            EOP_MAINTAIN_ROUTE_DETAIL_DELETE,           //维护路由明细信息
//            EOP_MAINTAIN_ROUTE_DETAIL_EXPORT,           //维护路由明细信息
//            EOP_MAINTAIN_ROUTE_DETAIL_PRINT,            //维护路由明细信息
//            EOP_MAINTAIN_ROUTE_DETAIL_SAVE,             //维护路由明细信息

        EOP_MAINTAIN_PLAN,                          //维护计划表计划
       //     EOP_MAINTAIN_PLAN_VIEW,                     //维护计划表查看
            EOP_MAINTAIN_PLAN_IMPORT,                   //维护计划表
            EOP_MAINTAIN_PLAN_EXPORT,                   //维护计划表
            EOP_MAINTAIN_PLAN_DELETE,                   //维护计划表

        EOP_MAINTAIN_SOLUTION,                      //维护方案表
       //     EOP_MAINTAIN_SOLUTION_VIEW,                 //维护方案表查看
            EOP_MAINTAIN_SOLUTION_IMPORT,               //维护方案表
            EOP_MAINTAIN_SOLUTION_EXPORT,               //维护方案表
            EOP_MAINTAIN_SOLUTION_DELETE,               //维护方案表
            EOP_MAINTAIN_SOLUTION_EVALUATE,             //维护方案表

        EOP_MAINTAIN_PERMISSION,                      //维护审批表
      //      EOP_MAINTAIN_PERMISSION_VIEW,                 //维护审批表查看
            EOP_MAINTAIN_PERMISSION_IMPORT,               //维护审批表
            EOP_MAINTAIN_PERMISSION_EXPORT,               //维护审批表
            EOP_MAINTAIN_PERMISSION_DELETE,               //维护审批表
            EOP_MAINTAIN_PERMISSION_EVALUATE,             //维护审批表

        EOP_MAINTAIN_REPORT,                        //维护报告
       //     EOP_MAINTAIN_REPORT_VIEW,                   //维护报告查看
            EOP_MAINTAIN_REPORT_EDIT,                   //维护报告
            EOP_MAINTAIN_REPORT_INSERT,                 //维护报告
            EOP_MAINTAIN_REPORT_ADD,                    //维护报告
            EOP_MAINTAIN_REPORT_DELETE,                 //维护报告
            EOP_MAINTAIN_REPORT_EXPORT,                 //维护报告
            EOP_MAINTAIN_REPORT_SAVE,                   //维护报告

        EOP_MAINTAIN_CABLE_DETECTION,               //维护海缆检测
      //      EOP_MAINTAIN_CABLE_DETECTION_VIEW,          //维护海缆检测查看
            EOP_MAINTAIN_CABLE_DETECTION_IMPORT,        //维护海缆检测
            EOP_MAINTAIN_CABLE_DETECTION_EXPORT,        //维护海缆检测
            EOP_MAINTAIN_CABLE_DETECTION_DELETE,        //维护海缆检测
            EOP_MAINTAIN_CABLE_DETECTION_EVALUATE,      //维护海缆检测

     //   EOP_MAINTAIN_CABLE_DETECTION_DETAIL,            //维护海缆检测明细
     //       EOP_MAINTAIN_CABLE_DETECTION_DETAIL_VIEW,       //维护海缆检测明细查看

    //维护操作
    EOP_PROJECT_MAINTAIN,           //10
        EOP_ROUTE_MANAGE,           //路由管理
            EOP_ROUTE_VIEW,         //路由表(南中海)
            EOP_ROUTE_ADD,          //新增和编辑
            EOP_ROUTE_DELETE,
            EOP_ROUTE_BACKUP,
            EOP_ROUTE_RECOVER,
//            EOP_ROUTE_HISTORY,
//            EOP_ROUTE_REVERSE,
        EOP_DRAWZONE_MANAGE,        //19区域绘制
            EOP_DRAWZONE_ADD,
            EOP_DRAWLINE_ADD,
            EOP_DRAWRISKZONE_ADD,
        EOP_SETTINGZONE_MANAGE,     //22区域设置
            EOP_ZONE_MODIFY,
            EOP_RISKZONE_MODIFY,
        EOP_EVENSIMULATION_MANAGE,  //25事件模拟
        EOP_HISTORYTRACK_SHIPTRACK, //历史轨迹
        EOP_REALTIME_SHIPTRACK,     //26实时轨迹
        EOP_MENAGEMENT_MANAGE,      //27消息管理
        EOP_CHANNEL_MANAGEMENT,     //航道管理
        EOP_MOORING_MANAGEMENT,     //锚泊管理
    EOP_DATA_MANAGE,                //28数据管理
        EOP_SHIPARCHIVES_MANAGE,    //29船舶档案
            EOP_SHIPARCHIVES_VIEW,  //船舶档案窗口
            EOP_ADD_SHIPARCHIVES,   //新增/修改船舶档案
            EOP_DELETE_SHIPARCHIVES,//删除船舶档案
            EOP_OUTPUT_SHIPARCHIVES,//导出船舶档案
        EOP_VOYAGERECORD_MANAGE,    //航行记录
        EOP_TIDEDATA_MANAGE,        //潮汐记录
            EOP_TIDEVIEW_MANAGE,    //潮汐窗口
            EOP_DELETETIDE_MANAGE,  //删除潮汐记录
            EOP_OUTPUTTIDE_MANAGE,  //导入潮汐记录
        EOP_TIDEDATAVIEW_MANAGE,    //潮汐曲线图
        EOP_TRENDDATA_MANAGE,       //潮流记录
            EOP_TRENDVIEW_MANAGE,   // 潮流窗口
            EOP_DELETETREND_MANAGE, //删除潮流记录
            EOP_OUTPUTTREND_MANAGE, //导出潮流记录
        EOP_TRENDDATAVIEW_MANAGE,   //潮流曲线图
        EOP_WEATHERDATA_MANAGE,     //气象数据
            EOP_WEATHERDATA_VIEW,   //预览气象窗口
            EOP_OUTWEATHERDATA,     //导出气象数据
        EOP_WEATHERWARNING_MANAGE,    //气象报警参数
            EOP_WEATHERWARNING_DATA,    //气象报警窗口
            EOP_MODIFY_WEATHERWARNING,  //修改气象参数
        EOP_EMERGENCYPLAN_MANAGE,       //应急预案管理.
            EOP_EMERGENCYPLAN_VIEW,     //应急预案窗口
            EOP_EMERGENCYPLAN_MODIFY,   //新增和修改应急预案
            EOP_EMERGENCYPLAN_DELETE,   //删除应急预案
            EOP_EMERGENCYPLAN_OUTPUT,   //导出应急预案
        EOP_POWERGRID_ROUTE_VIEW,       //路由表(电网)
    EOP_DATASTATICTIS_MANAGE,           //数据统计
//        EOP_ALARMSTATICTIS_MANAGE,      //报警统计
        EOP_ZONESTATICTIS_MANAGE,       //区域统计
//            EOP_ZONESTATICTIS_COUNT,    //区域是否统计设置
//            EOP_ZONESTATICTIS_QUERY,    //区域统计查询
        EOP_FLOWSTATICTIS_MANAGE,       //流量统计查询
        EOP_PREPUSH_MANAGE,             //模拟外推
        EOP_HISTORYPLAYBACK_MANAGE,     //历史回放
    EOP_CONFIG_MANAGE,                  //配置
        EOP_USER_CONFIG,                //用户管理
        EOP_SMS_CONFIG,                 //金笛短信配置
        EOP_SYSTEM_CONFIG,              //系统配置
    EOP_OTHERFUNCTION_MANAGE,                //其他操作 ？？？？
        EOP_OTHERFUNCTION_CPA,               //cpa
        EOP_OTHERFUNCTION_FIXPOS,            //固定参考点
        EOP_OTHERFUNCTION_POSMARK,           //位置标注
        EOP_OTHERFUNCTION_FLOWLINR,          //自定义流量线
        EOP_OTHERFUNCTION_TRAGETINFO,        //目标信息
        EOP_OTHERFUNCTION_TRAGETLIST,        //目标列表
        EOP_OTHERFUNCTION_PROFILE,           //剖面
        //总数
    EOP_TOTAL_COUNT
};
#endif
//----------------------------------------------------------------------------------------------------------
// Message Type  This data item allows for a more convenient handling of the messages
// at the receiver side by further defining the type of transaction.
//----------------------------------------------------------------------------------------------------------
//-|num|-------------Status Discription---------|------------中文-------------------
// | 0 | Undefined                              |  未定义(默认)
// | 1 | Target Report                          |  目标报告
// | 2 | Start of Update Cycle                  |  更新周期开始
// | 3 | Periodic Status Message                |  周期性的状态信息
// | 4 | Event-triggered Status Message         |  事件驱动的状态信息
//----------------------------------------------------------------------------------------------------------
enum MSGTYP {
    MSGTYP_UNDEFINED  = 0,
    TARGET_REPORT = 1,
    START_OF_UPDATE_CYCLE,
    PERIODIC_STATUS = 3,
    EVENT_TRIGGERED_STATUS
};

//----------------------------------------------------------------------------------------------------------
// Track Type  航迹状态
//----------------------------------------------------------------------------------------------------------
//-|num|-------------Status Discription---------|------------中文-------------------
// | 0 | Confirmed track                        |  表示确认的航迹
// | 1 | Track in initiation phase              |  初始状态的航迹
// | 2 | Unknown track                          |  未知跟踪
//----------------------------------------------------------------------------------------------------------
enum CNF {
    CONFIRMED_TRACK  = 0,
    TENTATIVE_TRACK ,
    UNKNOWN_TRACK
};
//----------------------------------------------------------------------------------------------------------
// extrapolation  外推法
//----------------------------------------------------------------------------------------------------------
//-|num|-------------Status Discription-----------------------------|------------中文-------------------
// | 0 | No extrapolation                                           |  没有推算
// | 1 | Predictable extrapolation due to sensor refresh period     |  在感知设备刷新周期中预推算
// | 2 | Predictable extrapolation in masked area                   |  在遮挡去中预推算
// | 3 | Extrapoltion due to unpredictable absence of detection     |  在盲区中预推算
//----------------------------------------------------------------------------------------------------------
enum CST {
    CST_UNDEFINED  = 0,
    PREDICTABLE_EXTRAPOLATION_DUE_PERIOD ,
    PREDICTABLE_EXTRAPOLATION_IN_AREA ,
    EXTRAPOLATION_DUE_UNPREDICTABLE_DETECTION
};
//----------------------------------------------------------------------------------------------------------
// Track Position Code  位置来历
//----------------------------------------------------------------------------------------------------------
//-|num|-------------Status Discription---------|------------中文-------------------
// | 0 | Measured position                      |  测量出的位置
// | 1 | Smoothed position                      |  平滑插值的位置
//----------------------------------------------------------------------------------------------------------
enum STH {
    MEASURED_POSITION  = 0,
    SMOOTHED_POSITION
};

//----------------------------------------------------------------------------------------------------------
// Navigation Status  航行状态
//----------------------------------------------------------------------------------------------------------
//-|num|-------------Status Discription---------------------------------|------------中文-------------------
// | 0 | Under way using engine                                         |  在航(主机推动)
// | 1 | At anchor                                                      |  锚泊
// | 2 | Not under command                                              |  失控
// | 3 | Restricted manoeuverability                                    |  操纵受限
// | 4 | Constrained by her draught                                     |  吃水受限
// | 5 | Moored                                                         |  靠泊
// | 6 | Aground                                                        |  搁浅
// | 7 | Engaged in Fishing                                             |  从事捕鱼
// | 8 | Under way sailing                                              |  靠船帆提供动力
// | 9 | Reserved for future amendment of Navigational Status for HSC   |  为将来船舶运输 DG,HS,MP或IMO规定的有毒或污染C(HSC)类货物的航行状态修正 所保留
// | 10 | Reserved for future amendment of Navigational Status for WIG  |  为将来船舶运输 DG,HS,MP或IMO规定的有毒或污染A(WIG)类货物的航行状态修正 所保留
// | 11 ~ 14 | Reserved for future use                                  |  为将来使用所保留
// | 15 | Not defined (default)                                         |  未定义(默认)
//----------------------------------------------------------------------------------------------------------

enum NAVI_STATUS{
    UNDER_WAY_USING_ENGINE = 0,
    AT_ANCHOR ,
    NOT_UNDER_COMMAND,
    RESTRICTED_MANOEUVER,
    CONSTRAINED_BY_DRAUGHT,
    MOORED,
    AGROUND,
    ENGAGED_IN_FISHING,
    UNDER_WAY_SAILING,
    RESERVED_FOR_FUTURE_AMENDMENT_FOR_HSC,
    RESERVED_FOR_FUTURE_AMENDMENT_FOR_WIG ,
    RESERVED_FOR_FUTURE_USE_1 ,
    RESERVED_FOR_FUTURE_USE_2 ,
    RESERVED_FOR_FUTURE_USE_3 ,
    RESERVED_FOR_FUTURE_USE_4 ,
    NOT_DEFINED
};

//----------------------------------------------------------------------------------------------------------
// EPFD  电子定位装置类型
//----------------------------------------------------------------------------------------------------------
//-|num|-------------Status Discription---------|------------中文-------------------
// | 0 | Undefined                              |  未定义(默认)
// | 1 | GPS                                    |  GPS
// | 2 | GLONASS                                |  GLONASS
// | 3 | Combined GPS/GLONASS                   |  组合 GPS/GLONASS
// | 4 | Loran-C                                |  Loran-C
// | 5 | Chayka                                 |  Chayka
// | 6 | Integrated navigation system           |  综合导航系统
// | 7 | Surveyed                               |  观测
// | 8 - 15 | Galileo                           |  不用
//----------------------------------------------------------------------------------------------------------

enum EPFD{
    UNDEFINED  = 0,
    GPS = 1,
    GLONASS = 2,
    COMBINED_GPS_AND_GLONASS = 3,
    LORAN_C = 4,
    CHAYKA = 5,
    INTEGRATED_NAVIGATION_SYSTEM = 6,
    SURVEYED = 7,
    GALILEO = 8
};

struct TimeSpanVo{
    QString name;
    int defence_time_id;
    QString starttime;
    QString endtime;
    int defence_id;
    int time_cycle;
};

typedef struct tagLatLon
{
    double lat;
    double lon;
}LatLon;

typedef struct tagElePos{
    int  x;
    int  y;
}ElePos;

struct RadarMeet
{
    int trackNumber;		       // 航迹号 I010/161
    QString id;                //ais id
    double lat;			   // WGS84坐标计算纬度 I010/041
    double lon;			   // WGS84坐标计算经度 I010/041
    double disrance;				//距离
    qint64 UTC;                    // 时间标记

    RadarMeet()
    {
        trackNumber = 0;
        id = "";
        lat = 0;
        lon = 0;
        disrance = 0;
        UTC = 0;
    }
    RadarMeet & operator = (const RadarMeet &other)
    {
        trackNumber = other.trackNumber;
        id = other.id;
        lat = other.lat;
        lon = other.lon;
        disrance = other.disrance;
        UTC = other.UTC;
        return *this;
    }
};

struct AISCollide
{
    QString id;                // 唯一识别码("AIS"+消息识别码+"__"+用户识别码)
    double lat;		           // WGS84坐标计算纬度 I010/041
    double lon;		           // WGS84坐标计算经度 I010/041
    double disrance;	       // 距离
    QString collideTime;       // 碰撞时间（格式为2018-08-21 13:30:12）

    AISCollide()
    {
        id = "";
        lat = 0;
        lon = 0;
        disrance = 0;
        collideTime = "";
    }
    AISCollide & operator = (const AISCollide &other)
    {
        id = other.id;
        lat = other.lat;
        lon = other.lon;
        disrance = other.disrance;
        collideTime = other.collideTime;
        return *this;
    }
};

enum WARN_STATUS{
    WARN_NONE = 0,
    WARN_FOCUS_AREA,        // 入侵预警
    WARN_OVERSPEED,         // 超速预警
    WARN_DANGER_CIRCLE,     // 危险圈预警
    WARN_CHANNEL_YAW,       // 偏航预警
    WARN_CHANNEL_COLLISION, // 碰撞预警
    WARN_SPEED_BORDER,      // 越线预警
    WARN_CHANNEL_MOORING,   // 锚泊预警
    WARN_OTHER_WARNING,     // 其他预警
    WARN_BLACKLIST,         // 黑名单预警
    WARN_AISUNOPEN,         // AIS未开启
    WARN_OVERLOAD,          // 超载航行
    WARN_RETROGRADE,        // 逆行报警
    WARN_OVERLENGTH,        // 超长报警
    WARN_OVERTAKING,        // 追越报警
    WARN_ERROR_DROP_ANCHOR, // 违规抛锚预警
};

//radar数据
typedef struct tagITF_RadarPoint
{
    int uuid;
    double lat;
    double lon;
    std::vector<RadarMeet> RadarMeetVec; //交汇点集合
    std::vector<std::pair<double, double> > path;
    int systemAreaCode;			   // 数据源唯一标识符:系统区域代码 I010/010 _sac
    int systemIdentificationCode;   // 数据源唯一标识符:系统识别代码 I010/010 _sic
    MSGTYP  messageType;              // 消息类型 I010/001
    unsigned int trackNumber;		       // 航迹号 I010/161
    float cartesianPosX;	           // 笛卡尔坐标计算X位置 I010/042
    float cartesianPosY;			   // 笛卡尔坐标计算Y位置 I010/042
    double wgs84PosLat;			   // WGS84坐标计算纬度 I010/041
    double wgs84PosLong;			   // WGS84坐标计算经度 I010/041
    float timeOfDay;			       // 当日时间，世界标准时间(UTC)表示的绝对时间戳
    CNF   trackType ;			       // 航迹状态: I010/170_CNF
    bool  trackLastReport;   // 当前目标最后一次上报 I010/170_TRE
    CST   extrapolation;   // 外推法 I010/170_CST
    STH   trackPositionCode;         // 位置来历 I010/170_STH
    float sigmaX;                    // x轴标准差     I010/500
    float sigmaY ;                    // y轴标准差     I010/500
    float sigmaXY;                   // 2轴平方方差     I010/500
    float ampOfPriPlot;              // 震荡波强度检测   I010/131
    double cartesianTrkVel_vx;       // 迪卡尔坐标航迹计算x速度(米/秒)  I010/202
    double cartesianTrkVel_vy;       // 迪卡尔坐标航迹计算y速度(米/秒)  I010/202
    double cog;                      // 方位角
    double sog ;                      // 速度
    //gou yuan jie add
    int  status;                   //预警
    QString trackby;                  // 半自动跟踪
    int  cameraId;                 // 锁定摄像头编号
    quint64   timeStamp;                // 进入报警区域时间  2015-1-4
    uint isSmuggle;                //1为走私 0为非走私
    double distance;
    QString warn_color;               //报警颜色
    uint targetType;               //目标类型  0: 未知   4: 人   2: 车   6: 船
    uint sign_window;              // 提示窗口 1:有提示窗口 0:不显示提示窗口
    uint is_warn;                  //离岸预警
    QString rtsp;                     // rtsp视频地址
    uint    fllow;                    // 1:转动  2:联动跟踪 3:停止跟踪
    uint    mode;                      // 模式 1:自动 2:手动
//    QMap<int, QColor>          mWarnStsColorMap;
    QColor      warnStatusColor;
    //添加是否显示关注,是否显示尾迹
    bool        isConcern;
    bool        isTailTrack;

}ITF_RadarPoint;

//ais数据
enum SHIP_ONLINE_STATUS{
    SHIP_UNKONWN = 0,
    SHIP_NEW_COMING,
    SHIP_NORMAL,
    SHIP_FORECAST,
    SHIP_OFFLINE,
};


////ais预警状态
//enum AIS_WARN_STATUS {
//    AIS_WARN_STATUS_CHANNEL_YAW = 21,  // 航道偏航预警
//    AIS_WARN_STATUS_CHANNEL_COLLISION, // 航道碰撞预警
//    AIS_WARN_STATUS_CHANNEL_SPEED,     // 航道限速预警
//    AIS_WARN_STATUS_DANGEROUS_CICLE,   // 危险圈预警
//    AIS_WARN_STATUS_CHANNEL_MOORING    // 锚泊预警
//};

enum SHIP_BLACK_WHITE{
    SHPT_BW_UNKNOWN = 0,
    SHIP_BW_BLACK = 1,
    SHIP_BW_WHITE,
};

struct ExtrapolateParam{
    QString mID;
    double  mVal;
    ExtrapolateParam() {mID = ""; mVal = 0.0;}
    ExtrapolateParam(const QString& id, double val = 0.0) {mID = id; mVal = val;}
    bool operator ==(const ExtrapolateParam& other) const{
        return mID == other.mID;
    }
};

typedef QList<ExtrapolateParam> ExtrapolateList;

class ZCHX_ECDIS_EXPORT ITF_AIS
{
    std::vector<std::pair<double, double> > _path;
public:
    ITF_AIS();

    std::vector<RadarMeet> RadarMeetVec;  //交汇点集合
    int type;                             //类型 0：ais，1：北斗，2：CMDA
    QString id;                           // 唯一识别码("AIS"+消息识别码+"__"+用户识别码)
    qint32 mmsi;                          // 用户识别码 长度 9
    QString shiptype;                     // 船舶种类(A类，BCS类,BSO类)	长度 3
    NAVI_STATUS navStatus;                // 船舶航行状态
    double lon;
    double lat;
    float rot;                            // 船舶转向率 degree/min
    float sog;                            //对地航速
    float cog;                            //对地航向
    float heading ;                       //航首向
    qint32 imo;                           // IMO 号码	长度 20
    QString callSign;                     // Call Sign 呼号	长度 20
    QString shipName;                     // 船名	长度 30
    qint32  cargoType;                    // 船舶类型	长度 15
    QString country; 	                  // 国籍	长度 3
    QString vendorID;                     // Vendor ID 制造商	长度 30
    double shipLength;                    // 船长
    double shipWidth;                     // 船高
    qint32 toBow;                         // dim to a
    qint32 toStern ;                      // dim to b
    qint32 toPort;                        // dim to c
    qint32 toStarboard ;                  // dim to d
    EPFD fixType ;                        // EPFD 电子定位装置类型
    QString eta;                          // 预计到达时间    (存储外推时间)
    float draught;                        // 最大吃水      numeric(6,2)
    QString dest;	                      // 目的地
    quint64 UTC;                          // 时间标记
    qint32  status;                       //-1异常或离线; 0正常;
    qint32 warn_status;                   //预警 1重点区域 2度监控区 3速边界 4危险圈 11一级 12 二级预警 13 三级预警 14 四级预警 15 五级预警
    QColor      warnStatusColor;
    qint32     icon_number;               //图标编号
    QByteArray   icon_stream;             //图标二进制流
    QString   icon_mark;                  //图标描述
    QString   cdma_phone;                 //终端号码(CDMA)
    QString    bd_id;                     // 终端号码(北斗)
    QString   ship_no;                    //渔船编码
    QString   dist_ship_district;         //所属地区
    QString  ship_port;                   //船籍港
    double  ship_deep;                    // 型深
    double  ship_tot_ton;                 //总吨位
    double  ship_net_ton;                 //净吨位
    double  ship_tot_power;               // 主机总功率
    QString  dict_ship_material;          //船体材质
    QString  ship_build_comp_date;        //建造完工日期
    QString  owner_name;                  //所有者名称
    QString   owner_addr;                 //所有者地址
    QString   owner_tel;                  //所有者电话
    QString   fishing_permit_period_date; //捕捞许可有效期
    QString   fishing_permit_number;      //捕捞许可证编号
    QString  vessel_cert_period_date;     //船检证书有效期
    QString   vessel_cert_number;         //船舶检验证书编号
    QString   register_period_date;       //登记证书有效期
    QString    register_number;           //登记证书编号
    QString group_name;                   //所属分组名称
    qint32  pos_type;                     // 位置类型  0：定时回传位置； 1：单次回传位置 ；100：报警回传位置 ；200：出港报； 201：进港报
    QString vdesc;                        // 备注，位置描述
    int onlineStatus;                     //在线状态  1:新来 2:正常 3:预推 4:离线
    QColor      onlineStatusColor;
    int markType;                         //0 未知 1 黑名单 2 白名单
    QString cpaAttr;                      //CPA 属性， lon,lat,distance
    bool is_fleet;                        //是否属于船队 true：是，false：否
    float danger_circle_radius;           //危险圈半径 单位：米（M）
    ZCHX::Data::AISCollide objCollide;    //AIS碰撞数据
    bool is_construction_ship;            //是否是施工船,如果是,强制图片显示

    std::vector<std::pair<double, double> > _touchdown; //海缆轨迹

    std::vector<std::pair<double, double> > getPath() const;
    void setPath(const std::vector<std::pair<double, double> > &value);
    std::vector<std::pair<double, double> > getTouchdown() const;
    void setTouchdown(const std::vector<std::pair<double, double> > &value);
//    QMap<int, QColor>        mOnlineStsColorMap;
//    QMap<int, QColor>          mWarnStsColorMap;

};

typedef struct tagITF_RadarEcho
{
    QPointF pos;
    QColor curColor;
    int amplitude;
}ITF_RadarEcho;

typedef QMap<int,ITF_RadarEcho> ITF_RadarEchoMap;
//导航
typedef struct NavigationITF
{
    int uuid;
    double lon;
    double lat;
    float heading ;

}ITF_Navigation;

//危險圈
typedef struct DangerousCircleITF
{
    QString id;                 //唯一识别码
    double  lat;                //經度
    double  lon;                //緯度
    double  direction;          //方向角
    double  dangerCircleRadius; //危险圈半径单位：米（M）

}ITF_DangerousCircle;

//人车船
typedef struct tagITF_VideoTarget
{
     int uuid;
     uint cameraId;		    	// 相机id
     QString objectID;           	 // 目标ID
     uint objectState;              // 目标状态
     uint objectType;              // 目标类型   0:人 1:车 2:船 4:未知
     uint  alarmType;				// 告警类型   0:区域入侵 1:越过绊线 2:人员徘徊 3:车辆
     QString timeStamp;				// 时间戳
     double objectMapPosX;            // 目标地图坐标经度
     double objectMapPosY;            // 目标地图坐标纬度
     double objectSpeed;         	// 目标速度
     QString alertAreaName;         // 预警区域名称
     uint isSmuggle;				//  1: 走私 0:非走私
     double node_num;				//节点数量
     QString warn_color;               //  报警颜色

}ITF_VideoTarget;

typedef struct tagITF_RadarArea
{
    int uuid;
    double radarX;
    double radarY;
    int centerLineAngel;
    int radius;
    int maxScanRangeANgle;
    int numberofChannele;
    int maxWakePointsNumber;
}ITF_RadarArea;

////雷达特征区域
typedef struct tagITF_EditRadarZone
{
    int zoneNumber;
    QString zoneName;
    int zoneType;
    int zoneAction;
    QList<LatLon> pointList;
}ITF_RadarFeaturesZone;

//特殊路由
class ZCHX_ECDIS_EXPORT SpecialRoutePoint
{
public:
    SpecialRoutePoint();
    SpecialRoutePoint &operator=(const SpecialRoutePoint &other);
    int     m_iFileId;
    int     m_iNumber;      //序号
    QString m_sCableType;   //编号
    double  m_dX;           //经度 lon
    double  m_dY;           //纬度 lat
    double  m_dKp;
    double  m_dActualDepth; //实际埋深（M）
    double  m_dTargetDepth; //目标埋深（M）
    double  m_dDepthDifference;     //埋深差值（M）
    double  m_dResistSnchorWeight;  //抗击锚重（kg）
    QString m_sRiprapProtection;    //抛石保护
    QString m_sRemarks;             //备注
    double  m_fWaterDepth;          //水深
};

class ZCHX_ECDIS_EXPORT SpecialRouteLine
{
public:
    SpecialRouteLine();
    SpecialRouteLine &operator=(const SpecialRouteLine &other);
    int     m_iId;
    int     m_iProjectId;       //项目ID
    QString m_sCreateTime;
    QString m_sFilename;
    QString m_sSeaCableType;     //海缆类型:A/B/C
    QString m_sOperator;
    int     m_dMainSeaCable;    //是否是主海缆
    std::vector<SpecialRoutePoint> points;
};

class ZCHX_ECDIS_EXPORT CableBaseData//海缆组件
{
public:
    CableBaseData();
    CableBaseData& operator=(const CableBaseData &other);

    QString     mName;
    QColor      mColor;
    int         mStyle;
    int         mWidth;
};

class ZCHX_ECDIS_EXPORT CableInterfaceData//海缆组件
{
public:
    CableInterfaceData();
    CableInterfaceData& operator=(const CableInterfaceData &other);

    QString     mName;
    int         mDiagramSymbol;
};



class ZCHX_ECDIS_EXPORT CableAssembly//海缆组件
{
public:
    CableAssembly();
    CableAssembly& operator=(const CableAssembly &other);

    int          m_uID;
    QString      m_sAssemblyName;
    int          m_uCompenCable;//是否是补偿海缆（0-不是，1-是）
    int          m_uRplIndex;//RPL表中对应点的索引
    int          m_uRouteID;
    int          m_uAssemblyType;//组件类型(1.海缆线，2.海缆接头，5.参考点(3和4暂时没用)3.海缆未定义，4.接头未定义)
    int          m_uAssemblyID;//组件编号,组件类型为海缆时，是海缆编号。组件类型为接头时，是接头编号。组件类型未定义时，是0
    QString      m_sCableTypeName;  //海缆类型名称
    double       m_dStartLon;
    double       m_dStartLat;
    double       m_dEndLon;
    double       m_dEndLat;
    float        m_fAssemblyLength;
    QString      m_sAssemblyRemark;
    QColor       m_sCableColor;//海缆颜色
    int          m_uCableType;//海缆类型
    int          m_uCableWidth;//海缆宽度
    int          m_uCableCategory;//海缆类别
    int          m_uDiagramSymbol;//线路图符号
    int          m_iProjectStage; //项目阶段(历史路由时候用)
};


class ZCHX_ECDIS_EXPORT RoutePoint
{
public:
    RoutePoint();
    RoutePoint & operator = (const RoutePoint &other);
    bool operator ==(const RoutePoint &other);
    QString getTypeStr() const;

    double lon;
    double lat;
    int pointType;//1-route;2-kp;3海缆接头4参考点5转换点6线段7转向补偿点
    int     m_idbID;                //编号
    QString m_sEvent;               // 事件
    float   m_fdirection;           //走向
    float   m_fRoutePointDistance;  //路由点间距离
    float   m_fRouteTotalDistance;  //路由总距离
    float   m_fRemain;              //余量
    float   m_fCablePointDistance;   //海缆点间距离
    float   m_fCableTotalDistance;   //海缆总距离
    float   m_fMoudleValue;         //为模贵重
    //float   m_fCableLength;         //海缆长度(暂时没用)
    float   m_fWaterDepth;          //水深
    float   m_fTemperature;         //温度
    //add
    float   m_fRouteEmbBuryDepth;     //海缆水深
    QString m_sRoutePath;             //路径
    QString m_sRouteIndex;            //索引
    QString m_sRouteTag;              //标签
    QString m_sRouteComment;          //注释
    float   m_fRouteDepth;            //路由点深度
    float   m_fRouteKpValue;          //Kp值
    float   m_fRouteBottomDis;        //底部距离
    bool    m_bRouteUseful;           //有效性
    int     m_uRoutePlough;           //犁设备或拖缆事件类型(0：不使用犁设备，1：Plow Down，2：Plow Up，3：Post Lay Burial Start，4：Post Lay Burial End)
    QString m_sRouteEmbTraverse;      //海缆横贯
    float   m_fRouteElseCost;         //其他费用
    qint64  m_routeDelayTime;         //延迟时间
    int     m_uRouteRouId;            //路由路径编号
    int     m_uRoutePointType;        //是否是连接点（0-否,1-是）备注：使用的以前的字段
    float   m_fRouteBottomMargin;     //底部余量
    float   m_fRouteFaceMargin;       //表面余量
    int     m_dRoundNumber;           //序号
    QString m_sAutoTag;               //自动标签
    float   m_dSteer;                 //转向角
    QString m_sMainType;              //主体类型 (接头)
    QString m_sRoudEmbType;           //海缆类型
    //int     m_uRoudEmbTyID;           //海缆组件ID，通过这个值在组件中获取海缆信息
    float   m_sCableLenght;            //海缆类型长度
    float   m_fRoudCableCompen;        //海缆补偿
    int     m_uRoudSlackChange;        //是否改变余量(0：不改变，1：改变)
    int     m_uRoudSlackType;          //余量类型(1：表面余量，2：底部余量)
    int     m_uRoudRouteReference;     //路由点是否兼具参考点类型(0:否，1：是)
    int     m_iRoudRouteReJoint;        //路由是否有接头属性 0  1
    //
    bool    mAcPoint;
};

class ZCHX_ECDIS_EXPORT RouteLine
{
public:
    RouteLine();
    RouteLine & operator = (const RouteLine &other);
    bool operator ==(const RouteLine &other);

    int routeID;
    int projectID;
    QString projectName;
    QString name;
    std::vector<RoutePoint> points;
    std::vector<CableAssembly> cableAssemblyVec;
    std::vector<RoutePoint> recondPoints;
    std::vector<CableAssembly> recondCableAssemblyVec;
    int routeType;
    int routeAssessStatus;//0.未审批 1.已审批
    int routeProjectStage;
    double startLat;
    double startLon;
    double endLat;
    double endLon;
    double routeLenth;
    qint64 updateTime;
    int    m_iHistoryBatches;       //维护阶段路由历史批次(历史路由时候用)
};

//路由交越数据
class ZCHX_ECDIS_EXPORT ITF_RouteCross
{
public:
    ITF_RouteCross();
    ITF_RouteCross &operator =(const ITF_RouteCross &other);
    int        m_uKeyID;
    QString    m_sOriginalRouteName;    //原路由名称
    QString    m_sCrossoverRouteName;   //交越路由名称
    double     m_fOriginalKP;           //原路由KP
    double     m_fCrossoverKP;          //交越KP
    double     m_fCrossLat;             //交越经度
    double     m_fCrossLon;             //交越纬度
    float      m_fCrossoverDepth;       //交越深度
    QString    m_sProtectedType;        //保护方式

};


class ZCHX_ECDIS_EXPORT ITF_Multibeam
{
public:
    ITF_Multibeam();
    ITF_Multibeam &operator =(const ITF_Multibeam &other);
    int m_uRouteID;
    double m_dLon;
    double m_dLat;
    double m_dHeight;
    double m_dX;
    double m_dY;
};

class ZCHX_ECDIS_EXPORT ShipPlanPoint
{
public:
    ShipPlanPoint();
    ShipPlanPoint & operator = (const ShipPlanPoint &other);
    bool operator ==(const ShipPlanPoint &other);

    int m_dId;                  //
    int m_dShipPlanIndex;       //船舶计划明细点索引
    QString m_dShipPlanLabel;   //船舶计划明细点标签
    double m_dShipLon;          //经度
    double m_dShipLat;          //纬度
    double m_sDiatance;         //总距离
    float m_dShipSpeed;         //船舶速度
    QString m_sShipSlack;       //ship slack 表面余量
    double m_sPayoutSpeed;      //payout speed 铺设速度 ()
    double m_sCableDis;         //海缆总距离
    QString m_lSpacingTime;     //间距时间
    QString m_sComment;         //备注
    int     m_iShipId;          //船舶计划索引主键
    QString m_sAutoLabel;       //自动标签
    float   m_dShipBearing;     //方位角
    float   m_dShipAlterCourse; //转向角
    float   m_dShipSegment;     //点间距离
    float   m_dCableSegment;    //海缆间距
    QString m_dDay;             //天数
    qint64  m_sTime;            //时间
    int     m_dPotType;         //点类型  1 路由点  2 转换点  3 Skp点 4 线 5加减速点 6速度改变结束点(2和5一样的处理方式) 7 坡度点
    int     m_sCableId;         //海缆组件ID
    QString m_sCbaleTypeName;   //海缆类型名称
    double  m_dWaterDepth;      //水深
    double  m_pWaterAngle;      //入水角
    double  m_dSlopeValue;      //坡度
    double  m_dTension;         //张力
};

class ZCHX_ECDIS_EXPORT ShipPlanLine
{
public:
    ShipPlanLine();
    ShipPlanLine & operator = (const ShipPlanLine &other);
    bool operator ==(const ShipPlanLine &other);

    int m_dShipPlanId;                  //
    int m_iStartPosType;        //路径开始方式  :  路由 ,KP , 海缆
    double m_sStartPosValue;    //开始点位置的值(由值和单位组合的字符串) double
    int m_iStartCondType ;      //船舶开始类型
    double m_sStartCondValue;   //船舶开始类型值                     double
    int m_iEndPosType;          //路径结束方式:路由 ,KP , 海缆
    double m_sEndPosValue;      //结束点的值                         double
    int m_dSpeedModel;          //速度模式
    int m_dSlackModel;          //拖缆模式  1.estimate ship slack, 2.constant ship slack
    float m_dSlackModeValue;    //拖缆模式的值
    int m_iSlopeRangeOne;       //底部斜率角度范围条件1(less than range_one)
    int m_iClearAnceOne;        //条件1间隙值
    int m_iSlopeRangeTwo;       //底部斜率角度范围条件2(less than range_one)
    int m_iClearAnceTwo;        //条件2间隙值
    int m_iSlopeRangeThr;       //底部斜率角度范围条件3(less than range_one)
    int m_iClearAnceThr;        //条件3间隙值
    int m_iSlopeRangeFor;       //底部斜率角度范围条件4(less than range_one)
    int m_iClearAnceFor;        //条件4间隙值
    int m_iSlopeRangeFiv;       //底部斜率角度范围条件5(less than range_one)
    int m_iClearAnceFiv;        //条件5间隙值
    float m_dSlowDown;          //斜率改变前的减速距离
    float m_dSlowDownPlus;      //减速距离余量
    float m_dSpeedUp;           //斜率改变后的加速距离
    float m_dSpeedUpPlus;       //加速距离余量
    int m_dRouteID;             //路由路径编号
    int m_dProjectID;           //项目编号
    QString m_sShipPlanName;    //项目名称
    //7.20 add by dug
    int m_iEmbraceStatus;       //缆型状态字段(收缆 布缆 埋设 检修,其他;
    double m_dShipPlanKp;       //船舶计划开始点对应路由的kp值
    double m_dRemainderEmbraceLength;   //剩余海缆长度(对应RPL中还未使用的海缆)
    double m_dRemainderDistance;        //剩余距离
    double m_dDistanceDifference;       //距离差
    std::vector<ShipPlanPoint> m_dShipPlanPoints;
    std::vector<ShipPlanPoint> m_dShipRecondPoints;
};

typedef struct WaterPoint
{
    int id;
    double dx;
    double dy;
    double lat;
    double lon;
    double Length;
    double slope;
    int indexId;        //文件索引ID
    int routeId;        //路由路径ID
    int lineType;//剖面有多条线段，用于区分属于那条线段
    double slopeValue;
}ITF_WaterDepth;

//相机视场范围
typedef struct tagITF_CameraView
{
    double lat;
    double lon;
    double center_line; //中心线角度  pan postion
    double zone_length; //长度
    double zone_angle;  //角度宽度      mag
    QString id;
}ITF_CameraView;

typedef struct tagITF_WarringZone
{
    int      id;                      // 防区id号
    QString  name;                    // 防区名称
    int      shape;                   // 防区形状 1:多边形 2:圆 3:线
    double   circleRadius;            // 圆半径形状为圆必须存在单位：米（M）
    double   circleLon;               // 圆心经度形状为圆必须存在
    double   circleLat;               // 圆心纬度形状为圆必须存在
    QString  borderColor;             // 边线颜色
    QString  fillColor;               // 填充颜色
    QString  warnColor;               // 告警颜色
    double   speedLimit;              // 限速单位：节（KN）
    double   dropAnchorThreshold;     // 抛锚门限单位：米（M）
    int      countTrack;              // 是否开启轨迹统计 1:开启 2:不开启
    bool     isWarn;                  // 是否预警true：是 false：否
    int      defType;                 // 防区类型 1：准入 2：禁入
    QString  ascription;              // 归属
    QString  contactNum;              // 联系电话
    QString  remark;                  // 描述

    std::vector<std::pair<double, double> > path;

    bool operator ==(const tagITF_WarringZone& other) const
    {
        return this->id == other.id || this->name == other.name;
    }

}ITF_WarringZone;

typedef struct tagITF_CoastData
{
    int         id;                     // 主键号
    QString     name;                   // 名称
    QString     manageOrganization;     // 监管机构
    QString     responsibilityAccount;  // 负责人
    double      beachLength;            // 岸线长
    QString     color;                  // 颜色
    QString     style;                  // 样式
    std::vector<std::pair<double, double> > path; // 经纬度集合
    double      height;                 // 高度
}ITF_CoastData;

typedef struct tagITF_SeabedPipeLine
{
    int         id;                     // 主键号
    QString     name;                   // 名称
    QString     manageOrganization;     // 监管机构
    QString     responsibilityAccount;  // 负责人
    double      lineLength;             // 线长
    QString     color;                  // 颜色
    QString     style;                  // 样式
    std::vector<std::pair<double, double> > path; // 经纬度集合
    double      depth;                  // 深度
}ITF_SeabedPipeLine;

typedef struct tagITF_Structure
{
    int         id;                     // 主键号
    QString     name;                   // 名称
    double      area;                   // 面积
    QString     color;                  // 颜色
    QString     style;                  // 样式
    std::pair<double, double> point;    // 经纬度
    double      depth;                  // 深度
}ITF_Structure;

typedef struct tagITF_AreaNet
{
    int         id;                     // 主键号
    QString     name;                   // 名称
    double      area;                   // 面积
    QString     color;                  // 颜色
    QString     style;                  // 样式
    std::vector<std::pair<double, double> > path; // 经纬度集合
}ITF_AreaNet;

typedef struct tagITF_ChannelLine
{
    int      lineType;    // 航道线类型1：航道入口 2：航道出口3：航道边线4：特殊边线
    bool     isSelected;  // 是否选中
    double   startLon;    // 开始经度
    double   startLat;    // 开始纬度
    double   endLon;      // 结束经度
    double   endLat;      // 结束纬度

    bool operator ==(const tagITF_ChannelLine &other);

}ITF_ChannelLine;

typedef struct tagITF_Channel
{
    int      id;                   // 航道编号
    QString  name;                 // 航道名称
    int      shape;                // 形状 1：多边形 2：圆
    QString  borderColor;          // 边线颜色
    QString  fillColor;            // 填充颜色
    QString  warnColor;            // 告警颜色
    double   radius;               // 半径形状为圆必须存在单位：米（M）
    int      courseType;           // 航道类型 1：单行航道 2：双行航道 3：限制性航道4：其他
    double   speedLimit;           // 限速单位：节（KN）
    double   collisionThreshold;   // 碰撞门限单位：米（M）
    double   yawThreshold;         // 偏航门限单位：米（M）
    double   overtakThreshold;     // 追越门限单位：米（M）
    double   dropAnchorThreshold;  // 抛锚门限单位：米（M）
    double   overtakShortDis;      // 追越最短距离单位：米（M）
    double   overtakVerticalDis;   // 追越垂直距离单位：米（M）
    double   acceleraThreshold;    // 加速度阀值单位：米（M）
    double   acceleraDfference;    // 加速度差值单位：米（M）
    bool     isWarn;               // 是否预警true：是 false：否
    QString  ascription;           // 归属
    QString  remark;               // 描述
    int      directionType;        // 航向类型1：东、2：西、3：南、4：北、5：东北、6：西北、7：东南、8：西南
    int      waterLevel;           // 水位1：雨季、2：旱季、3：正常
    double   courseCorrect;        // 航道修正 单位：米（M）
    bool     isConverseWarn;       // 是否逆行预警 true：是 false：否
    bool     isOverloadWarn;       // 是否超载预警 true：是 false：否
    int      countTrack;           // 是否开启轨迹统计 1:开启2:不开启
    std::vector<std::pair<double, double> > path; // 经纬度集合
    std::vector<ITF_ChannelLine> lineList; // 航道线集合
}ITF_Channel;

typedef struct tagITF_Mooring
{
    int      id;            // 锚泊编号
    QString  name;          // 锚泊名称
    int      shape;         // 形状 1：多边形 2：圆
    QString  borderColor;   // 边线颜色
    QString  fillColor;     // 填充颜色
    QString  warnColor;     // 告警颜色
    double   radius;        // 半径形状为圆必须存在单位：米（M）
    int      anchorType;    // 锚泊类型 1：小型锚区90米以下、2：中型锚区90-150米、3：重型锚区150 -200米，4：专业锚区200-300米、5：特殊锚区超过300米
    double   displaceDis;   // 位移距离单位：米（M）
    int      displaceCycle; // 位移周期单位：秒（S）
    bool     isWarn;        // 是否预警true：是 false：否
    QString  ascription;    // 归属
    QString  contactNum;    // 联系电话
    QString  remark;        // 描述
    int      countTrack;    // 是否开启轨迹统计 1:开启 2:不开启

    std::vector<std::pair<double, double> > path; // 经纬度集合
}ITF_Mooring;

typedef struct tagITF_CardMouth
{
    int            id;              // 卡口编号
    QString        name;            // 卡口名称
    int            shape;           // 形状 1：多边形 2：圆3：线
    QString        borderColor;     // 边线颜色
    QString        fillColor;       // 填充颜色
    QString        warnColor;       // 告警颜色
    double         circleRadius;    // 圆半径形状为圆必须存在单位：米（M）
    double         circleLon;       // 圆心经度形状为圆必须存在
    double         circleLat;       // 圆心纬度形状为圆必须存在
    int            caMouCapConType; // 基建结构类型 1：单向、2：双向
    double         overlength;      // 超长限制单位：米（M）
    bool           isWarn;          // 是否预警true：是 false：否
    bool           isAisUnopened;   // 是否开启检测AIS未开启true：是 false：否
    QString        ascription;      // 归属
    QString        contactNum;      // 联系电话
    QString        remark;          // 描述
    int            countTrack;      // 是否开启轨迹统计 1:开启 2:不开启

    std::vector<std::pair<double, double> > path; // 经纬度集合

}ITF_CardMouth;

typedef struct tagITF_ShipAlarmAscend
{
    QString  sShipName;       //船名
    qint64   immsi;           //mmsi
    double   lon;             // 经度
    double   lat;             // 纬度
    int      alarmType;       // 预警类型 1:入侵预警、2:超速预警、3:危险圈预警、4:偏航预警、5:碰撞预警、
                              //6:越线预警、7:锚泊预警、8:其他预警、9:黑名单预警、10:AIS未开启、
                              //11:超载航行、12:逆行报警、13:超长报警、14:追越报警、15:违规抛锚
    QString  ereaName;        // 区域名称
    QString  time;            // 时间格式为2018-11-06 11:21:02
}ITF_ShipAlarmAscend;

typedef struct tagITF_Fleet
{
    QString   aisId;                   // AIS编号
    QString   name;                    // 船名
    int       mmsi;                    // MMSI
    QString   callSign;                // 呼号
    float     draught;                 // 吃水
    float     shipLength;              // 船长
    float     shipWidth;               // 船宽
    int       cargoType;               // 船舶类型
    QString   shiptype;                // 船舶种类(A类，BCS类,BSO类)
    float     yawMinDis;               // 偏航最小距离（船首离边界最短距离）单位：米（M）
    float     collisionMaxDis;         // 碰撞最大距离单位：米（M）
    QString   dangerCircleName;        // 危险圈名称
    float     dangerCircleRadius;      // 危险圈半径单位：米（M）
}ITF_Fleet;

typedef struct tagITF_CloudHotSpot
{
    enum FLLOW_TYPE{
        FLLOW_TYPE_TURN = 1,
        FLLOW_TYPE_LINKAGE_TRACKING,
        FLLOW_TYPE_STOP_TRACKING
    };
    enum MODE{
        MODE_AUTO = 1,
        MODE_HANDLE
    };
    int     fllow;        // 1:转动 2:联动跟踪 3:停止跟踪
    int     mode;         // 模式(预留属性) 1:自动 2:手动
    QString targetNumber; // 目标编号(直接点击地图就没有该编号)
    int     targetType;   // 目标类型 1:AIS 2:雷达
    double  targetLon;    // 目标经度
    double  targetLat;    // 目标纬度
}ITF_CloudHotSpot;


typedef struct tagITF_FlowLine
{
    int     m_iLineId;
    QString m_sName;
    double  m_iLineLength;
    QString m_sLineColor;
    std::vector<std::pair<double, double> > path;

}CustomFlowLine;
//环岛线
typedef struct tagITF_IslandLine{
    int uuid;
    std::vector<std::pair<double, double> > path;
    int type;
    int id; //--->id
    QString name; //--->name
    int extrapolate; //外推米数
    QString llSet; //--->lon_lat经纬度集合
    int warnFlash;//报警闪烁 1:闪烁 0:不闪烁
    int signWindows; //报警提示窗 1:提示 0:不提示
    int warnVoice;   //报警声音   1:有. 0 没有
    int showMv;      //弹出视频  1.弹 0 不弹
    QString warnColor; //报警颜色
    QString tts;
    QString content; //--->remark
}ITF_IslandLine;
//需求提出的元素结构
//圆
typedef struct tagITF_EleEllipse
{
    tagITF_EleEllipse()
        : radius(0.0)
        , radiusVertical(-1.0) //用于标识为圆形，大于0的值为椭圆
    {

    }

    double radius; //绘制圆, 或者绘制椭圆时的横轴半径
    double radiusVertical; //用于绘制椭圆时的纵轴半径
    LatLon ll; //经纬度点
    ElePos pos; //像素位置
    bool   showCircleCenter;//是否需要显示圆心
    QPen   pen;
    QBrush brush;
}ITF_EleEllipse;

//三角形
typedef struct tagITF_EleTriangle
{
    LatLon ll; //经纬度点ss
    int   alpha; //透明度0~255
    double angle; //与正北方向的夹角, 顺时针方向
    QPen   pen;
    QBrush brush;
    bool   isdy; //是否是等腰三角形，　默认等边三条边都是30px，等腰底边20px 另两条边30px
    bool   isfill; //是否需要填充
}ITF_EleTriangle;

//直线
typedef struct tagITF_EleLine
{
    LatLon ll1; //启点
    LatLon ll2; //终点
    QPen pen;
    QBrush brush;

}ITF_EleLine;

//矩形
typedef struct tagITF_EleRect
{
    LatLon ll;
    int w;
    int h;
    QPen pen;
    QBrush brush;
}ITF_EleRect;

//摄像杆状态
enum CAMERAROD_STATUS{
    CAMERAROD_OK, //杆上所有相机全亮
    CAMERAROD_WARRING, //杆上最小有一个相机异常
    CAMERAROD_ERROR    //杆上所有相机全不亮
};

//摄像杆
typedef struct tagITF_CameraRod{
    int                 nUUID;
    LatLon              nLatLon;
    QString             szID;
    QString             szName;
    CAMERAROD_STATUS    nStatus;
    tagITF_CameraRod()
    {
        nStatus = CAMERAROD_OK;
    }
}ITF_CameraRod;

struct IPCastDevice                 // 号角设备
{
    uint    id;                     // 终端ID
    uint    sessionID;              // 活动会话ID
    int     status;                 // 终端状态：-1-不连通，0-空闲, >0-使用中
    int     vol;                    // 音量：0~56，0最小，56最大
    QString ip;                     // 终端IP地址
    QString relayIP;                // 中继服务器IP地址
    QString name;                   // 终端名称
    QString rodID;                  // 所在电线杆的ID
    double  lat;                    //号角设备一般挂在杆上.不设定经纬度值  独立显示时请设定经纬度
    double  lon;
};

//光电仪-照相机-球机(一切能绑在电杆上的,光电仪除外)
struct ITF_CameraDev
{
    enum ITF_CAMERA_PARENT{
        PARENT_NONE = 0,
        PARENT_ROD,
        PARENT_AIS,
    };

    ITF_CameraDev():mParentEleID(""), nStatus(1) {}       //默认正常

    uint                nUUID;
    uint                nDBID;                      //数据库ID
    CAMERATYPE          nType;                       //相机类型
    LatLon              nLatLon;                         //相机经纬度
    QString             szPreviewRtspUrl;           //预览rtsp地址大码流
    QString             szPreviewRtspUrlLite;       //预览rtsp地址小码流
    QString             szRecordRtspUrl;            //录像rtsp地址
    QString             szVdeoAnalysisRtspUrl;      //视频分析rtsp地址
    QString             szCameraIP;                 //IP地址
    QString             szCameraUid;                //用户名
    QString             szCameraPwd;                //密码
    QString             szControlIP;                //相机控制IP
    QString             szCamName;                  //相机名称
    uint                nFramerate;                 //相机帧率
    uint                nCameraPort;                //相机直连端口
    uint                nControlPort;               //相机控制端口
    uint                nPreViewChan;               //相机直连时的预览通道号
    uint                nLeftViewAngle;             //相机安装时左视角以正北为0 正南为180
    uint                nRightViewAngle;            //右侧视角      //相机视场范围使用
    //QString             szSite;                     //杆的ID 名称
    uint                nCameraColor;               //红光相机，或者白光相机
    double              nMaxRange;                  //最大跟踪范围
    uint                nStatus;                    //相机状态1：正常（在线）、2：掉线（不在线）、512：异常
    QString             szAscription;               // 归属
    QString             szContactNum;               // 联系电话
    QString             szRemark;                   // 描述
    QList<int>          mMaskList;                  // 相机的掩码 1：视频；2：音频；4：相机控制
    IPCastDevice        mIpcastDevice;              //相机对应的号角设备
    QString             mParentEleID;               //相机是否挂在了其他设备上
};

//巡逻站 和雷达站
typedef struct tagITF_PatrolStation
{
    int uuid;
    LatLon ll;
    ECDIS_STATION type;
    QString name;
    QString sbid;
}ITF_PastrolStation;

//位置标注
typedef struct tagITF_LocalMark{
    int uuid; //唯一索引
    LatLon ll;         //位置标注坐标(WGS84)
    QString localName; //位置标注名称
    QString localMark; //简介
}ITF_LocalMark;

//GPS
struct GPSPoint
{
    enum Type{GPS, BIGDIPPER, CDMA};
    Type type;

    QString imei;
    QString deviceName;
    QDateTime sysDate;

    int status;
    QColor warn_color;
    int warn_status;
    QDateTime warn_utc;

    double course; //航向
    double speed;

    double lon;
    double lat;

    std::list<std::shared_ptr<GPSPoint> > track;
};

//回波数据
class ZCHX_ECDIS_EXPORT ITF_RadarVideo
{
public:
    int    uMsgIndex;             //扇区索引
    double dCentreLon;            //中心点经度
    double dCentreLat;            //中心点纬度
    double dAzimuth;              //方位角
    double dStartDis;             //开始距离
    double dDisInterval;          //距离间隔
    int    uBitResolution;        //分辨率
    QList<int> amplitudeList;     //该方位角线上点集合(总共1364个点)
    QList<int> indexList;         //该方位角线上点集合(总共1364个点)
};

//视频联动跟踪
typedef struct tagITF_CameraTrackTarget
{
    int type;           //1 船 2 雷达  3 其他
    QString id;         //船 名和雷达 ID
    double lat;
    double lon;

}ITF_CameraTrackTarget;
typedef struct  tagITF_NetGrid{
    long gridId;        //	网格编号	long	否
    QString gridName;	//网格名称	String	否
    double leftTopLon;	//左上角经度	Double	否
    double leftTopLat;	//左上角纬度	Double	否
    double leftLowerLon;	//左下角经度	Double	否
    double leftLowerLat;	//左下角纬度	Double	否
    double rightTopLon;	//右上角经度	Double	否
    double rightTopLat;	//右上角纬度	Double	否
    double rightLowerLon;	//右下角经度	Double	否
    double rightLowerLat;	//右下角纬度	Double	否
}ITF_NetGrid;

typedef struct tagITF_CameraNetGrid{
    QString                 mCameraID;
    QList<ITF_NetGrid>      mNetGridList;
}ITF_CameraNetGrid;

}

enum tagsFocus{
    tags_HasFocus = 0x00000001,
    tags_MoveCenter = 0x00000010
};

class ZCHX_ECDIS_EXPORT Utils
{
    static Utils *d;
    Utils();
public:
    static Utils *instance();
    void test();

    static void distbear_to_latlon(double lat1, double lon1, double dist,
                                   double brng, double &lat_out, double &lon_out );
    static double getDistanceByPixel(zchxMapFrameWork *f, double lat1, double lon1, double dist, double brng, QPointF pos);
    static double calcAzimuth(double lon1, double lat1, double lon2, double lat2);//已知两点坐标，求两点连线的方位角
    static double getDis(double startX, double startY, double endX, double endY);//求两点连线之间的距离
    static void getCenterPosValueByPosList(const QList<QPointF> &posList,QPointF &centerPos);//求一系列点的中心点
    static double toRad(double deg);
    static double toDeg(double rad);
    static double ang2Cir(double angle);
    /*!
     * \coordinate 经度值或者纬度值
     * \latflag   经纬度标志 ture 为lat  false 为lon
     */
    QString latLonToString(double coordinate, bool latflag);

    /*!
     * \brief 获取自定义图标
     * \param imageID 从1到N
     */
    static QPixmap getAisImage(int imageID);
    static QString getAisImagePath(int imageID);

    /*!
     * \brief 当前的项目, 实现不同的项目使用不同的图标
     */
    static QString gCurrProject; //valid project: sdasid
    /*!
     * \brief 获取图片路径
     * \param path 图片路径
     * \param project 项目名称
     * \return 如果 project 不为空 则用 project 替换element; 否则用 gCurrProject 替换
     */
    static QString getImagePath(const QString &path, const QString &project = QString());
    /*!
     * \brief 获取图片
     * \param path 图片路径
     * \param project 项目名称
     * \return 如果 project 不为空 则用 project 替换element; 否则用 gCurrProject 替换
     */
    static QPixmap getImage(const QString &path, const QString &project = QString());
    /*!
     * \brief 获取不同类型的相机, 在不同状态下的图片路径
     */
    static QString getImagePath(Data::CAMERATYPE type, uint status);
    /*!
     * \brief 获取不同类型的相机, 在不同状态下的图片
     * \param currScale 当前缩放级别
     */
    static QPixmap getImage(Data::CAMERATYPE type, uint status, int currScale);
    /*!
     * \brief 获取图片, 不同的缩放
     * \param 图片路径
     * \param color 大比例时填充的颜色
     * \param currScale 当前缩放级别
     */
    static QPixmap getImage(const QString &path, QColor color, int currScale);


    static QString getWarnName(int warnStatus);

    static QString getAlarmImg(int type);

    static int      rounds(double);
};

//layer
const char LAYER_ISLAND[]             = "lay_island";
const char LAYER_WIREROD[]            = "lay_wirerod";
const char LAYER_GDY[]                = "lay_gdy";
const char LAYER_UAV[]                = "lay_uav";
const char LAYER_DEFENCE[]            = "lay_defence";
const char LAYER_LOCALMARK[]          = "lay_local_mark";
const char LAYER_PATROL_RADAR_SITE[]  = "lay_patrol_radar_site";
const char LAYER_RADAR_SITE[]         = "lay_radar_site";
const char LAYER_PATROL_SITE[]        = "lay_patrol_site";
const char LAYER_AIS[]                = "lay_ais";
const char LAYER_AIS_CURRENT[]        = "lay_ais_current";
const char LAYER_AIS_TRACK[]          = "lay_ais_track";
const char LAYER_AIS_CABLE_TOUCHDOWN[]    = "lay_ais_cable_touchdown";
const char LAYER_AIS_LAW[]            = "lay_law_enforcement_vessel"; //执法船
const char LAYER_RADAR[]              = "lay_radar";
const char LAYER_RADAR_CURRENT[]      = "lay_radar_current";
const char LAYER_RADAR_TRACK[]        = "lay_radar_track";
const char LAYER_RADAR_AREA[]         = "lay_radar_area";
const char LAYER_RADAR_ECHOES[]       = "lay_radar_echoes";
const char LAYER_WARNING_TARGET[]     = "lay_warning_target";
const char LAYER_ROUTE_LINE[]         = "lay_route_line";           //路由线
const char LAYER_CURRENT_ROUTE_LINE[] = "lay_current_route_line";   //当前工程路由线
const char LAYER_HISTORY_ROUTE_LINE[] = "lay_history_route_line";   //历史路由线
const char LAYER_ROUTE_CROSS[]        = "lay_route_cross";           //交越路由
const char LAYER_MULTIBEAM[]          = "lay_multibeam";           //多波束
const char LAYER_RADARVIDEO[]         = "lay_radarvideo";           //雷达回波
const char LAYER_DANGEROUS_CIRLE[]    = "lay_dangerous_cirle";      //危险圈
const char LAYER_CAMERA_VIEW[]        = "lay_camera_view";          //相机视场
const char LAYER_RADAR_FRETURE_AREA[] = "lay_radar_feature_area";   //雷达特征区域
const char LAYER_LONLAT_GIRD[]        = "lay_lonlat_gird";          //经纬网格
const char LAYER_GPS[]                = "lay_gps";
const char LAYER_GPS_CURRENT[]        = "lay_gps_current";
const char LAYER_GPS_TRACK[]          = "lay_gps_track";
const char LAYER_CAMERA[]             = "lay_camera";               //相机直接显示的图层
const char LAYER_SHIPPLAN_LINE[]      = "lay_shipplan_line";       //船舶计划航线
const char LAYER_CURRENT_SHIPPLAN_LINE[]      = "lay_current_shipplan_line";//当前项目船舶计划航线
const char LAYER_CDMA[]               = "lay_cdma";                 //CDMA定位
const char LAYER_CDMA_TARGET[]        = "lay_cdma_target";          //CDMA当前位置
const char LAYER_CDMA_TRACK[]         = "lay_cdma_track";           //CDMA历史轨迹
const char LAYER_BIGDIPPER[]          = "lay_big_dipper";           //北斗定位
const char LAYER_BIGDIPPER_TARGET[]   = "lay_big_dipper_target";    //北斗当前位置
const char LAYER_BIGDIPPER_TRACK[]    = "lay_big_dipper_track";     //北斗历史轨迹
const char LAYER_TOWER_ROD[]          = "lay_tower_rod";            //塔杆
const char LAYER_NAVIGATION_MARK[]    = "lay_navigation_mark";      //航标
const char LAYER_HISTORY_AIS[]        = "lay_history_ais";          //历史AIS
const char LAYER_HISTORY_RADAR[]      = "lay_history_radar";        //历史Radar
const char LAYER_SPECIAL_ROUTELINE[]  = "lay_special_routeline";    //特殊路由
const char LAYER_HIGHLEVEL[]          = "lay_high_level";           //施工高层图
const char LAYER_IMAGEMAP[]           = "lay_image_map";            //施工影像图
const char LAYER_COASTDATA[]          = "lay_coast_data";           //海岸数据
const char LAYER_SEABEDPIPELINE[]     = "lay_seabed_pipeline";      //海底管线
const char LAYER_STRUCTURE[]          = "lay_structure";            //结构物
const char LAYER_AREANET[]            = "lay_area_net";             //地理区域网络
const char LAYER_CHANNEL[]            = "lay_channel";              //航道
const char LAYER_MOORING[]            = "lay_mooring";              //锚泊
const char LAYER_DEFINEZONE[]         = "lay_defineZone";           //自定义区域
const char LAYER_CARDMOUTH[]          = "lay_cardMouth";            //卡口
const char LAYER_ALARMASCEND[]        = "lay_alarmAscend";          //预警追溯轨迹线
const char LAYER_CAMERANETGRID[]             = "camera_netGrid";    //相机网格


//layer translate
const char TR_LAYER_TOWER_ROD[]           = QT_TRANSLATE_NOOP("TranslationManager", "Tower Rod");
const char TR_LAYER_NAVIGATION_MARK[]     = QT_TRANSLATE_NOOP("TranslationManager", "Navigation Mark");
const char TR_LAYER_ISLAND[]              = QT_TRANSLATE_NOOP("TranslationManager", "Island Line");
const char TR_LAYER_WIREROD[]             = QT_TRANSLATE_NOOP("TranslationManager", "Wire rod");
const char TR_LAYER_GDY[]                 = QT_TRANSLATE_NOOP("TranslationManager", "Photoelectric instrument");
const char TR_LAYER_UAV[]                 = QT_TRANSLATE_NOOP("TranslationManager", "UAV");
const char TR_LAYER_DEFENCE[]             = QT_TRANSLATE_NOOP("TranslationManager", "Defence area");
const char TR_LAYER_LOCALMARK[]           = QT_TRANSLATE_NOOP("TranslationManager", "Position mark");
const char TR_LAYER_PATROL_RADAR_SITE[]   = QT_TRANSLATE_NOOP("TranslationManager", "Patrol and radar station");
const char TR_LAYER_PATROL[]              = QT_TRANSLATE_NOOP("TranslationManager","Patrol");
const char TR_LAYER_RADAR_STATION[]       = QT_TRANSLATE_NOOP("TranslationManager","Radar Station");
const char TR_LAYER_AIS[]                 = QT_TRANSLATE_NOOP("TranslationManager", "AIS");
const char TR_LAYER_AIS_CURRENT[]         = QT_TRANSLATE_NOOP("TranslationManager", "AIS current position");
const char TR_LAYER_AIS_TRACK[]           = QT_TRANSLATE_NOOP("TranslationManager", "AIS history track");
const char TR_LAYER_AIS_CABLE_TOUCHDOWN[] = QT_TRANSLATE_NOOP("TranslationManager", "AIS cable touchdown");
const char TR_LAYER_RADAR[]               = QT_TRANSLATE_NOOP("TranslationManager", "Radar");
const char TR_LAYER_RADAR_CURRENT[]       = QT_TRANSLATE_NOOP("TranslationManager", "Radar current position");
const char TR_LAYER_RADAR_TRACK[]         = QT_TRANSLATE_NOOP("TranslationManager", "Historical track radar");
const char TR_LAYER_RADAR_AREA[]          = QT_TRANSLATE_NOOP("TranslationManager", "Radar area");
const char TR_LAYER_RADAR_ECHOES[]        = QT_TRANSLATE_NOOP("TranslationManager", "Radar echoes");
const char TR_LAYER_WARNING_TARGET[]      = QT_TRANSLATE_NOOP("TranslationManager", "Warning target");
const char TR_LAYER_ROUTE_LINE[]          = QT_TRANSLATE_NOOP("TranslationManager", "Route line");//路由线
const char TR_LAYER_ROUTE_CROSS[]         = QT_TRANSLATE_NOOP("TranslationManager", "Route cross");//交越路由
const char TR_LAYER_MULTIBEAM[]           = QT_TRANSLATE_NOOP("TranslationManager", "Multibeam");//多波束
const char TR_LAYER_RADARVIDEO[]          = QT_TRANSLATE_NOOP("TranslationManager", "Radarvideo");//雷达回波
const char TR_LAYER_HISTORY_ROUTE_LINE[]  = QT_TRANSLATE_NOOP("TranslationManager", "History Route line");//历史路由线
const char TR_LAYER_CURRENT_ROUTE_LINE[]  = QT_TRANSLATE_NOOP("TranslationManager", "Current Route line");//当前工程路由线
const char TR_LAYER_DANGEROUS_CIRLE[]     = QT_TRANSLATE_NOOP("TranslationManager", "Dangerous cirle");//危险圈
const char TR_LAYER_CAMERA_VIEW[]         = QT_TRANSLATE_NOOP("TranslationManager", "Camera view");//相机视场
const char TR_LAYER_RADAR_FRETURE_AREA[]  = QT_TRANSLATE_NOOP("TranslationManager", "Radar feature_area");//雷达特征区域
const char TR_LAYER_LONLAT_GIRD[]         = QT_TRANSLATE_NOOP("TranslationManager", "Lon and lat gird");//经纬网格
const char TR_LAYER_GPS[]                 = QT_TRANSLATE_NOOP("TranslationManager", "GPS");
const char TR_LAYER_GPS_CURRENT[]         = QT_TRANSLATE_NOOP("TranslationManager", "GPS current position");
const char TR_LAYER_GPS_TRACK[]           = QT_TRANSLATE_NOOP("TranslationManager", "GPS history track");
const char TR_LAYER_CAMERA[]              = QT_TRANSLATE_NOOP("TranslationManager", "Camera");//相机图层
const char TR_LAYER_SHIPPLAN_LINE[]       = QT_TRANSLATE_NOOP("TranslationManager", "Ship plan line");//船舶计划
const char TR_LAYER_CURRENT_SHIPPLAN_LINE[]       = QT_TRANSLATE_NOOP("TranslationManager", "Current Ship plan line");//当前项目船舶计划
const char TR_KILOMETER[]                 = QT_TRANSLATE_NOOP("TranslationManager", "KiloMeter");//单位米;
const char TR_METER[]                     = QT_TRANSLATE_NOOP("TranslationManager", "Meter");//单位米;
const char TR_FOOT[]                      = QT_TRANSLATE_NOOP("TranslationManager", "Foot");//单位英尺;
const char TR_INCH[]                      = QT_TRANSLATE_NOOP("TranslationManager", "Inch");//单位英寸;
const char TR_NAUTICALMILE[]              = QT_TRANSLATE_NOOP("TranslationManager", "NauticalMile");//单位海里;
const char TR_LAYER_HISTORY_AIS[]         = QT_TRANSLATE_NOOP("TranslationManager", "History AIS");         //历史AIS
const char TR_LAYER_HISTORY_RADAR[]       = QT_TRANSLATE_NOOP("TranslationManager", "History Radar");       //历史Radar
const char TR_LAYER_SPECIAL_ROUTELINE[]   = QT_TRANSLATE_NOOP("TranslationManager", "Special Route Line");       //历史Radar
const char TR_LAYER_HIGHLEVEL[]           = QT_TRANSLATE_NOOP("TranslationManager", "High_level");           //施工高层图
const char TR_LAYER_IMAGEMAP[]            = QT_TRANSLATE_NOOP("TranslationManager", "Image_map");            //施工影像图
const char TR_LAYER_COASTDATA[]           = QT_TRANSLATE_NOOP("TranslationManager", "Coast Data");           //海岸数据
const char TR_LAYER_SEABEDPIPELINE[]      = QT_TRANSLATE_NOOP("TranslationManager", "Seabed Pipeline");      //海底管线
const char TR_LAYER_STRUCTURE[]           = QT_TRANSLATE_NOOP("TranslationManager", "Structure");            //结构物
const char TR_LAYER_AREANET[]             = QT_TRANSLATE_NOOP("TranslationManager", "Area Net");             //地理区域网络
const char TR_LAYER_CHANNEL[]             = QT_TRANSLATE_NOOP("TranslationManager", "Channel");              //航道
const char TR_LAYER_MOORING[]             = QT_TRANSLATE_NOOP("TranslationManager", "Mooring");              //锚泊
const char TR_LAYER_DEFINEZONE[]          = QT_TRANSLATE_NOOP("TranslationManager", "DefineZone");           //自定义区域
const char TR_LAYER_CARDMOUTH[]           = QT_TRANSLATE_NOOP("TranslationManager", "CardMouth");            //卡口
const char TR_LAYER_ALARMASCEND[]         = QT_TRANSLATE_NOOP("TranslationManager", "AlarmAscend");          //预警追溯轨迹线
const char TR_LAYER_CAMERANETGRID[]             =  QT_TRANSLATE_NOOP("TranslationManager", "CameraNetGrid");    //相机网格

}
}
