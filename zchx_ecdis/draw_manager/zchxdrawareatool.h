#ifndef ZCHXDRAWAREATOOL_H
#define ZCHXDRAWAREATOOL_H

#include "zchxdrawtool.h"
namespace qt{
class zchxDrawAreaTool : public zchxDrawTool
{
    Q_OBJECT
public:
    explicit zchxDrawAreaTool(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter *painter, double offset_x, double offset_y);

signals:

public slots:
};
}

#endif // ZCHXDRAWAREATOOL_H
