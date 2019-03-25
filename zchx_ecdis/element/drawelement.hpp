#pragma once

#include "IDrawElement.hpp"
#include "aiselement.hpp"
#include "rodelement.h"
#include "cameraelement.h"
#include "radarelement.h"
#include "ipcelement.h"
#include "videotargetelement.h"
#include "islandlineelement.h"
#include "shipalarmascendelement.h"
#include "structureelement.h"
#include "patrolstationelement.h"
#include "gridelement.h"
#include "radarvideoelement.h"
#include "zchxmoveelements.h"
#include "zchxfixelements.h"
#include <QtCore>

namespace qt
{

//路由线
class RouteLineElement:public Element
{
    Q_OBJECT
public:
    explicit RouteLineElement(const ZCHX::Data::RouteLine &ele);
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

    void updateGeometry(QPointF, qreal){}

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
class RouteCrossElement:public Element
{
public:
    explicit RouteCrossElement(const ZCHX::Data::ITF_RouteCross &ele);
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
class ShipPlanLineElement:public Element
{
public:
    explicit ShipPlanLineElement(const ZCHX::Data::ShipPlanLine &ele);
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

    void updateGeometry(QPointF, qreal){}
private:
    ZCHX::Data::ShipPlanLine m_data;
    ZCHX::Data::ShipPlanLine m_backData;    //备份的数据
    int m_activePathPoint;                  //当前处于活动的路径点的索引
    int m_uDisplayFlag;//1:显示设计船舶；2显示记录船舶;0:一起显示
};

//特殊路由(南方电网)
class SpecialRouteLineElement:public Element
{
public:
    explicit SpecialRouteLineElement(const ZCHX::Data::SpecialRouteLine &ele);
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


/*标准图元*/

class  PastrolStation : public Element
{
public:
    explicit PastrolStation(const ZCHX::Data::ITF_PastrolStation &data);
    ZCHX::Data::ITF_PastrolStation data() const;

    void updateGeometry(QPointF, qreal){}
private:
    ZCHX::Data::ITF_PastrolStation m_data;
};


//namespace end
}
