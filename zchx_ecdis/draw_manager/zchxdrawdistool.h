#ifndef ZCHXDRAWDISTOOL_H
#define ZCHXDRAWDISTOOL_H

#include "zchxdrawtool.h"
namespace qt{
class zchxDrawDistanceTool : public zchxDrawTool
{
    Q_OBJECT
public:
    explicit zchxDrawDistanceTool(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter *painter, double offset_x, double offset_y);

signals:

public slots:
};
}

#endif // ZCHXDRAWDISTOOL_H
