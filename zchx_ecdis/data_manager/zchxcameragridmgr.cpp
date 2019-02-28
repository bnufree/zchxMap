#include "zchxcameragridmgr.h"
#include "zchxmapframe.h"

namespace qt {
zchxCameraGridDataMgr::zchxCameraGridDataMgr(zchxMapWidget *w, QObject *parent):
    zchxTemplateDataMgr<CameraGridElement, ZCHX::Data::ITF_NetGrid>(w, DATA_MGR_CAMERA_NET_GRID, ZCHX::LAYER_CAMERANETGRID)
{

}

bool zchxCameraGridDataMgr::updateActiveItem(const QPoint &pt)
{
    if( !MapLayerMgr::instance()->isLayerVisible(layerName()) || mData.empty()) return false;
    for(std::shared_ptr<CameraGridElement> ele : mData)
    {
        if(ele->contains(pt)){
            mDisplayWidget->setCurrentSelectedItem(ele.get());
            return true;
        }
    }
    return false;
}

void zchxCameraGridDataMgr::setCameraNetGridList(const QList<ZCHX::Data::ITF_CameraNetGrid> &list)
{
    QList<ZCHX::Data::ITF_NetGrid> res;
    for(ZCHX::Data::ITF_CameraNetGrid data : list)
    {
        res.append(data.mNetGridList);
    }
    setData(res);
}

void zchxCameraGridDataMgr::show(QPainter *painter)
{
    //检查是否正在鼠标操作
    if(isDraw()){
        ZCHX::Data::ITF_CameraNetGrid res = makeCameraGrid(mStartPnt, mEndPnt);
        if(res.mNetGridList.size() > 0 )
        {
            setCameraNetGridList(QList<ZCHX::Data::ITF_CameraNetGrid>() << res);
        }
    }
    zchxTemplateDataMgr::show(painter);
}


void zchxCameraGridDataMgr::setCameraGridParam(const QString &id, const QSizeF &size)
{
    mCameraID = id;
    mSize = size;
    mData.clear();
}

void zchxCameraGridDataMgr::setDrawLonLat(const LatLon &start, const LatLon &end)
{
    mStartPnt = start;
    mEndPnt = end;
}

ZCHX::Data::ITF_CameraNetGrid zchxCameraGridDataMgr::makeCameraGrid(const LatLon &sll, const LatLon &ell)
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
    ZCHX::Data::ITF_CameraNetGrid res;
    res.mCameraID = mCameraID;
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
            ZCHX::Data::ITF_NetGrid grid;
            grid.id = id;
            grid.name = QString("%1_%2").arg(mCameraID).arg(id);
            grid.leftTopLat = top_left_ll.lat;
            grid.leftTopLon = top_left_ll.lon;
            grid.rightLowerLat = low_right_ll.lat;
            grid.rightLowerLon = low_right_ll.lon;
            //qDebug()<<grid.gridId<<grid.gridName<<grid.leftTopLat<<grid.leftTopLon<<grid.rightLowerLat<<grid.rightLowerLon<<k<<i<<sll.lat<<sll.lon;
            res.mNetGridList.append(grid);
        }
    }

    return res;
}




}

