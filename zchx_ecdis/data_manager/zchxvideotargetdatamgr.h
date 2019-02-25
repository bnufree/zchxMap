#ifndef ZCHXCAMERAVIDEOTARGETDATAMGR_H
#define ZCHXCAMERAVIDEOTARGETDATAMGR_H

#include "zchxecdisdatamgr.h"

namespace qt {
class zchxVideoTargetDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxVideoTargetDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter *painter);
    bool updateActiveItem(const QPoint &pt);

    //更新数据
    void setData(const QList<ZCHX::Data::ITF_VideoTarget> &list);

signals:

public slots:

private:
    QMap<QString, std::shared_ptr<VideoTargetElement>> mTargetMap;
};
}

#endif // ZCHXCAMERAVIDEOTARGETDATAMGR_H
