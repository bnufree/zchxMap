#include "zchxdrawtool.h"


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
    //检查是否与前一个点相同,相同就不添加
    if(mPoints.size() > 0 && mPoints.last() == pnt) return;
    mPoints.append(pnt);
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




