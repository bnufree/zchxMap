#include "zchxdrawlocalmarktool.h"
#include "zchxmapframe.h"
#include "localmarkdlg.h"

using namespace qt;
zchxDrawLocalMarkTool::zchxDrawLocalMarkTool(zchxMapWidget* w, QObject *parent) : zchxDrawTool(w, qt::eTool::DRAWDIRANGLE, parent)
{

}

void zchxDrawLocalMarkTool::appendPoint(const QPointF &pnt)
{
    if(mPoints.size() == 0) {
        mPoints.append(pnt);
    } else {
        mPoints[0] = pnt;
    }

}

//void zchxDrawLocalMarkTool::show(QPainter *painter, double offset_x, double offset_y)
//{
//    if(!isReady()) return;
//    PainterPair chk(painter);
//    painter->setRenderHint(QPainter::Antialiasing);
//    //在当前位置画图片
//    painter->drawPixmap(mPoints[0], QPixmap(":/mouseCursor/mousecursor/positionMark.svg"));
//}

void zchxDrawLocalMarkTool::endDraw()
{
    if(!isReady()) return ;
    LatLon ll = mWidget->framework()->Pixel2LatLon(mPoints[0]);
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
