#include "zchxmaploadthread.h"
#include <QThreadPool>
#include <QDebug>
#include <QApplication>
#include <zchxtileimagethread.h>

zchxMapLoadThread::zchxMapLoadThread(TILE_ORIGIN_POS pos, QObject *parent) : QThread(parent),
    mOriginPos(pos)
{
    mTaskList.clear();
    qRegisterMetaType<MapLoadSetting>("const MapLoadSetting&");
    qRegisterMetaType<TileImageList>("const TileImageList&");
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
    mTaskList.removeFirst();
    return true;
}


void zchxMapLoadThread::run()
{
    while (true) {
        MapLoadSetting task;
        if(!taskNow(task))
        {
            QThread::msleep(50);
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
        MapBounds view_bounds = {task.mMapRange.mLowerLeft.mX, task.mMapRange.mLowerLeft.mY, task.mMapRange.mTopRight.mX, task.mMapRange.mTopRight.mY};
        double resolution = task.mResolution;
        qDebug()<<"view_bounds:"<<FLOAT_STRING(view_bounds.min_x, 6)<<FLOAT_STRING(view_bounds.min_y, 6)<<FLOAT_STRING(view_bounds.max_x, 6)<<FLOAT_STRING(view_bounds.max_y, 6);
        qDebug()<<"resolution:"<<task.mResolution<<task.mZoom;
        //需要根据远点的不同位置计算瓦片的起始数据
        //谷歌XYZ：Z表示缩放层级，Z=zoom；XY的原点在左上角，X从左向右，Y从上向下。
        //TMS：开源产品的标准，Z的定义与谷歌相同；XY的原点在左下角，X从左向右，Y从下向上。
        //QuadTree：微软Bing地图使用的编码规范，Z的定义与谷歌相同，同一层级的瓦片不用XY两个维度表示，而只用一个整数表示，该整数服从四叉树编码规则
        //百度XYZ：Z从1开始，在最高级就把地图分为四块瓦片；XY的原点在经度为0纬度位0的位置，X从左向右，Y从下向上。
        //取得对应的各个网格对应的地图瓦片数据索引
        int total_tile_X = floor(((total_bounds.max_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
        int total_tile_Y =  floor(((total_bounds.max_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
        int tile_start_x = 0, tile_start_y = 0, tile_end_x = 0, tile_end_y = 0;
        //计算第一福瓦片对应的像素位置
        Point2D pos;
        if(mOriginPos == TILE_ORIGIN_TOPLEFT)
        {
            tile_start_x = floor(((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
            tile_start_y = total_tile_Y - floor(((view_bounds.max_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
            tile_end_x = floor(((view_bounds.max_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
            tile_end_y = total_tile_Y - floor(((view_bounds.min_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
            qDebug()<<((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE<<"tile range:(x0, y0)--(x1, y1)"<<tile_start_x<<tile_start_y<<tile_end_x<<tile_end_y <<"total "<<total_tile_X<<total_tile_Y;
            //计算第一福瓦片对应的墨卡托坐标  左上位置
            Mercator first_tile(0, 0);
            first_tile.mX = total_bounds.min_x + (tile_start_x * MAP_IMG_SIZE * resolution);
            while (first_tile.mX > view_bounds.min_x)
            {
                tile_start_x--;
                first_tile.mX = total_bounds.min_x + (tile_start_x * MAP_IMG_SIZE * resolution);
            }
            first_tile.mY = total_bounds.max_y - (tile_start_y * MAP_IMG_SIZE * resolution);
            while (first_tile.mY < view_bounds.max_y)
            {
                tile_start_y --;
                first_tile.mY = total_bounds.max_y - (tile_start_y * MAP_IMG_SIZE * resolution);
            }

            qDebug()<<"first tile mercator (x, y) = "<<FLOAT_STRING(first_tile.mX, 2)<<FLOAT_STRING(first_tile.mY, 2);
            pos.x = (first_tile.mX - view_bounds.min_x) / resolution;
            pos.y = (view_bounds.max_y - first_tile.mY) / resolution;
        } else
        {
            tile_start_x = floor(((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
            tile_start_y = floor(((view_bounds.min_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
            tile_end_x = floor(((view_bounds.max_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
            tile_end_y = floor(((view_bounds.max_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
            qDebug()<<((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE<<"tile range:(x0, y0)--(x1, y1)"<<tile_start_x<<tile_start_y<<tile_end_x<<tile_end_y <<"total "<<total_tile_X<<total_tile_Y;
            //计算第一福瓦片对应的墨卡托坐标 左下位置
            Mercator first_tile(0, 0);
            first_tile.mX = total_bounds.min_x + (tile_start_x * MAP_IMG_SIZE * resolution);
            while (first_tile.mX > view_bounds.min_x)
            {
                tile_start_x--;
                first_tile.mX = total_bounds.min_x + (tile_start_x * MAP_IMG_SIZE * resolution);
            }
            first_tile.mY = total_bounds.min_y + (tile_start_y * MAP_IMG_SIZE * resolution);
            while (first_tile.mY > view_bounds.min_y)
            {
                tile_start_y --;
                first_tile.mY = total_bounds.min_y + (tile_start_y * MAP_IMG_SIZE * resolution);
            }

            qDebug()<<"first tile mercator (x, y) = "<<FLOAT_STRING(first_tile.mX, 2)<<FLOAT_STRING(first_tile.mY, 2);
            pos.x = (first_tile.mX - view_bounds.min_x) / resolution;
            pos.y = (view_bounds.max_y - first_tile.mY) / resolution;
        }

        qDebug()<<"first tile pos:"<<pos.x<<pos.y;
        //获取各个瓦片的数据
        mTileImgList.clear();
        QThreadPool pool;
        pool.setMaxThreadCount(16);
        emit signalSendNewMap(task.mCenter.mLon, task.mCenter.mLat, task.mZoom);
        for(int i=tile_start_x; i<=tile_end_x; i++){
            for(int k=tile_start_y; k<=tile_end_y; k++){
                //google
                //QString url = QString("http://mt2.google.cn/vt/lyrs=m@167000000&hl=zh-CN&gl=cn&x=%1&y=%2&z=%3&s=Galil").arg(i).arg(k).arg(task.mZoom);
                //shipxc
                QString url = QString("http://m2.shipxy.com/tile.c?l=Na&m=o&x=%1&y=%2&z=%3").arg(i).arg(k).arg(task.mZoom);
                if(task.mMode == 0)
                {
                    url = QString("%1/data/JMtms/%2/%3/%4.png").arg(QApplication::applicationDirPath()).arg(task.mZoom).arg(i).arg(k);
                }
                int pos_x = pos.x + (i-tile_start_x) * MAP_IMG_SIZE;
                int pos_y = pos.y + (k-tile_start_y) * MAP_IMG_SIZE * (mOriginPos == TILE_ORIGIN_TOPLEFT? 1 : -1);
                QString name = QString("%1-%2").arg(i).arg(k);
                zchxTileImageThread *thread = new zchxTileImageThread(url, name, pos_x, pos_y, false, this);
                thread->setAutoDelete(true);
                connect(thread, SIGNAL(signalSend(QPixmap,int,int)), this, SIGNAL(signalSendCurPixmap(QPixmap, int, int)));
                pool.start(thread);
            }
        }
        pool.waitForDone();
        if(mTileImgList.size() > 0)
        {
            emit signalSendImgList(mTileImgList);
        }
    }
}

void zchxMapLoadThread::appendTileImg(const QPixmap& img, int x, int y,  const QString& name)
{
    QMutexLocker locker(&mImgMutex);
    mTileImgList.append(TileImage(img, x, y, name));
}
