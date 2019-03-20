#ifndef ZCHXEDITZONETOOL_H
#define ZCHXEDITZONETOOL_H

#include "zchxdrawtool.h"

namespace qt {
class zchxEditZoneTool : public zchxDrawTool
{
    Q_OBJECT
public:
    explicit zchxEditZoneTool(zchxMapWidget* w, int type, int num, QObject *parent = 0);
    virtual QList<QAction*> getRightMenuActions(const QPoint &pt);
    virtual void                moveWithOffset(double lat, double lon);
    virtual void                selectCtrlPoint(const QPoint& pt);
    virtual void                changeCtrlPoint(const QPoint& pt);
    virtual void                addCtrlPoint(const QPoint &pt);
    virtual void                delCtrlPoint(const QPoint &pt);
    virtual void                updateOldPath();
    virtual bool                checkPnts();
    virtual void                endDraw();
    void                        setMinPntNum(int num) {mMinNum = num;}
signals:

public slots:
    void                        slotCancel();
private:
    int                         mMinNum;
};
}

#endif // ZCHXEDITZONETOOL_H
