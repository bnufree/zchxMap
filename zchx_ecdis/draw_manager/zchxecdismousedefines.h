#ifndef ZCHXECDISMOUSEDEFINES_H
#define ZCHXECDISMOUSEDEFINES_H

namespace qt {
enum eTool{
    DRAWNULL = 100,
    DRAWMEASUREAREA,
    DRAWDISTANCE,
    DRAWDIRANGLE,
    DRAWLOCALMARK,                  //位置标注 Add by yej
    DRAWPICKUP,
    DRAWGPS,
    ARESELECTD,                     //框选一片区域
    COMMONZONESELECT,               //选取区域
    ZONEDRAW,                       //绘制防区
    ZONESELECT,                     //选取防区
    ZONEMOVE,                       //移动防区
    ZONEMOVECTRL,                   //防区控制点微调
    ZONEDELCTRL,                    //删除控制点
    ZONEADDCTRL,                    //添加控制点
    ZONEDEL,                        //删除防区
    ISLANDLINEDRAW,                 //绘制环岛线
    ISLANDLINESELECT,               //选取环岛线
    ISLANDLINEMOVE,                 //移动环岛线
    ISLANDLINEMOVECTRL,             //环岛线控制点微调
    ISLANDLINEDELCTRL,              //删除环岛线控制点
    ISLANDLINEADDCTRL,              //添加环岛线控制点
    ISLANDLINEDEL,                  //删除环岛线
    ZONEDRAWRADAR,                  //绘制雷达特征区域
    ROUTELINEDRAW,                  //绘制路由线
    ROUTELINEELECT,                 //选取路由线
    ROUTELINEDEL,                   //删除路由线
    ROUTEMOVECTRL,                  //路由线控制点微调
    ROUTEDELCTRL,                   //删除路由线控制点
    ROUTEADDCTRL,                   //添加路由线控制
    ROUTEINSERTCTRL,                //插入路由线控制点
    ROUTEORCROSSPICKUP,             //拾取路由或者交越点（显示拾取信息）
    TRACKTARGET,                    //选取跟踪目标 (导航)
    CAMERATEACK,                    //视频跟踪目标
    SHIPPLANDRAW,                   //绘制船舶计划
    SHIPPLANSELECT,                 //选取船舶计划航线
    SHIPPLANDEL,                    //删除船舶计划航线
    SHIPPLANMOVECTRL,               //船舶计划航线点移动
    SHIPPLANEDELCTRL,               //删除船舶计划航线点
    SHIPPLANADDCTRL,                //添加船舶计划航线点
    SHIPPLANINSERTCTRL,             //插入船舶计划航线点
    SHIPSIMULATIONDRAW,             //绘制船舶模拟航线
    LOCALMARKPOSTION,               //标记点
    FIXEFREFERENCEPOINT,            //固定参考点
    CUSTOMFLOWLINE,                 //流量线
    COASTDATALINEDRAW,              //绘制海岸数据
    SEABEDPIPELINEDRAW,             //绘制海底管线
    STRUCTUREPOINTDRAW,             //绘制结构物
    AREANETZONEDRAW,                //绘制地理区域网络
    CHANNELMANAGER,                 //航道管理
    CHANNELSELECT,                  //选取航道
    CHANNELMOVE,                    //移动航道
    CHANNELMOVECTRL,                //航道控制点微调
    CHANNELDELCTRL,                 //删除控制点
    CHANNELADDCTRL,                 //添加控制点
    CHANNELDEL,                     //删除航道
    MOORINGMANAGER,                 //锚泊管理
    MOORINGSELECT,                  //选取锚泊
    MOORINGMOVE,                    //移动锚泊
    MOORINGMOVECTRL,                //锚泊控制点微调
    MOORINGDELCTRL,                 //删除控制点
    MOORINGADDCTRL,                 //添加控制点
    MOORINGDEL,                     //删除航道
    CARDMOUTHMANAGER,               //卡口管理
    CARDMOUTHSELECT,                //选取卡口
    CARDMOUTHMOVE,                  //移动卡口
    CARDMOUTHMOVECTRL,              //卡口控制点微调
    CARDMOUTHDELCTRL,               //删除控制点
    CARDMOUTHADDCTRL,               //添加控制点
    CARDMOUTHDEL,                   //删除航道
    DRAWCAMERANETGRID,              //相机网格图
    PICKUPPTZ,                      //PTZ获取
};
}
#endif // ZCHXECDISMOUSEDEFINES_H
