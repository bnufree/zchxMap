#ifndef ZCHXCAMERAGRIDMGR_H
#define ZCHXCAMERAGRIDMGR_H

#include "zchxtemplatedatamgr.h"

namespace qt {
class zchxCameraGridDataMgr : public zchxTemplateDataMgr<CameraGridElement, ZCHX::Data::ITF_NetGrid>
{
    Q_OBJECT
public:
    explicit zchxCameraGridDataMgr(zchxMapWidget* w, QObject *parent = 0);
    bool    updateActiveItem(const QPoint& pt);
    void    setCameraNetGridList(const QList<ZCHX::Data::ITF_CameraNetGrid> & list);
signals:




public slots:


private:
};
}

#endif // ZCHXCAMERAGRIDMGR_H
