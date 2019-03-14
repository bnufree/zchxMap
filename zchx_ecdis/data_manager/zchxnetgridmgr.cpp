#include "zchxnetgridmgr.h"
#include "zchxmapframe.h"

namespace qt {
zchxNetGridDataMgr::zchxNetGridDataMgr(zchxMapWidget *w, QObject *parent):
    zchxTemplateDataMgr<GridElement, ZCHX::Data::ITF_NetGrid>(w, ZCHX::DATA_MGR_NET_GRID, ZCHX::LAYER_CAMERANETGRID)
{

}

void zchxNetGridDataMgr::show(QPainter *painter)
{
    if( !painter || mData.empty()) return;
    for(std::shared_ptr<GridElement> ele : mData)
    {
        if(!MapLayerMgr::instance()->isLayerVisible(ele->layerName())) continue;
        if(ele.get() == mDisplayWidget->getCurrentSelectedElement()){
            ele->setIsActive(true);
        } else {
            ele->setIsActive(false);
        }
        ele->drawElement(painter);
    }
}

bool zchxNetGridDataMgr::updateActiveItem(const QPoint &pt)
{
    //网格不作为item被选择
//    if( !MapLayerMgr::instance()->isLayerVisible(layerName()) || mData.empty() || !isPickupAvailable()) return false;
//    for(std::shared_ptr<CameraGridElement> ele : mData)
//    {
//        if(ele->contains(pt)){
//            mDisplayWidget->setCurrentSelectedItem(ele.get());
//            return true;
//        }
//    }
    return false;
}

//void zchxCameraGridDataMgr::setCameraNetGridList(const QList<ZCHX::Data::ITF_CameraNetGrid> &list)
//{
//    QList<ZCHX::Data::ITF_NetGrid> res;
//    for(ZCHX::Data::ITF_CameraNetGrid data : list)
//    {
//        res.append(data.mNetGridList);
//    }
//    setData(res);
//}
}

