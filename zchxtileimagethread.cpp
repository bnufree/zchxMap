#include "zchxtileimagethread.h"
#include "qhttpget.h"


zchxTileImageThread::zchxTileImageThread(const QString& url, int pos_x, int pos_y, QObject *parent) : QObject(parent),QRunnable(),
    mUrl(url),
    mPx(pos_x),
    mPy(pos_y)
{

}

void zchxTileImageThread::run()
{
    if(mUrl.isEmpty()) return;
    QPixmap img;
    QByteArray recv = QHttpGet::getContentOfURL(mUrl);
    if(recv.length() > 0)
    {
        img.loadFromData(recv, "PNG");
        emit signalSend(img, mPx, mPy);
    }
}
