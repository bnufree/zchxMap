#include "zchxmapthread.h"
#include "zchxtileimagethread.h"
#include <QDebug>
#include <QThreadPool>

#define     DBG_DOUBLE(val)      QString::number((val), 'f' , 2)


namespace qt {
const MapBounds total_bounds = {EARTH_HALF_CIRCUL_LENGTH * (-1), EARTH_HALF_CIRCUL_LENGTH * (-1), EARTH_HALF_CIRCUL_LENGTH, EARTH_HALF_CIRCUL_LENGTH};

zchxMapThread::zchxMapThread(QObject *parent) : QThread(parent)
{
    mTaskList.clear();
}

void zchxMapThread::appendTask(const zchxMapTask &task)
{
    QMutexLocker locker(&mMutex);
    mTaskList.append(task);
}

bool zchxMapThread::taskNow(zchxMapTask& task)
{
    QMutexLocker locker(&mMutex);
    if(mTaskList.size() == 0) return false;
    task = mTaskList.first();
    mTaskList.clear();
    return true;
}

//每像素对应的墨卡托坐标的长度
double zchxMapThread::calResolution(int zoom)
{
    return EARTH_HALF_CIRCUL_LENGTH * 2 / MAP_IMG_SIZE / pow(2, zoom);
}

MapBounds zchxMapThread::calViewBounds(double mct_x, double mct_y, double size_x, double size_y, double resolution)
{
    MapBounds bounds = { mct_x - resolution * size_x / 2.0,
                              mct_y - resolution * size_y / 2.0,
                              mct_x + resolution * size_x / 2.0,
                              mct_y + resolution * size_y / 2.0 };

    return bounds;

}

void zchxMapThread::run()
{
    while (true) {
        zchxMapTask task;
        if(!taskNow(task))
        {
            QThread::sleep(5);
            continue;
        }
        qDebug()<<"task:"<<task.lat<<task.lon<<task.zoom;
        //计算当前层级对应的分辨率
        double resolution = calResolution(task.zoom);
        qDebug()<<"resolution:"<<resolution;
        //计算当前中心经纬度对应的墨卡托坐标
        Mercator center_mct = zchxMapDataUtils::wgs84LonlatToMercator(LatLon(task.lat, task.lon));
        qDebug()<<"mercator (X, y) = "<<DBG_DOUBLE(center_mct.mX)<<DBG_DOUBLE(center_mct.mY);
        //计算层级的视窗对应的显示范围()
        MapBounds view_bounds = calViewBounds(center_mct.mX, center_mct.mY, task.view_x, task.view_y, resolution);
        qDebug()<<"view_bounds:"<<DBG_DOUBLE(view_bounds.min_x)<<DBG_DOUBLE(view_bounds.min_y)<<DBG_DOUBLE(view_bounds.max_x)<<DBG_DOUBLE(view_bounds.max_y);
        //取得对应的各个网格对应的地图瓦片数据索引
        int total_tile_X = floor(((total_bounds.max_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
        int total_tile_Y =  floor(((total_bounds.max_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
        int tile_start_x = floor(((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
        int tile_start_y = total_tile_Y - floor(((view_bounds.max_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
        int tile_end_x = floor(((view_bounds.max_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
        int tile_end_y = total_tile_Y - floor(((view_bounds.min_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
        qDebug()<<"tile range:(x0, y0)--(x1, y1)"<<tile_start_x<<tile_start_y<<tile_end_x<<tile_end_y <<"total "<<total_tile_X<<total_tile_Y;
        emit signalSendCurSize(tile_end_x-tile_start_x, tile_end_y-tile_start_y);
        //计算第一福瓦片对应的墨卡托坐标
        Mercator first_tile(0, 0);
        first_tile.mX = total_bounds.min_x + (tile_start_x * 256 * resolution);
//        while (first_tile.mX < view_bounds.min_x)
//        {
//            tile_start_x ++;
//            first_tile.mX = total_bounds.min_x + (tile_start_x * 256 * resolution);
//        }
        first_tile.mY = total_bounds.max_y - (tile_start_y * 256 * resolution);
        while (first_tile.mY < view_bounds.max_y)
        {
            tile_start_y --;
            first_tile.mY = total_bounds.max_y - (tile_start_y * 256 * resolution);
        }

        qDebug()<<"first tile mercator (X, y) = "<<DBG_DOUBLE(first_tile.mX)<<DBG_DOUBLE(first_tile.mY);
        //计算第一福瓦片对应的像素位置
        QPoint pos;
        pos.setX(floor(first_tile.mX - view_bounds.min_x) / resolution);
        pos.setY(floor(view_bounds.max_y - first_tile.mY) / resolution);
        qDebug()<<"top left corner:"<<pos;
        //获取各个瓦片的数据
        QThreadPool pool;
        pool.setMaxThreadCount(16);

        for(int i=tile_start_x; i<=tile_end_x; i++){
            for(int k=tile_start_y; k<=tile_end_y; k++){
                QString url = QString("http://mt2.google.cn/vt/lyrs=m@167000000&hl=zh-CN&gl=cn&x=%1&y=%2&z=%3&s=Galil").arg(i).arg(k).arg(task.zoom);
                int pos_x = pos.x() + (i-tile_start_x) * MAP_IMG_SIZE;
                int pos_y = pos.y() + (k-tile_start_y) * MAP_IMG_SIZE;
                QString name = QString("%1-%2").arg(i).arg(k);
                zchxTileImageThread *thread = new zchxTileImageThread(url, name, pos_x, pos_y, true, 0);
                thread->setAutoDelete(true);
                //qDebug()<<"url:"<<url<<"pos:"<<pos_x<<pos_y;
                connect(thread, SIGNAL(signalSend(QPixmap,int,int)), this, SIGNAL(signalSendCurPixmap(QPixmap, int, int)));
                //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
                pool.start(thread);
                //thread->start();
            }
        }
        pool.waitForDone();
    }
}
}


