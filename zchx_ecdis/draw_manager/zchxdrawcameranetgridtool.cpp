#include "zchxdrawcameranetgridtool.h"
#include "zchxmapframe.h"
#include "localmarkdlg.h"

using namespace qt;
using namespace ZCHX::Data;

zchxDrawCameraNetGridTool::zchxDrawCameraNetGridTool(zchxMapWidget* w, QObject *parent) : zchxDrawTool(w, qt::eTool::DRAWCAMERANETGRID, parent)
{

}

void zchxDrawCameraNetGridTool::appendPoint(const QPointF &pnt)
{
    if(mPoints.size() < 2) {
        mPoints.append(pnt);
    } else {
        mPoints[1] = pnt;
    }

}

void zchxDrawCameraNetGridTool::show(QPainter *painter)
{
    if( !painter || !MapLayerMgr::instance()->isLayerVisible(ZCHX::LAYER_CAMERANETGRID)) return;
    if(!isReady()) return;
    PainterPair chk(painter);
    painter->setRenderHint(QPainter::Antialiasing);
    if(mPoints.size() >= 2)
    {
        LatLon p1 = mWidget->framework()->Pixel2LatLon(mPoints[0]);
        LatLon p2 = mWidget->framework()->Pixel2LatLon(mPoints[1]);
        ZCHX::Data::ITF_NetGrid res = makeCameraGrid(p1, p2);
        if(res.mNetGridList.size() > 0)
        {
            std::shared_ptr<GridElement> ele(new GridElement(res, mWidget));
            ele->drawElement(painter);
        }

    }
}

void zchxDrawCameraNetGridTool::endDraw()
{
    if(mPoints.size() == 2)
    {
        //弹出对话框是否保存当前的网格
        QMessageBox box;
        box.setWindowTitle(tr("提示"));
        box.setText(tr("是否更新当前的相机网格信息?"));
        box.setStandardButtons (QMessageBox::Ok|QMessageBox::No);
        box.setButtonText(QMessageBox::Ok,tr("确认"));
        box.setButtonText(QMessageBox::No,tr("取消"));
        if(box.exec () == QMessageBox::Ok)
        {
            LatLon p1 = mWidget->framework()->Pixel2LatLon(mPoints[0]);
            LatLon p2 = mWidget->framework()->Pixel2LatLon(mPoints[1]);
            if(mWidget) mWidget->signalSendCameraNetGrid(makeCameraGrid(p1, p2));
        }
    }
    zchxDrawTool::endDraw();
    return;
}

void zchxDrawCameraNetGridTool::setCameraGridParam(const QString &id, const QSizeF &size)
{
    mCameraID = id;
    mSize = size;
}

ZCHX::Data::ITF_NetGrid zchxDrawCameraNetGridTool::makeCameraGrid(const LatLon &sll, const LatLon &ell)
{
    //需要先判断方向
    int lon_positive = (ell.lon >= sll.lon ? 1 : -1);
    int lat_positive = (ell.lat >= sll.lat ? 1 : -1);
    //计算当前放大水平下对应的网格大小对应的经纬度
    double latMeters = mSize.width();
    double lonMeters = mSize.height();
    double rect_width = zchxMapDataUtils::DistanceOnEarth(sll,  LatLon(sll.lat, ell.lon));
    double rect_height = zchxMapDataUtils::DistanceOnEarth(sll, LatLon(ell.lat, sll.lon));

    int num_x = rect_width / lonMeters;
    if(rect_width - num_x * lonMeters > 0.5 * lonMeters)
    {
        num_x++;
    }
    int num_y = rect_height / latMeters;
    if(rect_height - num_y * latMeters > 0.5 * latMeters)
    {
        num_y++;
    }

    lonMeters *= lon_positive;
    latMeters *= lat_positive;
    //qDebug()<<"row col:"<<num_y<<num_x;
    int id = 0;
    ZCHX::Data::ITF_NetGrid res;
    res.mColor = Qt::darkGray;
    res.mName = mCameraID;
    res.mType = ZCHX::Data::NetGrid_Camera;
    res.mLayer = ZCHX::LAYER_CAMERANETGRID;
    for(int i=0; i<num_y; i++)
    {
        for(int k=0; k<num_x; k++)
        {
            //计算左上角点的经纬度
            LatLon top_left_ll = zchxMapDataUtils::getSmPoint(sll, k * lonMeters, i * latMeters);
            //qDebug()<<"calc lat lon:"<<top_left_ll.lat<<top_left_ll.lon<<i*latMeters<<i*lonMeters;
            //计算右下角点的经纬度
            LatLon low_right_ll = zchxMapDataUtils::getSmPoint(sll, (k + 1) * lonMeters, (i + 1) * latMeters);
            if((low_right_ll.lon > ell.lon && lon_positive == 1) ||
                (low_right_ll.lon < ell.lon && lon_positive == -1))
            {
                low_right_ll.lon = ell.lon;
            }
            if((low_right_ll.lat > ell.lat && lat_positive == 1) ||
                    (low_right_ll.lat < ell.lat && lat_positive == -1))
            {
                low_right_ll.lat = ell.lat;
            }
            id++;
            ZCHX::Data::ITF_NetGridPolygon grid;
            grid.id = id;
            grid.name = QString("%1_%2").arg(mCameraID).arg(id);
            grid.mLatLonList.append(top_left_ll);
            grid.mLatLonList.append(low_right_ll);
            res.mNetGridList.append(grid);
        }
    }

    return res;
}
