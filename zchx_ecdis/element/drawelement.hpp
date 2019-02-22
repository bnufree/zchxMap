#pragma once

#include "IDrawElement.hpp"
#include "aiselement.hpp"
#include "camerarodelement.h"
#include "cameraelement.h"
#include "radarelement.h"
#include <QtCore>

namespace qt
{
//环岛线
class  IslandLine : public Element
{
public:
    explicit IslandLine(const ZCHX::Data::ITF_IslandLine &ele/*const std::vector<std::pair<double, double> > &path, const RADARTYPE &t*/);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    std::string content() const;
    void setContent(const std::string &content);

    ZCHX::Data::ITF_IslandLine data() const;

    /**
     * @brief 改变第几个点的位置
     * @param idx
     * @param lat
     * @param lon
     */
    void changePathPoint(double lat, double lon);

    /**
     * @brief 整体平移
     * @param lat
     * @param lon
     */
    void moveTo(double lat, double lon);

    /**
     * @brief updateOldPath
     * 更新初始路径
     */
    void updateOldPath();

    /**
     * @brief activePathPoint
     * @return
     */
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);
    void delPathPoint(int idx);
    void addCtrlPoint(std::pair<double, double> ps);

    QString color() const;
    void setColor(QString color);

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    RADARTYPE        m_type;
    std::string      m_name;
    int              m_id;
    std::string      m_content;
    ZCHX::Data::ITF_IslandLine m_data;
    int              m_activePathPoint; //当前处于活动的路径点的索引
    QString          m_color;
};
//路由线
class RouteLine:public Element
{
public:
    RouteLine(const ZCHX::Data::RouteLine &ele);
    void setNewChangeData(const ZCHX::Data::RouteLine &data);   //设置新的从数据库获取的数据
    void setData(const ZCHX::Data::RouteLine &data);            //设置编辑数据
    ZCHX::Data::RouteLine data() const;
    ZCHX::Data::RouteLine backData() const;
    void restoreBackData();                                     //还原到备份数据
    bool dataChanged();                                         //备份数据和编辑数据是否改变
    void setLinkInfor(bool bLinked,const double dLat,const double dLon);
    bool getLinked() const;
    void getLinkLatLon(double &dLat,double &dLon) const;
    void setLinkShowInfor(const double dKp,bool bShowLength,
                          const double dLength,bool bShowSlope,const double dSlope);
    void getLinkShowInfor(double &dKp,bool &bShowLength,
                          double &dLength,bool &bShowSlope,double &dSlope) const;
    bool routeApprovaled();                                     //该条路由线是否审批；
    void setRouteAssessStatus(int uAssessStatus);               //0.未审批 1.已审批
    //设置该条路由的点、类型、线、海缆是否显示
    void setRouteConfigDisplay( bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable);
    //获取该条路由的点、类型、线、海缆是否显示
    void getRouteConfigDisplay( int& bShowPoint,int& bShowType,int& bShowLine,int& bShowCable)const;

    //设置显示设计还是记录路由
    void setRouteDisplayFlag(const int uFlag);
    //获取显示设计还是记录路由
    int getRouteDisplayFlag()const;

    //设置该条路由是否显示拾取信息
    void setIsPickupShowInfor(bool bPickupShowInfor);
    bool getPickupShowInfor() const;
    //设置在那个kp值显示拾取信息
    void setShowInforKP(float fShowInforKP);
    float getShowInforKP() const;
    //设置拾取的经纬度
    void setPickupLonLat(double dLon,double dLat);
    void getPickupLonLat(double &dLon,double &dLat)const;
    //设置拾取点是从第几个点（路由点索引）到下一个点之间
    void setPickupRoutePointIndex(int uPickupRoutePointIndex);
    int getPickupRoutePointIndex()const;

    bool getSelectedLatlonPoint(std::pair<double, double> &latlon);

    /**
     * @brief 改变第几个点的位置
     * @param idx
     * @param lat
     * @param lon
     */
    void changePathPoint(double lat, double lon);

    /**
     * @brief 整体平移
     * @param lat
     * @param lon
     */
    void moveTo(double lat, double lon);

    /**
     * @brief activePathPoint
     * @return
     */
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);
    void delPathPoint(int idx);

    void updateGeometry(QPointF, int){}

    //获取路由对应的项目ID
    int projectID() const { return data().projectID;}

    //路由显示标记
    enum DISPLAY_MODE{
        DISPLAY_DESIGN_ROUTE = 1,
        DISPLAY_RECORD_ROUTE = 2,
        DISPLAY_ALL_ROUTE = DISPLAY_DESIGN_ROUTE | DISPLAY_RECORD_ROUTE,
    };

private:
    ZCHX::Data::RouteLine m_data;
    ZCHX::Data::RouteLine m_backData;   //备份的数据
    int m_activePathPoint;              //当前处于活动的路径点的索引
    //以下是与坡面联动使用的变量
    bool m_bLinked;
    double m_dLinkLat;
    double m_dLinkLon;
    double m_dKp;
    bool m_bShowLength;
    double m_dLength;
    bool m_bShowSlope;
    double m_dSlope;
    //以下是配置是否显示点、类型、线、海缆是变量
    bool mShowPoint;  //0 隐藏点  1 显示点
    bool mShowType;  //0隐藏类型  1显示类型
    bool mShowLine;    //0隐藏线  1显示线
    bool mShowCable;   //0 隐藏海缆 1渲染海缆
    //是否显示路由的上述信息，全局控制，不再是通过局部的单个路由

    //以下是拾取路由线的变量
    bool m_bPickupShowInfor;
    float m_fShowInforKP;
    double m_dLon;
    double m_dLat;
    int m_uPickupRoutePointIndex;

    int m_uRouteDisplayFlag;//1:显示设计路由；2显示记录路由;3:一起显示

};

//路由交越数据
class RouteCross:public Element
{
public:
    RouteCross(const ZCHX::Data::ITF_RouteCross &ele);
    ZCHX::Data::ITF_RouteCross data() const;
private:
    ZCHX::Data::ITF_RouteCross m_data;
};
//多波束数据
class Multibeam:public Element
{
public:
    Multibeam(const ZCHX::Data::ITF_Multibeam &ele);
    ZCHX::Data::ITF_Multibeam data()const;
    void setData(const ZCHX::Data::ITF_Multibeam &data);
private:
    ZCHX::Data::ITF_Multibeam m_data;
};

//船舶计划航线
class ShipPlanLine:public Element
{
public:
    ShipPlanLine(const ZCHX::Data::ShipPlanLine &ele);
    void setNewChangeData(const ZCHX::Data::ShipPlanLine &data);//设置新的从数据库获取的数据
    void setData(const ZCHX::Data::ShipPlanLine &data);         //设置编辑数据
    ZCHX::Data::ShipPlanLine data() const;
    ZCHX::Data::ShipPlanLine backData() const;
    void restoreBackData();                                     //还原到备份数据
    bool dataChanged();                                         //备份数据和编辑数据是否改变

    //设置显示设计还是记录路由
    void setShipDisplayFlag(const int uFlag);
    //获取显示设计还是记录路由
    int getShipDisplayFlag()const;

    bool getSelectedLatlonPoint(std::pair<double, double> &latlon);

    /**
     * @brief 改变第几个点的位置
     * @param idx
     * @param lat
     * @param lon
     */
    void changePathPoint(double lat, double lon);

    /**
     * @brief 整体平移
     * @param lat
     * @param lon
     */
    void moveTo(double lat, double lon);

    /**
     * @brief activePathPoint
     * @return
     */
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);
    void delPathPoint(int idx);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ShipPlanLine m_data;
    ZCHX::Data::ShipPlanLine m_backData;    //备份的数据
    int m_activePathPoint;                  //当前处于活动的路径点的索引
    int m_uDisplayFlag;//1:显示设计船舶；2显示记录船舶;0:一起显示
};

//特殊路由(南方电网)
class SpecialRouteLine:public Element
{
public:
    SpecialRouteLine(const ZCHX::Data::SpecialRouteLine &ele);
    void setNewChangeData(const ZCHX::Data::SpecialRouteLine &data);//设置新的从数据库获取的数据
    void setData(const ZCHX::Data::SpecialRouteLine &data);         //设置编辑数据
    ZCHX::Data::SpecialRouteLine data() const;
    bool getSelectedLatlonPoint(std::pair<double, double> &latlon);
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);

private:
    ZCHX::Data::SpecialRouteLine m_data;
    int m_activePathPoint;                      //当前处于活动的路径点的索引
};

class MoveElement : public Element
{
public:
    explicit MoveElement();
    void setData();

    virtual std::vector<std::pair<double, double> > path() const = 0;
    virtual void setPath(const std::vector<std::pair<double, double> > &path) = 0;

    virtual std::vector<std::pair<double, double> > & getPath() = 0;

    /**
     * @brief 改变第几个点的位置
     * @param idx
     * @param lat
     * @param lon
     */
    void changePathPoint(double lat, double lon);

    /**
     * @brief 整体平移
     * @param lat
     * @param lon
     */
    void moveTo(double lat, double lon);

    /**
     * @brief activePathPoint
     * @return
     */
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);
    void delPathPoint(int idx);
    void addCtrlPoint(std::pair<double, double> ps);

    //指定点是否在区域线上
    bool contains(zchxMapFrameWork *framework, int range, double x, double y) const;

    void updateGeometry(QPointF, int){}
protected:
    int              m_activePathPoint; //当前处于活动的路径点的索引
};

//防区及防线
class WarringZONE : public Element
{
public:
    explicit WarringZONE(const ZCHX::Data::ITF_WarringZone &ele/*const std::vector<std::pair<double, double> > &path, const RADARTYPE &t*/);

    void setData(const ZCHX::Data::ITF_WarringZone &ele);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_WarringZone data() const;

    /**
     * @brief 改变第几个点的位置
     * @param idx
     * @param lat
     * @param lon
     */
    void changePathPoint(double lat, double lon);

    /**
     * @brief 整体平移
     * @param lat
     * @param lon
     */
    void moveTo(double lat, double lon);

    /**
     * @brief updateOldPath
     * 更新初始路径
     */
    void updateOldPath();

    /**
     * @brief activePathPoint
     * @return
     */
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);
    void delPathPoint(int idx);
    void addCtrlPoint(std::pair<double, double> ps);

    QString getDefenceColor(){return m_data.fillColor;}
    QString getWarnColor(){return m_data.warnColor;}
    //指定点是否在告警区域线上
    bool contains(zchxMapFrameWork *framework, int range, double x, double y) const;

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_WarringZone m_data;
    int              m_activePathPoint; //当前处于活动的路径点的索引
};

//海岸数据
class CoastData : public Element
{
public:
    explicit CoastData(const ZCHX::Data::ITF_CoastData &ele);

    void setData(const ZCHX::Data::ITF_CoastData &ele);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_CoastData data() const;

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    //RADARTYPE        m_type;
    std::string      m_name;
    ZCHX::Data::ITF_CoastData m_data;
};

//海底管线
class SeabedPipeLine : public Element
{
public:
    explicit SeabedPipeLine(const ZCHX::Data::ITF_SeabedPipeLine &ele);

    void setData(const ZCHX::Data::ITF_SeabedPipeLine &ele);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_SeabedPipeLine data() const;

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    //RADARTYPE        m_type;
    std::string      m_name;
    ZCHX::Data::ITF_SeabedPipeLine m_data;
};

//结构物
class Structure : public Element
{
public:
    explicit Structure(const ZCHX::Data::ITF_Structure &ele);

    void setData(const ZCHX::Data::ITF_Structure &ele);

    std::pair<double, double> point() const;
    void setPoint(const std::pair<double, double> &point);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_Structure data() const;

    void updateGeometry(QPointF, int){}
private:
    std::pair<double, double> m_point; //初始路径
    //RADARTYPE        m_type;
    std::string      m_name;
    ZCHX::Data::ITF_Structure m_data;
};

//地理区域网络
class AreaNet : public Element
{
public:
    explicit AreaNet(const ZCHX::Data::ITF_AreaNet &ele);

    void setData(const ZCHX::Data::ITF_AreaNet &ele);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_AreaNet data() const;

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    //RADARTYPE        m_type;
    std::string      m_name;
    ZCHX::Data::ITF_AreaNet m_data;
};

//航道
class Channel : public MoveElement
{
public:
    explicit Channel(const ZCHX::Data::ITF_Channel &ele);

    void setData(const ZCHX::Data::ITF_Channel &ele);

    virtual std::vector<std::pair<double, double> > path() const;
    virtual void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_Channel data() const;

    void setLineSelected(int i, bool selectStatus);

    virtual std::vector<std::pair<double, double> > & getPath();

    void updateOldPath();

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    //RADARTYPE        m_type;
    std::string      m_name;
    ZCHX::Data::ITF_Channel m_data;
};

//锚泊
class Mooring : public MoveElement
{
public:
    explicit Mooring(const ZCHX::Data::ITF_Mooring &ele);

    void setData(const ZCHX::Data::ITF_Mooring &ele);

    virtual std::vector<std::pair<double, double> > path() const;
    virtual void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_Mooring data() const;

    virtual std::vector<std::pair<double, double> > & getPath();

    void updateOldPath();

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    //RADARTYPE        m_type;
    std::string      m_name;
    ZCHX::Data::ITF_Mooring m_data;
};

//卡口
class CardMouth : public MoveElement
{
public:
    explicit CardMouth(const ZCHX::Data::ITF_CardMouth &ele);

    void setData(const ZCHX::Data::ITF_CardMouth &ele);

    virtual std::vector<std::pair<double, double> > path() const;
    virtual void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_CardMouth data() const;

    virtual std::vector<std::pair<double, double> > & getPath();

    void updateOldPath();

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    //RADARTYPE        m_type;
    std::string      m_name;
    ZCHX::Data::ITF_CardMouth m_data;
};

// 预警追溯
class ShipAlarmAscend : public MoveElement
{
public:
    explicit ShipAlarmAscend(const ZCHX::Data::ITF_ShipAlarmAscend &ele);

    void setData(const ZCHX::Data::ITF_ShipAlarmAscend &ele);

    ZCHX::Data::ITF_ShipAlarmAscend data() const;

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_ShipAlarmAscend m_data;
};

//回波数据
class RadarVideoElement:public Element
{
public:
    RadarVideoElement(const ZCHX::Data::ITF_RadarVideo &ele,int uIndex = 0);
    ZCHX::Data::ITF_RadarVideo data()const;
    void setData(const ZCHX::Data::ITF_RadarVideo &data);
    void setMsgIndex(int uIndex);
    int getMsgIndex();
private:
    ZCHX::Data::ITF_RadarVideo m_data;
    int m_uMsgIndex;//扇区索引
};

//海图绘制元素
class EllipseElement : public Element
{
public:
    explicit  EllipseElement(const ZCHX::Data::ITF_EleEllipse &ele);
    ZCHX::Data::ITF_EleEllipse ele() const;
    void setEle(const ZCHX::Data::ITF_EleEllipse &ele);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_EleEllipse m_ele;

};

//三角形
class TriangleElement : public Element
{
public:
    explicit TriangleElement(const ZCHX::Data::ITF_EleTriangle &ele);

    ZCHX::Data::ITF_EleTriangle ele() const;
    void setEle(const ZCHX::Data::ITF_EleTriangle &ele);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_EleTriangle m_ele;
};

//直线
class LineElement : public Element
{
public:
    explicit LineElement(const ZCHX::Data::ITF_EleLine &ele);

    ZCHX::Data::ITF_EleLine ele() const;
    void setEle(const ZCHX::Data::ITF_EleLine &ele);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_EleLine m_ele;
};

//矩形
class RectElement : public Element
{
public:
    explicit  RectElement(const ZCHX::Data::ITF_EleRect &ele);

    ZCHX::Data::ITF_EleRect ele() const;
    void setEle(const ZCHX::Data::ITF_EleRect &ele);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_EleRect m_ele;
};
/*标准图元*/

class  PastrolStation : public Element
{
public:
    explicit PastrolStation(const ZCHX::Data::ITF_PastrolStation &data);
    ZCHX::Data::ITF_PastrolStation data() const;

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_PastrolStation m_data;
};

//位置标注
class LocalMark :public Element
{
public:
    explicit LocalMark(const ZCHX::Data::ITF_LocalMark & data);

    ZCHX::Data::ITF_LocalMark data() const;

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_LocalMark m_data;
};

//人车船
class  CameraVideoWarn : public Element
{
public:
    explicit CameraVideoWarn(const ZCHX::Data::ITF_CameraVideoWarn & data);

//    ZCHX::Data::ITF_PointNealyCamera data() const;
    const ZCHX::Data::ITF_CameraVideoWarn & data() const;
    uint getTargetStatus() const;
    uint getTargetType() const;
    uint getAlarmType() const;
    QString getObjId() const;
    QString getAlarmColor() const;

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_CameraVideoWarn m_data;
};

//导航
class  Navigation : public Element
{
public:
    explicit Navigation(const ZCHX::Data::ITF_Navigation & data);
    ZCHX::Data::ITF_Navigation data() const;
    void setData(const ZCHX::Data::ITF_Navigation &data);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_Navigation m_data;

};

//危险圈
class DangerousCircle : public Element
{
public:
    explicit DangerousCircle(const ZCHX::Data::ITF_DangerousCircle &data);
    ZCHX::Data::ITF_DangerousCircle data() const;
    void setDangerousCircleData(const ZCHX::Data::ITF_DangerousCircle &data);
    void setDangerousCircleRange(int range);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_DangerousCircle  m_data;
    int m_range;
};

class RadarFeatureZone : public Element
{
public:
    explicit RadarFeatureZone(const ZCHX::Data::ITF_RadarFeaturesZone &data);
    ZCHX::Data::ITF_RadarFeaturesZone data() const;
    void setRadarFeatureZoneData(const ZCHX::Data::ITF_RadarFeaturesZone &data);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_RadarFeaturesZone  m_data;
};

class CameraObservationZone : public Element
{
public:
    explicit CameraObservationZone(const ZCHX::Data::ITF_CameraObservationZone &data);
    ZCHX::Data::ITF_CameraObservationZone data() const;
    void setRadarFeatureZoneData(const ZCHX::Data::ITF_CameraObservationZone &data);

    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_CameraObservationZone  m_data;
};
//namespace end
}
