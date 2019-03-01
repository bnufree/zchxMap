#include "zchxdrawtool.h"


namespace qt {
zchxDrawTool::zchxDrawTool(zchxMapWidget* w, int type, QObject *parent) : QObject(parent)
  ,mWidget(w)
  ,mType(type)
{

}


void zchxDrawTool::show(QPainter* painter, double offset_x, double offset_y)
{
    return;
}

void zchxDrawTool::startDraw()
{
    clearPoints();
}

void zchxDrawTool::appendPoint(const QPointF&)
{
    mPoints.append(pnt);
}

void zchxDrawTool::endDraw()
{
    clearPoints();
}

void zchxDrawTool::clearPoints()
{
    mPoints.clear();
}

bool zchxDrawTool::isReady()
{
    if(!painter || !mWidget || !mWidget->framework() || mPoints.empty()) return false;
    return true;
}


}




