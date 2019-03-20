#ifndef ZCHXDRAWLOCALMARKTOOL_H
#define ZCHXDRAWLOCALMARKTOOL_H

#include "zchxdrawtool.h"
namespace qt{
class zchxDrawLocalMarkTool : public zchxDrawTool
{
    Q_OBJECT
public:
    explicit zchxDrawLocalMarkTool(zchxMapWidget* w, QObject *parent = 0);
    //void show(QPainter *painter);
    void appendPoint(const QPointF& pnt);
    void endDraw();

signals:

public slots:
};
}

#endif // ZCHXDRAWAREATOOL_H
