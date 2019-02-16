#ifndef ZCHXMAPWIDGET_H
#define ZCHXMAPWIDGET_H

#include <QtWidgets>
#include <QtSvg>
#include <QGeoCoordinate>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QPixmap>
#include "zchxMapDataUtils.h"
#include "zchxutils.hpp"
#include "localmarkdlg.h"
#include "element/drawelement.hpp"

typedef     std::vector<std::pair<double, double> >     POLYLINE;
typedef     QList<POLYLINE>                             POLYLINES;

class ZCHXDrawMultibeam;
class ZCHXDrawRadarVideo;
class QDomElement;
class zchxMapFrameWork;
class zchxMapLoadThread;
class QScaleSlider;
class zchxMapLayerMgr;
class zchxCameraDatasMgr;
class zchxMapLayerMgr;
class zchxAisDataMgr;
class zchxRadarDataMgr;
class zchxUserDefinesDataMgr;
class zchxRouteDataMgr;
class zchxShipPlanDataMgr;
class zchxEcdisDataMgr;

class zchxMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit zchxMapWidget(QWidget *parent = 0);
    ~zchxMapWidget();
    //地图显示
    void setCurZoom(int zoom);
    int  zoom() const;
    void setCenterLL(const LatLon& pnt );
    void setCenterAndZoom(const LatLon &ll, int zoom);
    void setCenterAtTargetLL(double lat, double lon);
    LatLon centerLatLon() const;
    void setSource(int source);
    void setDisplayImgeNum(bool sts) {mDisplayImageNum = sts; update();}
    zchxMapFrameWork* framework() const {return mFrameWork;}
    void setScaleControl(QScaleSlider * pScale);
    void setMapStyle(MapStyle mapStyle);

    //地图模式切换接口
    void   setStyleAutoChange(bool val);
    void   setGISDayBrightTime(const QTime& t);//设置GIS白天的开始时间
    void   setGISDuskTime(const QTime& t);//设置GIS黄昏的开始时间
    void   setGISNightTime(const QTime& t);//设置GIS黑夜模式的显示区间(时间)

    //设置报警图标的透明度
    void   setWarnColorAlphaStep(int val);
    int    getWarnColorAlphaStep();

    //数据控制
    zchxMapLayerMgr* getMapLayerMgr();
    zchxCameraDatasMgr* getCameraMgr();
    zchxAisDataMgr* getAisDataMgr();
    zchxUserDefinesDataMgr* getUserDefinesDataMgr();
    zchxRadarDataMgr*       getRadarDataMgr();
    zchxRouteDataMgr*   getRouteDataMgr();
    zchxShipPlanDataMgr*    getShipPlanDataMgr();

    //地图旋转
    double zchxUtilToolAngle4north(); //返回当前正北方向角
    void   zchxUtilToolSetAngle4north(double ang); //设置当前地图的旋转角度
    void   setMapRotateAtLL(double lat, double lon, double ang);//设置地图旋转角度
    void   resetMapRotate(double lat, double lon);//恢复地图旋转
    void   reset();
    //经纬度和屏幕坐标的转换
    bool   zchxUtilToolPoint4CurWindow(double lat, double lon,QPointF &p);//返回经纬度所对应当前窗口的坐标
    bool   zchxUtilToolLL4CurPoint(const QPointF &p, double &lat,double &lon); //返回当前窗口上的某个点对应于海图上的经续度
    void   zchxUtilToolGetDis4Point(QPointF star, QPointF end, double &latoff, double &lonoff); //取得两个像素点的坐标移动距离,转化成的经纬度变化范围
    //海图缩放级别显示
    uint   zchxUtilToolCurZoom();          //返回当前海图的缩放级别
    void   zchxSetMapMinScale(int minVisibleZoom); //设置地图的最小显示级别
    void   zchxSetMapMaxScale(int maxVisibleZoom); //设置地图的最小显示级别
    //当前海图的使用模式(编辑或者纯显示)
    ZCHX::Data::ECDIS_PLUGIN_USE_MODEL curUserModel() const;
    void setCurPluginUserModel(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL &curUserModel);
    //设置当前的拾取元素的类型(默认拾取全部,可以定制只拾取某种类型的数据)
    ZCHX::Data::ECDIS_PICKUP_TYPE getCurPickupType() const;
    void setCurPickupType(const ZCHX::Data::ECDIS_PICKUP_TYPE &curPickupType);
    // 获取图层中当前被选中的元素
    std::shared_ptr<DrawElement::Element> getCurrentSelectedElement();    
    void setCurrentSelectedItem(std::shared_ptr<DrawElement::Element> item);


    //海图显示隐藏控制
    bool getIsHideMap() const {return mIsMapHidden;}
    void setIsHideMap(bool value) {mIsMapHidden = value;}//是否隐藏海图（sccmms项目）

    //海图导航模式
    void setIsNavigation(bool value) {mIsNavigation = value;}//是否正在导航（msd项目）
    bool getIsNavigation() const {return mIsNavigation;}

    //会遇显示
    void setIsOpenMeet(bool value) {mIsOpenMeet = value;}     //是否启动会遇(msd项目)
    bool getIsOpenMeet() const {return mIsOpenMeet;}

    //雷达目标跟踪
    void setEnableRadarTrackPoint(bool b) {mIsRadarTagetTrack = b;}//启动雷达跟踪的标志（msd项目）
    bool getEnableCameraTargerTrack() const {return mIsRadarTagetTrack;}

    //相机联动跟踪
    void setEnableCameracombinedTrack(bool b) {mIsCameraCombinedTrack = b;}//是否启动相机联动跟踪(Msd)
    bool getEnableCameracombinedTrack() const {return mIsCameraCombinedTrack;}

    //相机显示.显示在杆上还是直接显示
    void setEnableDisplayCameraWithoutRod(bool b) {mIsCameraDisplayWithoutRod = b;} //是否直接显示相机
    bool getEnableDisplayCameraWithoutRod() const {return mIsCameraDisplayWithoutRod;}

    //路由直方图
    void setEnableRouteHistogram(bool b) {mRouteHistogram = b;}
    bool getEnableRouteHistogram() const {return mRouteHistogram;}




private:
    void updateCurrentPos(const QPoint& p);
    void autoChangeCurrentStyle();
    void zchxShowCameraInfo(QPainter *painter);

protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent * e) override;
    void mouseDoubleClickEvent(QMouseEvent * e) override;
    void mouseMoveEvent(QMouseEvent * e) override;
    void mouseReleaseEvent(QMouseEvent * e) override;
    void wheelEvent(QWheelEvent * e) override;
//    void keyPressEvent(QKeyEvent * e) override;
//    void keyReleaseEvent(QKeyEvent * e) override;
//    bool event(QEvent *e);
    void resizeEvent(QResizeEvent *);

signals:
    void signalDisplayCurPos(double lon, double lat);
    void signalSendNewMap(double, double, int);

public slots:
    void append(const QPixmap& img, int x, int y);
    void append(const TileImageList& list);
    void clear() {mDataList.clear(); /*update();*/}
    void slotRecvNewMap(double lon, double lat, int zoom);

    //旧地图的接口
public:
    /*!
        * \brief 设置GPS列表
        * 会存储所有设置的GPS列表, 使用每个列表的第一个元素的imei属性作为Key
        * 每设置一次, 更新指定Key的GPS列表
        * 列表的第一个元素作为GPS的当前位置, 后续作为轨迹
        */
    void setGPSDataList(std::list<std::shared_ptr<ZCHX::Data::GPSPoint> > list);
    void setGPSData(std::shared_ptr<ZCHX::Data::GPSPoint> data);
    /*!
        * \brief 清理所有的GPS数据和轨迹
        */
    void clearGPSData();

    /*!
        * \brief 设置创建的防区是否有报警类型
        */
    //void  setIsWarningType(bool bWarningType);

    /*!
        * \brief 设置地图单位
        * \param unit Metric: 米; Foot: 英尺; NauticalMile: 海里
        */
    void setMapUnit(const MapUnit &unit) {mUnit = unit;}

    /*!
        * \brief 获取地图单位
        */
    MapUnit getMapUnit() const {return mUnit;}
    /*!
        * \brief 转换到指定单位的长度
        * \param length 长度(米)
        */
    //qreal convertLengthToUnit(qreal length, const QString &unit) const;
    /*!
        * \brief 转换到指定单位的面积
        * \param area 面积(平方米)
        */
    //qreal convertAreaToUnit(qreal area, const QString &unit) const;
    /*!
        * \brief 获取翻译后的单位
        */
    //QString getTrUnit() const;

    void setUseRightKey(bool bUseRightKey);

    //设置海缆基础数据
    void setCableBaseDataList(const QList<ZCHX::Data::CableBaseData>& list){mBaseCableDataList = list;}
    //ZCHX::Data::CableBaseData getCableByName(const QString& name);
    void setCableInterfaceDataList(const QList<ZCHX::Data::CableInterfaceData>& list){mInterfaceCableDataList = list;}
    //ZCHX::Data::CableInterfaceData getInterfaceByName(const QString& name);

public Q_SLOTS:
    //void ScalePlus();
    //void ScaleMinus();
    //void ScalePlusLight();
    //void ScaleMinusLight();

//    void ShowAll();
//    void ScaleChanged(int action);
//    void SliderPressed();
//    void SliderReleased();

//    void SetPickUpHistoryRadarInfo(qint32 tracknumber);
//    void SetPickUpHistoryAisInfo(QString id);
//    void SetPickUpRadarInfo(qint32 tracknumber);
//    void SetPickUpAisInfo(QString id);
//    void SetPickUpPosition(const QString &id);
//    void SetPickUpRouteCross(const int id);//设置当前选中的交越点

//    void setETool2DrawNull();
//    void setETool2DrawArea();
//    void setETool2DrawDirAngle();
//    void setETool2DrawDistance();
    void setETool2DrawPickup();
//    void setETool2DrawTrackTarget();
//    void setETool2DrawCameraTrackTarget();
//    void setETool2DrawGps();
//    void setETool2DrawRouteOrCross();//拾取路由或者交越点
//    //雷达特征区域
//    void setETool2DrawRadarZONE();
//    //以下两项操作属于海图编辑
//    void setETool2SelectCommonZONE();
//    //防区操作
//    void setETool2DrawZONE();
//    void setETool2SelectZONE();                 //选取当前防区
//    void setETool2moveZONE();                   //移动选中的防区
//    void setETool2ctrlZONE();                   //微调控制点
//    void setETool2addCtrlZONE();                //添加控制点
//    void setETool2delCtrlZONE();                //删除控制点

//    //海岸数据
//    void setETool2Draw4CoastDataLine();

//    //海底管线
//    void setETool2Draw4SeabedPipeLineLine();

//    //结构物
//    void setETool2Draw4StructurePoint();

//    //地理区域网络
//    void setETool2Draw4AreaNetZone();

//    //航道
//    void setETool2Draw4ChannelArea();
//    void setETool2SelectChannel();                 //选取当前航道
//    void setETool2moveChannel();                   //移动选中的航道
//    void setETool2ctrlChannel();                   //微调控制点
//    void setETool2addCtrlChannel();                //添加控制点
//    void setETool2delCtrlChannel();                //删除控制点

//    //锚泊
//    void setETool2Draw4MooringArea();
//    void setETool2SelectMooring();                 //选取当前锚泊
//    void setETool2moveMooring();                   //移动选中的锚泊
//    void setETool2ctrlMooring();                   //微调控制点
//    void setETool2addCtrlMooring();                //添加控制点
//    void setETool2delCtrlMooring();                //删除控制点

//    //卡口
//    void setETool2Draw4CardMouthArea();
//    void setETool2SelectCardMouth();                 //选取当前卡口
//    void setETool2moveCardMouth();                   //移动选中的卡口
//    void setETool2ctrlCardMouth();                   //微调控制点
//    void setETool2addCtrlCardMouth();                //添加控制点
//    void setETool2delCtrlCardMouth();                //删除控制点

//    //船舶计划航线
//    void setETool2DrawShipPlanLine();
//    void setETool2SelectShipPlanLine();         //选取当前船舶计划线
//    void setETool2insertCtrlPointShipPlanLine();//插入船舶计划控制点
//    void setETool2moveCtrlPointShipPlanLine();  //微调船舶计划控制点
//    void setETool2addCtrlPointShipPlanLine();   //添加船舶计划控制点
//    void setETool2delCtrlPointShipPlanLine();   //删除船舶计划控制点
//    void setShipPlanWaterDepth(QList<ZCHX::Data::ITF_WaterDepth> &WaterData);

//    //设置显示样式
//    void setHistoryTrackStyle(const QString &color, const int lineWidth);
//    void setPrepushTrackStyle(const QString &color, const int lineWidth);
//    void SetEnableShipTag(const QList<bool> &bList);
//    //环岛线操作
//    void setETool2Draw4IslandLine();
//    void setETool2Select4IslandLine();          //选取当前环岛线
//    void setETool2move4IslandLine();            //移动选中的环岛线
//    void setETool2moveCtrlPoint4IslandLine();   //微调环岛线控制点
//    void setETool2addCtrlPoint4IslandLine();    //添加环岛线控制点
//    void setETool2delCtrlPoint4IslandLine();    //删除环岛线控制点

//    //位置标注 Add by yej
//    void setETool2DrawLocalMark();

    //set data
    //    void setAisData(const std::vector<DrawElement::AisElement> &data);//设置<ais>轨迹数据
    //    void setAisDataFromCopy(const std::vector<DrawElement::AisElement> &data); //通过数据copy设置<AIS>数据
//    bool setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data);          //设置船<ais>轨迹数
//    void removeAisHistoryData(QString id);
//    void setAisData(const QList<ZCHX::Data::ITF_AIS> &data);                //设置<ais>轨迹数据
//    void setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data);         //设置历史回放<ais>轨迹数据
//    void setConsAisData(const ZCHX::Data::ITF_AIS &aisdata);
//    void setClearHistoryData(bool states);
//    DrawElement::AisElement *getCurrentAis();
//    DrawElement::AisElement *getHistoryCurrentAis();
//    void updateAisElement(const QList<ZCHX::Data::ITF_AIS> &data);
//    void setRadarPointData(const std::vector<DrawElement::RadarPointElement> &data);//设置雷达点数据
//    bool getPreRadarData(ZCHX::Data::ITF_RadarPoint& data, int trackNum);
//    void setHistoryRadarPointData(const std::vector<DrawElement::RadarPointElement> &data);//设置历史回放雷达点数据
//    void setRadarAreaData(const std::vector<DrawElement::RadarAreaElement> & data); //设置雷达扫描区域数据
//    void setCameraRodData(const std::vector<DrawElement::CameraRodElement> & data); //设置摄像杆数据
//    void setCameraDevData(const std::vector<DrawElement::CameraElement> & data); //设置相机设备数据
//    void setCameraGdyDevData(const std::vector<DrawElement::CameraElement> & data);    //设置光电仪设备数据
//    void setCameraPlanDevData(const std::vector<DrawElement::CameraElement> & data);   //设置无人机设备数据
    //设置船上的相机.相机初始化设定时设定相机的站点名对应的是船舶Ais的MMSI.站点的经纬度设定为(0,0);
//    void setAisCameraDevData(const std::vector<DrawElement::CameraElement> & data);         //设置船上的相机

//    void setPastrolStation(const std::vector<DrawElement::PastrolStation> & data);      //设置巡逻站
//    void setWarringZONEData(const std::vector<DrawElement::WarringZONE> & data);        //设置告警区域数据

//    void setShipSiumtionData(const std::vector<std::pair<double,double> > &data);          //设置船舶航线模拟数据
//    void setAisDataToSiumtion(const QList<ZCHX::Data::ITF_AIS> &data);
//    void setShipSimulationExtrapolationDatas(const QList<ZCHX::Data::ITF_AIS> &DataList);   //船舶外推模拟数据
//    void setRealTimeShipTrailDatas(const QMap<QString, QList<ZCHX::Data::ITF_AIS> > &DataList);
//    void setRadarTailTracklist(const QList<int>& trackList, bool fromMenu = false);
//    void setRouteHistogramData(const int RouteId, const QMap<int, QList<int> > &DataList);
//    void setRealTimeFlowLineHistogramData(const QMap<int, QList<int> > &DataList);
//    void setInTheAreaOfTrackData(const QList<QList<ZCHX::Data::LatLon> > &data);
//    void setRolePrivilegeList(const QList<int> &list);

    void setFleet(const QMap<QString, ZCHX::Data::ITF_Fleet> &fleetMap); //设置船队

    // 设置预警追溯
//    void setShipAlarmAscendMap(const QMap<QString, ZCHX::Data::ITF_ShipAlarmAscend> &shipAlarmAscendMap);
//    void setRadarFeatureZoneData(const std::vector<DrawElement::RadarFeatureZone> &data);
//    void setCameraObservationZoneData(const std::vector<DrawElement::CameraObservationZone> &data);
//    void setRadarEchoData(const QMap<QDateTime,ZCHX::Data::ITF_RadarEchoMap> &data);    //设置雷达回波数据

    //设置回波数据，uDisplayType==1回波显示，uDisplayType==2余辉显示
//    void setRadarVideoData(double dCentreLon, double dCentreLat, double dDistance, int uDisplayType, int uLoopNum);
//    void setMultibeamDataByImg(int iRouteID, const QPixmap &samplePixmap, const QPixmap &objPixmap, double dMinLon, double dMinLat, double dMaxLon, double dMaxLat); //设置多波束数据

//    void setAllProjectRouteLineData(const std::vector<DrawElement::RouteLine> &data); //设置所有项目路由数据
//    void setRouteLineData(const std::vector<DrawElement::RouteLine> &data);         //设置路由数据(当前项目)
//    void setALLSpecialRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data); //设置(所有)特殊路由数据(南方电网)
//    void setSpacilRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data);    //设置(当前项目)特殊路由数据(南方电网)
//    void setPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data);
//    void setRouteCrossData(const std::vector<DrawElement::RouteCross> &data);         //设置交越路由数据（全工程）
//    void setHistoryRouteLineData(const std::vector<DrawElement::RouteLine> &data);  //设置历史路由线数据接口(一条路由每个阶段的历史路由)
//    void setRouteDefaultCableData(const ZCHX::Data::CableAssembly &data, int uSlack);
//    void setRouteConfigDisplay(int uRouteID, bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable, bool bShowIndex);//设置该条路由的点、类型、线、海缆是否显示
//    void setRouteDisplayFlag(int uRouteID,int uDisplayFlag);//设置路由显示设计路由还是记录路由或者同时显示
//    void closeCurRouteLineData();//关闭当前项目的路由数据
//    void deleteRouteLineDataFromProjectID(const int uProjectID);//删除项目ID为uProjectID的路由数据
//    void changeCurrentRouteLine(const ZCHX::Data::RouteLine &data,int uRouteID);//改变当前路由线数据接口
//    void changeRoutePathAssessStatus(int uAssessStatus,int uRouteID);//改变路由的审批状态;0.未审批 1.已审批
//    void setAllProjecrShipPlanLineData(const std::vector<DrawElement::ShipPlanLine> &data);
//    void setShipPlanLineData(const std::vector<DrawElement::ShipPlanLine> &data);
//    void setShipPlanProjectID(const int ProjectID);
//    void setRouteLinePickupLocation(const int uRouteID,const double dLat,const double dLon);//设置路由显示被拾取的点
//    void setRouteLinePickupInfor(const double dKp, bool bShowLength, const double dLength,
//                                 bool bShowSlope, const double dSlope);//设置路由显示被拾取点的信息
//    void setSimulateData(int uSimulateDataID,int uSimulateFlag);//设置模拟数据的ID和模拟的标志(1-rpl表模拟，2-船舶计划表模拟)
//    void setSimulateLocation(float fCurSimulateKP);//设置模拟船的位置

//    void appendElementItem(const ZCHX::Data::ITF_EleEllipse &item); //设置图元数据
//    void appendElementItem(const ZCHX::Data::ITF_EleTriangle &item);//设置图元数据
//    void appendElementItem(const ZCHX::Data::ITF_EleLine &item);    //设置图元数据
//    void appendElementItem(const ZCHX::Data::ITF_EleRect &item);    //设置图元数据
//    void removeElementItem(DrawElement::Element *pitem);            //删除图元
//    void clearElementData();                                        //清除图元指针数据

    /**
        * @brief setActiveDrawElement
        * @param pos
        * @author:zxl
        * 拾取元素
        */
//    void setActiveDrawElement(const Point2D&pos, const QGeoCoordinate &geoPos = QGeoCoordinate(), bool dbClick = false);       //设置选中元素

    /*!
        * \brief 选择各个图层的元素
        * \param pos 鼠标的像素坐标
        * \param geoPos 鼠标的经纬度坐标（lon，lat）
        */
//    bool pickUpLayerElement(QPointF pos, const QGeoCoordinate &geoPos);
//    void enterPickUp();
//    void setPickUpNavigationTarget(const Point2D &pos);  //设置导航跟踪目标
//    void setSelectedCameraTrackTarget(const Point2D &pos);//设置相机跟踪
//    void setHoverDrawElement(const Point2D &pos);        //设置悬浮状态
//    void getPointNealyCamera(const Point2D &pos);        //设置手动跟踪
//    void routeLineRightKeyOKSlot();                         //路由操作右键菜单确认槽
//    void routeLineRightKeyCancelSlot();                     //路由操作右键菜单取消槽
//    void releaseDrawStatus();                               //释放绘制状态
//    void selectAnRegion();                                  //框选一片区域,统计ais数量
//    void setShipSimulation();                               //船舶模拟
//    void setLocationMark();                                 //本地标记
//    void setFixedReferencePoint();                          //固定参考点
//    void invokeHotSpot();                                   //热点
//    void invokeLinkageSpot();                               //联动
//    void invokeLinkageSpotForRadar();                       //联动
//    void setCustomFlowLine();                               //流量统计线
//    void setSimulationExtrapolation();                      //模拟外推
//    void setHistoryTraces();                                //历史轨迹
//    void setRealTimeTraces();                               //实时轨迹
//    void setBlackList();                                    //设置黑名单
//    void setWhiteList();                                    //设置白名单
//    void setCPATrack();                                     //设置CPA
//    void setPictureInPicture();                             //画中画
//    void setFleet();                                        //设置船队
//    void shipPlanLineRightKeyOkSlot();                      //船舶计划操作右键菜单确认槽
//    void shipPlanLineRightKeyCancelSlot();                  //船舶计划操作右键菜单确认槽
//    void warringZoneRightKeyOkSlot();
//    void warringZoneRightKeyCancelSlot();
//    void shipSlumtionLineRightKeyOKSlot();
//    void shipSlumtionLineRightKeyCancelSlot();
//    void customFlowLineRightKeyOKSlot();
//    void customFlowLineRightKeyCancelSlot();

//    void coastDataRightKeyOkSlot();
//    void coastDataRightKeyCancelSlot();

//    void seabedPipeLineRightKeyOkSlot();
//    void seabedPipeLineRightKeyCancelSlot();

//    void structureRightKeyOkSlot();
//    void structureRightKeyCancelSlot();

//    void areaNetRightKeyOkSlot();
//    void areaNetRightKeyCancelSlot();

//    void channelRightKeyOkSlot();
//    void channelRightKeyCancelSlot();

//    void mooringRightKeyOkSlot();
//    void mooringRightKeyCancelSlot();

//    void cardMouthRightKeyOkSlot();
//    void cardMouthRightKeyCancelSlot();

//    //防区相关编辑操作
//    void zoneEditSelect(const Point2D &pt);              //激活一个防区成为当前防区
//    void zoneEditSelectCtrlPoint(const Point2D &pt);     //选择活动防区内的一个点为控制点
//    void zoneEditAddCtrlPoint(const Point2D &pt);        //添加一个控制点暂时添加到最后
//    void zoneEditDelCtrlPoint(const Point2D &pt);        //删除一个控制点
//    //路由线相关编辑操作
//    void routeOrCrossPickup(const Point2D &pt);              //拾取路由或者交越点
//    void routeLineEditSelect(const Point2D &pt);             //激活一个路由线成为当前路由
//    bool routeLineEditSelectCtrlPoint(const Point2D &pt);    //选择活动路由线内的一个点为控制点
//    void routeLineEditAddCtrlPoint(const QPointF &objCusP);     //从后面添加一个路由线控制点
//    void routeLineEditInsertCtrlPoint(const QPointF &objCusP);  //插入一个路由线控制点
//    void routeLineEditDelCtrlPoint(const Point2D &pt);       //删除一个路由线控制点
//    void addRoutePoint(const QPointF &objCusP,DrawElement::RouteLine &objCurrentLine);  //加路由
//    void addRoutePointLL(const LatLon& pnt, DrawElement::RouteLine &objCurrentLine);
//    void addKPPoint(const QPointF &objCusP,DrawElement::RouteLine &objCurrentLine);     //加pk
//    void delRoutePointKPPoint(int uIndex,DrawElement::RouteLine &objCurrentLine);       //删除点
//    void moveRoutePointKPPoint(int uIndex,const double dLon,const double dLat,DrawElement::RouteLine &objCurrentLine);//移动点
//    //船舶计划相关操作
//    void shipPlanEditSelect(const Point2D &pt);                  //激活一个船舶计划
//    bool shipPlanEditSelectCtrlPoint(const Point2D &pt);         //选择活动船舶计划线内的一个点为控制点
//    void shipPlanLineEditAddCtrlPoint(const QPointF &objCusP);      //从最后面追加一个船舶计划线控制点
//    void shipPlanLineEditInsertCtrlPoint(const QPointF &objCusP);   //插入一个路由线控制点
//    void shipPlanLineEditDelCtrlPoint(const Point2D &pt);        //删除一个船舶计划线控制点
//    void addShipPlanPoint(const QPointF &objCusP,DrawElement::ShipPlanLine &objCurrentLine);  //最后追加点
//    void addShipPlanPointLL(const LatLon& pnt, DrawElement::ShipPlanLine &objCurrentLine);
//    void addSKPPoint(const QPointF &objCusP,DrawElement::ShipPlanLine &objCurrentLine);       //插入skp点
//    void delShipPlanPointKPPoint(int uIndex,DrawElement::ShipPlanLine &objCurrentLine);       //删除点
//    void moveShipPlanPointKPPoint(int uIndex,const double dLon,const double dLat,DrawElement::ShipPlanLine &objCurrentLine);//移动点

//    //环岛线相关编辑操作
//    void islandLineEditSelect(const Point2D &pt);            //激活一个环岛线成为当前防区
//    void islandLineEditSelectCtrlPoint(const Point2D &pt);   //选择活动环岛线内的一个点为控制点
//    void islandLineEditAddCtrlPoint(const Point2D &pt);      //添加一个环岛线控制点暂时添加到最后
//    void islandLineEditDelCtrlPoint(const Point2D &pt);      //删除一个环岛线控制点
//    void islandLineEditAcitive4uuid(int uuid);                  //由于环岛线的特殊性,可能存在包含的情况.因此可以使用id来设置活动的环岛线

//    //航道相关编辑操作
//    void channelEditSelect(const Point2D &pt);              //激活一个航道成为当前航道
//    void channelEditSelectCtrlPoint(const Point2D &pt);     //选择活动航道内的一个点为控制点
//    void channelEditAddCtrlPoint(const Point2D &pt);        //添加一个控制点暂时添加到最后
//    void channelEditDelCtrlPoint(const Point2D &pt);        //删除一个控制点

//    //锚泊相关编辑操作
//    void mooringEditSelect(const Point2D &pt);              //激活一个锚泊成为当前锚泊
//    void mooringEditSelectCtrlPoint(const Point2D &pt);     //选择活动锚泊内的一个点为控制点
//    void mooringEditAddCtrlPoint(const Point2D &pt);        //添加一个控制点暂时添加到最后
//    void mooringEditDelCtrlPoint(const Point2D &pt);        //删除一个控制点

//    //卡口相关编辑操作
//    void cardMouthEditSelect(const Point2D &pt);              //激活一个卡口成为当前卡口
//    void cardMouthEditSelectCtrlPoint(const Point2D &pt);     //选择活动卡口内的一个点为控制点
//    void cardMouthEditAddCtrlPoint(const Point2D &pt);        //添加一个控制点暂时添加到最后
//    void cardMouthEditDelCtrlPoint(const Point2D &pt);        //删除一个控制点
//    //目标关注操作
//    void appendFocusRadarPoint(const QList<int>& trackNum);
//    void removeFocusRadarPoint(const QList<int>& trackNum);
//    void appendFocusAis(const QStringList& id);
//    void removeFocusAis(const QStringList& id);

    /**
        * @brief 更新相机状态
        * @param cameid 相机ID数据库ID
        * @param st:状态值
        */
//    void zchxUpdateCameraDevStatus(uint cameid, uint st);

    /**
        * @brief 根据相机的ID设置其所在的电线杆的焦点状态
        * @param cameid 相机的ID
        * @param tags 0x00000001：开(有焦点) 0x00000010: 居中
        */
//    void zchxCameraRodFocus(uint cameid, int tags);

//    void openCameraDisplayWin();//wangliang

//    void zchxOpenAisCameraListFromMenu();//通过右键菜单查看相机

    /*!
        * \brief 根据缩放比例, 获取不同的长度尺寸
        */
//    int getDrawScaleSize() const;

    //    void setMultibeamPixmap(const QPixmap &objPixmap);
//    void setRadarVideoPixmap(int uIndex,const QPixmap &objPixmap,const QPixmap &prePixmap);
//    void setCurrentRadarVideoPixmap(const QPixmap &objPixmap); //只接收当前的图片
//    //    void addElement(DrawElement::Element *item);
//    //    const QList<DrawElement::Element*>& getAllElement() const;
//    //设定当前的项目ID
//    void setCurrentProjectID(int id);
    //路由显示控制
    void setDisplayRoutePoint(bool display){mShowRoutePoint = display;}
    void setDisplayRouteIndex(bool display){mShowRouteIndex = display;}
    void setDisplayRouteType(bool display){mShowRouteType = display;}
    void setDisplayRouteCable(bool display){mShowRouteLine = display;}
    void setDisplayRouteAc(bool display){mShowAcFlag = display;}
    bool displayRoutePoint() const {return mShowRoutePoint;}
    bool displayRouteIndex()const {return mShowRouteIndex;}
    bool displayRouteType()const {return mShowRouteType;}
    bool displayRouteCable()const {return mShowRouteLine;}
    bool displayRouteAc() const {return mShowAcFlag;}
    //相机网格
    //void setETool2DrawCameraNetGrid(const QSizeF& size, const QString& camera);
    //void setCameraNetGridList(const QList<ZCHX::Data::ITF_CameraNetGrid> & list);
    //void slotSetRadarTailTrack();


private slots:
    /*!
        * \brief 处理图层的 visible 状态的变化
        */
    //void _maplayerVisibleChanged(bool visible);

protected:
    //void drawLayers(QPainter *painter);

private:
//    void SubmitFakeLocationPoint(Point2D const & pt);
//    void SubmitRoutingPoint(Point2D const & pt);
//    void ShowInfoPopup(QMouseEvent * e, Point2D const & pt);
//    void UpdateScaleControl();
////    inline int L2D(int px) const { return px * m_ratio; }
//    Point2D GetDevicePoint(QMouseEvent * e) const;


//    void zchxDrawDistance(QPainter *painter);
//    void zchxDrawMeasureArea(QPainter *painter);
//    void zchxDrawDirAngle(QPainter *painter);           //绘制方向角

    //需要在海图编辑模式下才可使用的
//    void zchxDrawShipSimulation(QPainter *painter);     //绘制船舶模拟线
//    void zchxDrawCustomFlowLine(QPainter *painter);     //绘制流量线
//    void zchxDrawSelectdAre(QPainter *painter);         //绘制框选区域
//    void zchxDrawShipPlanLine(QPainter *painter);       //船舶计划航线
//    void zchxDrawPoint(QPainter *painter);              //创建结构物
//    void zchxDrawZONE(QPainter *painter);               //创建防区
//    void zchxDrawRadarFeatureZONE(QPainter *painter);
//    void zchxDrawRouteLine(QPainter *painter);          //创建路由
//    void zchxDrawMoveRoutePoint(QPainter *painter);
//    void zchxDrawMoveShipPlanLinePoint(QPainter *painter);
//    void zchxDrawTrafficStatistic(QPainter *painter);
//    void zchxDrawRealTimeFlowStatistic(QPainter *painter);
//    void drawRouteTrafficHistogram(QPainter *painter, const DrawElement::RouteLine &ele);
//    void drawSpacialRouteTrafficHistogram(QPainter *painter, const DrawElement::SpecialRouteLine &ele);
//    void zchxShowAllProjectRouteLine(QPainter *painter);//显示所有项目路由
//    void zchxShowRouteLine(QPainter *painter);          //显示路由
//    void zchxShowALLProjectSpecialRouteLine(QPainter *painter);
//    void zchxShowSpecialRouteLine(QPainter *painter);   //显示特殊路由 (南方电网)
//    void zchxShowStatisticsShipTrackOnDefence(QPainter *painter);   //绘制防区上统计的轨迹
//    void zchxShowShipTrackPixmapOnDefence(QPainter *painter);       //绘制防区上统计的轨迹 叠图片
    //路由显示
//    void zchxShowAllProjectRouteLineHighlightRepeat(QPainter *painter);//显示所有项目路由(高亮和防区重复部分)
//    void zchxShowRouteLineHighlightRepeat(QPainter *painter);          //显示路由(高亮和防区重复部分)

//    void zchxShowRouteCross(QPainter *painter);                 //显示交越路由
//    void zchxShowHistoryRouteLine(QPainter *painter);           //显示历史路由

//    void zchxShowMultibeam(QPainter *painter);           //显示多波束数据

//    void zchxShowRadarVideo(QPainter *painter);           //显示雷达回波
//    void zchxShowAfterglow(QPainter *painter);           //显示余辉

//    void drawOneDesignRouteLine(QPainter *painter, const DrawElement::RouteLine &ele, bool bCurrentProject);
//    void drawOneRecondRouteLine(QPainter *painter, const DrawElement::RouteLine &ele, bool bCurrentProject);
//    void drawOneRouteLine(QPainter *painter, const DrawElement::RouteLine &ele, bool bCurrentProject);
//    void drawOneRepateRouteLine(QPainter *painter, const DrawElement::RouteLine &ele,QStringList latlon, bool bCurrentProject);

//    void zchxShowDangerousCircle(QPainter *painter);    //显示危险圈
//    void zchxShowRadarFeatureZone(QPainter * painter);  //显示雷达特征区域
//    void zchxShowSimulateShip(QPainter *painter);       //显示模拟船
//    void zchxShowShipPlanLine(QPainter *painter);       //显示船舶航线
//    void zchxShowAllProjectShipPlanLine(QPainter *painter);     //显示所有船舶计划
//    void drawShipPlanLine(QPainter *painter, const DrawElement::ShipPlanLine &ele, bool bCurrentProject);
//    void drawShipDesignLine(QPainter *painter, const DrawElement::ShipPlanLine &ele, bool bCurrentProject);
//    void drawShipRecondLine(QPainter *painter, const DrawElement::ShipPlanLine &ele, bool bCurrentProject);

//    void zchxShowRadarEcho(QPainter *painter);                  //雷达回波显示
//    void zchxShowCameraObservationZone(QPainter * painter);     //相机视场显示
//    void zchxShowCamera(QPainter * painter);

//    void zchxShowTowerRod(QPainter * painter);          //绘制塔杆
//    void zchxShowNavigationMark(QPainter * painter);    //绘制航标
//    void zchxShowAis(QPainter *painter);                //draw雷达轨迹数据
//    void zchxShowSingleAisTrail(QPainter *painter);     //绘制单条船的历史轨迹
//    void zchxShowSingleAisHistoryTrail(QPainter *painter);//绘制单条船的历史轨迹(新)
//    void zchxShowHistoryAis(QPainter *painter);        //draw历史ais数据
//    void zchxShowAisToSiumtion(QPainter *painter);
//    void zchxShowShipSiumtion(QPainter *painter);
//    void zchxShowShipSiumtionExtrapolation(QPainter *painter);
//    void zchxShowRealTimeShipTrail(QPainter *painter);  //实时统计轨迹

//    void zchxShowRadarPoint(QPainter *painter);     //draw雷达点数据
//    void zchxShowHistoryRadarPoint(QPainter *painter); //draw雷达点数据
//    void zchxShowRadarArea(QPainter *painter);      //draw雷达区域范围
//    void zchxShowCameraGdyDev(QPainter *painter);   //显示光电仪
//    void zchxShowCameraPlanDev(QPainter *painter);  //显示无人机
//    void zchxShowPastrolStation(QPainter *painter); //显示巡逻站
//    void zchxShowCameraVideoWarn(QPainter *painter);//显示人车船
//    void zchxShowWarringZone(QPainter *painter);    //draw告警管理数据
//    void zchxShowLocalMark(QPainter *painter);      //显示位置标注
//    void zchxShowIslandLine(QPainter *painter);     //显示环岛线
//    void zchxShowGpsData(QPainter *painter);        //显示GPS数据
//    void zchxShowCameraTackTarget(QPainter *painter);//显示相机跟踪
//    void zchxShowCameraDev(QPainter *painter);  //显示相机(枪机,球机);
//    void zchxShowCoastDataLINE(QPainter *painter);    //draw海岸数据
//    void zchxShowSeabedPipeLineLINE(QPainter *painter);    //draw海底管线
//    void zchxShowStructurePoint(QPainter *painter);    //draw结构物
//    void zchxShowAreaNetZone(QPainter *painter);    //draw地理区域网络
//    void zchxShowChannelZone(QPainter *painter);    //draw航道
//    void zchxShowMooringZone(QPainter *painter);    //draw锚泊
//    void zchxShowCardMouthZone(QPainter *painter);  //draw卡口

//    void zchxShowShipAlarmAscend(QPainter *painter);  //draw预警追溯轨迹线

//    void zchxShowEleData(QPainter *painter); //显示自定义图元
//    void zchxShowEleEllipseItem(QPainter *painter,const DrawElement::EllipseElement *item);
//    void zchxShowEleTriangleItem(QPainter *painter,const DrawElement::TriangleElement *item);
//    void zchxShowEleLineItem(QPainter *painter,const DrawElement::LineElement *item);
//    void zchxShowEleRectItem(QPainter *painter,const DrawElement::RectElement *item);

//    //施工高层图
//    void zchxShowHighLevelPixmap(QPainter *painter);
//    //施工影像图
//    void zchxShowImagePixmap(QPainter *painter);

//    //相机网格
//    void zchxShowCameraNetGrid(QPainter *painter);
//    void zchxShowCameraNetGrid(QPainter *painter, QList<ZCHX::Data::ITF_NetGrid>& list, const QColor& lineColor);
//    void zchxShowCameraNetGrid(QPainter *painter, const LatLon& start, const LatLon& end); //临时描绘时使用
//    ZCHX::Data::ITF_CameraNetGrid makeCameraGrid(const LatLon& start, const LatLon& end);





//    //显示经纬网线
//    void zchxShowLatitudeLongitudeGrid(QPainter *painter);
//    void calcGridSpacing(int scale, double &MajorSpacing, double &MijorSpacing);
//    QString calcGridText(double latlon, double spacing, bool bPostfix);

//    void doubleClickCompleteDraw();//双击鼠标完成绘制的操作

//    //对点重新编号
//    void reStatisticsRoutePoints(std::vector<ZCHX::Data::RoutePoint> &objLinePoints);
//    //对路由线的点重新编号及接头和参考点的处理和海缆处理
//    void reStatisticsRoutePoints(ZCHX::Data::RouteLine &objCurrentLine);
//    //对船舶计划点重新编号
//    void reStatisticsShipPlanPoints(ZCHX::Data::ShipPlanLine &objCurrentLine);

//    bool isOnlyGetZoneName() const;

//    void setTowerRodData();
//    void setNavigationMarkData();
//    QString getPolygonOfGravityPoint(const std::vector<std::pair<double, double>> &Points); //求多边形重心, 返回 (lon,lat)

signals: //发送外部信号
    //创建告警区域数据
    void signalCreateWarringZONE(ZCHX::Data::ITF_WarringZone/*std::vector<std::pair<double, double> >  path*/);
    //防区 防线编辑状态  type ：0为防线 1位防区 state：0为编辑 1取消编辑
    void signalUpdateWarringZoneOrLineState(const int type,const int State);
    //创建环岛线数据
    void signalCreateIslandLINE(ZCHX::Data::ITF_IslandLine);
    //创建位置标注数据
    void signalCreateLocalMark(ZCHX::Data::ITF_LocalMark);
    void signalSendCurPos(double lat, double lon);

    //创建海岸数据
    void signalCreateCoastDataLINE(ZCHX::Data::ITF_CoastData/*std::vector<std::pair<double, double> >  path*/);

    //创建海底管线
    void signalCreateSeabedPipeLineLINE(ZCHX::Data::ITF_SeabedPipeLine/*std::vector<std::pair<double, double> >  path*/);

    //创建结构物
    void signalCreateStructurePoint(ZCHX::Data::ITF_Structure);

    //创建地理区域网络
    void signalCreateAreaNetZone(ZCHX::Data::ITF_AreaNet/*std::vector<std::pair<double, double> >  path*/);

    //创建航道
    void signalCreateChannelZone(ZCHX::Data::ITF_Channel);
    void signalUpdateChannelZoneState(const int type, const int State);

    //创建锚泊
    void signalCreateMooringZone(ZCHX::Data::ITF_Mooring);
    void signalUpdateMooringZoneState(const int type, const int State);

    //创建卡口
    void signalCreateCardMouthZone(ZCHX::Data::ITF_CardMouth);
    void signalUpdateCardMouthZoneState(const int type, const int State);

    //船队
    void signalAddFleet(const ZCHX::Data::ITF_AIS& data);
    void signalRemoveFleet(const ZCHX::Data::ITF_AIS& data);

    /**
        * @brief 发送当前需要跟踪的雷达，或经纬度位置
        * @param trackNumber
        * @param wgs84PosLat
        * @param wgs84PosLon
        */
    void signalSendPointNealyCamera(int trackNumber, double wgs84PosLat ,double wgs84PosLon);

    //定义当前被选中的元素信息信号
    void signalIsSelected4Ais(const ZCHX::Data::ITF_AIS &info, bool autoChange, bool dbClick = false);
    void signalIsSelected4RadarPoint(const ZCHX::Data::ITF_RadarPoint &info, bool autoChange);
    void signalIsSelected4RadarArea(const ZCHX::Data::ITF_RadarArea &info);
    void signalIsSelected4CameraRod(const ZCHX::Data::ITF_CameraRod &info);
    void signalIsSelected4WarringLine(const ZCHX::Data::ITF_WarringZone); //选中的环岛线
    void signalIsSelected4WarringZone(const ZCHX::Data::ITF_WarringZone &info);//选中的防区
    void signalIsSelected4ChannelZone(const ZCHX::Data::ITF_Channel &info);//选中的航道
    void signalIsSelected4MooringZone(const ZCHX::Data::ITF_Mooring &info);//选中的锚泊
    void signalIsSelected4CardMouthZone(const ZCHX::Data::ITF_CardMouth &info);//选中的卡口
    void signalIsSelected4PastrolStation(const ZCHX::Data::ITF_PastrolStation &info);
    void signalIsSelected4LocalMark(const ZCHX::Data::ITF_LocalMark &info);
    void signalIsSelected4IslandLine(const ZCHX::Data::ITF_IslandLine &info); //选中的环岛线
    void signalIsSelected4CameraVideoWarn(const ZCHX::Data::ITF_CameraVideoWarn &info); //选中了视频分析目标
    void signalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机点击
    void signalIsDoubleClicked4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机双击
    //定义当前被双击的图元的信息
    void signalIsDoubleClicked4Ais(const ZCHX::Data::ITF_AIS &info/*, bool autoChange*/);//Ais双击
    void signalIsDoubleClicked4RadarPoint(const ZCHX::Data::ITF_RadarPoint &info/*, bool autoChange*/); //雷达目标双击
    void signalIsDoubleClicked4RadarArea(const ZCHX::Data::ITF_RadarArea &info); //雷达区域双击
    void signalIsDoubleClicked4CameraRod(const ZCHX::Data::ITF_CameraRod &info);//相机杆双击
    void signalIsDoubleClicked4WarringZone(const ZCHX::Data::ITF_WarringZone &info);//防区双击
    void signalIsDoubleClicked4PastrolStation(const ZCHX::Data::ITF_PastrolStation &info);//巡逻站双击
    void signalIsDoubleClicked4LocalMark(const ZCHX::Data::ITF_LocalMark &info);    //关注点双击
    void signalIsDoubleClicked4IslandLine(const ZCHX::Data::ITF_IslandLine &info); //环岛线双击
    void signalIsDoubleClicked4CameraVideoWarn(const ZCHX::Data::ITF_CameraVideoWarn &info); //双击了视频分析目标
    void signalIsDoubleClicked4ChannelZone(const ZCHX::Data::ITF_Channel &info);//双击航道
    void signalIsDoubleClicked4MooringZone(const ZCHX::Data::ITF_Mooring &info);//双击锚泊
    void signalIsDoubleClicked4CardMouthZone(const ZCHX::Data::ITF_CardMouth &info);//双击卡口

    void signalOpenCameraDisplayWin();

    //创建雷达特征区数据
    void signalCreateRadarFeatureZONE(ZCHX::Data::ITF_RadarFeaturesZone);
    //创建路由
    void signalCreateRouteLINE(ZCHX::Data::RouteLine);
    void signalCancelRouteLine();
    //创建船舶计划航线
    void signalCreateShipPlanLine(ZCHX::Data::ShipPlanLine);
    void signalCreateShipSimutionLine(std::vector<std::pair<double, double> >);
    //创建流量线
    void signalCreateFlowLine(const ZCHX::Data::CustomFlowLine &Data);
    //定义当前被选中的元素信息信号
    void signalIsSelected4TrackRadarOrbit(const ZCHX::Data::ITF_AIS/*ITF_RadarOrbit*/ &info, bool autoChange); //发送跟踪导航目标
    void signalIsEcdisScales(double data);                                    //测试获取海图缩放比例
    void signalIsEcdisDoubleClickCoordinate(double lat, double lon);          //获取在海图上双击的经纬度
    void signalIsEcdisCameraTrackTarget(const ZCHX::Data::ITF_CameraTrackTarget &info); //获取海图上相机跟踪的目标
    /*!
        * \brief 图层中的元素的选中状态有变化
        */
    void sigLayerElementSelectionChanged();

    void signalIsSelected4RouteLine(const ZCHX::Data::RouteLine);       //选中的路由线
    void signalDeleteRoutePoint(const ZCHX::Data::RouteLine,int uIndex);//发送删除路由点的索引
    void signalModfiyRoutePoint(const ZCHX::Data::RouteLine,int uIndex);//发送编辑路由点的索引
    void signalChangeRouteLine(const ZCHX::Data::RouteLine);            //发送改变整条路由及其明细
    void signalCancelChangeRouteLine();
    void signalNoShipPlanSelected();
    void signalNoRouteLineSelected();

    void signalIsSelected4ShipPlanLine(const ZCHX::Data::ShipPlanLine);       //选中的船舶计划
    void signalDeleteShipPlanPoint(const ZCHX::Data::ShipPlanLine,int uIndex);//发送删除船舶计划点的索引
    void signalModfiyShipPlanPoint(const ZCHX::Data::ShipPlanLine,int uIndex);//发送编辑船舶计划点的索引
    void signalChangeShipPlanLine(const ZCHX::Data::ShipPlanLine);            //发送改变整条船舶计划及其明细
    void signalCancelChangeShipPlanLine();

    void signalScreenShot();                                                                //截图
    void signalSendSeletedAISTarget(const QList<ZCHX::Data::ITF_AIS> &dataList);            //框选
    void signalSendPictureInPictureTarget(const int TargetType, const QString &TargetID);   //画中画
    void signalSendLocalMarkPos(const double Lat, const double Lon);
    void signalSendReferencePos(const double Lat, const double Lon);
    void signalShowLocationMarkOrReferencePos(bool result);
    void signalInvokeHotSpot(const ZCHX::Data::ITF_CloudHotSpot & data);
    void signalAddShipExtrapolation(const QString &data,bool result); //result true add  or result false delete
    void signalSendHistoryTrail(const QString &Id, bool states);
    void signalSendRealTimeTrail(const QString &Id, bool states);

    void signalIsSelectedSpecialRoutePoint(const ZCHX::Data::SpecialRoutePoint &data); //发送选中的特殊点
    void signalCreateBlackOrWhiteList(const QString shipId,const int Type);
    void signalCreateCPATrack(const QString &shipId);
    void signalMapIsRoaming();

    void signalLoading();//发送开始加重地图
    void signalLoadOver();//发送地图加载完毕

    void BeforeEngineCreation();
    void engineCreationFinished();

    //回传相机网格
    void signalSendCameraNetGrid(const ZCHX::Data::ITF_CameraNetGrid& data);

public:
//    static std::vector<std::pair<double, double>> convertLatLonPath(const std::vector<LatLon> &path);
//    static void convertLatLonPath(const std::vector<LatLon> &path, std::vector<std::pair<double, double>> &retPath);
//    static void convertLatLonPath(const std::vector<LatLon> &path, QList<ZCHX::Data::LatLon> &retPath);

//    bool getIsAtiAdaptor() const;
//    /*!
//           * \brief 设置是否采用ATI显卡的地图引擎初始化方式
//           * 地图框架默认会自动根据显卡型号判断使用哪种方式
//           * 一下情况下可以使用此接口或者配置文件（IsAtiAdapter字段）强制制定显卡：
//           * 1. 地图框架识别显卡型号失败（从data目录下的log文件分析是否出现此情况）
//           * 2. 测试或者特殊需要下，强制指定
//           */
//    void setIsAtiAdaptor(bool isAtiAdaptor);
private:
//    //路由显示综合
//    void   drawRouteLineItem(QPainter *painter, const DrawElement::RouteLine &ele, const QStringList& lonlat = QStringList());
//    bool   isCurrentPointDraw(int index, int size, bool ac);
//    void   zchxDrawRoutePoint(const QPointF& pnt, int type, const QString& name, bool isActive, QPainter* painter);
//    void   zchxDrawRouteLine(const QLineF &line, const QColor& color,Qt::PenStyle style, int width, QPainter *painter);
//    void   zchxDrawRouteText(const QPointF& pos, const QString& text, const QColor& color, QPainter* painter);
//    void   zchxDrawLonLatText(const QPointF& pos, double lon, double lat, const QColor& color, QPainter* painter);
//    void   zchxDrawOverlapLine(const POLYLINES& polylines,QPainter* painter);
//    POLYLINES  parseRouteWarnOverlapSegments(const QStringList& lonlat);
//    void   zchxDrawRoutePickUpInfo(const DrawElement::RouteLine &ele);
//    void   zchxRoutePickupInfo(QString& text, int& width, int& height, const ZCHX::Data::RoutePoint& point, const QFont& font);
//    int    widthOfTextList(const QStringList& list, const QFont& font);
//    //绘制备份路由
//    void   zchxDrawBackUpRoutePoints(const std::vector<ZCHX::Data::RoutePoint>& pnts, bool active, QPainter* painter);
//    void   zchxDrawRoutePoints(const std::vector<ZCHX::Data::RoutePoint>& pnts, const QString& routeName, const QColor& nameColor, int stage, bool active, QPainter* painter);
//    void   zchxDrawRouteLinkedInfo(const DrawElement::RouteLine &ele, QPainter* painter);
private:
    TileImageList               mDataList;
    zchxMapFrameWork*           mFrameWork;
    zchxMapLoadThread*          mMapThread;
    LatLon                      mCenter;
    qint64                      mLastWheelTime;
    bool                        mDrag;
    QPoint                      mPressPnt;
    bool                        mDisplayImageNum;
    bool                        mIsMapHidden;                       //地图是否显示
    bool                        mIsNavigation;    //是否正在导航
    bool                        mIsOpenMeet;        //会遇显示
    bool                        mIsRadarTagetTrack;     //雷达目标跟踪
    bool                        mIsCameraCombinedTrack; //相机联动跟踪
    bool                        mIsCameraDisplayWithoutRod;  //相机直接显示?
    bool                        mRouteHistogram;             //路由直方图显示
    bool                        mUseRightKey;//是否使用右键
    int                         mDx;
    int                         mDy;
    ZCHX::Data::ECDIS_PLUGIN_USE_MODEL  mCurPluginUserModel;
    ZCHX::Data::ECDIS_PICKUP_TYPE       mCurPickupType;
    std::shared_ptr<DrawElement::Element> mCurrentSelectElement;

    zchxMapLayerMgr             *mLayerMgr;                     //地图图层管理
    zchxCameraDatasMgr          *mCameraDataMgr;                //相机相关的数据管理
    zchxAisDataMgr              *mAisDataMgr;                   //船舶数据管理
    zchxUserDefinesDataMgr      *mUseDataMgr;                   //用户在地图上绘制的数据管理
    zchxRadarDataMgr            *mRadarDataMgr;                 //雷达数据管理
    zchxRouteDataMgr            *mRouteDataMgr;                 //路由数据管理
    zchxShipPlanDataMgr         *mShipPlanDataMgr;              //船舶计划数据管理
    QList<zchxEcdisDataMgr*>    mDataMgrList;                  //总的数据管理刷新是使用
    //路由信息控制显示
    bool mShowRoutePoint;  //0 隐藏点  1 显示点
    bool mShowRouteType;  //0隐藏类型  1显示类型
    bool mShowRouteLine;    //0隐藏线  1显示线
    bool mShowRouteIndex;
    bool mShowAcFlag;

    //海缆基础数据
    QList<ZCHX::Data::CableBaseData>        mBaseCableDataList;
    QList<ZCHX::Data::CableInterfaceData>        mInterfaceCableDataList;
    //地图的鼠标操作
    std::vector<LatLon>             m_eToolPoints;          //用户的操作点
    eTool                           m_eTool;                //用户当前的操作模式
    bool                            isActiveETool;          //用户操作时是否允许地图移动
    //GPS数据
    std::list<std::shared_ptr<ZCHX::Data::GPSPoint>> m_gpsTracks;
    QMutex                                           m_gpsTracksMutex;
    //地图单位
    MapUnit                         mUnit;

};
#endif // ZCHXMAPWIDGET_H
