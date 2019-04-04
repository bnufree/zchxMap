#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include "zchx_ecdis_global.h"
#include "zchxmapwidget.h"

namespace Ui {
class MainWindow;
}
namespace qt {

class ZCHX_ECDIS_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *e);
    void closeEvent(QCloseEvent *);
    //void loadEcdis();
    zchxMapWidget * getMapWidget()const {return mMapWidget;}
public slots:
    void slotUpdateCurrentPos(double lon, double lat);
    void slotDisplayMapCenterAndZoom(double lon, double lat, int zoom);
    void slotUpdateDownloadProgress(int val);
private slots:
    void on_load_clicked();

    void on_download_clicked();

    void on_source_activated(const QString &arg1);

    void on_source_currentIndexChanged(int index);

    void on_image_num_clicked(bool checked);

    //从旧的海图移植的接口
private:
    void initSignalConnect(); //初始化信号连接
public:
    //其他工具接口
    double itfzchxUtilToolAngle4north(); //返加当前地图正北方向的方向角
    void   itfzchxUtilToolSetAngle4north(double ang); //设置地图的旋转角度
    bool   itfzchxUtilToolPoint4CurWindow(double lat,double lon, QPointF &p); //返回经纬度点在当前窗口中的坐标点
    bool   itfzchxUtilToolLL4CurPoint(const QPointF &p, double &lat,double &lon);
    uint   itfzchxUtilToolCurZoom(); //返回当前海图的缩放级别
    /*!
         * \brief 设置GIS中心点及地图显示级别(初始化地图显示位置)
         * \param lat 中心点 经度
         * \param lon 中心点 纬度
         * \param zoom 地图缩放级别 -1代表不改变当前缩放级别
         */
    void   setMapCenterAndZoom(double lat, double lon, int zoom = -1);
    /*!
         * \brief 移动地图中心点到 lat 和 lon 指定的位置, 并旋转地图
         * \param ang 旋转角度, 单位弧度
         * \note 不会改变初始点,不改变缩放级别
         */
    void   setMapCenterAndRotate(double lat, double lon, double ang);

    /*!
         * \brief 移动地图中心点到 lat 和 lon 指定的位置, 并旋转地图
         * \param ang 旋转角度, 单位度
         * \note 不会改变初始点,该接口在地图中会记住已经旋转的角度,并且传的参数是角度
         */
    void   setMapCenterAndRotate_new(double lat, double lon, double ang);

    /*!
         * \brief 恢复已经调用了旋转地图中心点 lat 和 lon的地图指向0度
         * \note 不会改变初始点
         */
    void   resetMapRotate(double lat, double lon);

    /*!
         * \brief 移动地图中心点到 lat 和 lon 指定的位置
         * \note 不会改变初始点
         */
    void   setMapCenter(double lat, double lon);

    void   resetMap();//复位地图
    void   zchxSetOriginIslandLines(QList<QPointF> &originIslandLine);      //初始化环岛线设置
    void   zchxSetIslandInterval(int innerInterval,int centralInterval,int outsideInterval);        //设置环岛线外推间隔(单位米)
    void   zchxSetStyleAutoChange(bool val);
    void   zchxSetGISDayBrightTime(const QTime& t);//设置GIS白天的开始时间
    void   zchxSetGISDuskTime(const QTime& t);//设置GIS黄昏的开始时间
    void   zchxSetGISNightTime(const QTime& t);//设置GIS黑夜模式的显示区间(时间)
    void   zchxClearAisData(qint64 cTime, uint sec); //通知海图清理AIS数据
    /**
         * @brief 根据相机的ID设置其所在的电线杆的焦点状态
         * @param cameid 相机的ID
         * @param tags 0x00000001：开(有焦点) 0x00000010: 居中
         */
    void   itfzchxUtilToolCameraRodFocus(uint cameid, int tag);
    //定义海图工具条接口

    void itfToolBarZoomIn();  //Zoom In
    void itfToolBarZoomOut(); //Zoom Out
    void itfToolBarRoam();    //漫游
    void itfToolBarMeasureDistance(); //测距离
    void itfToolBarMeasureArea();     //测面积
    void itfToolBarElecBearLine();    //电子方位线
    void itfToolBarPickup();          //拾取
    void itfToolBarShowAll();         // 显示世界地图（复位）
    void itfToolBarGps();             //跟踪

    //有关海图编辑的工具条接口
    void itfToolBarMapModelChanged(ZCHX::Data::ECDIS_DISPLAY_MODEL mod); //海图模式改变
    void itfToolBarColorModelChanged(ZCHX::Data::ECDISCOLOR mod);//颜色模式改变
    void itfToolBarLocalMark();       //位置标注
    void itfToolBarSelectAnRegion();    //框选
    void itfToolBarLocationMark();      //本地标注
    void itfToolBarFixedReferencePoint();//固定参考点

    void itfToolBarZONESelected(); //选择区域

    void itfToolBarDrawArea();        //画区域(添加防区)
    void itfToolBarWarringZONESelected(); //选择防区
    void itfToolBarWarringZONEmove(); //整体平移防区
    void itfToolBarWarringZONEmoveCtrlPoint();//微调整控制点
    void itfToolBarWarringZONEDelCtrlPoint(); //删除防区控制点
    void itfToolBarWarringZONEAddCtrlPoint(); //添加防区控制点
    void itfToolBarDrawRadarArea();                 //绘制雷达特征区域

    void itfToolBarCoastDataLineAdd(); //添加海岸数据
    void itfToolBarSeabedPipeLineAdd(); //添加海底管线
    void itfToolBarStructurePointAdd(); //添加结构物
    void itfToolBarAreaNetAreaAdd(); //添加地理区域网路

    void itfToolBarChannelAreaAdd(); //添加航道
    void itfToolBarChannelZONESelected(); //选择航道
    void itfToolBarChannelZONEmove(); //整体平移航道
    void itfToolBarChannelZONEmoveCtrlPoint();//微调整控制点
    void itfToolBarChannelZONEDelCtrlPoint(); //删除航道控制点
    void itfToolBarChannelZONEAddCtrlPoint(); //添加航道控制点

    void itfToolBarMooringAreaAdd(); //添加锚泊
    void itfToolBarMooringZONESelected(); //选择锚泊
    void itfToolBarMooringZONEmove(); //整体平移锚泊
    void itfToolBarMooringZONEmoveCtrlPoint();//微调整控制点
    void itfToolBarMooringZONEDelCtrlPoint(); //删除锚泊控制点
    void itfToolBarMooringZONEAddCtrlPoint(); //添加锚泊控制点

    void itfToolBarCardMouthAreaAdd(); //添加卡口
    void itfToolBarCardMouthZONESelected(); //选择卡口
    void itfToolBarCardMouthZONEmove(); //整体平移卡口
    void itfToolBarCardMouthZONEmoveCtrlPoint();//微调整控制点
    void itfToolBarCardMouthZONEDelCtrlPoint(); //删除锚泊控制点
    void itfToolBarCardMouthZONEAddCtrlPoint(); //添加锚泊控制点

    void itfToolBarIslandLineAdd();                 //添加环岛线
    void itfToolBarIslandLineSelected();            //选中当前环岛线
    void itfToolBarIslandLineMove();                //整体平移环岛线
    void itfToolBarIslandLineMoveCtrlPoint();       //微调环岛线控制点
    void itfToolBarIslandLineDelCtrlPoint();        //删除环岛线控制点
    void itfToolBarIslandLineAddCtrlPoint();        //添加环岛线控制点

    void itfToolBarShipPlanLineAdd();               //绘制船舶计划线路
    void itfToolBarShipPlanLineSelected();          //选中当前船舶计划线
    void itfToolBarShipPlanLineInsertCtrlPoint();   //插入船舶计划线控制点
    void itfToolBarShipPlanLineMoveCtrlPoint();     //微调整船舶计划线控制点
    void itfToolBarShipPlanLineDelCtrlPoint();      //删除船舶计划线控制点
    void itfToolBarShipPlanLineAddCtrlPoint();      //添加船舶计划线控制点
    void itfSetShipPlanWaterDepth(QList<ZCHX::Data::ITF_WaterDepth> &WaterData);

    //
    void itfToolBarCameraNetGridAdd(const QSizeF& size, const QString& camera);
    void itfSetCameraNetGridList(const QList<ZCHX::Data::ITF_NetGrid> & list);


    //自定义显示样式
    void itfSetHistoryTrackStyle(const QString &color, const int lineWidth); //历史轨迹
    void itfSetPrepushTrackStyle(const QString &color, const int lineWidth); //预推轨迹
    void itfSetMapUnit(const MapUnit& uint);                //设置单位 1为海里 2为千米  3为米

    //定义海图编辑获取数据接口
    //通过uuid取得海图上当前编辑后的数据
    /*!
         * \brief 通过 uuid   获取防区
         */
    void itfWarringZoneData4id(int uuid, ZCHX::Data::ITF_WarringZone &info, bool &ok);
    /*!
         * \brief 通过名称获取防区
         * \return 获取成功返回 true; 否则返回 false
         */
    //bool itfWarringZoneDataByName(const QString &name, ZCHX::Data::ITF_WarringZone &info);
    /*!
         * \brief 通过名称获取防区
         * \note 效率更高
         * \return 获取成功返回 true; 否则返回 false
         */
    //bool itfWarringZoneDataByName(const std::string &name, ZCHX::Data::ITF_WarringZone &info);
    void itfIslandData4id(int id, ZCHX::Data::ITF_IslandLine &info, bool &ok);

    /*!
         * \brief 设置Element闪烁的时候, Alpha值的步进速度
         * \param step 单次步进, 一般1-10就足够了
         * \see QColor::setAlpha()
         */
    void iftSetElementFlashStep(int step);

    /*!
         * \brief 设置创建的防区是否有报警类型(横琴、msd防区用的同一个防区，msd有报警类型，横琴没有)
         */
    void  iftSetIsWarningType(bool bWarningType);


    //interfaces of map layer
    /*!
         * \brief 添加图层 layer
         * \param layer 要添加的图层
         * \param parent 父图层, 默认为空
         */
    void itfAddLayer(std::shared_ptr<MapLayer> layer, std::shared_ptr<MapLayer> parent = 0);
    void itfAddLayer(const QString& curLayer, const QString& curDisplayName, bool curVisible, const QString &parentLayer = QString());
    /*!
         * \brief 判断地图中是否包含 type 指定的图层
         */
    bool itfContainsLayer(const QString &type) const;
    /*!
         * \brief 获取所有的图层的类型的列表
         */
    QStringList itfGetLayerList() const;
    /*!
         * \brief 获取 type 指定的图层
         */
    std::shared_ptr<MapLayer> itfGetLayer(const QString &type);
    /*!
         * \brief 获取所有的图层并保持树形结构
         * \see MapLayer::getChildLayers()
         */
    const std::list<std::shared_ptr<MapLayer> > &itfGetLayerTree();
    /*!
         * \brief 更新号角设备列表
         */
    void itfUpdateIPCastDeviceList(const QList<ZCHX::Data::IPCastDevice>& list);

    /*!
         * \brief 设置GPS列表
         * 会存储所有设置的GPS列表, 使用每个列表的第一个元素的imei属性作为Key
         * 每设置一次, 更新指定Key的GPS列表
         * 列表的第一个元素作为GPS的当前位置, 后续作为轨迹
         */
    void itfSetGPSDataList(std::list<std::shared_ptr<ZCHX::Data::GPSPoint> > list);
    void itfSetGPSData(std::shared_ptr<ZCHX::Data::GPSPoint> data);
    /*!
         * \brief 清理所有的GPS数据和轨迹
         */
    void itfClearGPSData();


    /*!
         * \brief 设置地图单位
         * \param unit Metric: 米; Foot: 英尺; NauticalMile: 海里; KiloMetric 公里
         */
    void setMapUnit(const MapUnit &unit);

    /*!
          \brief 设置是否显示状态栏，主要用于显示鼠标当前经纬度
          */
    void setIsShowStatusBar(bool bShow);

    /*!
         * \brief 获取图层中当前被选中的元素
         */
    Element* getCurrentSelectedElement();

    /*!
         * \brief 通过 uuid   获取航道
         */
    void itfChannelZoneData4id(int id, ZCHX::Data::tagITF_Channel &info, bool &ok);

    /*!
         * \brief 通过 uuid   获取锚泊
         */
    void itfMooringZoneData4id(int id, ZCHX::Data::tagITF_Mooring &info, bool &ok);

    /*!
         * \brief 通过 uuid   获取卡口
         */
    void itfCardMouthZoneData4id(int id, ZCHX::Data::tagITF_CardMouth &info, bool &ok);

    /*!
         * \brief 获取是否采用ATI显卡的地图引擎初始化方式
         */
    bool getIsAtiAdaptor() const;
    /*!
         * \brief 设置是否采用ATI显卡的地图引擎初始化方式
         * 地图框架默认会自动根据显卡型号判断使用哪种方式
         * 一下情况下可以使用此接口或者配置文件（IsAtiAdapter字段）强制制定显卡：
         * 1. 地图框架识别显卡型号失败（从data目录下的log文件分析是否出现此情况）
         * 2. 测试或者特殊需要下，强制指定
         */
    void setIsAtiAdaptor(bool isAtiAdaptor);

    void setUseRightKey(bool bUseRightKey);//设置是否能使用右键菜单
    //设定当前的项目ID
    void setCurrentProjectID(int id);

signals: //定义发送信号接口
    void itfSignalSendCurPos(double lat, double lon);//实时显示鼠标移动所在的位置信息
    void itfCreateRouteLine(ZCHX::Data::RouteLine);
    void itfCancelAddRouteLine();
    void itfCancelChangeRouteLine();
    void itfCancelChangeShipPlanLine();
    void itfNoRouteLineSelected();
    void itfNoShipPlanLineSelected();
    void itfSignalOpenCameraDisplayWin();
    void itfCreateRadarFeatureZONE(ZCHX::Data::ITF_RadarFeaturesZone);
    void itfSignalIsSelected4TrackAis(ZCHX::Data::ITF_AIS, bool);
    void itfCreateWarringZONE(ZCHX::Data::ITF_WarringZone);
    void itfCreateWarringLine(ZCHX::Data::ITF_WarringZone);
    void itfCreateIslandLine(ZCHX::Data::ITF_IslandLine);
    void itfCreateLocalMark(ZCHX::Data::ITF_LocalMark);
    void itfCreateShipPlanLine(ZCHX::Data::ShipPlanLine);

    void itfCreateCoastDataLine(ZCHX::Data::ITF_CoastData);
    void itfCreateSeabedPipeLineLine(ZCHX::Data::ITF_SeabedPipeLine);
    void itfCreateStructurePoint(ZCHX::Data::ITF_Structure);
    void itfCreateAreaNetZone(ZCHX::Data::ITF_AreaNet);

    void itfCreateChannelZone(ZCHX::Data::ITF_Channel);
    void itfSignalIsSelected4ChannelZone(const ZCHX::Data::ITF_Channel &info);
    void itfSignalUpdateChannelZoneState(int type, int state); // 编辑航道

    void itfCreateMooringZone(ZCHX::Data::ITF_Mooring);
    void itfSignalIsSelected4MooringZone(const ZCHX::Data::ITF_Mooring &info);
    void itfSignalUpdateMooringZoneState(int type, int state); // 编辑锚泊

    void itfCreateCardMouthZone(ZCHX::Data::ITF_CardMouth);
    void itfSignalIsSelected4CardMouthZone(const ZCHX::Data::ITF_CardMouth &info);
    void itfSignalUpdateCardMouthZoneState(int type, int state); // 编辑卡口

    void itfAddFleet(const ZCHX::Data::ITF_AIS&);
    void itfRemoveFleet(const ZCHX::Data::ITF_AIS&);


    /**
         * @brief 发送当前需要跟踪的数据
         * @param trackNumber
         * @param wgs84PosLat
         * @param wgs84PosLon
         */
    void itfSendPointNealyCamera(int trackNumber, double wgs84PosLat ,double wgs84PosLon);

    /**
         * @brief itfIsSelected4Ais
         * @param info
         * @author:zxl
         * 当前被选中的ais数据
         */

    void itfSignalIsSelected4Ais(const ZCHX::Data::ITF_AIS &info, bool autoChange, bool dbClick);
    void itfSignalIsSelected4RadarPoint(const ZCHX::Data::ITF_RadarPoint &info,bool autoChange);
    void itfSignalIsSelected4RadarArea(const ZCHX::Data::ITF_RadarArea &info);
    void itfSignalIsSelected4CameraRod(const ZCHX::Data::ITF_CameraRod &info);
    void itfSignalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info); //传送相机及光电仪还有无人机数据
    void itfSignalIsSelected4WarringZone(const ZCHX::Data::ITF_WarringZone &info); //防区
    void itfSignalIsSelected4WarringLine(const ZCHX::Data::ITF_WarringZone &info); //防线
    void itfSignalIsSelected4PastrolStation(const ZCHX::Data::ITF_PastrolRadarStation &info);
    void itfSignalIsSelected4LocalMark(const ZCHX::Data::ITF_LocalMark &info);
    void itfSignalIsSelected4IslandLine(const ZCHX::Data::ITF_IslandLine &info); //环岛线
    void itfSignalIsSelected4CameraVideoWarn(const ZCHX::Data::ITF_VideoTarget &info); //选中了视频分析目标
    void itfSignalIsEcdisResize();                                                      //海图窗口大小改变
    void itfSignalIsEcdisScales(double);
    void itfSignalIsEcdisDoubleClickCoordinate(double lat, double lon);
    void itfChangeLanguageEcdis();

    void itfSignalIsSelected4RouteLine(const ZCHX::Data::RouteLine &info); //路由线
    void itfSignalDeleteRoutePoint(const ZCHX::Data::RouteLine,int uIndex);//发送删除路由点的索引
    void itfSignalModfiyRoutePoint(const ZCHX::Data::RouteLine,int uIndex);//发送编辑路由点的索引
    void itfSignalChangeRouteLine(const ZCHX::Data::RouteLine);//发送改变整条路由及其明细
    void itfSignalIsEcdisCameraTrackTarget(const ZCHX::Data::ITF_CameraTrackTarget &info);//发送相机跟踪的目标信息

    void itfSignalIsSelected4ShipPlanLine(const ZCHX::Data::ShipPlanLine &info); //船舶计划
    void itfSignalDeleteShipPlanPoint(const ZCHX::Data::ShipPlanLine,int uIndex);//发送删除船舶计划点的索引
    void itfSignalModfiyShipPlanPoint(const ZCHX::Data::ShipPlanLine,int uIndex);//发送编辑船舶计划点的索引
    void itfSignalChangeShipPlanLine(const ZCHX::Data::ShipPlanLine);            //发送改变整条船舶计划及其明细

    void itfSignalIsSelectedSpecialRoutePoint(const ZCHX::Data::SpecialRoutePoint &data);   //拾取的特殊海缆点
    //目标双击
    void signalIsDoubleClicked4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机双击
    void signalIsDoubleClicked4Ais(const ZCHX::Data::ITF_AIS &info/*, bool autoChange*/);//Ais双击
    void signalIsDoubleClicked4RadarPoint(const ZCHX::Data::ITF_RadarPoint &info/*, bool autoChange*/); //雷达目标双击
    void signalIsDoubleClicked4RadarArea(const ZCHX::Data::ITF_RadarArea &info); //雷达区域双击
    void signalIsDoubleClicked4CameraRod(const ZCHX::Data::ITF_CameraRod &info);//相机杆双击
    void signalIsDoubleClicked4WarringLine(const ZCHX::Data::ITF_WarringZone); //警戒线双击
    void signalIsDoubleClicked4WarringZone(const ZCHX::Data::ITF_WarringZone &info);//防区双击
    void signalIsDoubleClicked4PastrolStation(const ZCHX::Data::ITF_PastrolRadarStation &info);//巡逻站双击
    void signalIsDoubleClicked4LocalMark(const ZCHX::Data::ITF_LocalMark &info);    //关注点双击
    void signalIsDoubleClicked4IslandLine(const ZCHX::Data::ITF_IslandLine &info); //环岛线双击
    void signalIsDoubleClicked4CameraVideoWarn(const ZCHX::Data::ITF_VideoTarget &info); //双击了视频分析目标
    void signalIsDoubleClicked4ChannelZone(const ZCHX::Data::ITF_WarringZone &info);//航道双击
    void signalIsDoubleClicked4MooringZone(const ZCHX::Data::ITF_WarringZone &info);//锚泊双击
    void signalIsDoubleClicked4CardMouthZone(const ZCHX::Data::ITF_WarringZone &info);//卡口双击

    //鼠标右键菜单信号
    void itfSignalScreenShot();
    void itfSignalSendSeletedAISTarget(const QList<ZCHX::Data::ITF_AIS> &dataList);
    void itfSignalSendPictureInPictureTarget(const int TargetType, const QString &TargetID);
    void itfSignalCreateShipSimutionLine(const std::vector<std::pair<double,double> > &data);           //船舶模拟
    void itfSignalSendLocalMarkPos(const double Lat,const double Lon);
    void itfSignalSendReferencePos(const double Lat,const double Lon);
    void itfSignalShowLocationMarkOrReferencePos(bool result);
    void itfSignalInvokeHotSpot(const ZCHX::Data::ITF_CloudHotSpot & data);
    void itfSignalAddShipExtrapolation(const QString &data, bool result);
    void itfSignalSendHistoryTrail(const QString &data, bool result);
    void itfSignalSendRealTimeTrail(const QString &data, bool result);
    void itfSignalCreateFlowLine(const ZCHX::Data::CustomFlowLine &data);
    void itfSignalCreateBlackOrWhiteList(const QString ShipId, const int markType);
    void itfSignalCreateCPATrack(const QString &ShipId);
    void itfSignalUpdateWarringZoneOrLineState(const int type,const int state);
    void itfSignalMapIsRoaming();
    void itfSignalSendPTZLocation(double lat, double lon);
    //图层
    /*!
         * \brief 图层中的元素的选中状态有变化
         */
    void sigLayerElementSelectionChanged(qt::Element* ele);
    void sigLayerElementHoverChanged(qt::Element* ele);

    //相机网格
    void itfSignalSendCameraNetGrid(const ZCHX::Data::ITF_NetGrid& data);

    void itfSignalLoading();//发送开始加重地图
    void itfSignalLoadOver();//发送地图加载完毕

    /*!
         * \brief 地图引擎加载及初始化完成
         */
    void sigMapEngineInitialized();
public slots: //定义Recive数据接口

    void itfSetRadarEchoData(const QMap<QDateTime,ZCHX::Data::ITF_RadarEchoMap> &data); //设置雷达回波数据
    void itfSetAisData(const QList<ZCHX::Data::ITF_AIS> &data); //设置AIS轨迹数据
    bool itfSetSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data);//设置历史轨迹
    void itfRemoveAisHistoryData(QString id);//删除历史轨迹
    void itfSetHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data); //设置历史AIS数据
    void itfSetClearHistoryData(bool states);               //true-清理历史ais/radatr  false-清理历史轨迹
    void itfSetConsAisData(const ZCHX::Data::ITF_AIS &data); //设置施工船舶
    void itfSetRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint>/*const std::vector<RadarPoint>*/ &data);//设置雷达点数据
    void itfSetHistoryRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint> &data); //设置雷达历史数据
    void itfSetRadarAreaData(const QList<ZCHX::Data::ITF_RadarArea> &data); //设置雷达扫描区域数据
    void itfSetCameraRodData(const QList<ZCHX::Data::ITF_CameraRod> &data); //设置摄像杆接口数据
    void itfSetCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data, ZCHX::Data::ITF_CameraDev::ITF_CAMERA_PARENT parent = ZCHX::Data::ITF_CameraDev::PARENT_NONE); //设置相机设备数据接口
    void itfSetAisCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data);  //设置船载相机的数据接口
    void itfSetPastrolStation(const QList<ZCHX::Data::ITF_PastrolRadarStation> &data); //设置巡逻站数据
    void itfSetRadarStation(const QList<ZCHX::Data::ITF_PastrolRadarStation> &data); //设置雷达站数据

    void itfSetWarringZoneData(const QList<ZCHX::Data::ITF_WarringZone> &data); //设置告警区域数据
    void itfSetShipSiumtionData(const std::vector<std::pair<double, double>> &data);    //事件模拟船舶航线
    void itfSetAISDataToSiumtion(const QList<ZCHX::Data::ITF_AIS> &data);           //时间模拟船舶数据

    void itfSetCoastData(const QList<ZCHX::Data::ITF_CoastData> &data);  //设置海岸数据
    void itfImportCoastData(const std::vector<std::pair<double, double> > &data); // 导入海岸数据

    void itfSetSeabedPipeLine(const QList<ZCHX::Data::ITF_SeabedPipeLine> &data);  //设置海底管线
    void itfImportSeabedPipeLine(const std::vector<std::pair<double, double> > &data); // 导入海底管线

    void itfSetStructure(const QList<ZCHX::Data::ITF_Structure> &data);  //设置结构物
    void itfImportStructure(const std::vector<std::pair<double, double> > &data); // 导入结构物

    void itfSetAreaNet(const QList<ZCHX::Data::ITF_AreaNet> &data);  //设置地理区域网络
    void itfImportAreaNet(const std::vector<std::pair<double, double> > &data); // 导入地理区域网络

    void itfSetChannel(const QList<ZCHX::Data::ITF_Channel> &data);  //设置航道
    void itfImportChannel(const std::vector<std::pair<double, double> > &data); // 导入航道
    void itfSelectChannelLine(const QString& name, const ZCHX::Data::ITF_ChannelLine & line); // 选择航道线
    void itfCancelChannelLine(const QString& name); // 取消航道线

    void itfSetMooring(const QList<ZCHX::Data::ITF_Mooring> &data);  //设置锚泊
    void itfImportMooring(const std::vector<std::pair<double, double> > &data); // 导入锚泊

    void itfSetCardMouth(const QList<ZCHX::Data::ITF_CardMouth> &data);  //设置卡口
    void itfImportCardMouth(const std::vector<std::pair<double, double> > &data); // 导入卡口

    void itfSetFleet(const QList<ZCHX::Data::ITF_Fleet> &data);  //设置船队

    // 设置预警追溯
    void itfSetShipAlarmAscendMap(const QMap<QString, ZCHX::Data::ITF_ShipAlarmAscend> &shipAlarmAscendMap);

    /*!
         * \brief 更新防区和防线数据, 不会删除现有防区
         * \note 防区和防线通用
         */
    void itfUpdateWarringZone(const QList<ZCHX::Data::ITF_WarringZone> &data);
    void itfUpdateWarringZone(const ZCHX::Data::ITF_WarringZone &zone);
    /*!
         * \brief 根据名称或者id(先匹配名称)删除防区或者防线
         * \note 防区和防线通用
         */
    void removeWarrningZone(const ZCHX::Data::ITF_WarringZone &zone);
    /*!
         * \brief 获取所有的防区
         * \note 防区和防线通用
         */
    QList<ZCHX::Data::ITF_WarringZone> getAllWarrningZone() const;

    void itfSetIslandLineData(const QList<ZCHX::Data::ITF_IslandLine> &data);    //设置环岛线数据
    void itfSetLocalMarkData(const QList<ZCHX::Data::ITF_LocalMark> & data);   //设置位置标注接口
    void itfSetCameraGdyData(const QList<ZCHX::Data::ITF_CameraDev> &data);          //设置光电仪数据接口
    void itfSetCameraPlanData(const QList<ZCHX::Data::ITF_CameraDev> &data);          //设置无人机数据接口
    void itfSetCameraVideoWarnData(const QList<ZCHX::Data::ITF_VideoTarget> &data);//设置人车船告警数据接口
    void itfSetDangerousCircleData(const QList<ZCHX::Data::ITF_DangerousCircle> &data); //设置危险圈数据
    void itfSetDangerousCircleRange(const double range);                                //设置危险圈数据
    void itfSetRadarFeatureZoneDagta(const QList<ZCHX::Data::ITF_RadarFeaturesZone> &data);
    void itfSetCameraObservationZoneData(const QList<ZCHX::Data::ITF_CameraView> &data);

    //设置回波数据，uType==1回波显示，uType==2余辉显示
    void itfSetRadarVideoData(double dCentreLon,double dCentreLat,double dDistance,int uType,int uLoopNum);
    void itfSetRadarVideoPixmap(int uIndex,const QPixmap &objPixmap,const QPixmap &prePixmap);
    void itfSetCurrentRadarVideoPixmap(const QPixmap &objPixmap);//只设置当前的图片
    //    void itfSetMultibeamData(const QList<ZCHX::Data::ITF_Multibeam> &data,
    //                             const double dMinLon,const double dMinLat,const double dMaxLon,const double dMaxLat,
    //                             const double dMinX,const double dMinY,const double dMinZ,const double dMaxX,const double dMaxY,const double dMaxZ);
    void itfSetMultibeamDataByImg(int iRouteID, const QPixmap &samplePixmap, const QPixmap &objPixmap, double dMinLon, double dMinLat, double dMaxLon, double dMaxLat);

    void itfSetAllProjectRouteLineData(const QList<ZCHX::Data::RouteLine> &data);       //设置所有项目的路由线数据接口
    void itfSetRouteCrossData(const QList<ZCHX::Data::ITF_RouteCross> &data);           //设置交越路由数据
    void itfSetRouteLineData(const QList<ZCHX::Data::RouteLine> &data);                 //设置路由线数据接口(当前项目)

    void itfSetALLSpecialRouteLineData(const QList<ZCHX::Data::SpecialRouteLine> &data);
    void itfSetSpacilRouteLineData(const QList<ZCHX::Data::SpecialRouteLine> &data);
    void itfSetPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data);

    void itfSetHistoryRouteLineData(const QList<ZCHX::Data::RouteLine> &data);          //设置历史路由线数据接口(一条路由每个阶段的历史路由)
    void itfSetRouteDefaultCableData(const ZCHX::Data::CableAssembly &data,int uSlack);
    void itfSetRouteConfigDisplay(int uRouteID, bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable, bool bShowIndex);//设置该条路由的点、类型、线、海缆是否显示
    void itfSetRouteDisplayFlag(int uRouteID,int uDisplayFlag);//设置路由显示设计路由还是记录路由或者同时显示
    void itfCloseCurRouteLineData();                                                    //关闭当前项目的路由数据
    void itfDeleteRouteLineDataFromProjectID(const int uProjectID);                     //删除项目ID为uProjectID的路由数据
    void itfSetShipPlanLineData(const QList<ZCHX::Data::ShipPlanLine> &data);           //设置船舶计划数据
    void itfSetAllProjectShipPlanLineData(const QList<ZCHX::Data::ShipPlanLine> &data); //设置所有项目的船舶计划
    void itfSetShipPlanProjectID(const int ProjectID);                                  //绘制一条只有的船舶计划航线 所需数据
    void itfSetRouteLinePickupLocation(const int uRouteID,const double dLat,const double dLon);//设置路由显示被拾取的点
    void itfSetRouteLinePickupInfor(const double dKp, bool bShowLength, const double dLength,
                                    bool bShowSlope, const double dSlope);//设置路由显示被拾取点的信息
    void itfChangeCurrentRouteLine(const ZCHX::Data::RouteLine &data,int uRouteID); //改变当前路由线数据接口
    void itfChangeRoutePathAssessStatus(int uAssessStatus,int uRouteID);            //改变路由的审批状态;0.未审批 1.已审批
    void itfSetSimulateData(int uSimulateDataID,int uSimulateFlag);//设置模拟数据的ID和模拟的标志(1-rpl表模拟，2-船舶计划表模拟)
    void itfSetSimulateLocation(float fCurSimulateKP);//设置模拟船的位置
    void itfAppendElementItem(const ZCHX::Data::ITF_EleEllipse &item);  //设置图元数据
    void itfAppendElementItem(const ZCHX::Data::ITF_EleTriangle &item); //设置图元数据
    void itfAppendElementItem(const ZCHX::Data::ITF_EleLine &item);     //设置图元数据
    void itfAppendElementItem(const ZCHX::Data::ITF_EleRect &item);     //设置图元数据
    void itfRemoveElementItem(Element *pitem);             //删除图元
    void itfClearElementData(); //清除图元指针数据
    void itfSetPickUpRadarInfo(qint32 tracknumber);
    void itfSetPickUpAisInfo(QString id);
    void itfSetPickUpPosition(QString id);
    void itfSetPickUpRouteCross(const int id);//设置当前选中的交越点
    //void itfSetShipSimulationExtrapolationDatas(const QList<ZCHX::Data::ITF_AIS> &DataList);
    void itfUpdateShipSimulationExtrapolationData(const QString& id, int time);
    void itfAppendShipSimulationExtrapolationData(const QString& id, int time);
    void itfDeleteShipSimulationExtrapolationData(const QString& id);
    ZCHX::Data::ExtrapolateList itfGetShipSimulationExtrapolationData();
    void itfSetRealTimeShipTrailDatas(const QMap<QString,QList<ZCHX::Data::ITF_AIS>> &DataList);
    void itfSetRouteHistogramData(const int RouteId, const QMap<int ,QList<int>> &DataList);
    void itfSetRealTimeFlowLineHistogramData(const QMap<int ,QList<int>> &DataList);
    void itfSetInTheAreaOfTrackData(const QList<QList<ZCHX::Data::LatLon>> &data);    //缓存数据实时绘制区域轨迹
    void itfSetRolePrivilegeList(const QList<int> &list);
    //定义flag
    void itfSetIsHideMap(bool b);                       //是否隐藏海图
    void itfSetNavigateionging(bool b);                 //是否正在导航
    void itfSetIsOpenMeet(bool b);                      //是否启动会遇
    void itfSetRadarTrackPoint(bool b);                 //是否启动跟踪雷达
    void itfSetEnableCameraTargerTrack(bool b);         //是否启动相机联动跟踪
    void itfSetEnableRouteHistogram(bool b);            //是否启动路由统计直方图
    void itfSetEnableShipTag(int tag); //船舶标签信息显示
    //定义拾取类型接口
    /**
         * @brief itfSetPuickupType
         * @param type
         * @author:zxl
         * 设置当前可使用的拾取类型
         */
    void itfSetPuickupType(const ZCHX::Data::ECDIS_PICKUP_TYPEs &type);

    /**
         * @brief 定义更新相机状态接口
         * @param cameid
         * @param st
         */
    void itfUpdateCameraDevStatus(uint cameid, uint st);

    /**
         * @brief 设置海图工具条是否可见
         * @param b
         */
    void itfSetEnableTooBarVisable(bool visible);

    /**
         * @brief 设置海图插件当前使用的模式(编辑/显示)
         */
    void itfsetPluginUseModel(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL & mod);
    /**
         * @brief 拾取一个ship ,进行跟踪导航
         *
         */
    void itfsetSelectedShip();
    /**
         * @brief 拾取一个任意目标, 进行相机跟踪
         */
    void itfsetSelectedAllTrack();
    /**
         * @brief 拾取路由点或者交越点 ,
         */
    void itfsetSelectedRouteOrCross();
    //设置海缆基础数据
    void itfSetCableBaseDataList(const QList<ZCHX::Data::CableBaseData>& list);
    //设置海缆接头数据
    void itfSetCableInterfaceDataList(const QList<ZCHX::Data::CableInterfaceData>& list);
    //设置是否显示路由编号
    void itfSetDisplayRouteIndex(bool display);
    void itfSetDisplayRouteAc(bool display);
    //设置雷达和AIS关注
    void itfAppendFocusRadar(const QStringList& list);
    void itfRemoveFocusRadar(const QStringList& list);
    void itfAppendFocusAis(const QStringList& list);
    void itfRemoveFocusAis(const QStringList& list);
    //设置雷达和AIS的历史尾迹显示
    void itfAppendRadarTailTrackList(const QStringList& list);
    void itfRemoveRadarTailTrackList(const QStringList& list);
    void itfAppendAisTailTrackList(const QStringList& list);
    void itfRemoveAisTailTrackList(const QStringList& list);
    //PTZ获取
    void itfPickUpPTZ();

    void itfSetZoomLableDisplay(bool display);
protected Q_SLOTS:
    void OnMeasureAreaAction();
    void OnDirAngleAction();
    void OnDistantcAction();
    void OnExitEToolAction();
    void OnPickupAction();
    void OnShowAllAction();
    void OnLocalMarkAction();
    void OnGpsAction();

    void OnSelBaseDisplayStyle();           //海图显示模式--基本
    void OnSelStandardDisplayStyle();       //海图显示模式--标准
    void OnSelAllDisplayStyle();            //海图显示模式--全部

    void OnSelDayBlackBackDisplyStyle();    //海图颜色模式--阴天
    void OnSelNightDisplyStyle();           //海图颜色模式--夜晚
    void OnSelDayDUSKDisplyStyle();         //海图颜色模式--黄昏
    void OnSelDayWhiteBackDisplyStyle();    //海图颜色模式--白昼
    void OnSelDayBrightDisplyStyle();       //海图颜色模式--晴天

    void OnZONESelected(); //选中当前区域

    //有关防区操作
    void OnWarringZONEAction(); //添加防区
    void OnWarringZONESelected(); //选中当前防区
    void OnWarringZONEmove(); //整体平移防区
    void OnWarringZONEmoveCtrlPoint();//微调整控制点
    void OnWarringZONEDelCtrlPoint(); //删除防区控制点
    void OnWarringZONEAddCtrlPoint(); //添加防区控制点

    void OnCoastDataLineAdd(); //添加海岸数据
    void OnSeabedPipeLineLineAdd(); //添加海底管线
    void OnStructurePointAdd(); //添加结构物
    void OnAreaNetZoneAdd(); //添加地理区域网络

    void OnChannelAreaAdd(); //添加航道
    void OnChannelZONESelected(); //选中当前航道
    void OnChannelZONEMove(); //整体平移航道
    void OnChannelZONEMoveCtrlPoint();//微调整航道控制点
    void OnChannelZONEDelCtrlPoint(); //删除航道控制点
    void OnChannelZONEAddCtrlPoint(); //添加航道控制点

    void OnMooringAreaAdd(); //添加锚泊
    void OnMooringZONESelected(); //选中当前锚泊
    void OnMooringZONEMove(); //整体平移锚泊
    void OnMooringZONEMoveCtrlPoint();//微调整锚泊控制点
    void OnMooringZONEDelCtrlPoint(); //删除锚泊控制点
    void OnMooringZONEAddCtrlPoint(); //添加锚泊控制点

    void OnCardMouthAreaAdd(); //添加卡口
    void OnCardMouthZONESelected(); //选中当前卡口
    void OnCardMouthZONEMove(); //整体平移卡口
    void OnCardMouthZONEMoveCtrlPoint();//微调整锚泊控制点
    void OnCardMouthZONEDelCtrlPoint(); //删除锚泊控制点
    void OnCardMouthZONEAddCtrlPoint(); //添加锚泊控制点

    //有关环岛线操作
    void OnIslandLineAdd(); //添加环岛线
    void OnIslandLineSelected(); //选中当前环岛线
    void OnIslandLineMove(); //整体平移环岛线
    void OnIslandLineMoveCtrlPoint();//微调环岛线控制点
    void OnIslandLineDelCtrlPoint(); //删除环岛线控制点
    void OnIslandLineAddCtrlPoint(); //添加环岛线控制点
    /*!
         * \brief 设置地图的旋转角度 angle(弧度)
         */
    void OnSetMapAngle(double angle);

private:
    Ui::MainWindow *ui;
    zchxMapWidget *mMapWidget;
    //bool          mStartLoad;
};
}

#endif // MAINWINDOW_H
