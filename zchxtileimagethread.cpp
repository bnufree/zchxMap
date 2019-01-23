#include "zchxtileimagethread.h"
#include "qhttpget.h"
#include <QFile>


zchxTileImageThread::zchxTileImageThread(const QString& url, int pos_x, int pos_y, QObject *parent) : QObject(parent),QRunnable(),
    mUrl(url),
    mPx(pos_x),
    mPy(pos_y)
{

}

void zchxTileImageThread::run()
{
    if(mUrl.isEmpty()) return;
    QPixmap img(256,256);
    //qDebug()<<"url:"<<mUrl;
    try {
        if(mUrl.contains("http"))
        {
            img.loadFromData(QHttpGet::getContentOfURL(mUrl), "PNG");
        } else
        {
            //读取本地文件
            img.load(mUrl, "PNG");
        }
    } catch(...)
    {
        img.fill(QColor(0,0,150,10));
    }

    if(!img.isNull())
    {
        emit signalSend(img, mPx, mPy);
    }
}
