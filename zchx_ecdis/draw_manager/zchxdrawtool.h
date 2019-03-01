#ifndef ZCHXDRAWTOOL_H
#define ZCHXDRAWTOOL_H

#include <QObject>
#include "zchxecdismousedefines.h"
#include "element/drawelement.hpp"
#include "map_layer/zchxmaplayermgr.h"
#include "../zchxutils.hpp"
#include "../zchxMapDatautils.h"
#include "../profiles.h"
#include "../zchxmapwidget.h"
#include <QPainter>
#include <QAction>

namespace qt {

class zchxDrawTool : public QObject
{
    Q_OBJECT
public:
    explicit        zchxDrawTool(zchxMapWidget* w, int type, QObject *parent = 0);
    int             getType() const {return mType;}
    virtual void    show(QPainter* painter, double offset_x, double offset_y);
    virtual void            appendPoint(const QPointF& pnt);
    void            clearPoints();
    virtual void    endDraw();
    virtual void    startDraw();

    virtual bool    isReady();

signals:

public slots:

protected:
    QList<QPointF>       mPoints;
    int                 mType;                      //数据管理类型
    zchxMapWidget*      mWidget;

};
}

#endif // ZCHXDRAWTOOL_H

