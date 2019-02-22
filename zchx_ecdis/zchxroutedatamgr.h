#ifndef ZCHXROUTEDATAMGR_H
#define ZCHXROUTEDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>
#include <QMutex>

namespace qt {
class zchxMapWidget;

class zchxRouteDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxRouteDataMgr(zchxMapWidget* w, QObject *parent = 0);
    //特殊路由
    void setALLSpecialRouteLineData(const std::vector<SpecialRouteLine> &data);
    void setSpacilRouteLineData(const std::vector<SpecialRouteLine> &data);
    void setPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data);
    //交越路由
    void setRouteCrossData(const std::vector<RouteCross> &data);
    //项目路由
    void setAllProjectRouteLineData(const std::vector<RouteLine> &data);
    void setRouteLineData(const std::vector<RouteLine> &data);
    void setHistoryRouteLineData(const std::vector<RouteLine> &data);
    void setRouteDefaultCableData(const ZCHX::Data::CableAssembly &data,int uSlack);


signals:

public slots:


private:
    zchxMapWidget       *mDisplayWidget;
    std::vector<SpecialRouteLine> m_AllProjectSpecialRoute;//所有项目特殊路由(南方电网)
    std::vector<SpecialRouteLine> m_SpecialRouteLine;      //当前项目特殊路由(南方电网)
    std::vector<RouteCross> m_RouteCross;                  //交越路由(全项目)
    int m_uCurrentProjectID;                                            //当前项目ID
    ZCHX::Data::CableAssembly m_CableAssembly;                          //创建路由时候的默认海缆
    int m_uSlack;                                                       //默认余量
    std::vector<RouteLine> m_AllProjectRouteLine;          //所有项目路由线
    std::vector<RouteLine> m_RouteLine;                    //路由线(当前项目)
    std::vector<RouteLine> m_historyRouteLine;             //历史路由线
};
}

#endif // ZCHXROUTEDATAMGR_H
