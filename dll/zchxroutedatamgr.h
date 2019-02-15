#ifndef ZCHXROUTEDATAMGR_H
#define ZCHXROUTEDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>
#include <QMutex>

class zchxMapWidget;

class zchxRouteDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxRouteDataMgr(zchxMapWidget* w, QObject *parent = 0);
    //特殊路由
    void setALLSpecialRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data);
    void setSpacilRouteLineData(const std::vector<DrawElement::SpecialRouteLine> &data);
    void setPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data);
    //交越路由
    void setRouteCrossData(const std::vector<DrawElement::RouteCross> &data);
    //项目路由
    void setAllProjectRouteLineData(const std::vector<DrawElement::RouteLine> &data);
    void setRouteLineData(const std::vector<DrawElement::RouteLine> &data);
    void setHistoryRouteLineData(const std::vector<DrawElement::RouteLine> &data);
    void setRouteDefaultCableData(const ZCHX::Data::CableAssembly &data,int uSlack);


signals:

public slots:


private:
    zchxMapWidget       *mDisplayWidget;
    std::vector<DrawElement::SpecialRouteLine> m_AllProjectSpecialRoute;//所有项目特殊路由(南方电网)
    std::vector<DrawElement::SpecialRouteLine> m_SpecialRouteLine;      //当前项目特殊路由(南方电网)
    std::vector<DrawElement::RouteCross> m_RouteCross;                  //交越路由(全项目)
    int m_uCurrentProjectID;                                            //当前项目ID
    ZCHX::Data::CableAssembly m_CableAssembly;                          //创建路由时候的默认海缆
    int m_uSlack;                                                       //默认余量
    std::vector<DrawElement::RouteLine> m_AllProjectRouteLine;          //所有项目路由线
    std::vector<DrawElement::RouteLine> m_RouteLine;                    //路由线(当前项目)
    std::vector<DrawElement::RouteLine> m_historyRouteLine;             //历史路由线
};

#endif // ZCHXROUTEDATAMGR_H
