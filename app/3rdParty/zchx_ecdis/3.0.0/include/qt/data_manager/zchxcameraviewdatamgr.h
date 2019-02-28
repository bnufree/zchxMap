#ifndef ZCHXCAMERAVIEWDATAMGR_H
#define ZCHXCAMERAVIEWDATAMGR_H

#include "zchxecdisdatamgr.h"
namespace qt {
class zchxCameraViewDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxCameraViewDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter *painter);
    bool updateActiveItem(const QPoint &pt);

    //更新数据
    void setData(const QList<ZCHX::Data::ITF_CameraView> &list);

signals:

public slots:
private:
    QMap<QString, std::shared_ptr<CameraViewElement>> mData;
};
}

#endif // ZCHXCAMERAVIEWDATAMGR_H
