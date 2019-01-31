#include "zchxmapwidget.h"
#include "zchxmapframe.h"
#include "zchxmaploadthread.h"
#include "map_layer/zchxmaplayermgr.h"
#include "camera_mgr/zchxcameradatasmgr.h"
#include <QPainter>
#include <QDebug>
#include <QDomDocument>


#define     DEFAULT_LON         113.093664
#define     DEFAULT_LAT         22.216150
#define     DEFAULT_ZOOM        13

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
    mLayerMgr(new zchxMapLayerMgr(this)),
    mCameraDataMgr(new zchxCameraDatasMgr(this))
{
    this->setMouseTracking(true);    
}

zchxMapWidget::~zchxMapWidget()
{
    if(!mFrameWork) delete mFrameWork;
}

void zchxMapWidget::resizeEvent(QResizeEvent *e)
{
    QSize size = e->size();
    if(size.width() > 0 && size.height() > 0)
    {
        if(!mFrameWork)
        {
            mFrameWork = new zchxMapFrameWork(DEFAULT_LAT, DEFAULT_LON, DEFAULT_ZOOM, size.width(), size.height());
            mMapThread = new zchxMapLoadThread;
            connect(mFrameWork, SIGNAL(UpdateMap(MapLoadSetting)), mMapThread, SLOT(appendTask(MapLoadSetting)));
            connect(mMapThread, SIGNAL(signalSendCurPixmap(QPixmap,int,int)), this, SLOT(append(QPixmap,int,int)));
            connect(mMapThread, SIGNAL(signalSendNewMap(double, double, int)), this, SLOT(slotRecvNewMap(double,double,int)));
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

void zchxMapWidget::slotRecvNewMap(double lon, double lat, int zoom)
{
    qDebug()<<"new map info:"<<lon<<lat<<zoom;
    mCenter.lon = lon;
    mCenter.lat = lat;
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
        mDx = 0;
        mDy = 0;
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
    if(!mFrameWork) return;
    LatLon ll = mFrameWork->Pixel2LatLon(pnt);
    emit signalDisplayCurPos(ll.lon, ll.lat);
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

std::shared_ptr<DrawElement::Element> zchxMapWidget::getCurrentSelectedElement()
{
    return mCurrentSelectElement;
}

bool zchxMapWidget::zchxWarringZoneData4id(int uuid, ZCHX::Data::ITF_WarringZone &info)
{
    std::vector<DrawElement::WarringZONE>::iterator it;
    for(it = m_WarringZone.begin(); it != m_WarringZone.end(); ++it)
    {
        int myid = (*it).data().id;
        if(uuid == myid)
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

bool zchxMapWidget::zchxWarringZoneDataByName(const QString &name, ZCHX::Data::ITF_WarringZone &info)
{
    return zchxWarringZoneDataByName(name.toStdString(), info);
}

bool zchxMapWidget::zchxWarringZoneDataByName(const std::string &name, ZCHX::Data::ITF_WarringZone &info)
{
    std::vector<DrawElement::WarringZONE>::iterator it;
    for(it=m_WarringZone.begin(); it != m_WarringZone.end(); ++it)
    {
        const DrawElement::WarringZONE &zone = (*it);
        if(zone.name().compare(name) == 0)
        {
            info = zone.data();
            return true;
        }
    }
    return false;
}

DrawElement::WarringZONE* zchxMapWidget::zchxWarringZoneItem(const std::string &name)
{
    for(int i = 0; i < m_WarringZone.size(); ++i)
    {
        if(m_WarringZone[i].name().compare(name) == 0)
        {
            return &m_WarringZone[i];
        }
    }
    return NULL;
}

bool zchxMapWidget::zchxChannelZoneData4id(int uuid, ZCHX::Data::tagITF_Channel &info)
{
    std::vector<DrawElement::Channel>::iterator it;
    for(it = m_channelZone.begin(); it != m_channelZone.end(); ++it)
    {
        int myid = (*it).data().id;
        if(uuid == myid)
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

DrawElement::Channel *zchxMapWidget::zchxChannelZoneItem(const std::string &name)
{
    for(int i = 0; i < m_channelZone.size(); ++i)
    {
        if(m_channelZone[i].name().compare(name) == 0)
        {
            return &m_channelZone[i];
        }
    }

    return NULL;
}

bool zchxMapWidget::zchxMooringZoneData4id(int uuid, ZCHX::Data::tagITF_Mooring &info)
{
    std::vector<DrawElement::Mooring>::iterator it;
    for(it = m_mooringZone.begin(); it != m_mooringZone.end(); ++it)
    {
        int myid = (*it).data().id;
        if(uuid == myid)
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

DrawElement::Mooring *zchxMapWidget::zchxMooringZoneItem(const std::string &name)
{
    for(int i = 0; i < m_mooringZone.size(); ++i)
    {
        if(m_mooringZone[i].name().compare(name) == 0)
        {
            return &m_mooringZone[i];
        }
    }

    return NULL;
}

bool zchxMapWidget::zchxCardMouthZoneData4id(int uuid, ZCHX::Data::tagITF_CardMouth &info)
{
    std::vector<DrawElement::CardMouth>::iterator it;
    for(it = m_cardMouthZone.begin(); it != m_cardMouthZone.end(); ++it)
    {
        int myid = (*it).data().id;
        if(uuid == myid)
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

DrawElement::CardMouth *zchxMapWidget::zchxCardMouthZoneItem(const std::__cxx11::string &name)
{
    for(int i = 0; i < m_cardMouthZone.size(); ++i)
    {
        if(m_cardMouthZone[i].name().compare(name) == 0)
        {
            return &m_cardMouthZone[i];
        }
    }

    return NULL;
}



DrawElement::CoastData* zchxMapWidget::zchxCoastDataZoneItem(const std::string &name)
{
    for(int i = 0; i < m_coastDataLine.size(); ++i)
    {
        if(m_coastDataLine[i].name().compare(name) == 0)
        {
            return &m_coastDataLine[i];
        }
    }

    return NULL;
}

DrawElement::SeabedPipeLine *zchxMapWidget::zchxSeabedPipeLineZoneItem(const std::__cxx11::string &name)
{
    for(int i = 0; i < m_seabedPipeLineLine.size(); ++i)
    {
        if(m_seabedPipeLineLine[i].name().compare(name) == 0)
        {
            return &m_seabedPipeLineLine[i];
        }
    }

    return NULL;
}

DrawElement::Structure *zchxMapWidget::zchxStructureZoneItem(const std::__cxx11::string &name)
{
    for(int i = 0; i < m_structurePoint.size(); ++i)
    {
        if(m_structurePoint[i].name().compare(name) == 0)
        {
            return &m_structurePoint[i];
        }
    }

    return NULL;
}

DrawElement::AreaNet *zchxMapWidget::zchxAreaNetZoneItem(const std::__cxx11::string &name)
{
    for(int i = 0; i < m_areaNetZone.size(); ++i)
    {
        if(m_areaNetZone[i].name().compare(name) == 0)
        {
            return &m_areaNetZone[i];
        }
    }

    return NULL;
}

bool zchxMapWidget::zchxIslandData4id(int uuid, ZCHX::Data::ITF_IslandLine &info)
{
    std::vector<DrawElement::IslandLine>::iterator it;
    for(it =m_IslandLine.begin(); it != m_IslandLine.end(); ++it)
    {
        if(uuid == (*it).id())
        {
            info = (*it).data();
            return true;
        }
    }
    return false;
}

void zchxMapWidget::zchxShowCameraInfo(QPainter *painter)
{
    mCameraDataMgr->showCameraRod(painter);
    mCameraDataMgr->showCamera(painter);
}









