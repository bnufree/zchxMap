#ifndef ZCHXNETGRIDMGR_H
#define ZCHXNETGRIDMGR_H

#include "zchxtemplatedatamgr.h"

namespace qt {
class zchxNetGridDataMgr : public zchxTemplateDataMgr<GridElement, ZCHX::Data::ITF_NetGrid>
{
    Q_OBJECT
public:
    explicit zchxNetGridDataMgr(zchxMapWidget* w, QObject *parent = 0);
    bool    updateActiveItem(const QPoint& pt);
    void    show(QPainter *painter);
    //void    setCameraNetGridList(const QList<ZCHX::Data::ITF_CameraNetGrid> & list);
signals:




public slots:


private:
};
}

#endif // ZCHXNETGRIDMGR_H
