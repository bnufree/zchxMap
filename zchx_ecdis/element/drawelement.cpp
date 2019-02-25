#include "drawelement.hpp"
#include "zchxmapframe.h"
#include "zchxmapwidget.h"
#include "zchxMapDatautils.h"
#include "profiles.h"
#include <QDebug>
#include "zchxutils.hpp"

namespace qt{

MoveElement::MoveElement()
    :Element(0,0,0)
{
    setData();
}

void MoveElement::setData()
{
    m_activePathPoint = -1;
}

void MoveElement::changePathPoint(double lat, double lon)
{
    std::vector<std::pair<double, double> > & path = getPath();
    if(0 <= m_activePathPoint && m_activePathPoint < path.size())
    {
        path[m_activePathPoint].first = lat;
        path[m_activePathPoint].second = lon;
    }
}

void MoveElement::moveTo(double lat, double lon)
{
    std::vector<std::pair<double, double> > & path = getPath();
    for(int i= 0; i< path.size(); ++i)
    {
        path[i].first  = path[i].first  + lat;
        path[i].second = path[i].second + lon;
    }
}

int MoveElement::activePathPoint() const
{
    return m_activePathPoint;
}

void MoveElement::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void MoveElement::delPathPoint(int idx)
{
    std::vector<std::pair<double, double> > & path = getPath();
    if(path.size() > idx)
    {
        path.erase(path.begin() + idx);
        m_activePathPoint = -1;
    }
}

void MoveElement::addCtrlPoint(std::pair<double, double> ps)
{
    std::vector<std::pair<double, double> > & path = getPath();
    path.push_back(ps);
    m_activePathPoint = -1;
}

bool MoveElement::contains(zchxMapFrameWork *framework, int range, double x, double y) const
{
    if(!framework)
        return false;

    std::vector<std::pair<double,double>> tmp_path = path();
    for(int i = 0; i < tmp_path.size() - 1; ++i)
    {
        std::pair<double, double> p1 = tmp_path[i];
        std::pair<double, double> p2 = tmp_path[i+1];
        Point2D start = framework->LatLon2Pixel(p1.first, p1.second);
        Point2D end = framework->LatLon2Pixel(p2.first, p2.second);

        //检查3点是否共线
        int p1x = start.x, p1y = start.y;
        int p2x = end.x, p2y = end.y;
        int check = (p1x - x) *(p2y - y) - (p2x - x) * (p1y - y);
        //qDebug()<<"start:"<<p1x<<" "<<p1y<<" end:"<<p2x<<" "<<p2y<<" cur:"<<x<<" "<<y<<" area:"<<check;
        if(abs(check) < range)
        {
            return true;
        }
    }

    return false;
}



CoastData::CoastData(const ZCHX::Data::ITF_CoastData &ele)
    :Element(0,0,0)
{
    setData(ele);
}

void CoastData::setData(const ZCHX::Data::ITF_CoastData &ele)
{
    m_path = ele.path;
    //m_type = (RADARTYPE)ele.type;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > CoastData::path() const
{
    return m_data.path;
}

void CoastData::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int CoastData::id() const
{
    return uuid;
}

void CoastData::setId(int id)
{
    uuid = id;
}

std::string CoastData::name() const
{
    return m_name;
}

void CoastData::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_CoastData CoastData::data() const
{
    return m_data;
}

SeabedPipeLine::SeabedPipeLine(const ZCHX::Data::ITF_SeabedPipeLine &ele)
    :Element(0,0,0)
{
    setData(ele);
}

void SeabedPipeLine::setData(const ZCHX::Data::ITF_SeabedPipeLine &ele)
{
    m_path = ele.path;
    //m_type = (RADARTYPE)ele.type;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > SeabedPipeLine::path() const
{
    return m_data.path;
}

void SeabedPipeLine::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int SeabedPipeLine::id() const
{
    return uuid;
}

void SeabedPipeLine::setId(int id)
{
    uuid = id;
}

std::string SeabedPipeLine::name() const
{
    return m_name;
}

void SeabedPipeLine::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_SeabedPipeLine SeabedPipeLine::data() const
{
    return m_data;
}

Structure::Structure(const ZCHX::Data::ITF_Structure &ele)
    :Element(0,0,0)
{
    setData(ele);
}

void Structure::setData(const ZCHX::Data::ITF_Structure &ele)
{
    m_point = ele.point;
    //m_type = (RADARTYPE)ele.type;
    elelat = ele.point.first;
    elelon = ele.point.second;
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::pair<double, double> Structure::point() const
{
    return m_data.point;
}

void Structure::setPoint(const std::pair<double, double> &point)
{
    m_data.point = point;
}

int Structure::id() const
{
    return uuid;
}

void Structure::setId(int id)
{
    uuid = id;
}

std::string Structure::name() const
{
    return m_name;
}

void Structure::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_Structure Structure::data() const
{
    return m_data;
}

AreaNet::AreaNet(const ZCHX::Data::ITF_AreaNet &ele)
    :Element(0,0,0)
{
    setData(ele);
}

void AreaNet::setData(const ZCHX::Data::ITF_AreaNet &ele)
{
    m_path = ele.path;
    //m_type = (RADARTYPE)ele.type;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > AreaNet::path() const
{
    return m_data.path;
}

void AreaNet::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int AreaNet::id() const
{
    return uuid;
}

void AreaNet::setId(int id)
{
    uuid = id;
}

std::string AreaNet::name() const
{
    return m_name;
}

void AreaNet::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_AreaNet AreaNet::data() const
{
    return m_data;
}


Channel::Channel(const ZCHX::Data::ITF_Channel &ele)
    :MoveElement()
{
    setData(ele);
}

void Channel::setData(const ZCHX::Data::ITF_Channel &ele)
{
    m_path = ele.path;
    //m_type = (RADARTYPE)ele.type;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > Channel::path() const
{
    return m_data.path;
}

void Channel::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int Channel::id() const
{
    return uuid;
}

void Channel::setId(int id)
{
    uuid = id;
}

std::string Channel::name() const
{
    return m_name;
}

void Channel::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_Channel Channel::data() const
{
    return m_data;
}

void Channel::setLineSelected(int i, bool selectStatus)
{
    if (i >= 0 && i < m_data.lineList.size())
    {
        m_data.lineList[i].isSelected = selectStatus;
    }
}

std::vector<std::pair<double, double> > & Channel::getPath()
{
    return m_data.path;
}

void Channel::updateOldPath()
{
    m_path = m_data.path;
}


Mooring::Mooring(const ZCHX::Data::ITF_Mooring &ele)
    :MoveElement()
{
    setData(ele);
}

void Mooring::setData(const ZCHX::Data::ITF_Mooring &ele)
{
    m_path = ele.path;
    //m_type = (RADARTYPE)ele.type;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > Mooring::path() const
{
    return m_data.path;
}

void Mooring::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int Mooring::id() const
{
    return uuid;
}

void Mooring::setId(int id)
{
    uuid = id;
}

std::string Mooring::name() const
{
    return m_name;
}

void Mooring::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_Mooring Mooring::data() const
{
    return m_data;
}

std::vector<std::pair<double, double> > & Mooring::getPath()
{
    return m_data.path;
}

void Mooring::updateOldPath()
{
    m_path = m_data.path;
}


CardMouth::CardMouth(const ZCHX::Data::ITF_CardMouth &ele)
    :MoveElement()
{
    setData(ele);
}

void CardMouth::setData(const ZCHX::Data::ITF_CardMouth &ele)
{
    m_path = ele.path;
    //m_type = (RADARTYPE)ele.type;
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    m_name = ele.name.toStdString();
}

std::vector<std::pair<double, double> > CardMouth::path() const
{
    return m_data.path;
}

void CardMouth::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int CardMouth::id() const
{
    return uuid;
}

void CardMouth::setId(int id)
{
    uuid = id;
}

std::string CardMouth::name() const
{
    return m_name;
}

void CardMouth::setName(const std::string &name)
{
    m_name = name;
}

ZCHX::Data::ITF_CardMouth CardMouth::data() const
{
    return m_data;
}

std::vector<std::pair<double, double> > & CardMouth::getPath()
{
    return m_data.path;
}

void CardMouth::updateOldPath()
{
    m_path = m_data.path;
}


ShipAlarmAscend::ShipAlarmAscend(const ZCHX::Data::ITF_ShipAlarmAscend &ele)
    :MoveElement()
{
    setData(ele);
}

void ShipAlarmAscend::setData(const ZCHX::Data::ITF_ShipAlarmAscend &ele)
{
    m_data = ele;
}

ZCHX::Data::ITF_ShipAlarmAscend ShipAlarmAscend::data() const
{
    return m_data;
}


////////////////////
/// \brief 环岛线
IslandLine::IslandLine(const ZCHX::Data::ITF_IslandLine &ele)
    : Element(0,0,0)
    , m_path(ele.path)
    , m_type((RADARTYPE)ele.type)
{
    if(!ele.path.empty())
    {
        elelat = ele.path.front().first;
        elelon = ele.path.front().second;
    }
    m_data = ele;
    uuid = ele.uuid;
    m_id = ele.id;
    m_name = ele.name.toStdString();
    m_content = ele.content.toStdString();
}

std::vector<std::pair<double, double> > IslandLine::path() const
{
    return m_data.path;
}

void IslandLine::setPath(const std::vector<std::pair<double, double> > &path)
{
    m_data.path = path;
}

int IslandLine::id() const
{
    return m_id;
}

void IslandLine::setId(int id)
{
    m_id = id;
}

std::string IslandLine::name() const
{
    return m_name;
}

void IslandLine::setName(const std::string &name)
{
    m_name = name;
}

std::string IslandLine::content() const
{
    return m_content;
}

void IslandLine::setContent(const std::string &content)
{
    m_content = content;
}

ZCHX::Data::ITF_IslandLine IslandLine::data() const
{
    return m_data;
}

void IslandLine::changePathPoint(double lat, double lon)
{
    if(0 <= m_activePathPoint && m_activePathPoint < m_data.path.size())
    {
        m_data.path[m_activePathPoint].first = lat;
        m_data.path[m_activePathPoint].second = lon;
    }
}

void IslandLine::moveTo(double lat, double lon)
{
    for(int i= 0; i< m_data.path.size(); ++i)
    {
        m_data.path[i].first  = m_path[i].first  + lat;
        m_data.path[i].second = m_path[i].second + lon;
    }
}

void IslandLine::updateOldPath()
{
    m_path = m_data.path;
}

int IslandLine::activePathPoint() const
{
    return m_activePathPoint;
}

void IslandLine::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void IslandLine::delPathPoint(int idx)
{
    if(m_data.path.size() > idx)
    {
        m_data.path.erase(m_data.path.begin()+idx);
        m_path = m_data.path;
        m_activePathPoint = -1;
    }
}

void IslandLine::addCtrlPoint(std::pair<double, double> ps)
{
    m_data.path.push_back(ps);
    m_path = m_data.path;
    m_activePathPoint = -1;
}

QString IslandLine::color() const
{
    return m_data.warnColor;
}

void IslandLine::setColor(QString color)
{
    m_data.warnColor = color;
}


////////
/// \brief EllipseElement::EllipseElement
/// \param ele
///

EllipseElement::EllipseElement(const ZCHX::Data::ITF_EleEllipse &ele)
    :Element(ele.ll.lat,ele.ll.lon, 0, ZCHX::Data::ELEELLIPSE)
{
    setEle(ele);
}

ZCHX::Data::ITF_EleEllipse EllipseElement::ele() const
{
    return m_ele;
}

void EllipseElement::setEle(const ZCHX::Data::ITF_EleEllipse &ele)
{
    m_ele = ele;
}

TriangleElement::TriangleElement(const ZCHX::Data::ITF_EleTriangle &ele)
    :Element(ele.ll.lat,ele.ll.lon, 0, ZCHX::Data::ELETRIANGLE)
{
    setEle(ele);
}

ZCHX::Data::ITF_EleTriangle TriangleElement::ele() const
{
    return m_ele;
}

void TriangleElement::setEle(const ZCHX::Data::ITF_EleTriangle &ele)
{
    m_ele = ele;
}

LineElement::LineElement(const ZCHX::Data::ITF_EleLine &ele)
    :Element(ele.ll1.lat,ele.ll1.lon, 0, ZCHX::Data::ELELINE)
{
    setEle(ele);
}

ZCHX::Data::ITF_EleLine LineElement::ele() const
{
    return m_ele;
}

void LineElement::setEle(const ZCHX::Data::ITF_EleLine &ele)
{
    m_ele = ele;
}

RectElement::RectElement(const ZCHX::Data::ITF_EleRect &ele)
    :Element(ele.ll.lat,ele.ll.lon, 0, ZCHX::Data::ELERECT)
{
    setEle(ele);
}

ZCHX::Data::ITF_EleRect RectElement::ele() const
{
    return m_ele;
}

void RectElement::setEle(const ZCHX::Data::ITF_EleRect &ele)
{
    m_ele = ele;
}


PastrolStation::PastrolStation(const ZCHX::Data::ITF_PastrolStation &data)
    :Element(data.ll.lat,data.ll.lon, 0)
{
    m_data = data;
    uuid = data.uuid;
}

ZCHX::Data::ITF_PastrolStation PastrolStation::data() const
{
    return m_data;
}





LocalMark::LocalMark(const ZCHX::Data::ITF_LocalMark &data)
    :Element(data.ll.lat, data.ll.lon, 0)
{
    m_data = data;
    uuid = data.uuid;
}

ZCHX::Data::ITF_LocalMark LocalMark::data() const
{
    return m_data;
}


Navigation::Navigation(const ZCHX::Data::ITF_Navigation &data)
    :Element(data.lat,data.lon, 0)
{

    m_data = data;
    uuid = data.uuid;
}

ZCHX::Data::ITF_Navigation Navigation::data() const
{
    return m_data;
}

void Navigation::setData(const ZCHX::Data::ITF_Navigation &data)
{
    m_data = data;
}

DangerousCircle::DangerousCircle(const ZCHX::Data::ITF_DangerousCircle &data)
    :Element(data.lat, data.lon, 0)
{
    m_data = data;
}

ZCHX::Data::ITF_DangerousCircle DangerousCircle::data() const
{
    return m_data;
}

void DangerousCircle::setDangerousCircleData(const ZCHX::Data::ITF_DangerousCircle &data)
{
    m_data = data;
}

void DangerousCircle::setDangerousCircleRange(int range)
{
    m_range = range;
}

RadarFeatureZone::RadarFeatureZone(const ZCHX::Data::ITF_RadarFeaturesZone &data)
    :Element(0, 0, 0)
{
    m_data = data;
}

ZCHX::Data::ITF_RadarFeaturesZone RadarFeatureZone::data() const
{
   return m_data;
}

void RadarFeatureZone::setRadarFeatureZoneData(const ZCHX::Data::ITF_RadarFeaturesZone &data)
{
    m_data = data;
}

RouteLine::RouteLine(const ZCHX::Data::RouteLine &ele)
    :Element(0,0, 0)
{
    m_data = ele;
    m_backData = ele;
    uuid = ele.routeID;
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

void RouteLine::setNewChangeData(const ZCHX::Data::RouteLine &data)
{
    m_data = data;
    m_backData = data;
    uuid = data.routeID;
}

void RouteLine::setData(const ZCHX::Data::RouteLine &data)
{
    m_data = data;
}

ZCHX::Data::RouteLine RouteLine::data() const
{
    return m_data;
}

ZCHX::Data::RouteLine RouteLine::backData() const
{
    return m_backData;
}

void RouteLine::restoreBackData()
{
    m_data = m_backData;
}

bool RouteLine::dataChanged()
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

void RouteLine::setLinkInfor(bool bLinked, const double dLat, const double dLon)
{
    m_bLinked = bLinked;
    m_dLinkLat = dLat;
    m_dLinkLon = dLon;
}

bool RouteLine::getLinked() const
{
    return m_bLinked;
}

void RouteLine::getLinkLatLon(double &dLat, double &dLon) const
{
    dLat = m_dLinkLat;
    dLon = m_dLinkLon;
}

void RouteLine::setLinkShowInfor(const double dKp, bool bShowLength, const double dLength, bool bShowSlope, const double dSlope)
{
     m_dKp = dKp;
     m_bShowLength = bShowLength;
     m_dLength = dLength;
     m_bShowSlope = bShowSlope;
     m_dSlope = dSlope;
}

void RouteLine::getLinkShowInfor(double &dKp, bool &bShowLength, double &dLength, bool &bShowSlope, double &dSlope) const
{
    dKp = m_dKp;
    bShowLength = m_bShowLength;
    dLength = m_dLength;
    bShowSlope = m_bShowSlope;
    dSlope = m_dSlope;
}

bool RouteLine::routeApprovaled()
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

void RouteLine::setRouteAssessStatus(int uAssessStatus)
{
    m_data.routeAssessStatus = uAssessStatus;
}

void RouteLine::setRouteConfigDisplay( bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable)
{
    mShowPoint = bShowPoint;
    mShowType = bShowType;
    mShowLine = bShowLine;
    mShowCable = bShowCable;
}

void RouteLine::getRouteConfigDisplay(int& bShowPoint,int& bShowType,int& bShowLine,int& bShowCable) const
{
    bShowPoint = mShowPoint;
    bShowType = mShowType;
    bShowLine = mShowLine;
    bShowCable = mShowCable;
}

void RouteLine::setRouteDisplayFlag(const int uFlag)
{
    m_uRouteDisplayFlag = uFlag;
}

int RouteLine::getRouteDisplayFlag()const
{
    return m_uRouteDisplayFlag;
}

void RouteLine::setIsPickupShowInfor(bool bPickupShowInfor)
{
    m_bPickupShowInfor = bPickupShowInfor;
}

bool RouteLine::getPickupShowInfor()const
{
    return m_bPickupShowInfor;
}

void RouteLine::setShowInforKP(float fShowInforKP)
{
    m_fShowInforKP = fShowInforKP;
}

float RouteLine::getShowInforKP()const
{
    return m_fShowInforKP;
}

void RouteLine::setPickupLonLat(double dLon, double dLat)
{
    m_dLon = dLon;
    m_dLat = dLat;
}

void RouteLine::getPickupLonLat(double &dLon, double &dLat) const
{
    dLon = m_dLon;
    dLat = m_dLat;
}

void RouteLine::setPickupRoutePointIndex(int uPickupRoutePointIndex)
{
    m_uPickupRoutePointIndex = uPickupRoutePointIndex;
}

int RouteLine::getPickupRoutePointIndex() const
{
    return m_uPickupRoutePointIndex;
}

bool RouteLine::getSelectedLatlonPoint(std::pair<double, double> &latlon)
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

void RouteLine::changePathPoint(double lat, double lon)
{
    if(0 <= m_activePathPoint  && m_activePathPoint < m_data.points.size())
    {
        m_data.points[m_activePathPoint].lat = lat;
        m_data.points[m_activePathPoint].lon = lon;
    }
}

void RouteLine::moveTo(double lat, double lon)
{
    for(int i= 0; i< m_data.points.size(); ++i)
    {
        m_data.points[i].lat  +=  lat;
        m_data.points[i].lon +=  lon;
    }
}


int RouteLine::activePathPoint() const
{
    return m_activePathPoint;
}

void RouteLine::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void RouteLine::delPathPoint(int idx)
{
    if(m_data.points.size() > idx)
    {
        m_data.points.erase(m_data.points.begin()+idx);
        m_activePathPoint = -1;
    }
}

ShipPlanLine::ShipPlanLine(const ZCHX::Data::ShipPlanLine &ele)
    :Element(0, 0, 0)
{
    m_data = ele;
    m_backData = ele;
    uuid = ele.m_dShipPlanId;
    m_uDisplayFlag = 1;
}

void ShipPlanLine::setNewChangeData(const ZCHX::Data::ShipPlanLine &data)
{
    m_data = data;
    m_backData = data;
    uuid = data.m_dShipPlanId;
}

void ShipPlanLine::setData(const ZCHX::Data::ShipPlanLine &data)
{
    m_data = data;
}

ZCHX::Data::ShipPlanLine ShipPlanLine::data() const
{
    return m_data;
}

ZCHX::Data::ShipPlanLine ShipPlanLine::backData() const
{
    return m_backData;
}

void ShipPlanLine::restoreBackData()
{
    m_data = m_backData;
}

bool ShipPlanLine::dataChanged()
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

void ShipPlanLine::setShipDisplayFlag(const int uFlag)
{
    m_uDisplayFlag = uFlag;
}

int ShipPlanLine::getShipDisplayFlag() const
{
    return m_uDisplayFlag;
}

bool ShipPlanLine::getSelectedLatlonPoint(std::pair<double, double> &latlon)
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

void ShipPlanLine::changePathPoint(double lat, double lon)
{
    if(0 <= m_activePathPoint  && m_activePathPoint < m_data.m_dShipPlanPoints.size())
    {
        m_data.m_dShipPlanPoints[m_activePathPoint].m_dShipLat = lat;
        m_data.m_dShipPlanPoints[m_activePathPoint].m_dShipLon = lon;
    }
}

void ShipPlanLine::moveTo(double lat, double lon)
{
    for(int i= 0; i< m_data.m_dShipPlanPoints.size(); ++i)
    {
        m_data.m_dShipPlanPoints[i].m_dShipLat  +=  lat;
        m_data.m_dShipPlanPoints[i].m_dShipLon +=  lon;
    }
}

int ShipPlanLine::activePathPoint() const
{
    return m_activePathPoint;
}

void ShipPlanLine::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

void ShipPlanLine::delPathPoint(int idx)
{
    if(m_data.m_dShipPlanPoints.size() > idx)
    {
        m_data.m_dShipPlanPoints.erase(m_data.m_dShipPlanPoints.begin()+idx);
        m_activePathPoint = -1;
    }
}

RouteCross::RouteCross(const ZCHX::Data::ITF_RouteCross &ele)
    :Element(ele.m_fCrossLat, ele.m_fCrossLon, 0)
{
    m_data = ele;
    uuid = ele.m_uKeyID;
}

ZCHX::Data::ITF_RouteCross RouteCross::data() const
{
    return m_data;
}

Multibeam::Multibeam(const ZCHX::Data::ITF_Multibeam &ele)
    :Element(ele.m_dLat,ele.m_dLon, 0)
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

RadarVideoElement::RadarVideoElement(const ZCHX::Data::ITF_RadarVideo &ele, int uIndex)
    :Element(ele.dCentreLat,ele.dCentreLon, 0),m_uMsgIndex(uIndex)
{
    m_data = ele;
}

ZCHX::Data::ITF_RadarVideo RadarVideoElement::data() const
{
    return m_data;
}

void RadarVideoElement::setData(const ZCHX::Data::ITF_RadarVideo &data)
{
    m_data = data;
}


void RadarVideoElement::setMsgIndex(int uIndex)
{
    m_uMsgIndex = uIndex;
}

int RadarVideoElement::getMsgIndex()
{
    return m_uMsgIndex;
}

SpecialRouteLine::SpecialRouteLine(const ZCHX::Data::SpecialRouteLine &ele)
    :Element(0, 0, 0)
{
    m_data = ele;
    uuid = ele.m_iId;
}

void SpecialRouteLine::setNewChangeData(const ZCHX::Data::SpecialRouteLine &data)
{
    m_data = data;
    uuid = data.m_iId;
}

void SpecialRouteLine::setData(const ZCHX::Data::SpecialRouteLine &data)
{
    m_data = data;
}

ZCHX::Data::SpecialRouteLine SpecialRouteLine::data() const
{
    return m_data;
}

bool SpecialRouteLine::getSelectedLatlonPoint(std::pair<double, double> &latlon)
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

int SpecialRouteLine::activePathPoint() const
{
    return m_activePathPoint;
}

void SpecialRouteLine::setActivePathPoint(int activePathPoint)
{
    m_activePathPoint = activePathPoint;
}

//namespace end
}
