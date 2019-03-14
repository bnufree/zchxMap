#include "zchxtileimagethread.h"
#include "qhttpget.h"
#include <QFile>
#include <QPixmapCache>

namespace qt {
zchxTileImageThread::zchxTileImageThread(const QString& url, const QString& name,int pos_x, int pos_y, bool imgSync, QObject* retobj, QObject *parent) : QObject(parent),QRunnable(),
    mImgSync(imgSync),
    mName(name),
    mUrl(url),
    mPx(pos_x),
    mPy(pos_y),
    mReturnObj(retobj)

{

}

void zchxTileImageThread::run()
{    
    QPixmap *img = loadImage();
    if(img)
    {
        if(!mImgSync)
        {
            emit signalSend(*img, mPx, mPy);
        } else
        {
            if(mReturnObj)
            QMetaObject::invokeMethod(mReturnObj,
                                      "appendTileImg",
                                      Qt::DirectConnection,
                                      Q_ARG(QPixmap, *img),
                                      Q_ARG(int, mPx),
                                      Q_ARG(int, mPy),
                                      Q_ARG(QString, mName)
                                      );
        }


        delete img;
    }
}

QPixmap* zchxTileImageThread::loadImage()
{
    return loadImageFromUrl(mUrl);
}

QPixmap* zchxTileImageThread::loadImageFromUrl(const QString &url)
{
    //qDebug()<<"start load image:"<<QDateTime::currentDateTime()<<url;
    QPixmap *img = new QPixmap(256, 256);
    //QPixmapCache::setCacheLimit(1);
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
        img->fill(QColor::fromRgb(255,251,191));
    }
    //qDebug()<<"end load image:"<<QDateTime::currentDateTime()<<" img:"<<url<<"sts:"<<sts;
    return img;
}
}
