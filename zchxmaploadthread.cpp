#include "zchxmaploadthread.h"

zchxMapLoadThread::zchxMapLoadThread(QObject *parent) : QObject(parent)
{
    mTaskList.clear();
}

void zchxMapLoadThread::appendTask(const MapLoadSetting &task)
{
    QMutexLocker locker(&mMutex);
    mTaskList.append(task);
}

bool zchxMapLoadThread::taskNow(MapLoadSetting& task)
{
    QMutexLocker locker(&mMutex);
    if(mTaskList.size() == 0) return false;
    task = mTaskList.first();
    mTaskList.clear();
    return true;
}


void zchxMapLoadThread::run()
{
    while (true) {
        MapLoadSetting task;
        if(!taskNow(task))
        {
            QThread::sleep(5);
            continue;
        }
        struct MapBounds{
            double     min_x;
            double     min_y;
            double     max_x;
            double     max_y;
        };
        //整个地球的范围
        static MapBounds total_bounds = {EARTH_HALF_CIRCUL_LENGTH * (-1), EARTH_HALF_CIRCUL_LENGTH * (-1), EARTH_HALF_CIRCUL_LENGTH, EARTH_HALF_CIRCUL_LENGTH};

        //取得当前视窗的显示范围
        MapBounds view_bounds = {task.mTopLeft.mMercatot.mX, task.mTopLeft.mMercatot.mY, task.mBottomRight.mMercatot.mX, task.mBottomRight.mMercatot.mY};
        qDebug()<<"view_bounds:"<<FLOAT_STRING(view_bounds.min_x, 6)<<FLOAT_STRING(view_bounds.min_y, 6)<<FLOAT_STRING(view_bounds.max_x, 6)<<FLOAT_STRING(view_bounds.max_y, 6);
        qDebug()<<"resolution:"<<task.mResolution;
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
                zchxTileImageThread *thread = new zchxTileImageThread(url, pos_x, pos_y);
                thread->setAutoDelete(true);
                //qDebug()<<"url:"<<url<<"pos:"<<pos_x<<pos_y;
                connect(thread, SIGNAL(signalSend(QPixmap,int,int)), this, SIGNAL(signalSendCurPixmap(QPixmap, int, int)));
                //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
                pool.start(thread);
                //thread->start();
            }
        }
        pool.waitForDone();
        QThread::msleep(500);
    }
}
