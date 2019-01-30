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

class zchxMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit zchxMapWidget(QWidget *parent = 0);
    ~zchxMapWidget();
    void setCurZoom(int zoom);
    int  zoom() const;
    void setCenterLL(const LatLon& pnt );
    void setCenterAndZoom(const LatLon &ll, int zoom);
    LatLon centerLatLon() const;
    void setSource(int source);
    void setDisplayImgeNum(bool sts) {mDisplayImageNum = sts; update();}
    zchxMapFrameWork* framework() const {return mFrameWork;}
    void setScaleControl(QScaleSlider * pScale);
    void setMapStyle(MapStyle mapStyle);

    //地图旋转
    double zchxUtilToolAngle4north(); //返回当前正北方向角
    void   zchxUtilToolSetAngle4north(double ang); //设置当前地图的旋转角度
    void   setMapRotateAtLL(double lat, double lon, double ang);//设置地图旋转角度
    void   resetMapRotate(double lat, double lon);//恢复地图旋转
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
    //防区操作
    bool zchxWarringZoneData4id(int uuid, ZCHX::Data::ITF_WarringZone &info); //id获取防区信息
    bool zchxWarringZoneDataByName(const QString &name, ZCHX::Data::ITF_WarringZone &info); //名称获取防区信息
    bool zchxWarringZoneDataByName(const std::string &name, ZCHX::Data::ITF_WarringZone &info);
    DrawElement::WarringZONE *zchxWarringZoneItem(const std::string &name);
    //航道
    bool zchxChannelZoneData4id(int uuid, ZCHX::Data::tagITF_Channel &info);
    DrawElement::Channel *zchxChannelZoneItem(const std::string &name);

    //锚泊
    bool zchxMooringZoneData4id(int uuid, ZCHX::Data::tagITF_Mooring &info);
    DrawElement::Mooring *zchxMooringZoneItem(const std::string &name);

    //卡口
    bool zchxCardMouthZoneData4id(int uuid, ZCHX::Data::tagITF_CardMouth &info);
    DrawElement::CardMouth *zchxCardMouthZoneItem(const std::string &name);

    //岸线
    DrawElement::CoastData *zchxCoastDataZoneItem(const std::string &name);

    //海底管线
    DrawElement::SeabedPipeLine *zchxSeabedPipeLineZoneItem(const std::string &name);

    //结构物
    DrawElement::Structure *zchxStructureZoneItem(const std::string &name);

    //区域网格
    DrawElement::AreaNet *zchxAreaNetZoneItem(const std::string &name);

    //环岛线
    bool zchxIslandData4id(int uuid, ZCHX::Data::ITF_IslandLine &info);

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
    void zchxShowCameraInfo(QPainter *painter);

protected:
    void paintEvent(QPaintEvent* e);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void resizeEvent(QResizeEvent *);
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent(QWheelEvent *);
#endif

signals:
    void signalDisplayCurPos(double lon, double lat);
    void signalSendNewMap(double, double, int);

public slots:
    void append(const QPixmap& img, int x, int y);
    void append(const TileImageList& list);
    void clear() {mDataList.clear(); /*update();*/}
    void slotRecvNewMap(double lon, double lat, int zoom);
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
    int                         mDx;
    int                         mDy;
    ZCHX::Data::ECDIS_PLUGIN_USE_MODEL  mCurPluginUserModel;
    ZCHX::Data::ECDIS_PICKUP_TYPE       mCurPickupType;
    std::shared_ptr<DrawElement::Element> mCurrentSelectElement;

    std::vector<DrawElement::WarringZONE> m_WarringZone;        //告警区域数据也叫防区
    std::vector<DrawElement::CoastData> m_coastDataLine;           //海岸数据
    std::vector<DrawElement::SeabedPipeLine> m_seabedPipeLineLine; //海底管线
    std::vector<DrawElement::Structure> m_structurePoint;          //结构物
    std::vector<DrawElement::AreaNet> m_areaNetZone;               //地理区域网络
    std::vector<DrawElement::Channel> m_channelZone;               //航道
    std::vector<DrawElement::Mooring> m_mooringZone;               //锚泊
    std::vector<DrawElement::CardMouth> m_cardMouthZone;               //卡口
    std::vector<DrawElement::IslandLine> m_IslandLine;          //环岛线
    zchxMapLayerMgr             *mLayerMgr;                     //地图图层管理
    zchxCameraDatasMgr          *mCameraDataMgr;                //相机相关的数据管理

};










#if 0

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
    void  setIsWarningType(bool bWarningType);

    /*!
     * \brief 设置地图单位
     * \param unit Metric: 米; Foot: 英尺; NauticalMile: 海里
     */
    void setMapUnit(const QString &unit);

    /*!
     * \brief 获取地图单位
     */
    QString getMapUnit() const;
    /*!
     * \brief 转换到指定单位的长度
     * \param length 长度(米)
     */
    qreal convertLengthToUnit(qreal length, const QString &unit) const;
    /*!
     * \brief 转换到指定单位的面积
     * \param area 面积(平方米)
     */
    qreal convertAreaToUnit(qreal area, const QString &unit) const;
    /*!
     * \brief 获取翻译后的单位
     */
    QString getTrUnit() const;

    void setUseRightKey(bool bUseRightKey);

    //设置海缆基础数据
    void setCableBaseDataList(const QList<ZCHX::Data::CableBaseData>& list){mBaseCableDataList = list;}
    ZCHX::Data::CableBaseData getCableByName(const QString& name);
    void setCableInterfaceDataList(const QList<ZCHX::Data::CableInterfaceData>& list){mInterfaceCableDataList = list;}
    ZCHX::Data::CableInterfaceData getInterfaceByName(const QString& name);

public Q_SLOTS:
    void CreateEngine();
    void ScalePlus();
    void ScaleMinus();
    void ScalePlusLight();
    void ScaleMinusLight();

    void ShowAll();
    void ScaleChanged(int action);
    void SliderPressed();
    void SliderReleased();

    void SetPickUpHistoryRadarInfo(qint32 tracknumber);
    void SetPickUpHistoryAisInfo(QString id);
    void SetPickUpRadarInfo(qint32 tracknumber);
    void SetPickUpAisInfo(QString id);
    void SetPickUpPosition(const QString &id);
    void SetPickUpRouteCross(const int id);//设置当前选中的交越点

    void setETool2DrawNull();
    void setETool2DrawArea();
    void setETool2DrawDirAngle();
    void setETool2DrawDistance();
    void setETool2DrawPickup();
    void setETool2DrawTrackTarget();
    void setETool2DrawCameraTrackTarget();
    void setETool2DrawGps();
    void setETool2DrawRouteOrCross();//拾取路由或者交越点
    //雷达特征区域
    void setETool2DrawRadarZONE();
    //以下两项操作属于海图编辑
    void setETool2SelectCommonZONE();
    //防区操作
    void setETool2DrawZONE();
    void setETool2SelectZONE();                 //选取当前防区
    void setETool2moveZONE();                   //移动选中的防区
    void setETool2ctrlZONE();                   //微调控制点
    void setETool2addCtrlZONE();                //添加控制点
    void setETool2delCtrlZONE();                //删除控制点

    //海岸数据
    void setETool2Draw4CoastDataLine();

    //海底管线
    void setETool2Draw4SeabedPipeLineLine();

    //结构物
    void setETool2Draw4StructurePoint();

    //地理区域网络
    void setETool2Draw4AreaNetZone();

    //航道
    void setETool2Draw4ChannelArea();
    void setETool2SelectChannel();                 //选取当前航道
    void setETool2moveChannel();                   //移动选中的航道
    void setETool2ctrlChannel();                   //微调控制点
    void setETool2addCtrlChannel();                //添加控制点
    void setETool2delCtrlChannel();                //删除控制点

    //锚泊
    void setETool2Draw4MooringArea();
    void setETool2SelectMooring();                 //选取当前锚泊
    void setETool2moveMooring();                   //移动选中的锚泊
    void setETool2ctrlMooring();                   //微调控制点
    void setETool2addCtrlMooring();                //添加控制点
    void setETool2delCtrlMooring();                //删除控制点

    //卡口
    void setETool2Draw4CardMouthArea();
    void setETool2SelectCardMouth();                 //选取当前卡口
    void setETool2moveCardMouth();                   //移动选中的卡口
    void setETool2ctrlCardMouth();                   //微调控制点
    void setETool2addCtrlCardMouth();                //添加控制点
    void setETool2delCtrlCardMouth();                //删除控制点

    //船舶计划航线
    void setETool2DrawShipPlanLine();
    void setETool2SelectShipPlanLine();         //选取当前船舶计划线
    void setETool2insertCtrlPointShipPlanLine();//插入船舶计划控制点
    void setETool2moveCtrlPointShipPlanLine();  //微调船舶计划控制点
    void setETool2addCtrlPointShipPlanLine();   //添加船舶计划控制点
    void setETool2delCtrlPointShipPlanLine();   //删除船舶计划控制点
    void setShipPlanWaterDepth(QList<ZCHX::Data::ITF_WaterDepth> &WaterData);

    //设置显示样式
    void setHistoryTrackStyle(const QString &color, const int lineWidth);
    void setPrepushTrackStyle(const QString &color, const int lineWidth);
    void setMapUint(const int uint);
    void SetEnableShipTag(const QList<bool> &bList);
    //环岛线操作
    void setETool2Draw4IslandLine();
    void setETool2Select4IslandLine();          //选取当前环岛线
    void setETool2move4IslandLine();            //移动选中的环岛线
    void setETool2moveCtrlPoint4IslandLine();   //微调环岛线控制点
    void setETool2addCtrlPoint4IslandLine();    //添加环岛线控制点
    void setETool2delCtrlPoint4IslandLine();    //删除环岛线控制点

    //位置标注 Add by yej
    void setETool2DrawLocalMark();

    //set data
//    void setAisData(const std::vector<DrawElement::AisElement> &data);//设置<ais>轨迹数据
//    void setAisDataFromCopy(const std::vector<DrawElement::AisElement> &data); //通过数据copy设置<AIS>数据
    bool setSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data);          //设置船<ais>轨迹数
    void removeAisHistoryData(QString id);
    void setAisData(const QList<ZCHX::Data::ITF_AIS> &data);                //设置<ais>轨迹数据
    void setHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data);         //设置历史回放<ais>轨迹数据
    void setConsAisData(const ZCHX::Data::ITF_AIS &aisdata);
    void setClearHistoryData(bool states);
    DrawElement::AisElement *getCurrentAis();
    DrawElement::AisElement *getHistoryCurrentAis();
    void updateAisElement(const QList<ZCHX::Data::ITF_AIS> &data);
    void setRadarPointData(const std::vector<DrawElement::RadarPoint> &data);//设置雷达点数据
    bool getPreRadarData(ZCHX::Data::ITF_RadarPoint& data, int trackNum);
    void setHistoryRadarPointData(const std::vector<DrawElement::RadarPoint> &data);//设置历史回放雷达点数据
    void setRadarAreaData(const std::vector<DrawElement::RadarArea> & data); //设置雷达扫描区域数据
    void setCameraRodData(const std::vector<DrawElement::CameraRod> & data); //设置摄像杆数据
    void setCameraDevData(const std::vector<DrawElement::CameraDev> & data); //设置相机设备数据
    void setCameraGdyDevData(const std::vector<DrawElement::CameraDev> & data);    //设置光电仪设备数据
    void setCameraPlanDevData(const std::vector<DrawElement::CameraDev> & data);   //设置无人机设备数据
    //设置船上的相机.相机初始化设定时设定相机的站点名对应的是船舶Ais的MMSI.站点的经纬度设定为(0,0);
    void setAisCameraDevData(const std::vector<DrawElement::CameraDev> & data);         //设置船上的相机

    void setPastrolStation(const std::vector<DrawElement::PastrolStation> & data);      //设置巡逻站
    void setWarringZONEData(const std::vector<DrawElement::WarringZONE> & data);        //设置告警区域数据

    void setShipSiumtionData(const std::vector<std::pair<double,double> > &data);          //设置船舶航线模拟数据
    void setAisDataToSiumtion(const QList<ZCHX::Data::ITF_AIS> &data);
    void setShipSimulationExtrapolationDatas(const QList<ZCHX::Data::ITF_AIS> &DataList);   //船舶外推模拟数据
    void setRealTimeShipTrailDatas(const QMap<QString, QList<ZCHX::Data::ITF_AIS> > &DataList);
    void setRadarTailTracklist(const QList<int>& trackList, bool fromMenu = false);
    void setRouteHistogramData(const int RouteId, const QMap<int, QList<int> > &DataList);
    void setRealTimeFlowLineHistogramData(const QMap<int, QList<int> > &DataList);
    void setInTheAreaOfTrackData(const QList<QList<ZCHX::Data::LatLon> > &data);
    void setRolePrivilegeList(const QList<int> &list);
    /*!
     * \brief 更新防区和防线数据, 不会删除现有防区
     * \note 防区和防线通用
     */
    void updateWarrningZone(const QList<ZCHX::Data::ITF_WarringZone> &zonelist);
    void updateWarrningZone(const ZCHX::Data::ITF_WarringZone &zone);
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

    /*!
     * \brief 更新海岸数据
     */
    void updateCoastDataZone(const QList<ZCHX::Data::ITF_CoastData> &zonelist);
    void updateCoastDataZone(const ZCHX::Data::ITF_CoastData &zone);

    /*!
     * \brief 更新海底管线
     */
    void updateSeabedPipeLineZone(const QList<ZCHX::Data::ITF_SeabedPipeLine> &zonelist);
    void updateSeabedPipeLineZone(const ZCHX::Data::ITF_SeabedPipeLine &zone);

    /*!
     * \brief 更新结构物
     */
    void updateStructureZone(const QList<ZCHX::Data::ITF_Structure> &zonelist);
    void updateStructureZone(const ZCHX::Data::ITF_Structure &zone);

    /*!
     * \brief 更新地理区域网络
     */
    void updateAreaNetZone(const QList<ZCHX::Data::ITF_AreaNet> &zonelist);
    void updateAreaNetZone(const ZCHX::Data::ITF_AreaNet &zone);

    /*!
     * \brief 更新航道
     */
    void updateChannelZone(const QList<ZCHX::Data::ITF_Channel> &zonelist);
    void updateChannelZone(const ZCHX::Data::ITF_Channel &zone);

    /*!
     * \brief 更新锚泊
     */
    void updateMooringZone(const QList<ZCHX::Data::ITF_Mooring> &zonelist);
    void updateMooringZone(const ZCHX::Data::ITF_Mooring &zone);

    /*!
     * \brief 更新卡口
     */
    void updateCardMouthZone(const QList<ZCHX::Data::ITF_CardMouth> &zonelist);
    void updateCardMouthZone(const ZCHX::Data::ITF_CardMouth &zone);

    void setIslandLineData(const std::vector<DrawElement::IslandLine> &data);    //设置环岛线数据
    void setLocalMarkData(const std::vector<DrawElement::LocalMark> & data);//设置位置标注数据

    void setCoastData(const std::vector<DrawElement::CoastData> & data); //设置海岸数据
    void importCoastData(const std::vector<std::pair<double, double> > & path); //导入海岸数据

    void setSeabedPipeLine(const std::vector<DrawElement::SeabedPipeLine> & data); //设置海底管线
    void importSeabedPipeLine(const std::vector<std::pair<double, double> > & path); //导入海底管线

    void setStructure(const std::vector<DrawElement::Structure> & data); //设置结构物
    //void importStructure(const std::vector<std::pair<double, double> > & path); //导入结构物

    void setAreaNet(const std::vector<DrawElement::AreaNet> & data); //设置地理区域网络
    void importAreaNet(const std::vector<std::pair<double, double> > & path); //导入地理区域网络

    void setChannel(const std::vector<DrawElement::Channel> & data); //设置航道
    void importChannel(const std::vector<std::pair<double, double> > & path); //导入航道
    void selectChannelLine(int channelId, const ZCHX::Data::ITF_ChannelLine & line); // 选择航道线
    void cancelChannelLine(int channelId); // 取消航道线

    void setMooring(const std::vector<DrawElement::Mooring> & data); //设置锚泊
    void importMooring(const std::vector<std::pair<double, double> > & path); //导入锚泊

    void setCardMouth(const std::vector<DrawElement::CardMouth> & data); //设置卡口
    void importCardMouth(const std::vector<std::pair<double, double> > & path); //导入卡口

    void setFleet(const QMap<QString, ZCHX::Data::ITF_Fleet> &fleetMap); //设置船队

    // 设置预警追溯
    void setShipAlarmAscendMap(const QMap<QString, ZCHX::Data::ITF_ShipAlarmAscend> &shipAlarmAscendMap);

    void setCameraVideoWarnData(const std::vector<DrawElement::CameraVideoWarn> &data); //设置人车船数据
    void setDangerousCircleData(const std::vector<DrawElement::DangerousCircle> &data); //危险圈位置信息
    void setDangerousCircleRange(const double range);                                   //危险圈范围大小
    void setRadarFeatureZoneData(const std::vector<DrawElement::RadarFeatureZone> &data);
    void setCameraObservationZoneData(const std::vector<DrawElement::CameraObservationZone> &data);
    void setRadarEchoData(const QMap<QDateTime,ZCHX::Data::ITF_RadarEchoMap> &data);    //设置雷达回波数据

    //设置回波数据，uDisplayType==1回波显示，uDisplayType==2余辉显示
    void setRadarVideoData(double dCentreLon, double dCentreLat, double dDistance, int uDisplayType, int uLoopNum);

//    void setMultibeamData(const std::vector<DrawElement::Multibeam> &data,
//                          const double dMinLon, const double dMinLat, const double dMaxLon, const double dMaxLat,
//                          const double dMinX, const double dMinY, const double dMinZ, const double dMaxX, const double dMaxY, const double dMaxZ);         //设置多波束数据
    void setMultibeamDataByImg(int iRouteID, const QPixmap &samplePixmap, const QPixmap &objPixmap, double dMinLon, double dMinLat, double dMaxLon, double dMaxLat); //设置多波束数据

    void setAllProjectRouteLineData(const std::vector<DrawElement::RouteLine> &data); //设置所有项目路由数据
    void setRouteLineData(const std::vector<DrawElement::RouteLine> &data);         //设置路由数据(当前项目)
    void setALLSpecialRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data); //设置(所有)特殊路由数据(南方电网)
    void setSpacilRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data);    //设置(当前项目)特殊路由数据(南方电网)
    void setPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data);
    void setRouteCrossData(const std::vector<DrawElement::RouteCross> &data);         //设置交越路由数据（全工程）
    void setHistoryRouteLineData(const std::vector<DrawElement::RouteLine> &data);  //设置历史路由线数据接口(一条路由每个阶段的历史路由)
    void setRouteDefaultCableData(const ZCHX::Data::CableAssembly &data, int uSlack);
    void setRouteConfigDisplay(int uRouteID, bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable, bool bShowIndex);//设置该条路由的点、类型、线、海缆是否显示
    void setRouteDisplayFlag(int uRouteID,int uDisplayFlag);//设置路由显示设计路由还是记录路由或者同时显示
    void closeCurRouteLineData();//关闭当前项目的路由数据
    void deleteRouteLineDataFromProjectID(const int uProjectID);//删除项目ID为uProjectID的路由数据
    void changeCurrentRouteLine(const ZCHX::Data::RouteLine &data,int uRouteID);//改变当前路由线数据接口
    void changeRoutePathAssessStatus(int uAssessStatus,int uRouteID);//改变路由的审批状态;0.未审批 1.已审批
    void setAllProjecrShipPlanLineData(const std::vector<DrawElement::ShipPlanLine> &data);
    void setShipPlanLineData(const std::vector<DrawElement::ShipPlanLine> &data);
    void setShipPlanProjectID(const int ProjectID);
    void setRouteLinePickupLocation(const int uRouteID,const double dLat,const double dLon);//设置路由显示被拾取的点
    void setRouteLinePickupInfor(const double dKp, bool bShowLength, const double dLength,
                                 bool bShowSlope, const double dSlope);//设置路由显示被拾取点的信息
    void setSimulateData(int uSimulateDataID,int uSimulateFlag);//设置模拟数据的ID和模拟的标志(1-rpl表模拟，2-船舶计划表模拟)
    void setSimulateLocation(float fCurSimulateKP);//设置模拟船的位置

    void appendElementItem(const ZCHX::Data::ITF_EleEllipse &item); //设置图元数据
    void appendElementItem(const ZCHX::Data::ITF_EleTriangle &item);//设置图元数据
    void appendElementItem(const ZCHX::Data::ITF_EleLine &item);    //设置图元数据
    void appendElementItem(const ZCHX::Data::ITF_EleRect &item);    //设置图元数据
    void removeElementItem(DrawElement::Element *pitem);            //删除图元
    void clearElementData();                                        //清除图元指针数据

    /**
     * @brief setActiveDrawElement
     * @param pos
     * @author:zxl
     * 拾取元素
     */
    void setActiveDrawElement(const m2::PointD &pos, const QGeoCoordinate &geoPos = QGeoCoordinate(), bool dbClick = false);       //设置选中元素

    /*!
     * \brief 选择各个图层的元素
     * \param pos 鼠标的像素坐标
     * \param geoPos 鼠标的经纬度坐标（lon，lat）
     */
    bool pickUpLayerElement(QPointF pos, const QGeoCoordinate &geoPos);
    void enterPickUp();
    void setPickUpNavigationTarget(const m2::PointD &pos);  //设置导航跟踪目标
    void setSelectedCameraTrackTarget(const m2::PointD &pos);//设置相机跟踪
    void setHoverDrawElement(const m2::PointD &pos);        //设置悬浮状态
    void getPointNealyCamera(const m2::PointD &pos);        //设置手动跟踪
    void routeLineRightKeyOKSlot();                         //路由操作右键菜单确认槽
    void routeLineRightKeyCancelSlot();                     //路由操作右键菜单取消槽
    void releaseDrawStatus();                               //释放绘制状态
    void selectAnRegion();                                  //框选一片区域,统计ais数量
    void setShipSimulation();                               //船舶模拟
    void setLocationMark();                                 //本地标记
    void setFixedReferencePoint();                          //固定参考点
    void invokeHotSpot();                                   //热点
    void invokeLinkageSpot();                               //联动
    void invokeLinkageSpotForRadar();                       //联动
    void setCustomFlowLine();                               //流量统计线
    void setSimulationExtrapolation();                      //模拟外推
    void setHistoryTraces();                                //历史轨迹
    void setRealTimeTraces();                               //实时轨迹
    void setBlackList();                                    //设置黑名单
    void setWhiteList();                                    //设置白名单
    void setCPATrack();                                     //设置CPA
    void setPictureInPicture();                             //画中画
    void setFleet();                                        //设置船队
    void shipPlanLineRightKeyOkSlot();                      //船舶计划操作右键菜单确认槽
    void shipPlanLineRightKeyCancelSlot();                  //船舶计划操作右键菜单确认槽
    void warringZoneRightKeyOkSlot();
    void warringZoneRightKeyCancelSlot();
    void shipSlumtionLineRightKeyOKSlot();
    void shipSlumtionLineRightKeyCancelSlot();
    void customFlowLineRightKeyOKSlot();
    void customFlowLineRightKeyCancelSlot();

    void coastDataRightKeyOkSlot();
    void coastDataRightKeyCancelSlot();

    void seabedPipeLineRightKeyOkSlot();
    void seabedPipeLineRightKeyCancelSlot();

    void structureRightKeyOkSlot();
    void structureRightKeyCancelSlot();

    void areaNetRightKeyOkSlot();
    void areaNetRightKeyCancelSlot();

    void channelRightKeyOkSlot();
    void channelRightKeyCancelSlot();

    void mooringRightKeyOkSlot();
    void mooringRightKeyCancelSlot();

    void cardMouthRightKeyOkSlot();
    void cardMouthRightKeyCancelSlot();

    //防区相关编辑操作
    void zoneEditSelect(const m2::PointD &pt);              //激活一个防区成为当前防区
    void zoneEditSelectCtrlPoint(const m2::PointD &pt);     //选择活动防区内的一个点为控制点
    void zoneEditAddCtrlPoint(const m2::PointD &pt);        //添加一个控制点暂时添加到最后
    void zoneEditDelCtrlPoint(const m2::PointD &pt);        //删除一个控制点
    //路由线相关编辑操作
    void routeOrCrossPickup(const m2::PointD &pt);              //拾取路由或者交越点
    void routeLineEditSelect(const m2::PointD &pt);             //激活一个路由线成为当前路由
    bool routeLineEditSelectCtrlPoint(const m2::PointD &pt);    //选择活动路由线内的一个点为控制点
    void routeLineEditAddCtrlPoint(const QPointF &objCusP);     //从后面添加一个路由线控制点
    void routeLineEditInsertCtrlPoint(const QPointF &objCusP);  //插入一个路由线控制点
    void routeLineEditDelCtrlPoint(const m2::PointD &pt);       //删除一个路由线控制点
    void addRoutePoint(const QPointF &objCusP,DrawElement::RouteLine &objCurrentLine);  //加路由
    void addRoutePointLL(const ms::LatLon& pnt, DrawElement::RouteLine &objCurrentLine);
    void addKPPoint(const QPointF &objCusP,DrawElement::RouteLine &objCurrentLine);     //加pk
    void delRoutePointKPPoint(int uIndex,DrawElement::RouteLine &objCurrentLine);       //删除点
    void moveRoutePointKPPoint(int uIndex,const double dLon,const double dLat,DrawElement::RouteLine &objCurrentLine);//移动点
    //船舶计划相关操作
    void shipPlanEditSelect(const m2::PointD &pt);                  //激活一个船舶计划
    bool shipPlanEditSelectCtrlPoint(const m2::PointD &pt);         //选择活动船舶计划线内的一个点为控制点
    void shipPlanLineEditAddCtrlPoint(const QPointF &objCusP);      //从最后面追加一个船舶计划线控制点
    void shipPlanLineEditInsertCtrlPoint(const QPointF &objCusP);   //插入一个路由线控制点
    void shipPlanLineEditDelCtrlPoint(const m2::PointD &pt);        //删除一个船舶计划线控制点
    void addShipPlanPoint(const QPointF &objCusP,DrawElement::ShipPlanLine &objCurrentLine);  //最后追加点
    void addShipPlanPointLL(const ms::LatLon& pnt, DrawElement::ShipPlanLine &objCurrentLine);
    void addSKPPoint(const QPointF &objCusP,DrawElement::ShipPlanLine &objCurrentLine);       //插入skp点
    void delShipPlanPointKPPoint(int uIndex,DrawElement::ShipPlanLine &objCurrentLine);       //删除点
    void moveShipPlanPointKPPoint(int uIndex,const double dLon,const double dLat,DrawElement::ShipPlanLine &objCurrentLine);//移动点

    //环岛线相关编辑操作
    void islandLineEditSelect(const m2::PointD &pt);            //激活一个环岛线成为当前防区
    void islandLineEditSelectCtrlPoint(const m2::PointD &pt);   //选择活动环岛线内的一个点为控制点
    void islandLineEditAddCtrlPoint(const m2::PointD &pt);      //添加一个环岛线控制点暂时添加到最后
    void islandLineEditDelCtrlPoint(const m2::PointD &pt);      //删除一个环岛线控制点
    void islandLineEditAcitive4uuid(int uuid);                  //由于环岛线的特殊性,可能存在包含的情况.因此可以使用id来设置活动的环岛线

    //航道相关编辑操作
    void channelEditSelect(const m2::PointD &pt);              //激活一个航道成为当前航道
    void channelEditSelectCtrlPoint(const m2::PointD &pt);     //选择活动航道内的一个点为控制点
    void channelEditAddCtrlPoint(const m2::PointD &pt);        //添加一个控制点暂时添加到最后
    void channelEditDelCtrlPoint(const m2::PointD &pt);        //删除一个控制点

    //锚泊相关编辑操作
    void mooringEditSelect(const m2::PointD &pt);              //激活一个锚泊成为当前锚泊
    void mooringEditSelectCtrlPoint(const m2::PointD &pt);     //选择活动锚泊内的一个点为控制点
    void mooringEditAddCtrlPoint(const m2::PointD &pt);        //添加一个控制点暂时添加到最后
    void mooringEditDelCtrlPoint(const m2::PointD &pt);        //删除一个控制点

    //卡口相关编辑操作
    void cardMouthEditSelect(const m2::PointD &pt);              //激活一个卡口成为当前卡口
    void cardMouthEditSelectCtrlPoint(const m2::PointD &pt);     //选择活动卡口内的一个点为控制点
    void cardMouthEditAddCtrlPoint(const m2::PointD &pt);        //添加一个控制点暂时添加到最后
    void cardMouthEditDelCtrlPoint(const m2::PointD &pt);        //删除一个控制点
    //目标关注操作
    void appendFocusRadarPoint(const QList<int>& trackNum);
    void removeFocusRadarPoint(const QList<int>& trackNum);
    void appendFocusAis(const QStringList& id);
    void removeFocusAis(const QStringList& id);

    /**
     * @brief 更新相机状态
     * @param cameid 相机ID数据库ID
     * @param st:状态值
     */
    void zchxUpdateCameraDevStatus(uint cameid, uint st);

    /**
     * @brief 根据相机的ID设置其所在的电线杆的焦点状态
     * @param cameid 相机的ID
     * @param tags 0x00000001：开(有焦点) 0x00000010: 居中
     */
    void zchxCameraRodFocus(uint cameid, int tags);

    void openCameraDisplayWin();//wangliang

    void zchxOpenAisCameraListFromMenu();//通过右键菜单查看相机

    /*!
     * \brief 根据缩放比例, 获取不同的长度尺寸
     */
    int getDrawScaleSize() const;

//    void setMultibeamPixmap(const QPixmap &objPixmap);
    void setRadarVideoPixmap(int uIndex,const QPixmap &objPixmap,const QPixmap &prePixmap);
    void setCurrentRadarVideoPixmap(const QPixmap &objPixmap); //只接收当前的图片
//    void addElement(DrawElement::Element *item);
//    const QList<DrawElement::Element*>& getAllElement() const;
    //设定当前的项目ID
    void setCurrentProjectID(int id);
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
    void setETool2DrawCameraNetGrid(const QSizeF& size, const QString& camera);
    void setCameraNetGridList(const QList<ZCHX::Data::ITF_CameraNetGrid> & list);
    void slotSetRadarTailTrack();


private slots:
    /*!
     * \brief 处理图层的 visible 状态的变化
     */
    void _maplayerVisibleChanged(bool visible);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    /// @name Overriden from QOpenGLWindow.
    //@{
    void mousePressEvent(QMouseEvent * e) override;
    void mouseDoubleClickEvent(QMouseEvent * e) override;
    void mouseMoveEvent(QMouseEvent * e) override;
    void mouseReleaseEvent(QMouseEvent * e) override;
    void wheelEvent(QWheelEvent * e) override;
    void keyPressEvent(QKeyEvent * e) override;
    void keyReleaseEvent(QKeyEvent * e) override;
    bool event(QEvent *e);
    //@}

    void drawLayers(QPainter *painter);

private:
    void SubmitFakeLocationPoint(m2::PointD const & pt);
    void SubmitRoutingPoint(m2::PointD const & pt);
    void ShowInfoPopup(QMouseEvent * e, m2::PointD const & pt);

    void OnViewportChanged(ScreenBase const & screen);
    void UpdateScaleControl();
    df::Touch GetTouch(QMouseEvent * e);
    df::Touch GetSymmetrical(const df::Touch & touch);
    df::TouchEvent GetTouchEvent(QMouseEvent * e, df::TouchEvent::ETouchType type);

    inline int L2D(int px) const { return px * m_ratio; }
    m2::PointD GetDevicePoint(QMouseEvent * e) const;


    void zchxDrawDistance(QPainter *painter);
    void zchxDrawMeasureArea(QPainter *painter);
    void zchxDrawDirAngle(QPainter *painter);           //绘制方向角

    //需要在海图编辑模式下才可使用的
    void zchxDrawShipSimulation(QPainter *painter);     //绘制船舶模拟线
    void zchxDrawCustomFlowLine(QPainter *painter);     //绘制流量线
    void zchxDrawSelectdAre(QPainter *painter);         //绘制框选区域
    void zchxDrawShipPlanLine(QPainter *painter);       //船舶计划航线
    void zchxDrawPoint(QPainter *painter);              //创建结构物
    void zchxDrawZONE(QPainter *painter);               //创建防区
    void zchxDrawRadarFeatureZONE(QPainter *painter);
    void zchxDrawRouteLine(QPainter *painter);          //创建路由
    void zchxDrawMoveRoutePoint(QPainter *painter);
    void zchxDrawMoveShipPlanLinePoint(QPainter *painter);
    void zchxDrawTrafficStatistic(QPainter *painter);
    void zchxDrawRealTimeFlowStatistic(QPainter *painter);
    void drawRouteTrafficHistogram(QPainter *painter, const DrawElement::RouteLine &ele);
    void drawSpacialRouteTrafficHistogram(QPainter *painter, const DrawElement::SpecialRouteLine &ele);
    void zchxShowAllProjectRouteLine(QPainter *painter);//显示所有项目路由
    void zchxShowRouteLine(QPainter *painter);          //显示路由
    void zchxShowALLProjectSpecialRouteLine(QPainter *painter);
    void zchxShowSpecialRouteLine(QPainter *painter);   //显示特殊路由 (南方电网)
    void zchxShowStatisticsShipTrackOnDefence(QPainter *painter);   //绘制防区上统计的轨迹
    void zchxShowShipTrackPixmapOnDefence(QPainter *painter);       //绘制防区上统计的轨迹 叠图片
    //路由显示
    void zchxShowAllProjectRouteLineHighlightRepeat(QPainter *painter);//显示所有项目路由(高亮和防区重复部分)
    void zchxShowRouteLineHighlightRepeat(QPainter *painter);          //显示路由(高亮和防区重复部分)

    void zchxShowRouteCross(QPainter *painter);                 //显示交越路由
    void zchxShowHistoryRouteLine(QPainter *painter);           //显示历史路由

    void zchxShowMultibeam(QPainter *painter);           //显示多波束数据

    void zchxShowRadarVideo(QPainter *painter);           //显示雷达回波
    void zchxShowAfterglow(QPainter *painter);           //显示余辉

    void drawOneDesignRouteLine(QPainter *painter, const DrawElement::RouteLine &ele, bool bCurrentProject);
    void drawOneRecondRouteLine(QPainter *painter, const DrawElement::RouteLine &ele, bool bCurrentProject);
    void drawOneRouteLine(QPainter *painter, const DrawElement::RouteLine &ele, bool bCurrentProject);
    void drawOneRepateRouteLine(QPainter *painter, const DrawElement::RouteLine &ele,QStringList latlon, bool bCurrentProject);

    void zchxShowDangerousCircle(QPainter *painter);    //显示危险圈
    void zchxShowRadarFeatureZone(QPainter * painter);  //显示雷达特征区域
    void zchxShowSimulateShip(QPainter *painter);       //显示模拟船
    void zchxShowShipPlanLine(QPainter *painter);       //显示船舶航线
    void zchxShowAllProjectShipPlanLine(QPainter *painter);     //显示所有船舶计划
    void drawShipPlanLine(QPainter *painter, const DrawElement::ShipPlanLine &ele, bool bCurrentProject);
    void drawShipDesignLine(QPainter *painter, const DrawElement::ShipPlanLine &ele, bool bCurrentProject);
    void drawShipRecondLine(QPainter *painter, const DrawElement::ShipPlanLine &ele, bool bCurrentProject);

    void zchxShowRadarEcho(QPainter *painter);                  //雷达回波显示
    void zchxShowCameraObservationZone(QPainter * painter);     //相机视场显示
    void zchxShowCamera(QPainter * painter);

    void zchxShowTowerRod(QPainter * painter);          //绘制塔杆
    void zchxShowNavigationMark(QPainter * painter);    //绘制航标
    void zchxShowAis(QPainter *painter);                //draw雷达轨迹数据
    void zchxShowSingleAisTrail(QPainter *painter);     //绘制单条船的历史轨迹
    void zchxShowSingleAisHistoryTrail(QPainter *painter);//绘制单条船的历史轨迹(新)
    void zchxShowHistoryAis(QPainter *painter);        //draw历史ais数据
    void zchxShowAisToSiumtion(QPainter *painter);
    void zchxShowShipSiumtion(QPainter *painter);
    void zchxShowShipSiumtionExtrapolation(QPainter *painter);
    void zchxShowRealTimeShipTrail(QPainter *painter);  //实时统计轨迹

    void zchxShowRadarPoint(QPainter *painter);     //draw雷达点数据
    void zchxShowHistoryRadarPoint(QPainter *painter); //draw雷达点数据
    void zchxShowRadarArea(QPainter *painter);      //draw雷达区域范围
    void zchxShowCameraGdyDev(QPainter *painter);   //显示光电仪
    void zchxShowCameraPlanDev(QPainter *painter);  //显示无人机
    void zchxShowPastrolStation(QPainter *painter); //显示巡逻站
    void zchxShowCameraVideoWarn(QPainter *painter);//显示人车船
    void zchxShowWarringZone(QPainter *painter);    //draw告警管理数据
    void zchxShowLocalMark(QPainter *painter);      //显示位置标注
    void zchxShowIslandLine(QPainter *painter);     //显示环岛线
    void zchxShowGpsData(QPainter *painter);        //显示GPS数据
    void zchxShowCameraTackTarget(QPainter *painter);//显示相机跟踪
    void zchxShowCameraDev(QPainter *painter);  //显示相机(枪机,球机);
    void zchxShowCoastDataLINE(QPainter *painter);    //draw海岸数据
    void zchxShowSeabedPipeLineLINE(QPainter *painter);    //draw海底管线
    void zchxShowStructurePoint(QPainter *painter);    //draw结构物
    void zchxShowAreaNetZone(QPainter *painter);    //draw地理区域网络
    void zchxShowChannelZone(QPainter *painter);    //draw航道
    void zchxShowMooringZone(QPainter *painter);    //draw锚泊
    void zchxShowCardMouthZone(QPainter *painter);  //draw卡口

    void zchxShowShipAlarmAscend(QPainter *painter);  //draw预警追溯轨迹线

    void zchxShowEleData(QPainter *painter); //显示自定义图元
    void zchxShowEleEllipseItem(QPainter *painter,const DrawElement::EllipseElement *item);
    void zchxShowEleTriangleItem(QPainter *painter,const DrawElement::TriangleElement *item);
    void zchxShowEleLineItem(QPainter *painter,const DrawElement::LineElement *item);
    void zchxShowEleRectItem(QPainter *painter,const DrawElement::RectElement *item);

    //施工高层图
    void zchxShowHighLevelPixmap(QPainter *painter);
    //施工影像图
    void zchxShowImagePixmap(QPainter *painter);

    //相机网格
    void zchxShowCameraNetGrid(QPainter *painter);
    void zchxShowCameraNetGrid(QPainter *painter, QList<ZCHX::Data::ITF_NetGrid>& list, const QColor& lineColor);
    void zchxShowCameraNetGrid(QPainter *painter, const ms::LatLon& start, const ms::LatLon& end); //临时描绘时使用
    ZCHX::Data::ITF_CameraNetGrid makeCameraGrid(const ms::LatLon& start, const ms::LatLon& end);





    //显示经纬网线
    void zchxShowLatitudeLongitudeGrid(QPainter *painter);
    void calcGridSpacing(int scale, double &MajorSpacing, double &MijorSpacing);
    QString calcGridText(double latlon, double spacing, bool bPostfix);

    void doubleClickCompleteDraw();//双击鼠标完成绘制的操作

    //对点重新编号
    void reStatisticsRoutePoints(std::vector<ZCHX::Data::RoutePoint> &objLinePoints);
    //对路由线的点重新编号及接头和参考点的处理和海缆处理
    void reStatisticsRoutePoints(ZCHX::Data::RouteLine &objCurrentLine);
    //对船舶计划点重新编号
    void reStatisticsShipPlanPoints(ZCHX::Data::ShipPlanLine &objCurrentLine);

    bool isOnlyGetZoneName() const;

    void setTowerRodData();
    void setNavigationMarkData();
    QString getPolygonOfGravityPoint(const std::vector<std::pair<double, double>> &Points); //求多边形重心, 返回 (lon,lat)

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
    static std::vector<std::pair<double, double>> convertLatLonPath(const std::vector<ms::LatLon> &path);
    static void convertLatLonPath(const std::vector<ms::LatLon> &path, std::vector<std::pair<double, double>> &retPath);
    static void convertLatLonPath(const std::vector<ms::LatLon> &path, QList<ZCHX::Data::LatLon> &retPath);

    bool getIsAtiAdaptor() const;
    /*!
        * \brief 设置是否采用ATI显卡的地图引擎初始化方式
        * 地图框架默认会自动根据显卡型号判断使用哪种方式
        * 一下情况下可以使用此接口或者配置文件（IsAtiAdapter字段）强制制定显卡：
        * 1. 地图框架识别显卡型号失败（从data目录下的log文件分析是否出现此情况）
        * 2. 测试或者特殊需要下，强制指定
        */
    void setIsAtiAdaptor(bool isAtiAdaptor);
private:
    double getDistance(const std::vector<std::pair<double, double> > & pointList);
    double getArea(const std::vector<std::pair<double, double> > &pointList);
    //路由显示综合
    void   drawRouteLineItem(QPainter *painter, const DrawElement::RouteLine &ele, const QStringList& lonlat = QStringList());
    bool   isCurrentPointDraw(int index, int size, bool ac);
    void   zchxDrawRoutePoint(const QPointF& pnt, int type, const QString& name, bool isActive, QPainter* painter);
    void   zchxDrawRouteLine(const QLineF &line, const QColor& color,Qt::PenStyle style, int width, QPainter *painter);
    void   zchxDrawRouteText(const QPointF& pos, const QString& text, const QColor& color, QPainter* painter);
    void   zchxDrawLonLatText(const QPointF& pos, double lon, double lat, const QColor& color, QPainter* painter);
    void   zchxDrawOverlapLine(const POLYLINES& polylines,QPainter* painter);
    POLYLINES  parseRouteWarnOverlapSegments(const QStringList& lonlat);
    void   zchxDrawRoutePickUpInfo(const DrawElement::RouteLine &ele);
    void   zchxRoutePickupInfo(QString& text, int& width, int& height, const ZCHX::Data::RoutePoint& point, const QFont& font);
    int    widthOfTextList(const QStringList& list, const QFont& font);
    //绘制备份路由
    void   zchxDrawBackUpRoutePoints(const std::vector<ZCHX::Data::RoutePoint>& pnts, bool active, QPainter* painter);
    void   zchxDrawRoutePoints(const std::vector<ZCHX::Data::RoutePoint>& pnts, const QString& routeName, const QColor& nameColor, int stage, bool active, QPainter* painter);
    void   zchxDrawRouteLinkedInfo(const DrawElement::RouteLine &ele, QPainter* painter);



    QScaleSlider * m_pScale;
    bool m_enableScaleUpdate;

    bool m_emulatingLocation;

    void InitRenderPolicy();

    unique_ptr<gui::Skin> m_skin;

    /**
     * @brief 控制告警区域是否开始移动
     */
    bool zoneIsMove;

    drape_ptr<QtOGLContextFactory> m_contextFactory;
    unique_ptr<Framework> m_framework;
    QOpenGLShaderProgram* m_openglShaderProgram;

    qreal m_ratio;

    ZCHX::Data::ECDIS_PLUGIN_USE_MODEL m_curPluginUserModel; //当前海图的使用模式
    QMutex mutex;
    eTool m_eTool;
    QPointF m_startPos, m_endPos;
    bool isActiveETool;
    bool isActiveDrawDirAngle;

    QList<ZCHX::Data::ITF_AIS> m_pSelectedAISList;         //框选中的ais目标
    ZCHX::Data::ECDIS_PICKUP_TYPE m_curPickupType;         //当前可以拾取的类型
    QSvgRenderer m_svgRender;                              //用来加载和渲染svg图片

    bool enableGetMouseClickPos;
    bool isHideMap;                                                     //是否隐藏海图
    QList<ZCHX::Data::ITF_WaterDepth> m_pWaterDepth;
    QMap<QDateTime,ZCHX::Data::ITF_RadarEchoMap>  m_RadarEchoMap;       //雷达回波信息
    std::vector<DrawElement::Navigation> m_Navigation;                  //导航

    std::vector<DrawElement::DangerousCircle> m_DangerousCircle;        //危险圈数据
//    double dangerous_circle_range;                                      //危险圈大小

    std::vector<DrawElement::RadarFeatureZone> m_radarFeatureZone;
    std::vector<DrawElement::CameraObservationZone> m_cameraObservationZone;    //相机视场

    std::vector<DrawElement::RouteLine> m_historyRouteLine;             //历史路由线
    std::vector<DrawElement::RouteLine> m_RouteLine;                    //路由线(当前项目)

    std::vector<DrawElement::SpecialRouteLine> m_AllProjectSpecialRoute;//所有项目特殊路由(南方电网)
    std::vector<DrawElement::SpecialRouteLine> m_SpecialRouteLine;      //当前项目特殊路由(南方电网)
    std::vector<DrawElement::RouteCross> m_RouteCross;                  //交越路由(全项目)
    std::vector<DrawElement::ShipPlanLine> m_ShipPlanLine;              //船舶计划(当前项目)
    std::vector<DrawElement::ShipPlanLine> m_AllShipPlanLines;          //
    int m_uCurrentProjectID;                                            //当前项目ID
    ZCHX::Data::CableAssembly m_CableAssembly;                          //创建路由时候的默认海缆
    int m_uSlack;                                                       //默认余量
    std::vector<DrawElement::RouteLine> m_AllProjectRouteLine;          //所有项目路由线
    std::vector<DrawElement::Multibeam> m_Multibeam;                    //多波束数据
    std::vector<DrawElement::RadarVideo> m_RadarVideo;                  //回波数据

    int m_uSimulateDataID;//模拟数据的ID（rplID或者船舶计划表ID）
    int m_uSimulateFlag;//1-rpl表模拟，2-船舶计划表模拟
    float  m_fCurSimulateKP;//模拟船当前走的kp值
    QLabel *m_pSimulateLabel;//显示模拟信息
    QLabel *m_pRouteCrossLabel;//显示交越点信息
    QLabel *m_pLinkInforLabel;//显示坡面联动点信息
    QLabel *m_pRoutePickupInforLabel;//显示路由拾取信息

    QPixmap m_samplePixmap;//多波束色阶条图片
    QMap<int, MultiBeamImg> m_multibeamImgMap;//多波束数据图片
    ZCHXDrawMultibeam *m_pDrawMultibeamThread;
    bool m_bMultibeamShow;

    //回波
    QPixmap m_radarVideoPixmap;
    //ZCHXDrawRadarVideo *m_pDrawRadarVideoThread;
    double m_dCentreLon;//回波中心经度
    double m_dCentreLat;//回波中心纬度
    double m_dDistace;  //半径距离
    bool m_bRadarVideoShow;

    int m_uDisplayType;//1回波显示，2余辉显示
    //余辉
    QPixmap m_afterglowPixmap[12];
    int m_uAfterglowType;//1,3,6,12
    qint64 m_uAfterglowIndex;//余辉图片索引

    //当前跟踪导航的船
    QString m_curTrackShip;
    bool m_bSelectedCamera;

    //创建的防区、防线是否有报警类型（横琴没有类型，msd有类型，处理有所不同）暂时用该变量区分
    bool m_bHaveWarningType;

    bool m_bNavigateionging;    //是否正在导航
    double m_dRotateAngle;      //地图旋转的角度
    bool m_bRouteLineMove;
    bool m_bShipPlanLineMove;
    bool m_bRouteHistogram;
    ZCHX::Data::ITF_CameraTrackTarget m_cameraTrackTarget;
    bool m_bCameraTargerTrack;              //enableCameraTargerTrack是否进行相机联动跟踪
    QMap<int, double> m_pRadarPointHistory; //雷达的转向数据(历史)
    //add 移动路由点所选择到点索引
    int m_uSelectedRoutePointIndex;
    int m_uSelectedShipPlanPointIndex;
    ms::LatLon m_eToolPoint;
    std::vector<ms::LatLon> m_eToolPoints;

    QHash<QString, DrawElement::AisElement*> m_aisData;         //船舶模拟数据
    QHash<QString, DrawElement::AisElement*> m_aisMap;          //AIS对象
    QMap<QString, QList<ZCHX::Data::ITF_AIS> > m_aisTraceMap;    //船舶历史轨迹
    QMap<QString, int> m_aisIndexMap;
    QHash<QString, DrawElement::AisElement*> m_historyAisMap;   //AIS历史对象
    std::vector<DrawElement::RadarPoint> m_RadarPoint;          //雷达点数据
    std::vector<DrawElement::RadarPoint> m_HistoryRadarPoint;   //雷达历史点数据
    //QHash<qint32, DrawElement::AisElement*> m_RadarPoint;     //雷达点数据
    std::vector<DrawElement::RadarArea> m_RadarArea;            //雷达扫描区域数据



    std::vector<DrawElement::CameraVideoWarn> m_CameraVideoWarn;//人车船
    std::vector<DrawElement::PastrolStation> m_PastrolStation;  //巡逻站
    std::vector<DrawElement::Element *> m_eleData;              //图元数据
    std::vector<DrawElement::LocalMark> m_LocalMark;            //位置标注对像
    std::list<std::shared_ptr<ZCHX::Data::GPSPoint>> m_gpsTracks;


//    QSet<QString> m_fleetSet;                                      //船队

    QMap<QString, ZCHX::Data::ITF_ShipAlarmAscend> m_shipAlarmAscendMap; // 设置预警追溯

    std::vector<ZCHX::Data::ITF_LocalMark> m_pTowerRod;         //塔杆 (南方电网)
    std::vector<ZCHX::Data::ITF_LocalMark> m_pNavigationMark;   //航标 (南方电网)
    std::vector<std::pair<double, double>> m_pShipSiumtion;     //船舶模拟航线
    QList<ZCHX::Data::ITF_AIS>  m_pShipSiumtionExtrapolation;
    QMap<QString, QList<ZCHX::Data::ITF_AIS> > m_pRealTimeShipTrail;
    int m_pCurrentRouteIdForFlow;
    QMap<int, QList<int> > m_pRouteHistogramData;
    QMap<int, QList<int> > m_pRealTimeHistogramData;
    QList<QList<ZCHX::Data::LatLon> > m_pTrackDataOnDefence;    //在防区里面的轨迹
    QList<std::pair<double,double>> m_lDefencePos;              //叠轨迹图片的防区的经纬度
    QList<int> m_pRolePrivilegeList;
    QPixmap m_AISTrackPixmap;
    ZCHX::Data::CustomFlowLine m_pRealTimeFlowLine;             //实时流量统计线
    QMutex m_gpsTracksMutex;

    int m_pShipPlanProjectID;
    //当前选中的船舶的雷达trackNumber或者AIS is
    QString m_curActivShip;

    m2::PointD m_curPt;
    int m_iTargetType;      //画中画目标类型 1AIS 2雷达

    int m_minVisibleZoom;  //地图可见的最小缩放级别

    QLabel m_hoverEventLabel; //悬浮对像信息签

    int m_flashAlhpa; //控制闪烁
    int m_flashAlphaStep; //控制闪烁的速度

    void *m_SelectedElementInVector;         //选中图元所在的组
    int   m_SelectedIndex;                   //图元所在的数组的下标

    /**
     * @brief 用于记录当前选中防区列表中的的索引值(-1为无效果)
     */
    uint m_curWarringZoneIdx;
    bool m_bIsOpenMeet;         //是否开启会遇显示

    std::shared_ptr<DrawElement::Element> m_currentSelectElement;
    QString m_mapUnit;

    QStringList m_cableJoin3List;
    QStringList m_cableJoinList;
    QStringList m_renchechuanList;
    QString m_pSelectionPoint;
    bool m_leftMousePress;

    int m_projectionLocation;
    int m_samplerLocation;
    bool m_bFirstPaintEvent;
    //显示样式
    QString m_sHistoryTrackStyle;   //历史轨迹线
    int m_iHistoryTrackWidth;
    QString m_sPrepushTrackStyle;   //预推轨迹线
    int m_iPrepushTrackWidth;
    QList<bool> m_pShipTagState;    //船舶标签状态
    int m_iUint;                    //设置单位 1为海里 2为千米  3为米

    bool m_isAtiAdaptor;

    bool m_bUseRightKey;//是否使用右键
    //路由信息控制显示
    bool mShowRoutePoint;  //0 隐藏点  1 显示点
    bool mShowRouteType;  //0隐藏类型  1显示类型
    bool mShowRouteLine;    //0隐藏线  1显示线
    bool mShowRouteIndex;
    bool mShowAcFlag;
    //海缆基础数据
    QList<ZCHX::Data::CableBaseData>        mBaseCableDataList;
    QList<ZCHX::Data::CableInterfaceData>        mInterfaceCableDataList;
    //单船历史轨迹设定
    int         mAisHistoryMaxNum;
    bool        mReplaceTrackWherOver;
    //
    QList<int>  mRadarConcernList;        //雷达关注
    QStringList mAisConcernList;         //Ais关注列表
    QList<int>  mRadarTailTrackList;      //雷达尾迹列表
    //
    QList<ZCHX::Data::ITF_CameraNetGrid>            mCameraNetGridList;
    QSizeF                                           mCameraNetGridSize;
    QString                                         mCurNetGridCamera;
};
}
#endif

#endif // ZCHXMAPWIDGET_H
