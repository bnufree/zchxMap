#ifndef ZCHXDATAMGRFACTORY_H
#define ZCHXDATAMGRFACTORY_H

#include "zchxdatamanagers.h"

namespace qt {
class   zchxMapWidget;

class zchxDataMgrFactory
{
public:
    explicit zchxDataMgrFactory(zchxMapWidget* w);
    std::shared_ptr<zchxEcdisDataMgr>   createManager(int type);
    QList<std::shared_ptr<zchxEcdisDataMgr>>    getManagers() const;
    std::shared_ptr<zchxEcdisDataMgr> getManager(int type);


    bool removeDataMgr(std::shared_ptr<zchxEcdisDataMgr> mgr);
    bool appendDataMgr(std::shared_ptr<zchxEcdisDataMgr> mgr);
    //具体各个数据类型获取
    zchxAisDataMgr* getAisDataMgr();
    zchxRadarDataMgr* getRadarDataMgr();
    zchxRodDataMgr* getRodDataMgr();
    zchxCameraDataMgr* getCameraDataMgr();
    zchxCameraViewDataMgr* getCameraViewMgr();
    zchxVideoTargetDataMgr* getVideoDataMgr();

private:
    zchxMapWidget*      mWidget;
    QMap<int, std::shared_ptr<zchxEcdisDataMgr>>  mMgrList;

};
}

#endif // ZCHXDATAMGRFACTORY_H
