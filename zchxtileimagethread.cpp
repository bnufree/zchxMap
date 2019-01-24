#include "zchxtileimagethread.h"
#include "qhttpget.h"
#include <QFile>
#include <QPixmapCache>


zchxTileImageThread::zchxTileImageThread(const QString& url, int pos_x, int pos_y, QObject *parent) : QObject(parent),QRunnable(),
    mUrl(url),
    mPx(pos_x),
    mPy(pos_y)
{

}

void zchxTileImageThread::run()
{    
    QPixmap *img = loadImage();
    if(img)
    {
        emit signalSend(*img, mPx, mPy);
        delete img;
    }
}

QPixmap* zchxTileImageThread::loadImage()
{
    return loadImageFromUrl(mUrl);
}

QPixmap* zchxTileImageThread::loadImageFromUrl(const QString &url)
{
    //qDebug()<<"start load image:"<<QDateTime::currentDateTime();
    QPixmap *img = new QPixmap(256, 256);
    QPixmapCache::setCacheLimit(1);
    bool sts = false;
    if(!url.isEmpty()){
        if(url.contains("http"))
        {
            sts = img->loadFromData(QHttpGet::getContentOfURL(url), "PNG");
        } else if(QFile::exists(url))
        {
            sts = img->load(url, "PNG");
        }
    }
    if(!sts)
    {
        img->fill(Qt::transparent);
    }
    //qDebug()<<"end load image:"<<QDateTime::currentDateTime()<<" img:"<<img<<"sts:"<<sts;
    return img;
}
