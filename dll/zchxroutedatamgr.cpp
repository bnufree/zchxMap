#include "zchxroutedatamgr.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"

zchxRouteDataMgr::zchxRouteDataMgr(zchxMapWidget* w, QObject *parent) : QObject(parent),mDisplayWidget(w)
{
}

void zchxRouteDataMgr::setALLSpecialRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data)
{
    m_AllProjectSpecialRoute = data;
}

void zchxRouteDataMgr::setSpacilRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data)
{
    m_SpecialRouteLine.clear();
    for(int i =0; i< data.size();++i)
    {
        DrawElement::SpecialRouteLine TempData = data.at(i);
        TempData.setActivePathPoint(-1);
        TempData.setIsActive(false);
        m_SpecialRouteLine.push_back(TempData);
    }
    if(!m_SpecialRouteLine.empty())
    {
        DrawElement::SpecialRouteLine objLine = data.at(0);
        ZCHX::Data::SpecialRouteLine objData = objLine.data();
        m_uCurrentProjectID = objData.m_iProjectId;
    }
}

void zchxRouteDataMgr::setPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data)
{
    if(m_SpecialRouteLine.size()>0)
    {
        std::vector<DrawElement::SpecialRouteLine>::iterator it;
        bool hasSelectedOne = false;
        for(it = m_SpecialRouteLine.begin(); it != m_SpecialRouteLine.end(); ++it)
        {
            (*it).setIsActive(false);
            (*it).setActivePathPoint(-1);
        }
        for(it = m_SpecialRouteLine.begin(); it != m_SpecialRouteLine.end(); ++it)
        {
            ZCHX::Data::SpecialRouteLine objData = (*it).data();
            std::vector<ZCHX::Data::SpecialRoutePoint> points = objData.points;
            if(points.size()<=1)
                continue;
            bool bOnLine = false;
            if(objData.m_iId == data.m_iFileId)
            {
                bOnLine = true;
                (*it).setIsActive(true);
                (*it).setActivePathPoint(data.m_iNumber);
            }
            if(!hasSelectedOne && bOnLine)
            {

                (*it).setIsActive(true);
                hasSelectedOne = true;
                int pointNum = (*it).activePathPoint();
                ZCHX::Data::SpecialRouteLine item = (*it).data();
                emit mDisplayWidget->signalIsSelectedSpecialRoutePoint(item.points.at(pointNum));
            }
            else
            {
                (*it).setIsActive(false);
            }
        }
    }
}

void zchxRouteDataMgr::setRouteCrossData(const std::vector<DrawElement::RouteCross> &data)
{
    m_RouteCross = data;
}

void zchxRouteDataMgr::setAllProjectRouteLineData(const std::vector<DrawElement::RouteLine> &data)
{
    m_AllProjectRouteLine = data;
}

void zchxRouteDataMgr::setRouteLineData(const std::vector<DrawElement::RouteLine> &data)
{
    //qDebug()<<__FUNCTION__<<__LINE__;
    m_RouteLine = data;
#if 0
    //更新Active
    int activeRouteID = -1;
    if(Settings::Get("ActiveRoute", activeRouteID))
    {
        //qDebug()<<__FUNCTION__<<__LINE__<<" activeRoute:"<<activeRouteID;
        std::vector<DrawElement::RouteLine>::iterator it;
        for(it=m_RouteLine.begin(); it != m_RouteLine.end(); ++it)
        {
            DrawElement::RouteLine *ptr = &(*it);
            if(ptr->data().routeID == activeRouteID)
            {
                //qDebug()<<__FUNCTION__<<__LINE__<<" route found break";
                ptr->setIsActive(true);
                emit signalIsSelected4RouteLine(ptr->data());
                //根据不同的状态进行处理
                int toolMode = -1;
                if(Settings::Get("eTool", toolMode))
                {
                    if(toolMode == ROUTEADDCTRL)
                    {
                        //追加点的情况
                        //需要保存客户端状态，在画路由的过程中退出客户端，路由点列需要使用上次退出时的点列
                        std::string src("");
                        if(Settings::Get("RouteAddPnt", src))
                        {
                            QString pnts = QString::fromStdString(src);
                            if(pnts.length() > 0)
                            {
                                QStringList pntslist = pnts.split(",");
                                for(int i=0; i+1<pntslist.size(); i=i+2)
                                {
                                    QString lonstr = pntslist[i];
                                    QString latstr = pntslist[i+1];
                                    addRoutePointLL(ms::LatLon(latstr.toDouble(), lonstr.toDouble()), *ptr);
                                }
                            }
                        }
                    }
                }


                break;
            }
        }

        foreach (DrawElement::RouteLine line, m_RouteLine) {
            //qDebug()<<"id:"<<line.data().routeID<<" active:"<<line.getIsActive()<<endl;
        }
    }
#endif
    if(!m_RouteLine.empty())
    {
        DrawElement::RouteLine objLine = data.at(0);
        ZCHX::Data::RouteLine objData = objLine.data();
        m_uCurrentProjectID = objData.projectID;
    }
    std::vector<DrawElement::RouteLine>::iterator it;
    for(it=m_AllProjectRouteLine.begin(); it != m_AllProjectRouteLine.end(); ++it)
    {
        DrawElement::RouteLine ele = (*it);
        ZCHX::Data::RouteLine routeLine = ele.data();
        if(routeLine.projectID == m_uCurrentProjectID)
        {
            m_AllProjectRouteLine.erase(it);
            --it;
        }

    }
    for(int i = 0;i<m_RouteLine.size();i++)
    {
        DrawElement::RouteLine objLine = data.at(i);
        m_AllProjectRouteLine.push_back(objLine);
    }

}

void zchxRouteDataMgr::setHistoryRouteLineData(const std::vector<DrawElement::RouteLine> &data)
{
    m_historyRouteLine = data;
}

void zchxRouteDataMgr::setRouteDefaultCableData(const ZCHX::Data::CableAssembly &data,int uSlack)
{
    m_CableAssembly = data;
    m_uSlack = uSlack;
}

