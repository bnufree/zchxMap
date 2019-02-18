#include "zchxmaploadthread.h"
#include <QThreadPool>
#include <QDebug>
#include <QApplication>
#include <zchxtileimagethread.h>

zchxMapLoadThread::zchxMapLoadThread(QObject *parent) : QThread(parent)
{
    mTaskList.clear();
    qRegisterMetaType<MapLoadSetting>("const MapLoadSetting&");
    qRegisterMetaType<TileImageList>("const TileImageList&");
    mImgSync = true;
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
        //开始计算对应的各个网格对应的地图瓦片数据索引
        int total_tile_X = floor(((total_bounds.max_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
        int total_tile_Y =  floor(((total_bounds.max_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
        int tile_start_x = 0, tile_start_y = 0, tile_end_x = 0, tile_end_y = 0;
        //1)计算视窗区域和全部地球区域对应的间隔部分可能占用的图片数
        int left_tile_num = floor(((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE);
        int right_tile_num = floor(((total_bounds.max_x - view_bounds.max_x) / resolution) / MAP_IMG_SIZE);
        int top_tile_num = floor(((total_bounds.max_y - view_bounds.max_y) / resolution) / MAP_IMG_SIZE);
        int bottom_tile_num = floor(((view_bounds.min_y - total_bounds.min_y) / resolution) / MAP_IMG_SIZE);
        //2)计算瓦片数据的编号x,y, 确保图片完全覆盖了整个视窗区域或者超出视窗区域一部分
        if(task.mTilePos == TILE_ORIGIN_TOPLEFT)
        {
            tile_start_x = left_tile_num;
            tile_end_x = total_tile_X - right_tile_num - 1;
            tile_start_y = top_tile_num;
            tile_end_y = total_tile_Y - bottom_tile_num - 1;
        } else
        {
            tile_start_x = left_tile_num;
            tile_end_x = total_tile_X - right_tile_num - 1;
            tile_start_y = bottom_tile_num;
            tile_end_y = total_tile_Y - top_tile_num - 1;
        }
        qDebug()<<((view_bounds.min_x - total_bounds.min_x) / resolution) / MAP_IMG_SIZE<<"tile range:(x0, y0)--(x1, y1)"<<tile_start_x<<tile_start_y<<tile_end_x<<tile_end_y <<"total "<<total_tile_X<<total_tile_Y;

        //计算左上位置的第一张图片对应的墨卡托坐标位置
        Mercator first_tile(0, 0);
        first_tile.mX = total_bounds.min_x + left_tile_num * MAP_IMG_SIZE * resolution;
        first_tile.mY = total_bounds.max_y - top_tile_num * MAP_IMG_SIZE * resolution;
        //计算左上第一福瓦片对应的像素位置
        Point2D pos;
        pos.x = (first_tile.mX - view_bounds.min_x) / resolution;
        pos.y = (view_bounds.max_y - first_tile.mY) / resolution;
        qDebug()<<"first tile mercator (x, y) = "<<FLOAT_STRING(first_tile.mX, 2)<<FLOAT_STRING(first_tile.mY, 2)<<" pix pos"<<pos.x<<pos.y;
#if 0
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
#endif
        //获取各个瓦片的数据
        mTileImgList.clear();
        QThreadPool pool;
        pool.setMaxThreadCount(16);
        emit signalSendNewMap(task.mCenter.lon, task.mCenter.lat, task.mZoom, mImgSync);
        //获取在y方向上的瓦片数据个数
        int y_num = tile_end_y - tile_start_y + 1;
        int y_pos_0 = tile_start_y;
        int y_pos_coeff = 1;
        if(task.mTilePos == TILE_ORIGIN_BOTTEMLEFT)
        {
            y_pos_0 = tile_end_y;
            y_pos_coeff = -1;
        }
        for(int i=tile_start_x; i<=tile_end_x; i++){
            for(int j=0; j<y_num;j++){
                int k = y_pos_0 + j*y_pos_coeff;
                //google
                QString url;
                if(task.mSource == TILE_GOOGLE)
                {
                    url = QString("http://mt2.google.cn/vt/lyrs=m@167000000&hl=zh-CN&gl=cn&x=%1&y=%2&z=%3&s=Galil").arg(i).arg(k).arg(task.mZoom);
                } else if(task.mSource == TILE_TMS)
                {
                    url = QString("%1/data/JMtms/%2/%3/%4.png").arg(QApplication::applicationDirPath()).arg(task.mZoom).arg(i).arg(k);
                }
                int pos_x = pos.x + (i-tile_start_x) * MAP_IMG_SIZE;
                int pos_y = pos.y + j * MAP_IMG_SIZE;
                QString name = QString("%1-%2").arg(i).arg(k);
                zchxTileImageThread *thread = new zchxTileImageThread(url, name, pos_x, pos_y, mImgSync, this);
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
