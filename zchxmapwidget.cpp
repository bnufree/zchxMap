#include "zchxmapwidget.h"
#include <QPainter>

zchxMapWidget::zchxMapWidget(QWidget *parent) : QWidget(parent)
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
    painter.fillRect(0,0,width(),height(), Qt::transparent);
    foreach(MapData data, mDataList)
    {
        painter.drawPixmap(data.x, data.y, data.img);
    }
}
