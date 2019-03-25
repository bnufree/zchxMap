#include "zchxshipplandatamgr.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
//#include "coastdatainfodialog.h"
//#include "cardmouthinfodialog.h"
//#include "channelinfodialog.h"
//#include "mooringinfodialog.h"

namespace qt {
zchxShipPlanDataMgr::zchxShipPlanDataMgr(zchxMapWidget* w, QObject *parent) : QObject(parent),mDisplayWidget(w)
{
}


void zchxShipPlanDataMgr::setAllProjecrShipPlanLineData(const std::vector<ShipPlanLineElement*> &data)
{
    m_AllShipPlanLines = data;
}

void zchxShipPlanDataMgr::setShipPlanLineData(const std::vector<ShipPlanLineElement*> &data)
{
    m_ShipPlanLine = data;
#if 0
    //更新Active
    int activePlanID = -1;
    if(Settings::Get("ActivePlan", activePlanID))
    {
        std::vector<ShipPlanLine>::iterator it;
        for(it=m_ShipPlanLine.begin(); it != m_ShipPlanLine.end(); ++it)
        {
            ShipPlanLine *ptr = &(*it);
            if(ptr->data().m_dShipPlanId == activePlanID)
            {
                ptr->setIsActive(true);
                emit signalIsSelected4ShipPlanLine(ptr->data());
                //根据不同的状态进行处理
                int toolMode = -1;
                if(Settings::Get("eTool", toolMode))
                {
                    if(toolMode == SHIPPLANADDCTRL)
                    {
                        //追加点的情况
                        //需要保存客户端状态，在画计划的过程中退出客户端，计划点列需要使用上次退出时的点列
                        std::string src("");
                        if(Settings::Get("PlanAddPnt", src))
                        {
                            QString pnts = QString::fromStdString(src);
                            if(pnts.length() > 0)
                            {
                                QStringList pntslist = pnts.split(",");
                                for(int i=0; i+1<pntslist.size(); i=i+2)
                                {
                                    QString lonstr = pntslist[i];
                                    QString latstr = pntslist[i+1];
                                    addShipPlanPointLL(ms::LatLon(latstr.toDouble(), lonstr.toDouble()), *ptr);
                                }
                            }
                        }
                    }
                }


                break;
            }
        }

    }
#endif

    if(!m_ShipPlanLine.empty())
    {
        ShipPlanLineElement* objLine = data.at(0);
        ZCHX::Data::ShipPlanLine objData = objLine->data();
        m_uCurrentProjectID = objData.m_dProjectID;
    }
    std::vector<ShipPlanLineElement*>::iterator it;
    for(it = m_AllShipPlanLines.begin(); it != m_AllShipPlanLines.end(); ++it)
    {
        ShipPlanLineElement* ele = (*it);
        ZCHX::Data::ShipPlanLine shipPlanLine = ele->data();
        if(shipPlanLine.m_dProjectID == m_uCurrentProjectID)
        {
            m_AllShipPlanLines.erase(it);
            --it;
        }

    }
    for(int i = 0;i<m_ShipPlanLine.size();i++)
    {
        ShipPlanLineElement* objLine = data.at(i);
        m_AllShipPlanLines.push_back(objLine);
    }
}

void zchxShipPlanDataMgr::setShipPlanProjectID(const int ProjectID)
{
    m_uCurrentProjectID = ProjectID;
}
}

