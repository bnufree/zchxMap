#include "zchxcameragridmgr.h"
#include "zchxmapframe.h"

namespace qt {
zchxCameraGridDataMgr::zchxCameraGridDataMgr(zchxMapWidget *w, QObject *parent):
    zchxTemplateDataMgr<CameraGridElement, ZCHX::Data::ITF_NetGrid>(w, ZCHX::DATA_MGR_CAMERA_NET_GRID, ZCHX::LAYER_CAMERANETGRID)
{

}

bool zchxCameraGridDataMgr::updateActiveItem(const QPoint &pt)
{
    if( !MapLayerMgr::instance()->isLayerVisible(layerName()) || mData.empty() || !isPickupAvailable()) return false;
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
}

