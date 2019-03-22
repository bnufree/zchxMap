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
    explicit                    zchxDrawTool(zchxMapWidget* w, int type, QObject *parent = 0);
    virtual                     ~zchxDrawTool();
    int                         getType() const {return mType;}
    int                         getPointSize();
    virtual void                show(QPainter* painter);
    virtual QList<QAction*>     getRightMenuActions(const QPoint& pt);
    QAction*                    addAction(const QString &text, const QObject *obj, const char* slot, void* userData);
    //edit
    void                        setElement(MoveElement *ele);
    MoveElement*                element();
    virtual void                moveWithOffset(double lat, double lon) {}
    virtual void                selectCtrlPoint(const QPoint& pt) {}
    virtual void                changeCtrlPoint(const QPoint& pt) {}
    virtual void                addCtrlPoint(const QPoint& pt) {}
    virtual void                delCtrlPoint(const QPoint& pt) {}
    virtual void                updateOldPath() {}
    bool                        isStartMove() const {return mIsStartMove;}
    void                        setStartMove(bool sts) {mIsStartMove = sts;}

public slots:
    virtual void                appendPoint(const QPointF& pnt);
    void                        clearPoints();
    virtual void                endDraw();
    virtual void                startDraw();
    virtual bool                isReady();


signals:
    void    stopTool();
public slots:

protected:
    QList<QPointF>       mPoints;
    int                 mType;                      //数据管理类型
    zchxMapWidget*      mWidget;
    MoveElement*        mEle;
    bool                mIsStartMove;

};
}

#endif // ZCHXDRAWTOOL_H

