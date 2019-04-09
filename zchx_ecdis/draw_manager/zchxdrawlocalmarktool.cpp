#include "zchxdrawlocalmarktool.h"
#include "zchxmapframe.h"
#include "dialog/localmarkdlg.h"

using namespace qt;
using namespace ZCHX::Data;

zchxDrawLocalMarkTool::zchxDrawLocalMarkTool(zchxMapWidget* w, QObject *parent) : zchxDrawTool(w, qt::eTool::DRAWLOCALMARK, parent)
{

}

void zchxDrawLocalMarkTool::appendPoint(const QPointF &pnt)
{
    if(!mWidget) return;
    if(mPoints.size() == 0) {
        mPoints.append(mWidget->framework()->Pixel2LatLon(pnt));
    } else {
        mPoints[0] = mWidget->framework()->Pixel2LatLon(pnt);
    }

}

void zchxDrawLocalMarkTool::endDraw()
{
    if(!isReady()) return ;
    LatLon ll = mPoints[0];
    LocalMarkDlg d;
    d.setLocalMarkPos(ll.lon, ll.lat);
    d.move(QCursor::pos().x() - d.width() / 2, QCursor::pos().y() - d.height() / 2 );
    if(d.exec() == QDialog::Accepted)
    {
        ZCHX::Data::ITF_LocalMark mark;
        d.getLocalMardData(mark);
        emit mWidget->signalCreateLocalMark(mark);
    }
    zchxDrawTool::endDraw();
    return;
}
