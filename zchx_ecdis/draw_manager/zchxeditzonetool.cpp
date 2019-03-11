#include "zchxeditzonetool.h"
#include "element/moveelement.h"
#include "zchxmapframe.h"

using namespace qt;

zchxEditZoneTool::zchxEditZoneTool(zchxMapWidget* w, int type, int num,  QObject *parent) : zchxDrawTool(w, type, parent),mMinNum(num)
{
}

bool zchxEditZoneTool::checkPnts()
{
    if(!element()) return false;
    std::vector<std::pair<double, double>> path = element()->path();
    if(path.size() <= mMinNum) {
        QMessageBox box;
        box.setWindowTitle(tr("Warring"));
        box.setText(tr("At least %1 points. delete cannot de done.").arg(mMinNum));
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,tr("OK"));
        box.exec ();
        return false;
    }
    return true;
}

QList<QAction*> zchxEditZoneTool::getRightMenuActions(const QPoint& pt)
{
    QList<QAction*> list;
    list.append(addAction(tr("确认"),this, SLOT(endDraw()), 0));
    list.append(addAction(tr("取消"), this, SLOT(slotCancel()), 0));
    return list;
}


void zchxEditZoneTool::moveWithOffset(double lat, double lon)
{
    if(!element() || !isStartMove()) return;
    element()->moveTo(lat, lon);
}

void zchxEditZoneTool::selectCtrlPoint(const QPoint &pt)
{
    if(!mWidget || !mWidget->framework()) return;
    MoveElement *ele = element();
    if(!ele) return;
    if(ele->activePathPoint() >= 0)
    {
        //确认当前的修改
        ele->setActivePathPoint(-1);
        return;
    }

    std::vector<std::pair<double, double>> path = ele->path();
    for(int i = 0; i < path.size(); ++i)
    {
        std::pair<double, double> ll = path[i];
        ZCHX::Data::Point2D pos = mWidget->framework()->LatLon2Pixel(ll.first, ll.second);
        QRect rect(0, 0, 5, 5);
        rect.moveCenter(pos.toPoint());
        if(rect.contains(pt)){
            ele->setActivePathPoint(i);
            break;
        }
    }
}

void zchxEditZoneTool::changeCtrlPoint(const QPoint &pt)
{
    if(!mWidget || !mWidget->framework()) return;
    MoveElement *ele = element();
    if(!ele) return;

    ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(pt);
    ele->changePathPoint(ll.lat, ll.lon);
}

void zchxEditZoneTool::addCtrlPoint(const QPoint &pt)
{
    if(!mWidget || !mWidget->framework()) return;
    MoveElement *ele = element();
    if(!ele) return;

    ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(pt);
    ele->addCtrlPoint(std::pair<double, double>(ll.lat, ll.lon));
}

void zchxEditZoneTool::delCtrlPoint(const QPoint &pt)
{
    if(!mWidget || !mWidget->framework()) return;
    if(!checkPnts()) return;
    MoveElement *ele = element();
    std::vector<std::pair<double, double>> path = ele->path();
    for(int i = 0; i < path.size(); ++i)
    {
        std::pair<double, double> ll = path[i];
        ZCHX::Data::Point2D pos = mWidget->framework()->LatLon2Pixel(ll.first, ll.second);
        QRect rect(0, 0, 5, 5);
        rect.moveCenter(pos.toPoint());
        if(rect.contains(pt)){
            ele->delPathPoint(i);
            break;
        }
    }
}

void zchxEditZoneTool::updateOldPath()
{
    if(element())element()->updateOldPath();
}

void zchxEditZoneTool::endDraw()
{
    if(element() && mWidget){
        if(element()->getElementType() == ZCHX::Data::ELE_WARNING_ZONE)
        {
            emit mWidget->signalUpdateWarringZoneOrLineState(1, 1);
        } else if(element()->getElementType() == ZCHX::Data::ELE_CHANNEL)
        {
            emit mWidget->signalUpdateChannelZoneState(1, 1);
        } else if(element()->getElementType() == ZCHX::Data::ELE_CARD_MOUTH)
        {
            emit mWidget->signalUpdateCardMouthZoneState(1, 1);
        } else if(element()->getElementType() == ZCHX::Data::ELE_MOOR)
        {
            emit mWidget->signalUpdateMooringZoneState(1, 1);
        }
    }
    zchxDrawTool::endDraw();
}

void zchxEditZoneTool::slotCancel()
{
    if(element() && mWidget){
        if(element()->getElementType() == ZCHX::Data::ELE_WARNING_ZONE)
        {
            emit mWidget->signalUpdateWarringZoneOrLineState(1, 0);
        } else if(element()->getElementType() == ZCHX::Data::ELE_CHANNEL)
        {
            emit mWidget->signalUpdateChannelZoneState(1, 0);
        } else if(element()->getElementType() == ZCHX::Data::ELE_CARD_MOUTH)
        {
            emit mWidget->signalUpdateCardMouthZoneState(1, 0);
        } else if(element()->getElementType() == ZCHX::Data::ELE_MOOR)
        {
            emit mWidget->signalUpdateMooringZoneState(1, 0);
        }
    }
    zchxDrawTool::endDraw();
}



