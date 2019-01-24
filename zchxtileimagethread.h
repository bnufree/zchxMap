#ifndef ZCHXTILEIMAGETHREAD_H
#define ZCHXTILEIMAGETHREAD_H

#include <QRunnable>
#include <QPixmap>

class zchxTileImageThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit zchxTileImageThread(const QString& url, const QString& name,int pos_x, int pos_y, bool sendMsg, QObject* retobj,  QObject *parent = 0);
    void run();
    QPixmap* loadImageFromUrl(const QString& url);
    QPixmap* loadImage();

signals:
    void signalSend(const QPixmap& img, int x, int y);
public slots:
private:
    QString mUrl;
    int mPx;
    int mPy;
    bool mSendMsg;
    QString mName;
    QObject* mReturnObj;
};

#endif // ZCHXTILEIMAGETHREAD_H
