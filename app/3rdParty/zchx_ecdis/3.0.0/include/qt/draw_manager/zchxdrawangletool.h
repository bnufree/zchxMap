#ifndef ZCHXDRAWANGLETOOL_H
#define ZCHXDRAWANGLETOOL_H

#include "zchxdrawtool.h"
namespace qt{
class zchxDrawAngleTool : public zchxDrawTool
{
    Q_OBJECT
public:
    explicit zchxDrawAngleTool(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter *painter);
    void appendPoint(const QPointF& pnt);

signals:

public slots:
};
}

#endif // ZCHXDRAWAREATOOL_H
