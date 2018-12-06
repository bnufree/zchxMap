#include "zchxmapwidget.h"
#include <QPainter>
#include <QDebug>

zchxMapWidget::zchxMapWidget(QWidget *parent) : QWidget(parent),mCurrentZoom(9)
{

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

}

void zchxMapWidget::mouseMoveEvent(QMouseEvent *e)
{

}

void zchxMapWidget::wheelEvent(QWheelEvent *e)
{
    qDebug()<<__FUNCTION__<<__LINE__<<e->delta();
    if(e->delta() > 0)
    {
        mCurrentZoom++;
        if(mCurrentZoom >= 20) mCurrentZoom = 20;
    } else
    {
        mCurrentZoom --;
        if(mCurrentZoom <= 5) mCurrentZoom = 5;
    }

    emit signalDisplayCurMap(mCenterLonLat.x(), mCenterLonLat.y(), mCurrentZoom);
    e->accept();
}
