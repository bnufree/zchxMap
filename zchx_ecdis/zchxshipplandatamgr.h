#ifndef ZCHXSHIPPLANDATAMGR_H
#define ZCHXSHIPPLANDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>
#include <QMutex>

namespace qt {
class zchxMapWidget;

class zchxShipPlanDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxShipPlanDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void setAllProjecrShipPlanLineData(const std::vector<ShipPlanLineElement*> &data);
    void setShipPlanLineData(const std::vector<ShipPlanLineElement*> &data);
    void setShipPlanProjectID(const int ProjectID);

signals:

public slots:


private:
    zchxMapWidget       *mDisplayWidget;
    std::vector<ShipPlanLineElement*> m_ShipPlanLine;              //船舶计划(当前项目)
    std::vector<ShipPlanLineElement*> m_AllShipPlanLines;          //
    int m_uCurrentProjectID;                                            //当前项目ID
};
}

#endif // ZCHXSHIPPLANDATAMGR_H
