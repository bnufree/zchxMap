#include "drawelement.hpp"
#include "zchxmapframe.h"
#include "zchxmapwidget.h"
#include "zchxMapDatautils.h"
#include "profiles.h"
#include <QDebug>
#include "zchxutils.hpp"
#include "map_layer/zchxmaplayermgr.h"

namespace qt{
















////////////////////
/// \brief 环岛线

RouteLineElement::RouteLineElement(const ZCHX::Data::RouteLine &ele)
    :Element(0,0, 0, ZCHX::Data::ELE_ROUTE_LINE)
{
    m_data = ele;
    m_backData = ele;
    m_bLinked = false;
    m_dLinkLat = 0;
    m_dLinkLon = 0;
    m_dKp = 0;
    m_bShowLength = false;
    m_dLength = 0;
    m_bShowSlope = false;
    m_dSlope = 0;
    mShowPoint = true;
    mShowType = true;
    mShowLine = true;
    mShowCable = true;

    m_uRouteDisplayFlag = DISPLAY_DESIGN_ROUTE;//默认显示设计路由

    m_bPickupShowInfor = false;
    m_fShowInforKP = 0;
}

void RouteLineElement::setNewChangeData(const ZCHX::Data::RouteLine &data)
{
    m_data = data;
    m_backData = data;
}

void RouteLineElement::setData(const ZCHX::Data::RouteLine &data)
{
    m_data = data;
}

ZCHX::Data::RouteLine RouteLineElement::data() const
{
    return m_data;
}

ZCHX::Data::RouteLine RouteLineElement::backData() const
{
    return m_backData;
}

void RouteLineElement::restoreBackData()
{
    m_data = m_backData;
}

bool RouteLineElement::dataChanged()
{
    if(m_data == m_backData)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void RouteLineElement::setLinkInfor(bool bLinked, const double dLat, const double dLon)
{
    m_bLinked = bLinked;
    m_dLinkLat = dLat;
    m_dLinkLon = dLon;
}

bool RouteLineElement::getLinked() const
{
    return m_bLinked;
}

void RouteLineElement::getLinkLatLon(double &dLat, double &dLon) const
{
    dLat = m_dLinkLat;
    dLon = m_dLinkLon;
}

void RouteLineElement::setLinkShowInfor(const double dKp, bool bShowLength, const double dLength, bool bShowSlope, const double dSlope)
{
     m_dKp = dKp;
     m_bShowLength = bShowLength;
     m_dLength = dLength;
     m_bShowSlope = bShowSlope;
     m_dSlope = dSlope;
}

void RouteLineElement::getLinkShowInfor(double &dKp, bool &bShowLength, double &dLength, bool &bShowSlope, double &dSlope) const
{
    dKp = m_dKp;
    bShowLength = m_bShowLength;
    dLength = m_dLength;
    bShowSlope = m_bShowSlope;
    dSlope = m_dSlope;
}

bool RouteLineElement::routeApprovaled()
{
    if(m_data.routeAssessStatus == 0)
    {
        return false;
    }
    else
    {
       return true;
    }
}

void RouteLineElement::setRouteAssessStatus(int uAssessStatus)
{
    m_data.routeAssessStatus = uAssessStatus;
}

void RouteLineElement::setRouteConfigDisplay( bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable)
{
    mShowPoint = bShowPoint;
    mShowType = bShowType;
    mShowLine = bShowLine;
    mShowCable = bShowCable;
}

void RouteLineElement::getRouteConfigDisplay(int& bShowPoint,int& bShowType,int& bShowLine,int& bShowCable) const
{
    bShowPoint = mShowPoint;
    bShowType = mShowType;
    bShowLine = mShowLine;
    bShowCable = mShowCable;
}

void RouteLineElement::setRouteDisplayFlag(const int uFlag)
{
    m_uRouteDisplayFlag = uFlag;
}

int RouteLineElement::getRouteDisplayFlag()const
{
    return m_uRouteDisplayFlag;
}

void RouteLineElement::setIsPickupShowInfor(bool bPickupShowInfor)
{
    m_bPickupShowInfor = bPickupShowInfor;
}

bool RouteLineElement::getPickupShowInfor()const
{
    return m_bPickupShowInfor;
}

void RouteLineElement::setShowInforKP(float fShowInforKP)
{
    m_fShowInforKP = fShowInforKP;
}

float RouteLineElement::getShowInforKP()const
{
    return m_fShowInforKP;
}

void RouteLineElement::setPickupLonLat(double dLon, double dLat)
{
    m_dLon = dLon;
    m_dLat = dLat;
}

void RouteLineElement::getPickupLonLat(double &dLon, double &dLat) const
{
    dLon = m_dLon;
    dLat = m_dLat;
}

void RouteLineElement::setPickupRoutePointIndex(int uPickupRoutePointIndex)
{
    m_uPickupRoutePointIndex = uPickupRoutePointIndex;
}

int RouteLineElement::getPickupRoutePointIndex() const
{
    return m_uPickupRoutePointIndex;
}

bool RouteLineElement::getSelectedLatlonPoint(std::pair<double, double> &latlon)
{
    if(m_activePathPoint<0||m_activePathPoint>=m_data.points.size())
        return false;
    else
    {
        latlon.first = m_data.points[m_activePathPoint].lat;
        latlon.second = m_data.points[m_activePathPoint].lon;
        return true;
    }
}

void RouteLineElement::changePathPoint(double lat, double lon)
{
    if(0 <= m_activePathPoint  && m_activePathPoint < m_data.points.size())
    {
        m_data.points[m_activePathPoint].lat = lat;
        m_data.points[m_activePathPoint].lon = lon;
    }
}

void RouteLineElement::moveTo(double lat, double lon)
{
    for(int i= 0; i< m_data.points.size(); ++i)
    {
        m_data.points[i].lat  +=  lat;
        m_data.points[i].lon +=  lon;
    }
}


int RouteLineElement::activePathPoint() const
{
    return m_activePathPoint;
}

void RouteLineElement::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void RouteLineElement::delPathPoint(int idx)
{
    if(m_data.points.size() > idx)
    {
        m_data.points.erase(m_data.points.begin()+idx);
        m_activePathPoint = -1;
    }
}

ShipPlanLineElement::ShipPlanLineElement(const ZCHX::Data::ShipPlanLine &ele)
    :Element(0, 0, 0, ZCHX::Data::ELE_PLAN_LINE)
{
    m_data = ele;
    m_backData = ele;
    m_uDisplayFlag = 1;
}

void ShipPlanLineElement::setNewChangeData(const ZCHX::Data::ShipPlanLine &data)
{
    m_data = data;
    m_backData = data;
}

void ShipPlanLineElement::setData(const ZCHX::Data::ShipPlanLine &data)
{
    m_data = data;
}

ZCHX::Data::ShipPlanLine ShipPlanLineElement::data() const
{
    return m_data;
}

ZCHX::Data::ShipPlanLine ShipPlanLineElement::backData() const
{
    return m_backData;
}

void ShipPlanLineElement::restoreBackData()
{
    m_data = m_backData;
}

bool ShipPlanLineElement::dataChanged()
{
    if(m_data == m_backData)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void ShipPlanLineElement::setShipDisplayFlag(const int uFlag)
{
    m_uDisplayFlag = uFlag;
}

int ShipPlanLineElement::getShipDisplayFlag() const
{
    return m_uDisplayFlag;
}

bool ShipPlanLineElement::getSelectedLatlonPoint(std::pair<double, double> &latlon)
{
    if(m_activePathPoint<0||m_activePathPoint>=m_data.m_dShipPlanPoints.size())
        return false;
    else
    {
        latlon.first = m_data.m_dShipPlanPoints[m_activePathPoint].m_dShipLat;
        latlon.second = m_data.m_dShipPlanPoints[m_activePathPoint].m_dShipLon;
        return true;
    }
}

void ShipPlanLineElement::changePathPoint(double lat, double lon)
{
    if(0 <= m_activePathPoint  && m_activePathPoint < m_data.m_dShipPlanPoints.size())
    {
        m_data.m_dShipPlanPoints[m_activePathPoint].m_dShipLat = lat;
        m_data.m_dShipPlanPoints[m_activePathPoint].m_dShipLon = lon;
    }
}

void ShipPlanLineElement::moveTo(double lat, double lon)
{
    for(int i= 0; i< m_data.m_dShipPlanPoints.size(); ++i)
    {
        m_data.m_dShipPlanPoints[i].m_dShipLat  +=  lat;
        m_data.m_dShipPlanPoints[i].m_dShipLon +=  lon;
    }
}

int ShipPlanLineElement::activePathPoint() const
{
    return m_activePathPoint;
}

void ShipPlanLineElement::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void ShipPlanLineElement::delPathPoint(int idx)
{
    if(m_data.m_dShipPlanPoints.size() > idx)
    {
        m_data.m_dShipPlanPoints.erase(m_data.m_dShipPlanPoints.begin()+idx);
        m_activePathPoint = -1;
    }
}

RouteCrossElement::RouteCrossElement(const ZCHX::Data::ITF_RouteCross &ele)
    :Element(ele.m_fCrossLat, ele.m_fCrossLon, 0, ZCHX::Data::ELE_ROUTE_CROSS_POINT)
{
    m_data = ele;
}

ZCHX::Data::ITF_RouteCross RouteCrossElement::data() const
{
    return m_data;
}

Multibeam::Multibeam(const ZCHX::Data::ITF_Multibeam &ele)
    :Element(ele.m_dLat,ele.m_dLon, 0, ZCHX::Data::ELE_MULTIBEAM)
{
    m_data = ele;
}

ZCHX::Data::ITF_Multibeam Multibeam::data() const
{
    return m_data;
}

void Multibeam::setData(const ZCHX::Data::ITF_Multibeam &data)
{
    m_data = data;
}




SpecialRouteLineElement::SpecialRouteLineElement(const ZCHX::Data::SpecialRouteLine &ele)
    :Element(0, 0, 0, ZCHX::Data::ELE_ROUTE_LINE)
{
    m_data = ele;
}

void SpecialRouteLineElement::setNewChangeData(const ZCHX::Data::SpecialRouteLine &data)
{
    m_data = data;
}

void SpecialRouteLineElement::setData(const ZCHX::Data::SpecialRouteLine &data)
{
    m_data = data;
}

ZCHX::Data::SpecialRouteLine SpecialRouteLineElement::data() const
{
    return m_data;
}

bool SpecialRouteLineElement::getSelectedLatlonPoint(std::pair<double, double> &latlon)
{
    if(m_activePathPoint<0||m_activePathPoint>=m_data.points.size())
        return false;
    else
    {
        latlon.first = m_data.points[m_activePathPoint].m_dY;
        latlon.second = m_data.points[m_activePathPoint].m_dX;
        return true;
    }
}

int SpecialRouteLineElement::activePathPoint() const
{
    return m_activePathPoint;
}

void SpecialRouteLineElement::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

//namespace end
}
