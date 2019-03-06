#include "zchxdrawtool.h"


namespace qt {
zchxDrawTool::zchxDrawTool(zchxMapWidget* w, int type, QObject *parent) : QObject(parent)
  ,mWidget(w)
  ,mType(type)
{

}


void zchxDrawTool::show(QPainter* painter)
{
    return;
}

void zchxDrawTool::startDraw()
{
    clearPoints();
}

void zchxDrawTool::appendPoint(const QPointF& pnt)
{
    mPoints.append(pnt);
}

int zchxDrawTool::getPointSize()
{
    return mPoints.size();
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
    if(!mWidget || !mWidget->framework() || mPoints.empty()) return false;
    return true;
}


}




