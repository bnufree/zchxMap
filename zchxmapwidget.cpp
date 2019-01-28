#include "zchxmapwidget.h"
#include "zchxmapview.h"
#include "zchxmaploadthread.h"
#include <QPainter>
#include <QDebug>

#define     DEFAULT_LON         113.093664
#define     DEFAULT_LAT         22.216150
#define     DEFAULT_ZOOM        13
zchxMapWidget::zchxMapWidget(QWidget *parent) : QWidget(parent),
    mLastWheelTime(0),
    mView(0),
    mMapThread(0),
    mDrag(0),
    mDisplayImageNum(false)
{
    this->setMouseTracking(true);    
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
        if(!mView)
        {
            mView = new zchxMapView(DEFAULT_LAT, DEFAULT_LON, DEFAULT_ZOOM, size.width(), size.height());
            mMapThread = new zchxMapLoadThread;
            connect(mView, SIGNAL(updateMap(MapLoadSetting)), mMapThread, SLOT(appendTask(MapLoadSetting)));
            connect(mMapThread, SIGNAL(signalSendCurPixmap(QPixmap,int,int)), this, SLOT(append(QPixmap,int,int)));
            connect(mMapThread, SIGNAL(signalSendNewMap(double, double, int)), this, SLOT(slotRecvNewMap(double,double,int)));
            connect(mMapThread, SIGNAL(signalSendImgList(TileImageList)), this, SLOT(append(TileImageList)));
            mMapThread->start();
        } else
        {
            //重新更新地图显示的大小
            mView->setViewSize(size.width(), size.height());
        }
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
    mDataList.append(TileImage(img, x, y));
    update();
}

void zchxMapWidget::append(const TileImageList &list)
{
    mDataList.append(list);
    update();
}

void zchxMapWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(), Qt::white);
    if(mDataList.size() == 0) return;
    foreach(TileImage data, mDataList)
    {
        painter.drawPixmap(data.mPosX - mDx, data.mPosY-mDy, data.mImg);
        if(mDisplayImageNum)painter.drawText(data.mPosX-mDx, data.mPosY-mDy, data.mName);
    }

    //画中心
    Point2D pnt = mView->lonlat2pix(mCenter);
    //qDebug()<<pnt.x<<pnt.y;
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(pnt.x, pnt.y, 5, 5);
}

void zchxMapWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        updateCurrentPos(e->pos());
        mPressPnt = e->pos();
    }
    e->accept();
}

void zchxMapWidget::mouseReleaseEvent(QMouseEvent *e)
{
    updateCurrentPos(e->pos());
    if(mDrag)
    {
        mDrag = false;
        mDx = 0;
        mDy = 0;
        QPoint pnt = e->pos();
        if(mView) mView->drag(mPressPnt.x()- pnt.x(), mPressPnt.y() - pnt.y());

    }
    e->accept();
}

void zchxMapWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(mView)
    {
        //地图移动到当前点作为中心点
        mView->updateCenter(Point2D(e->pos()));
        //更新当前点的经纬度
        updateCurrentPos(e->pos());
    }
    e->accept();
}

void zchxMapWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        //当前鼠标按住左键移动拖动地图
        mDrag = true;
        QPoint pnt = e->pos();
        mDx = mPressPnt.x() - pnt.x();
        mDy = mPressPnt.y() - pnt.y();
        update();

    } else {
        //单纯地移动鼠标
        updateCurrentPos(e->pos());

    }
    e->accept();
}

void zchxMapWidget::updateCurrentPos(const QPoint &p)
{
    //取得当前的屏幕坐标
    Point2D pnt(p);
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

void zchxMapWidget::setCenterAndZoom(const Wgs84LonLat &ll, int zoom)
{
    if(mView) mView->updateCenterAndZoom(ll, zoom);
}

Wgs84LonLat zchxMapWidget:: centerLonlat() const
{
    if(mView) return mView->center();
    return Wgs84LonLat();
}

void zchxMapWidget::wheelEvent(QWheelEvent *e)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<e->delta()<<e->angleDelta().x()<<e->angleDelta().y()<<e->phase();
    if(QDateTime::currentMSecsSinceEpoch() - mLastWheelTime >= 1* 1000)
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

void zchxMapWidget::setSource(int source)
{
    if(mView) mView->setSource(source);
}
