#include "zchxmapwidget.h"
#include "zchxmapframe.h"
#include "zchxmaploadthread.h"
#include "map_layer/zchxmaplayermgr.h"
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
    m_eTool(DRAWNULL),
    mLastWheelTime(0),
    mCurrentSelectElement(0),
    mUseRightKey(true),
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
    mCurPluginUserModel(ZCHX::Data::ECDIS_PLUGIN_USE_DISPLAY_MODEL),
    mCurPickupType(ZCHX::Data::ECDIS_PICKUP_TYPE::ECDIS_PICKUP_ALL),
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
    mDataMgrFactory->createManager(qt::ZCHX_DATA_MGR_CAMERA);
    mDataMgrFactory->createManager(qt::ZCHX_DATA_MGR_ROD);
    mDataMgrFactory->createManager(qt::ZCHX_DATA_MGR_CANMERA_VIEW);
    mDataMgrFactory->createManager(qt::ZCHX_DATA_MGR_VIDEO_TARGET);

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

bool zchxMapWidget::IsLeftButton(Qt::MouseButtons buttons)
{
    return buttons & Qt::LeftButton;
}

bool zchxMapWidget::IsLeftButton(QMouseEvent * e)
{
    return IsLeftButton(e->button()) || IsLeftButton(e->buttons());
}

bool zchxMapWidget::IsRightButton(Qt::MouseButtons buttons)
{
    return buttons & Qt::RightButton;
}

bool zchxMapWidget::IsRightButton(QMouseEvent * e)
{
    return IsRightButton(e->button()) || IsRightButton(e->buttons());
}

bool zchxMapWidget::IsRotation(QMouseEvent * e)
{
    return e->modifiers() & Qt::ControlModifier;
}

bool zchxMapWidget::IsRouting(QMouseEvent * e)
{
    return e->modifiers() & Qt::ShiftModifier;
}

bool zchxMapWidget::IsLocationEmulation(QMouseEvent * e)
{
    return e->modifiers() & Qt::AltModifier;
}

void zchxMapWidget::setActiveDrawElement(const Point2D &pos, bool dbClick)
{
    //检查当前的地图模式,如果是纯显示模式,重置当前的目标图元选择;编辑模式则保持不变
    if(ZCHX::Data::ECDIS_PLUGIN_USE_EDIT_MODEL == mCurPluginUserModel) return;
    //重置选择的目标为空
    setCurrentSelectedItem(0);

    //检查各个数据管理类,获取当前选择的目标
    foreach (std::shared_ptr<zchxEcdisDataMgr> mgr, mDataMgrFactory->getManagers()) {
        if(mgr->updateActiveItem(pos.toPoint())){
            break;
        }
    }
}



void zchxMapWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if(IsLeftButton(e))
    {
        //先更新当前鼠标点击的位置信息,给地图移动做准备
        updateCurrentPos(e->pos());
        mPressPnt = e->pos();
        setActiveDrawElement(e->pos(), false);

#if 0
        //检查不同的情况进行处理,地图的其他操作优先处理
         m_startPos = m_endPos = e->pos();
        if(isActiveETool) {
            //获取当前点的经纬度
            qt::LatLon ll =zchxUtilToolLL4CurPoint(e->pos());
            switch (m_eTool) {
            case DRAWMEASUREAREA:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case DRAWDIRANGLE:
            {
                isActiveDrawDirAngle = true;
                break;
            }
            case DRAWDISTANCE:
            {
                m_eToolPoints.push_back(ll);
            }
            case DRAWPICKUP:
            {
                setActiveDrawElement(pt, geoPos);
                break;
            }
            case CAMERATEACK:
            {
                setSelectedCameraTrackTarget(pt);
                break;
            }
            case TRACKTARGET:
            {
                setPickUpNavigationTarget(pt);
                break;
            }
            case DRAWGPS:
            {
                getPointNealyCamera(pt);
                break;
            }
            case DRAWNULL:
            {
                releaseDrawStatus();
                break;
            }
            case DRAWLOCALMARK:
            {
                //Add by yej
                double lon = 0.0;
                double lat = 0.0;
                QPointF posF;
                posF.setX(e->pos().x());
                posF.setY(e->pos().y());
                zchxUtilToolLL4CurPoint(posF,lat,lon);
                LocalMarkDlg d;
                d.setLocalMarkPos(lon,lat);
                d.move(QCursor::pos().x() - d.width() / 2, \
                       QCursor::pos().y() - d.height() / 2 );
                if(d.exec() == QDialog::Accepted)
                {
                    ZCHX::Data::ITF_LocalMark mark;
                    d.getLocalMardData(mark);
                    emit signalCreateLocalMark(mark);
                }
                releaseDrawStatus();
                break;
            }
            case ZONEDRAWRADAR:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case ZONEDRAW:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case COMMONZONESELECT:
            {
                zoneEditSelect(pt);
                channelEditSelect(pt);
                mooringEditSelect(pt);
                cardMouthEditSelect(pt);
                break;
            }
            case ZONESELECT:
            {
                zoneEditSelect(pt);
                //发送新的防区数据,以便更新
                break;
            }
            case ZONEMOVE:
            {
                zoneIsMove = true;
                break;
            }
            case ZONEMOVECTRL:
            {
                zoneEditSelectCtrlPoint(pt);
                zoneIsMove = true;
                break;
            }
            case ZONEADDCTRL:
            {
                zoneEditAddCtrlPoint(pt);
                break;
            }
            case ZONEDELCTRL:
            {
                zoneEditDelCtrlPoint(pt);
                break;
            }
            case ROUTELINEDRAW:
            {
                m_eToolPoints.push_back(ll);
                //将路由数据点转换成点列保存，在绘制路由菜单结束后，清除
                QStringList pntslist;
                foreach (ms::LatLon pnt, m_eToolPoints) {
                    pntslist.append(QString::number(pnt.lon, 'f', 6));
                    pntslist.append(QString::number(pnt.lat, 'f', 6));
                }
                std::string res = pntslist.join(",").toStdString();
                Settings::Set("RoutePath", res);
                break;
            }
            case SHIPPLANDRAW:
            {
                m_eToolPoints.push_back(ll);
                //将路由数据点转换成点列保存，在绘制路由菜单结束后，清除
                QStringList pntslist;
                foreach (ms::LatLon pnt, m_eToolPoints) {
                    pntslist.append(QString::number(pnt.lon, 'f', 6));
                    pntslist.append(QString::number(pnt.lat, 'f', 6));
                }
                std::string res = pntslist.join(",").toStdString();
                Settings::Set("PlanPath", res);
                break;
            }
            case SHIPPLANSELECT:
            {
                shipPlanEditSelect(pt);
                //发送新的船舶计划数据,以便更新
                break;
            }
            case SHIPPLANMOVECTRL:
            {
                m_bShipPlanLineMove = shipPlanEditSelectCtrlPoint(pt);
                break;
            }
            case SHIPPLANEDELCTRL:
            {
                shipPlanLineEditDelCtrlPoint(pt);
                break;
            }
            case SHIPPLANADDCTRL:
            {
                m_eToolPoints.push_back(ll);
                //将路由数据点转换成点列保存，在绘制路由菜单结束后，清除
                QStringList pntslist;
                foreach (ms::LatLon pnt, m_eToolPoints) {
                    pntslist.append(QString::number(pnt.lon, 'f', 6));
                    pntslist.append(QString::number(pnt.lat, 'f', 6));
                }
                std::string res = pntslist.join(",").toStdString();
                Settings::Set("PlanAddPnt", res);

                shipPlanLineEditAddCtrlPoint(e->pos());
                break;
            }
            case SHIPPLANINSERTCTRL:
            {
                shipPlanLineEditInsertCtrlPoint(e->pos());
                break;
            }
                //------------------
            case ROUTELINEELECT:
            {
                routeLineEditSelect(pt);
                //发送新的数据,以便更新
                break;
            }
            case ROUTEINSERTCTRL:
            {
                routeLineEditInsertCtrlPoint(e->pos());
                break;
            }
            case ROUTEMOVECTRL:
            {
                m_bRouteLineMove = routeLineEditSelectCtrlPoint(pt);
                //m_bRouteLineMove = true;
                break;
            }
            case ROUTEADDCTRL:
            {
                m_eToolPoints.push_back(ll);
                //将路由数据点转换成点列保存，在绘制路由菜单结束后，清除
                QStringList pntslist;
                foreach (ms::LatLon pnt, m_eToolPoints) {
                    pntslist.append(QString::number(pnt.lon, 'f', 6));
                    pntslist.append(QString::number(pnt.lat, 'f', 6));
                }
                std::string res = pntslist.join(",").toStdString();
                Settings::Set("RouteAddPnt", res);
                routeLineEditAddCtrlPoint(e->pos());
                break;
            }
            case ROUTEDELCTRL:
            {
                routeLineEditDelCtrlPoint(pt);
                break;
            }
            case COASTDATALINEDRAW:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case SEABEDPIPELINEDRAW:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case STRUCTUREPOINTDRAW:
            {
                m_eToolPoint = ll;
                break;
            }
            case AREANETZONEDRAW:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case CHANNELMANAGER:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case CHANNELSELECT:
            {
                channelEditSelect(pt);
                break;
            }
            case CHANNELMOVE:
            {
                zoneIsMove = true;
                break;
            }
            case CHANNELMOVECTRL:
            {
                channelEditSelectCtrlPoint(pt);
                zoneIsMove = true;
                break;
            }
            case CHANNELADDCTRL:
            {
                channelEditAddCtrlPoint(pt);
                break;
            }
            case CHANNELDELCTRL:
            {
                channelEditDelCtrlPoint(pt);
                break;
            }
            case MOORINGMANAGER:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case MOORINGSELECT:
            {
                mooringEditSelect(pt);
                break;
            }
            case MOORINGMOVE:
            {
                zoneIsMove = true;
                break;
            }
            case MOORINGMOVECTRL:
            {
                mooringEditSelectCtrlPoint(pt);
                zoneIsMove = true;
                break;
            }
            case MOORINGADDCTRL:
            {
                mooringEditAddCtrlPoint(pt);
                break;
            }
            case MOORINGDELCTRL:
            {
                mooringEditDelCtrlPoint(pt);
                break;
            }
            case CARDMOUTHMANAGER:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case CARDMOUTHSELECT:
            {
                cardMouthEditSelect(pt);
                break;
            }
            case CARDMOUTHMOVE:
            {
                zoneIsMove = true;
                break;
            }
            case CARDMOUTHMOVECTRL:
            {
                cardMouthEditSelectCtrlPoint(pt);
                zoneIsMove = true;
                break;
            }
            case CARDMOUTHADDCTRL:
            {
                cardMouthEditAddCtrlPoint(pt);
                break;
            }
            case CARDMOUTHDELCTRL:
            {
                cardMouthEditDelCtrlPoint(pt);
                break;
            }

                //环岛线操作
            case ISLANDLINEDRAW:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case ISLANDLINESELECT:
            {
                islandLineEditSelect(pt);
                //发送新的防区数据,以便更新
                break;
            }
            case ISLANDLINEMOVE:
            {
                zoneIsMove = true;
                break;
            }
            case ISLANDLINEMOVECTRL:
            {
                islandLineEditSelectCtrlPoint(pt);
                zoneIsMove = true;
                break;
            }
            case ISLANDLINEADDCTRL:
            {
                islandLineEditAddCtrlPoint(pt);
                break;
            }
            case ISLANDLINEDELCTRL:
            {
                islandLineEditDelCtrlPoint(pt);
                break;
            }
            case ARESELECTD:
            {
                m_leftMousePress = true;
                m_eToolPoints.clear();
                m_eToolPoints.push_back(ll);
                break;
            }
            case ROUTEORCROSSPICKUP://拾取路由或者交越点
            {
                routeOrCrossPickup(pt);
                break;
            }
            case SHIPSIMULATIONDRAW:
            {
                m_eToolPoints.push_back(ll);
                break;
            }
            case CUSTOMFLOWLINE://只允许有2个点
            {
                if(m_eToolPoints.size()<2)
                    m_eToolPoints.push_back(ll);
                else
                {
                    ms::LatLon FristPos = m_eToolPoints.at(0);
                    m_eToolPoints.clear();
                    m_eToolPoints.push_back(FristPos);
                    m_eToolPoints.push_back(ll);
                }
                break;
            }
            case LOCALMARKPOSTION:
            {
                emit signalSendLocalMarkPos(ll.lat, ll.lon);
                break;
            }
            case FIXEFREFERENCEPOINT:
            {
                emit signalSendReferencePos(ll.lat, ll.lon);
                break;
            }
            case DRAWCAMERANETGRID:
            {
                if(m_eToolPoints.size() == 0)
                {
                    m_eToolPoints.push_back(ll);
                } else if(m_eToolPoints.size() == 2)
                {
                    //弹出对话框是否保存当前的网格
                    QMessageBox box;
                    box.setWindowTitle(tr("提示"));
                    box.setText(tr("是否更新当前的相机网格信息?"));
                    box.setStandardButtons (QMessageBox::Ok|QMessageBox::No);
                    box.setButtonText(QMessageBox::Ok,tr("确认"));
                    box.setButtonText(QMessageBox::No,tr("取消"));
                    if(box.exec () == QMessageBox::Ok)
                    {
                        m_eToolPoints[1] = ll;
                        //生成网格进行回传到客户端
                        emit signalSendCameraNetGrid(makeCameraGrid(m_eToolPoints[0], m_eToolPoints[1]));
                    }
                    releaseDrawStatus();
                }
                break;
            }
            }
            // IF END

        }
        else
        {
            /************要求在拾取时，也可以移动地图时使用这样的方式*************/
            //                if(m_eTool == DRAWPICKUP)
            //                {

            //                    setActiveDrawElement(pt);
            ////                    releaseDrawStatus();//选择完毕后，释放
            //                }
            //                else
            //                {
            //                    setCursor(Qt::OpenHandCursor);
            //                    update();
            //                }
            setCursor(Qt::OpenHandCursor);
            //                update();
            m_framework->TouchEvent(GetTouchEvent(e, df::TouchEvent::TOUCH_DOWN));
        }
#endif
        //=======zxl end===========================@}
        //zxl禁用别人的
        //            m_framework->TouchEssvent(GetTouchEvent(e, df::TouchEvent::TOUCH_DOWN));
        //            setCursor(Qt::OpenHandCursor);
    } else if(IsRightButton(e) && mUseRightKey)
    {
        bool bShowOtherRightKeyMenu = Profiles::instance()->value(MAP_INDEX, MAP_DISPLAY_MENU, false).toBool();
        QMenu menu;
        if(curUserModel() == ZCHX::Data::ECDIS_PLUGIN_USE_DISPLAY_MODEL )
        {
            menu.addAction(tr("Scroll     "),this,SLOT(releaseDrawStatus()));
            //处于显示模式时.对各个数据对象进行检查,如果当前选择了目标,且当前鼠标位置在对应的目标范围内,则弹出目标对应的菜单,否则只显示基本的右键菜单
            foreach (std::shared_ptr<zchxEcdisDataMgr> mgr, mDataMgrFactory->getManagers()) {
                QList<QAction*> list =  mgr->getRightMenuActions(e->pos());
                if(list.size() > 0)
                {
                    menu.addActions(list);
                    break;
                }
            }

            if (bShowOtherRightKeyMenu)
            {
                menu.addAction(tr("Screen Shot"),this,SIGNAL(signalScreenShot()));
                menu.addAction(tr("Pick Up"),this, SLOT(enterPickUp()));
                menu.addAction(tr("Frame Selected"),this,SLOT(selectAnRegion()));
                menu.addAction(tr("Ship Siumtion"),this,SLOT(setShipSimulation()));
                menu.addAction(tr("Location Mark"),this,SLOT(setLocationMark()));
                menu.addAction(tr("Fixed Reference Point"),this,SLOT(setFixedReferencePoint()));
                menu.addAction(tr("热点"),this,SLOT(invokeHotSpot()));
            }

        } else
        {
#if 0
            //处于编辑模式时,则弹出对应操作的右键菜单
            //绘制路由线时候，增加右键结束绘制
            if ROUTELINEDRAW:
            {
                menu.addAction(tr("End draw"),  this,   SLOT(routeLineRightKeyOKSlot()));
                menu.addAction(tr("Cancel"),    this,   SLOT(routeLineRightKeyCancelSlot()));
            }
            if(isActiveETool && (m_eTool == ROUTEINSERTCTRL ||
                                 m_eTool == ROUTEMOVECTRL ||
                                 m_eTool == ROUTEDELCTRL ||
                                 m_eTool == ROUTEADDCTRL))
            {
                menu.addAction(tr("End edit"),this,SLOT(routeLineRightKeyOKSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(routeLineRightKeyCancelSlot()));
            }
            if(isActiveETool && (m_eTool == SHIPPLANINSERTCTRL ||
                                 m_eTool == SHIPPLANMOVECTRL ||
                                 m_eTool == SHIPPLANEDELCTRL ||
                                 m_eTool == SHIPPLANADDCTRL))
            {
                menu.addAction(tr("End edit"),this,SLOT(shipPlanLineRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(shipPlanLineRightKeyCancelSlot()));
            }
            if(isActiveETool && m_eTool == SHIPPLANDRAW)
            {
                menu.addAction(tr("End draw"),this,SLOT(shipPlanLineRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(shipPlanLineRightKeyCancelSlot()));
            }
            if(isActiveETool && m_eTool == COASTDATALINEDRAW)
            {
                menu.addAction(tr("End draw"),this,SLOT(coastDataRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(coastDataRightKeyCancelSlot()));
            }
            if(isActiveETool && m_eTool == SEABEDPIPELINEDRAW)
            {
                menu.addAction(tr("End draw"),this,SLOT(seabedPipeLineRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(seabedPipeLineRightKeyCancelSlot()));
            }
            if(isActiveETool && m_eTool == STRUCTUREPOINTDRAW)
            {
                menu.addAction(tr("End draw"),this,SLOT(structureRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(structureRightKeyCancelSlot()));
                return;
            }
            if(isActiveETool && m_eTool == AREANETZONEDRAW)
            {
                menu.addAction(tr("End draw"),this,SLOT(areaNetRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(areaNetRightKeyCancelSlot()));
            }
            if(isActiveETool && m_eTool == CHANNELMANAGER)
            {
                menu.addAction(tr("End draw"), this, SLOT(channelRightKeyOkSlot()));
                menu.addAction(tr("Cancel"), this, SLOT(channelRightKeyCancelSlot()));
            }
            if(isActiveETool && (m_eTool == CHANNELMOVE ||
                                 m_eTool == CHANNELMOVECTRL ||
                                 m_eTool == CHANNELDELCTRL ||
                                 m_eTool == CHANNELADDCTRL))
            {
                menu.addAction(tr("End draw"),this,SLOT(channelRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(channelRightKeyCancelSlot()));
                return;
            }
            if(isActiveETool && m_eTool == MOORINGMANAGER)
            {
                menu.addAction(tr("End draw"), this, SLOT(mooringRightKeyOkSlot()));
                menu.addAction(tr("Cancel"), this, SLOT(mooringRightKeyCancelSlot()));
            }
            if(isActiveETool && (m_eTool == MOORINGMOVE ||
                                 m_eTool == MOORINGMOVECTRL ||
                                 m_eTool == MOORINGDELCTRL ||
                                 m_eTool == MOORINGADDCTRL))
            {
                menu.addAction(tr("End draw"),this,SLOT(mooringRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(mooringRightKeyCancelSlot()));
            }
            if(isActiveETool && m_eTool == CARDMOUTHMANAGER)
            {
                menu.addAction(tr("End draw"), this, SLOT(cardMouthRightKeyOkSlot()));
                menu.addAction(tr("Cancel"), this, SLOT(cardMouthRightKeyCancelSlot()));
            }
            if(isActiveETool && (m_eTool == CARDMOUTHMOVE ||
                                 m_eTool == CARDMOUTHMOVECTRL ||
                                 m_eTool == CARDMOUTHDELCTRL ||
                                 m_eTool == CARDMOUTHADDCTRL))
            {
                menu.addAction(tr("End draw"),this,SLOT(cardMouthRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(cardMouthRightKeyCancelSlot()));
            }
            if(isActiveETool && m_eTool == ZONEDRAW)
            {
                menu.addAction(tr("End draw"),this,SLOT(warringZoneRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(warringZoneRightKeyCancelSlot()));
            }
            if(isActiveETool && (m_eTool == ZONEMOVE ||
                                 m_eTool == ZONEMOVECTRL ||
                                 m_eTool == ZONEDELCTRL ||
                                 m_eTool == ZONEADDCTRL))
            {
                menu.addAction(tr("End draw"),this,SLOT(warringZoneRightKeyOkSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(warringZoneRightKeyCancelSlot()));
            }

            if (isActiveETool && m_eTool == SHIPSIMULATIONDRAW)
            {

                menu.addAction(tr("End draw"),this,SLOT(shipSlumtionLineRightKeyOKSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(shipSlumtionLineRightKeyCancelSlot()));
            }
            if (isActiveETool && m_eTool == CUSTOMFLOWLINE)
            {
                menu.addAction(tr("End draw"),this,SLOT(customFlowLineRightKeyOKSlot()));
                menu.addAction(tr("Cancel"),this,SLOT(customFlowLineRightKeyCancelSlot()));
                return;
            }
#endif

        }
        menu.exec(QCursor::pos());
    }
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

qt::LatLon zchxMapWidget::zchxUtilToolLL4CurPoint(const QPointF &p)
{
    return mFrameWork->Pixel2LatLon(Point2D(p));
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

Element* zchxMapWidget::getCurrentSelectedElement()
{
    return mCurrentSelectElement;
}

void zchxMapWidget::setCurrentSelectedItem(Element* item)
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









