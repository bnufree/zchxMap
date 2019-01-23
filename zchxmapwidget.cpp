#include "zchxmapwidget.h"
#include "zchxmapview.h"
#include "zchxmaploadthread.h"
#include <QPainter>
#include <QDebug>

zchxMapWidget::zchxMapWidget(QWidget *parent) : QWidget(parent),
    mLastWheelTime(0)
{
    this->setMouseTracking(true);
    mView = new zchxMapView(22.274615, 112.982372, 9, 0, 0);
    mMapThread = new zchxMapLoadThread;
    connect(mView, SIGNAL(updateMap(MapLoadSetting)), mMapThread, SLOT(appendTask(MapLoadSetting)));
    connect(mMapThread, SIGNAL(signalSendCurPixmap(QPixmap,int,int)), this, SLOT(append(QPixmap,int,int)));
    connect(mMapThread, SIGNAL(signalSendNewMap(double, double, int)), this, SLOT(slotRecvNewMap(double,double,int)));
    mMapThread->start();
}

zchxMapWidget::~zchxMapWidget()
{
    if(!mView) delete mView;
}

void zchxMapWidget::resizeEvent(QResizeEvent *e)
{
    QSize size = e->size();
    if(size.width() > 0 && size.height() > 0)
    {
        if(mView)mView->setViewSize(size.width(), size.height());
    }
    QWidget::resizeEvent(e);
}

void zchxMapWidget::slotRecvNewMap(double lon, double lat, int zoom)
{
    qDebug()<<"new map info:"<<lon<<lat<<zoom;
    mCenter.mLon = lon;
    mCenter.mLat = lat;
    emit signalSendNewMap(lon, lat, zoom);
    clear();
}

void zchxMapWidget::append(const QPixmap &img, int x, int y)
{
    MapData data = {img, x, y};
    mDataList.append(data);
    update();
}

void zchxMapWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(), QColor(100, 100, 100, 100));
    foreach(MapData data, mDataList)
    {
        painter.drawPixmap(data.x, data.y, data.img);
    }

    //画中心
    Point2D pnt = mView->lonlat2pix(mCenter);
    //qDebug()<<pnt.x<<pnt.y;
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(pnt.x, pnt.y, 5, 5);
}

void zchxMapWidget::mousePressEvent(QMouseEvent *e)
{

}

void zchxMapWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void zchxMapWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    //将当前的坐标点作为中心坐标点
    Point2D pnt;
    pnt.x = e->pos().x();
    pnt.y = e->pos().y();
    if(mView) mView->updateCenter(pnt);
}

void zchxMapWidget::mouseMoveEvent(QMouseEvent *e)
{
    //取得当前的屏幕坐标
    Point2D pnt;
    pnt.x = e->pos().x();
    pnt.y = e->pos().y();
    //获取当前位置对应的经纬度坐标
    if(!mView) return;
    Wgs84LonLat ll = mView->pix2Lonlat(pnt);
    emit signalDisplayCurPos(ll.mLon, ll.mLat);
}

void zchxMapWidget::setCurZoom(int zoom)
{
    if(mView) mView->setZoom(zoom);
}


int  zchxMapWidget::zoom() const
{
    if(!mView) return 0;
    return mView->zoom();
}
void zchxMapWidget::setCenterLL(const Wgs84LonLat& pnt )
{
    if(mView) mView->updateCenter(pnt);
}

Wgs84LonLat zchxMapWidget:: centerLonlat() const
{
    if(mView) return mView->center();
    return Wgs84LonLat();
}

void zchxMapWidget::wheelEvent(QWheelEvent *e)
{
    qDebug()<<__FUNCTION__<<__LINE__<<e->delta()<<e->angleDelta().x()<<e->angleDelta().y()<<e->phase();
    if(QDateTime::currentMSecsSinceEpoch() - mLastWheelTime >= 3* 1000)
    {
        if(e->delta() > 0)
        {
            //放大
            if(mView) mView->zoomIn();
        } else
        {
            //缩小
            if(mView) mView->zoomOut();
        }
        mLastWheelTime = QDateTime::currentMSecsSinceEpoch();
    }
    e->accept();
}
