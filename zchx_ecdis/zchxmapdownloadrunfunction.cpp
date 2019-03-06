#include "zchxmapdownloadrunfunction.h"
#include "zchxMapDataUtils.h"
#include "qhttpget.h"
#include <QPixmap>
#include <QDebug>
#include <QDir>

namespace qt {
zchxMapDownloadRunFunction::zchxMapDownloadRunFunction(const zchxMapRange& range, int zoom):QRunnable(),mZoom(zoom), mRange(range)
{
    setAutoDelete(true);
}

void zchxMapDownloadRunFunction::run()
{
    double resolution = EARTH_HALF_CIRCUL_LENGTH * 2 / MAP_IMG_SIZE / pow(2, mZoom);
    //保存位置建立
    QString savePath = QDir::currentPath() + QObject::tr("/MapData/%1").arg(mZoom);
    QDir dir;
    if(!dir.exists(savePath))
    {
        dir.mkpath(savePath);
    }

    //计算显示位置对应的墨卡托坐标范围
    //计算当前中心经纬度对应的墨卡托坐标
    ZCHX::Data::Mercator bottom_left = zchxMapDataUtils::wgs84LonlatToMercator(ZCHX::Data::LatLon(mRange.bottemLat, mRange.bottemLon ));
    ZCHX::Data::Mercator top_right = zchxMapDataUtils::wgs84LonlatToMercator(ZCHX::Data::LatLon(mRange.topLat, mRange.topLon ));

    zchxMapBounds view_bounds = { bottom_left.mX, bottom_left.mY, top_right.mX, top_right.mY };
    zchxMapBounds total_bounds = {EARTH_HALF_CIRCUL_LENGTH *(-1), EARTH_HALF_CIRCUL_LENGTH * (-1), EARTH_HALF_CIRCUL_LENGTH, EARTH_HALF_CIRCUL_LENGTH};
    //取得对应的各个网格对应的地图瓦片数据索引
    int total_tile_X = floor(((EARTH_HALF_CIRCUL_LENGTH - EARTH_HALF_CIRCUL_LENGTH * (-1)) / resolution) / MAP_IMG_SIZE);
    int total_tile_Y = floor(((EARTH_HALF_CIRCUL_LENGTH - EARTH_HALF_CIRCUL_LENGTH * (-1)) / resolution) / MAP_IMG_SIZE);
    int tile_start_x = floor(((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
    int tile_start_y = total_tile_Y - floor(((view_bounds.max_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
    int tile_end_x = floor(((view_bounds.max_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
    int tile_end_y = total_tile_Y - floor(((view_bounds.min_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
    //计算第一福瓦片对应的墨卡托坐标
    ZCHX::Data::Mercator first_tile(0, 0);
    first_tile.mX = total_bounds.min_x + (tile_start_x * 256 * resolution);
    first_tile.mY = total_bounds.max_y - (tile_start_y * 256 * resolution);
    while (first_tile.mY < view_bounds.max_y)
    {
        tile_start_y --;
        first_tile.mY = total_bounds.max_y - (tile_start_y * 256 * resolution);
    }

    //获取各个瓦片的数据
    qDebug()<<"start x:"<<tile_start_x<<" y:"<<tile_start_y<<" end:"<<tile_end_x<<tile_end_y;
    for(int i=tile_start_x; i<=tile_end_x; i++){
        for(int k=tile_start_y; k<=tile_end_y; k++){
            //检查文件是否存在
            QString fileName = QString("%1/%2_%3.png").arg(savePath).arg(i).arg(k);
            QFile file(fileName);
            if(file.exists()) continue;
            QString url = QString("http://mt2.google.cn/vt/lyrs=m@167000000&hl=zh-CN&gl=cn&x=%1&y=%2&z=%3&s=Galil").arg(i).arg(k).arg(mZoom);
            QByteArray recv = QHttpGet::getContentOfURL(url);
            QPixmap img;
            img.loadFromData(recv, "PNG");
            img.save(fileName, "PNG");
        }
    }

    emit signalDownloadFinished(mZoom);
}
}
