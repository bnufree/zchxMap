#include "zchxmapwidget.h"
#include "zchxmapview.h"
#include <QPainter>
#include <QDebug>

zchxMapWidget::zchxMapWidget(QWidget *parent) : QWidget(parent)
{
    mView = new zchxMapView(13.9526789, 113.1234567, 9, 0, 0);
}

zchxMapWidget::~zchxMapWidget()
{
    if(!mView) delete mView;
}

void zchxMapWidget::resizeEvent(QResizeEvent *e)
{
    QSize size = e->size();
    if(mView)mView->setViewSize(size.width(), size.height());
    QWidget::resizeEvent(e);
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
    qDebug()<<ll.mLat<<ll.mLon;
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
    qDebug()<<__FUNCTION__<<__LINE__<<e->delta();
    if(e->delta() > 0)
    {
        //放大
        if(mView) mView->zoomIn();
    } else
    {
        //缩小
        if(mView) mView->zoomOut();
    }
    e->accept();
}
