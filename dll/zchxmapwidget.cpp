#include "zchxmapwidget.h"
#include "zchxmapframe.h"
#include "zchxmaploadthread.h"
#include "map_layer/zchxmaplayermgr.h"
#include "camera_mgr/zchxcameradatasmgr.h"
#include "data_manager/zchxdatamgrfactory.h"
#include "zchxuserdefinesdatamgr.h"
#include "zchxroutedatamgr.h"
#include "zchxshipplandatamgr.h"
#include <QPainter>
#include <QDebug>
#include <QDomDocument>
#include "profiles.h"


//#define     DEFAULT_LON         113.093664
//#define     DEFAULT_LAT         22.216150
//#define     DEFAULT_ZOOM        13
namespace qt {
zchxMapWidget::zchxMapWidget(QWidget *parent) : QWidget(parent),
    mLastWheelTime(0),
    mFrameWork(0),
    mMapThread(0),
    mDrag(0),
    mDx(0),
    mDy(0),
    mDisplayImageNum(false),
    mIsMapHidden(false),
    mIsNavigation(false),
    mIsOpenMeet(false),
    mIsRadarTagetTrack(false),
    mIsCameraCombinedTrack(false),
    mIsCameraDisplayWithoutRod(true),
    mCurPluginUserModel(ZCHX::Data::ECDIS_PLUGIN_USE_MODEL::ECDIS_PLUGIN_USE_DISPLAY_MODEL),
    mCurPickupType(ZCHX::Data::ECDIS_PICKUP_TYPE::ECDIS_PICKUP_ALL),
    mLayerMgr(new MapLayerMgr(this)),
    mCameraDataMgr(new zchxCameraDatasMgr(this)),
    mDataMgrFactory(new zchxDataMgrFactory(this)),
    mUseDataMgr(new zchxUserDefinesDataMgr(this)),
    mRouteDataMgr(new zchxRouteDataMgr(this)),
    mShipPlanDataMgr(new zchxShipPlanDataMgr(this))
{
    this->setMouseTracking(true);
    QTimer *timer = new QTimer;
    timer->setInterval(Profiles::instance()->value(MAP_INDEX, MAP_UPDATE_INTERVAL).toInt());
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
    //创建数据管理容器
    mDataMgrFactory->createManager(qt::ZCHX_DATA_MGR_AIS);
    mDataMgrFactory->createManager(qt::ZCHX_DATA_MGR_RADAR);

}

zchxMapWidget::~zchxMapWidget()
{
    if(!mFrameWork) delete mFrameWork;
}

void zchxMapWidget::setUseRightKey(bool bUseRightKey)
{
    mUseRightKey = bUseRightKey;
}

void zchxMapWidget::resizeEvent(QResizeEvent *e)
{
    QSize size = e->size();
    if(size.width() > 0 && size.height() > 0)
    {
        if(!mFrameWork)
        {
            double lat = Profiles::instance()->value(MAP_INDEX, MAP_DEFAULT_LAT).toDouble();
            double lon = Profiles::instance()->value(MAP_INDEX, MAP_DEFAULT_LON).toDouble();
            int     source = Profiles::instance()->value(MAP_INDEX, MAP_SOURCE).toInt();
            int zoom = Profiles::instance()->value(MAP_INDEX, MAP_DEFAULT_ZOOM).toInt();
            mFrameWork = new zchxMapFrameWork(lat, lon, zoom, size.width(), size.height(), source);
            mMapThread = new zchxMapLoadThread;
            connect(mFrameWork, SIGNAL(UpdateMap(MapLoadSetting)), mMapThread, SLOT(appendTask(MapLoadSetting)));
            connect(mMapThread, SIGNAL(signalSendCurPixmap(QPixmap,int,int)), this, SLOT(append(QPixmap,int,int)));
            connect(mMapThread, SIGNAL(signalSendNewMap(double, double, int, bool)), this, SLOT(slotRecvNewMap(double,double,int, bool)));
            connect(mMapThread, SIGNAL(signalSendImgList(TileImageList)), this, SLOT(append(TileImageList)));
            mMapThread->start();
        } else
        {
            //重新更新地图显示的大小
            mFrameWork->SetViewSize(size.width(), size.height());
        }
    }
    QWidget::resizeEvent(e);
}

void zchxMapWidget::slotRecvNewMap(double lon, double lat, int zoom, bool sync)
{
    qDebug()<<"new map info:"<<lon<<lat<<zoom<<sync;
    mCenter.lon = lon;
    mCenter.lat = lat;
    emit signalSendNewMap(lon, lat, zoom);
    if(!sync) clear();
}

void zchxMapWidget::append(const QPixmap &img, int x, int y)
{
    mDataList.append(TileImage(img, x, y));
    //update();
}

void zchxMapWidget::append(const TileImageList &list)
{
    qDebug()<<__FUNCTION__<<__LINE__;
    mDataList = list;
    mDx = 0;
    mDy = 0;
    //update();
}

void zchxMapWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(), Qt::white);
    //显示地图
    foreach(TileImage data, mDataList)
    {
        painter.drawPixmap(data.mPosX - mDx, data.mPosY-mDy, data.mImg);
        if(mDisplayImageNum)painter.drawText(data.mPosX-mDx, data.mPosY-mDy, data.mName);
    }
    //显示图元
    foreach (std::shared_ptr<zchxEcdisDataMgr> mgr, mDataMgrFactory->getManagers()) {
        mgr->show(&painter);
    }

    //显示当前的中心点
    Point2D pnt = mFrameWork->LatLon2Pixel(mCenter);
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
        //mDx = 0;
        //mDy = 0;
        QPoint pnt = e->pos();
        if(mFrameWork) mFrameWork->Drag(mPressPnt.x()- pnt.x(), mPressPnt.y() - pnt.y());

    }
    e->accept();
}

void zchxMapWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(mFrameWork)
    {
        //地图移动到当前点作为中心点
        mFrameWork->UpdateCenter(Point2D(e->pos()));
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
        //update();

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
    if(!mFrameWork) return;
    LatLon ll = mFrameWork->Pixel2LatLon(pnt);
    emit signalDisplayCurPos(ll.lon, ll.lat);
}

void zchxMapWidget::autoChangeCurrentStyle()
{
    if(!Profiles::instance()->value(MAP_INDEX, MAP_STYLE_AUTO_CHANGE, false).toBool()) return;
    //获取设定的白天,黄昏,夜晚的时间,根据当前时间自动设定地图颜色模式
    QString cur_str = QTime::currentTime().toString("hhmmss");
    if(cur_str < Profiles::instance()->value(MAP_INDEX, MAP_DAY_TIME).toString())
    {
        setMapStyle(MapStyleEcdisNight);
    } else if(cur_str < Profiles::instance()->value(MAP_INDEX, MAP_DUSK_TIME).toString())
    {
        setMapStyle(MapStyleEcdisDayBright);
    } else
    {
        setMapStyle(MapStyleEcdisDayDUSK);
    }

}

void zchxMapWidget::setCurZoom(int zoom)
{
    if(mFrameWork) mFrameWork->SetZoom(zoom);
}


int  zchxMapWidget::zoom() const
{
    if(!mFrameWork) return 0;
    return mFrameWork->Zoom();
}
void zchxMapWidget::setCenterLL(const LatLon& pnt )
{
    if(mFrameWork) mFrameWork->UpdateCenter(pnt);
}

void zchxMapWidget::setCenterAndZoom(const LatLon &ll, int zoom)
{
    if(mFrameWork) mFrameWork->UpdateCenterAndZoom(ll, zoom);
}

void zchxMapWidget::setCenterAtTargetLL(double lat, double lon)
{
    int zoom = Profiles::instance()->value(MAP_INDEX, MAP_DEFAULT_TARGET_ZOOM).toInt();
    setCenterAndZoom(LatLon(lat, lon), zoom);
}

LatLon zchxMapWidget::centerLatLon() const
{
    if(mFrameWork) return mFrameWork->Center();
    return LatLon();
}

void zchxMapWidget::wheelEvent(QWheelEvent *e)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<e->delta()<<e->angleDelta().x()<<e->angleDelta().y()<<e->phase();
    if(QDateTime::currentMSecsSinceEpoch() - mLastWheelTime >= 1* 1000)
    {
        if(e->delta() > 0)
        {
            //放大
            if(mFrameWork) mFrameWork->ZoomIn();
        } else
        {
            //缩小
            if(mFrameWork) mFrameWork->ZoomOut();
        }
        mLastWheelTime = QDateTime::currentMSecsSinceEpoch();
    }
    e->accept();
}

void zchxMapWidget::setSource(int source)
{
    if(mFrameWork) mFrameWork->SetSource(source);
}

void zchxMapWidget::setScaleControl(QScaleSlider * pScale)
{

}

void zchxMapWidget::setMapStyle(MapStyle mapStyle)
{
    if(mFrameWork) mFrameWork->SetMapStyle(mapStyle);
}

double zchxMapWidget::zchxUtilToolAngle4north()
{
    if(mFrameWork) return mFrameWork->GetRotateAngle();
    return 0.0;
}

void   zchxMapWidget::zchxUtilToolSetAngle4north(double ang)
{
    if(mFrameWork) mFrameWork->SetRotateAngle(ang);
}

void zchxMapWidget::setMapRotateAtLL(double lat, double lon, double ang)
{
    if(mFrameWork)
    {
        mFrameWork->UpdateCenter(lon, lat);
        mFrameWork->SetRotateAngle(ang);
    }
}

void zchxMapWidget::resetMapRotate(double lat, double lon)
{
    if(mFrameWork)
    {
        mFrameWork->UpdateCenter(lon, lat);
        mFrameWork->SetRotateAngle(0);
    }
}

void zchxMapWidget::reset()
{
    double lat = Profiles::instance()->value(MAP_INDEX, MAP_DEFAULT_LAT).toDouble();
    double lon = Profiles::instance()->value(MAP_INDEX, MAP_DEFAULT_LON).toDouble();
    double zoom = Profiles::instance()->value(MAP_INDEX, MAP_DEFAULT_ZOOM).toInt();
    setCenterAndZoom(LatLon(lat, lon), zoom);
}

void zchxMapWidget::setStyleAutoChange(bool val)
{
    Profiles::instance()->setValue(MAP_INDEX, MAP_STYLE_AUTO_CHANGE, val);
}

void zchxMapWidget::setGISDayBrightTime(const QTime &t)
{
    Profiles::instance()->setValue(MAP_INDEX, MAP_DAY_TIME, t.toString("hhmmss"));
}

void zchxMapWidget::setGISDuskTime(const QTime &t)
{
    Profiles::instance()->setValue(MAP_INDEX, MAP_DUSK_TIME, t.toString("hhmmss"));
}

void zchxMapWidget::setGISNightTime(const QTime &t)
{
    Profiles::instance()->setValue(MAP_INDEX, MAP_NIGHT_TIME, t.toString("hhmmss"));
}

void zchxMapWidget::setWarnColorAlphaStep(int val)
{
    Profiles::instance()->setValue(MAP_INDEX, WARN_FLAH_COLOR_ALPHA, val);
}

int zchxMapWidget::getWarnColorAlphaStep()
{
    return Profiles::instance()->value(MAP_INDEX, WARN_FLAH_COLOR_ALPHA).toInt();
}

MapLayerMgr * zchxMapWidget::getMapLayerMgr()
{
    return mLayerMgr;
}

zchxCameraDatasMgr* zchxMapWidget::getCameraMgr()
{
    return mCameraDataMgr;
}



zchxUserDefinesDataMgr* zchxMapWidget::getUserDefinesDataMgr()
{
    return mUseDataMgr;
}

zchxRouteDataMgr* zchxMapWidget::getRouteDataMgr()
{
    return mRouteDataMgr;
}

zchxShipPlanDataMgr* zchxMapWidget::getShipPlanDataMgr()
{
    return mShipPlanDataMgr;
}

bool zchxMapWidget::zchxUtilToolPoint4CurWindow(double lat, double lon, QPointF &p)
{
    Point2D pos = mFrameWork->LatLon2Pixel(lat, lon);
    p.setX(pos.x);
    p.setY(pos.y);
    return true;
}

bool zchxMapWidget::zchxUtilToolLL4CurPoint(const QPointF &p, double &lat, double &lon)
{
    Point2D pos(p.x(), p.y());
    LatLon ll = mFrameWork->Pixel2LatLon(pos);
    lat = ll.lat;
    lon = ll.lon;
    return true;
}

void zchxMapWidget::zchxUtilToolGetDis4Point(QPointF star, QPointF end, double &latoff, double &lonoff)
{
    LatLon endll = mFrameWork->Pixel2LatLon(Point2D(end.x(), end.y()));
    LatLon startll = mFrameWork->Pixel2LatLon(Point2D(star.x(), star.y()));
    lonoff = endll.lon - startll.lon;
    latoff = endll.lat - startll.lat;
}

uint zchxMapWidget::zchxUtilToolCurZoom()
{
    return mFrameWork->Zoom();
}

void zchxMapWidget::zchxSetMapMinScale(int minVisibleZoom)
{
    mFrameWork->SetMinZoom(minVisibleZoom);
}

void zchxMapWidget::zchxSetMapMaxScale(int maxVisibleZoom)
{
    mFrameWork->SetMaxZoom(maxVisibleZoom);
}

ZCHX::Data::ECDIS_PLUGIN_USE_MODEL zchxMapWidget::curUserModel() const
{
    return mCurPluginUserModel;
}

void zchxMapWidget::setCurPluginUserModel(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL &curUserModel)
{
    if(mCurPluginUserModel == ZCHX::Data::ECDIS_PLUGIN_USE_MODEL::ECDIS_PLUGIN_USE_DISPLAY_MODEL)
    {
        //releaseDrawStatus();
    }
    mCurPluginUserModel = curUserModel;
}

ZCHX::Data::ECDIS_PICKUP_TYPE zchxMapWidget::getCurPickupType() const
{
    return mCurPickupType;
}

void zchxMapWidget::setCurPickupType(const ZCHX::Data::ECDIS_PICKUP_TYPE &curPickupType)
{
    mCurPickupType = curPickupType;
}

std::shared_ptr<Element> zchxMapWidget::getCurrentSelectedElement()
{
    return mCurrentSelectElement;
}

void zchxMapWidget::setCurrentSelectedItem(std::shared_ptr<Element> item)
{
    mCurrentSelectElement = item;
}

//地图工作模式
void zchxMapWidget::setETool2DrawPickup()
{
    m_eToolPoints.clear();
    m_eTool = DRAWPICKUP;
    isActiveETool = true; //拾取时是否允许移动海图 true 不允许，false 允许
    setCursor(Qt::ArrowCursor);
}

void zchxMapWidget::zchxShowCameraInfo(QPainter *painter)
{
    mCameraDataMgr->showCameraRod(painter);
    mCameraDataMgr->showCamera(painter);
}

//GPS数据接口
void zchxMapWidget::setGPSDataList(std::list<std::shared_ptr<ZCHX::Data::GPSPoint> > list)
{
    QMutexLocker locker(&m_gpsTracksMutex);
    m_gpsTracks = list;
}

void zchxMapWidget::setGPSData(std::shared_ptr<ZCHX::Data::GPSPoint> data)
{
    QMutexLocker locker(&m_gpsTracksMutex);

    //查找是否需要更新数据
    std::list<std::shared_ptr<ZCHX::Data::GPSPoint> >::iterator it = m_gpsTracks.begin();
    it = std::find_if(it, m_gpsTracks.end(), [data](std::shared_ptr<ZCHX::Data::GPSPoint> one) -> bool {
            return (data->imei == one->imei);
});

    //更新数据
    if(it != m_gpsTracks.end())
    {
        std::shared_ptr<ZCHX::Data::GPSPoint> old = *it;
        old.reset(data.get());
    }
    else //添加数据
    {
        m_gpsTracks.push_back(data);
    }
}

void zchxMapWidget::clearGPSData()
{
    QMutexLocker locker(&m_gpsTracksMutex);
    m_gpsTracks.clear();
}

void zchxMapWidget::setFleet(const QMap<QString, ZCHX::Data::ITF_Fleet> &fleetMap)
{
    std::vector<DangerousCircle> list;
    // 设置危险圈
    QMap<QString, ZCHX::Data::ITF_Fleet>::const_iterator fleetIt = fleetMap.begin();
    for (; fleetIt != fleetMap.end(); ++fleetIt)
    {
        ZCHX::Data::ITF_Fleet fleetInfo = *fleetIt;
        if (fleetInfo.dangerCircleRadius > 0)
        {
            ZCHX::Data::ITF_DangerousCircle circle = {fleetIt.key(), 0, 0, 0, fleetInfo.dangerCircleRadius};
            list.push_back(DangerousCircle(circle));
        }
    }
    if(mUseDataMgr) mUseDataMgr->setDangerousCircleData(list);
}

zchxEcdisDataMgr* zchxMapWidget::getManager(int type)
{
    std::shared_ptr<zchxEcdisDataMgr> mgr = mDataMgrFactory->getManager(type);
    if(mgr) return mgr.get();
    return 0;
}
}









