#include "zchxdrawtool.h"
#include "zchxmapframe.h"


namespace qt {
zchxDrawTool::zchxDrawTool(zchxMapWidget* w, int type, QObject *parent) : QObject(parent)
  ,mWidget(w)
  ,mType(type)
  ,mEle(0)
  ,mIsStartMove(0)
{

}

zchxDrawTool::~zchxDrawTool()
{
    qDebug()<<" draw tool release now!"<<this->metaObject()->className();
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
    if(!mWidget) return;
    ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(ZCHX::Data::Point2D(pnt));
    //检查是否与前一个点相同,相同就不添加
    if(mPoints.size() > 0 && mPoints.last() == ll) return;
    mPoints.append(ll);
}

QPolygonF zchxDrawTool::convertLL2Polygon() const
{
    QPolygonF poly;
    if(mWidget)
    {
        foreach (ZCHX::Data::LatLon ll, mPoints) {
            poly.append(mWidget->framework()->LatLon2Pixel(ll).toPointF());
        }
    }
    return poly;
}

int zchxDrawTool::getPointSize()
{
    return mPoints.size();
}

void zchxDrawTool::endDraw()
{
    clearPoints();
    if(mWidget) mWidget->releaseDrawStatus();
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

QList<QAction*> zchxDrawTool::getRightMenuActions(const QPoint& pt)
{
    QList<QAction*> list;
    list.append(addAction(tr("重新开始"),this, SLOT(startDraw()), 0));
    if(mWidget){
        list.append(addAction(tr("结束"), mWidget, SLOT(releaseDrawStatus()), 0));
    }
    return list;
}

QAction* zchxDrawTool::addAction(const QString &text, const QObject *obj, const char* slot, void* userData)
{
    if(!obj || !slot || strlen(slot) == 0) return 0;

    QAction *result(new QAction(text, this));
    if(QString(slot).contains("(bool)"))
        QObject::connect(result, SIGNAL(triggered(bool)), obj, slot);
    else
        QObject::connect(result,SIGNAL(triggered()),obj,slot);

    if(userData) result->setData(QVariant::fromValue(userData));
    return result;
}

void zchxDrawTool::setElement(MoveElement *ele)
{
    mEle = ele;
}

MoveElement* zchxDrawTool::element()
{
    return mEle;
}


}




