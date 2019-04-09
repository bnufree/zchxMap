#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zchxmapthread.h"
#include "map_layer/zchxmaplayermgr.h"
#include "map_layer/zchxaismaplayer.h"
#include "map_layer/zchxradarvideolayer.h"

#include "zchxroutedatamgr.h"
#include "zchxshipplandatamgr.h"
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "profiles.h"

#include "dialog/coastdatainfodialog.h"
#include "dialog/cardmouthinfodialog.h"
#include "dialog/channelinfodialog.h"
#include "dialog/mooringinfodialog.h"

namespace qt {
MainWindow::MainWindow(const ZCHX::Data::zchxEcdisInitVal& val, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mMapWidget(0)
{
    ui->setupUi(this);
    QString style = QString("background-color:%1;").arg(Profiles::instance()->value(MAP_INDEX, MAP_BACK_GROUND).toString());
    ui->ecdis_frame->setStyleSheet(style);
    QPixmapCache::setCacheLimit(1);
    ui->statusBar->setVisible(false);
    ui->source->addItem(tr("TMS"), TILE_TMS);
    ui->source->addItem(tr("谷歌"), TILE_GOOGLE);
    ui->progressBar->setVisible(false);
    ui->download->setVisible(false);
    ui->load_frame->setVisible(false);
    ui->pos_frame->setVisible(false);
    ui->ctrl_frame->setVisible(false);
    mMapWidget = new zchxMapWidget(val, this);
    ui->ecdis_frame->layout()->addWidget(mMapWidget);
    connect(mMapWidget, SIGNAL(signalDisplayCurPos(double,double)), this, SLOT(slotUpdateCurrentPos(double,double)));
    connect(mMapWidget, SIGNAL(signalSendNewMap(double,double,int)), this, SLOT(slotDisplayMapCenterAndZoom(double,double,int)));
    initSignalConnect();
    MapLayerMgr::instance()->setDrawWidget(mMapWidget);
    MapLayerMgr::instance()->loadEcdisLayers();
}

MainWindow::~MainWindow()
{
    //删掉所有的层
    MapLayerMgr::instance()->clear();
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    //loadEcdis();
    emit itfSignalIsEcdisResize();
}

//void MainWindow::loadEcdis()
//{
//    if(!mStartLoad)
//    {
//        mStartLoad = true;
//    }
//    if(mMapWidget) mMapWidget->setGeometry(0, 0, ui->ecdis_frame->size().width(), ui->ecdis_frame->size().height());
//}



void MainWindow::on_load_clicked()
{
    //QRect rect =  ui->ecdis->geometry();
    double lon = ui->lon->text().toDouble();
    double lat = ui->lat->text().toDouble();
    int zoom = ui->zoom->text().toInt();
    if(mMapWidget) mMapWidget->setCenterAndZoom(ZCHX::Data::LatLon(lat, lon), zoom);
}

void MainWindow::slotUpdateCurrentPos(double lon, double lat)
{
    ui->pos_label->setText(QString("%1, %2").arg(lon, 0, 'f', 6).arg(lat, 0, 'f', 6));
    emit itfSignalSendCurPos(lat, lon);
}

void MainWindow::slotDisplayMapCenterAndZoom(double lon, double lat, int zoom)
{
    ui->lat->setText(FLOAT_STRING(lat, 6));
    ui->lon->setText(FLOAT_STRING(lon, 6));
    ui->zoom->setText(INT_STRING(zoom));
}

void MainWindow::on_download_clicked()
{

}

void MainWindow::slotUpdateDownloadProgress(int val)
{
    static int num = 1;
    ui->progressBar->setValue(num++);
    if(num == 23)
    {
        num = 1;
    }
}

void MainWindow::on_source_activated(const QString &arg1)
{
}

void MainWindow::on_source_currentIndexChanged(int index)
{
    if(mMapWidget) mMapWidget->setSource(ui->source->currentData().toInt());
}

void MainWindow::on_image_num_clicked(bool checked)
{
    if(mMapWidget) mMapWidget->setDisplayImgeNum(checked);
}

void MainWindow::initSignalConnect()
{
    connect(mMapWidget, SIGNAL(signalCreateRouteLINE(ZCHX::Data::RouteLine)),this,SIGNAL(itfCreateRouteLine(ZCHX::Data::RouteLine)));
    connect(mMapWidget, SIGNAL(signalCancelRouteLine()), this, SIGNAL(itfCancelAddRouteLine()));
    connect(mMapWidget, SIGNAL(signalCancelChangeRouteLine()), this, SIGNAL(itfCancelChangeRouteLine()));
    connect(mMapWidget, SIGNAL(signalCancelChangeShipPlanLine()), this, SIGNAL(itfCancelChangeShipPlanLine()));
    connect(mMapWidget, SIGNAL(signalNoRouteLineSelected()),this,SIGNAL(itfNoRouteLineSelected()));
    connect(mMapWidget, SIGNAL(signalNoShipPlanSelected()), this, SIGNAL(itfNoShipPlanLineSelected()));
    connect(mMapWidget, SIGNAL(signalCreateRadarFeatureZONE(ZCHX::Data::ITF_RadarFeaturesZone)),this,SIGNAL(itfCreateRadarFeatureZONE(ZCHX::Data::ITF_RadarFeaturesZone)));
    connect(mMapWidget, SIGNAL(signalCreateShipPlanLine(ZCHX::Data::ShipPlanLine)),this,SIGNAL(itfCreateShipPlanLine(ZCHX::Data::ShipPlanLine)));
    connect(mMapWidget, SIGNAL(signalIsSelected4TrackRadarOrbit(ZCHX::Data::ITF_AIS,bool)), this,SIGNAL(itfSignalIsSelected4TrackAis(ZCHX::Data::ITF_AIS,bool)));
    connect(mMapWidget, SIGNAL(signalIsEcdisScales(double)), this, SIGNAL(itfSignalIsEcdisScales(double)));
    connect(mMapWidget, SIGNAL(signalIsEcdisDoubleClickCoordinate(double, double)), this, SIGNAL(itfSignalIsEcdisDoubleClickCoordinate(double, double)));
    connect(mMapWidget, SIGNAL(signalCreateWarringZONE(ZCHX::Data::ITF_WarringZone)),this,SIGNAL(itfCreateWarringZONE(ZCHX::Data::ITF_WarringZone)));
    connect(mMapWidget, SIGNAL(signalCreateWarringLINE(ZCHX::Data::ITF_WarringZone)),this,SIGNAL(itfCreateWarringLine(ZCHX::Data::ITF_WarringZone)));
    connect(mMapWidget, SIGNAL(signalCreateIslandLINE(ZCHX::Data::ITF_IslandLine)),this,SIGNAL(itfCreateIslandLine(ZCHX::Data::ITF_IslandLine)));
    connect(mMapWidget, SIGNAL(signalCreateLocalMark(ZCHX::Data::ITF_LocalMark)), this,SIGNAL(itfCreateLocalMark(ZCHX::Data::ITF_LocalMark)));
    connect(mMapWidget, SIGNAL(signalSendCurPos(double,double)), this, SLOT(onSignalSendCurPos(double,double)), Qt::QueuedConnection);
    connect(mMapWidget, SIGNAL(signalIsSelected4CameraRod(ZCHX::Data::ITF_CameraRod)),this, SIGNAL(itfSignalIsSelected4CameraRod(ZCHX::Data::ITF_CameraRod)));
    connect(mMapWidget, SIGNAL(signalIsSelected4IslandLine(ZCHX::Data::ITF_IslandLine)),this,SIGNAL(itfSignalIsSelected4IslandLine(ZCHX::Data::ITF_IslandLine)));
    connect(mMapWidget, SIGNAL(signalIsSelected4LocalMark(ZCHX::Data::ITF_LocalMark)),this,SIGNAL(itfSignalIsSelected4LocalMark(ZCHX::Data::ITF_LocalMark)));
    connect(mMapWidget, SIGNAL(signalIsSelected4PastrolStation(ZCHX::Data::ITF_PastrolStation)),this,SIGNAL(itfSignalIsSelected4PastrolStation(ZCHX::Data::ITF_PastrolStation)));
    connect(mMapWidget, SIGNAL(signalIsSelected4RadarArea(ZCHX::Data::ITF_RadarArea)), this,SIGNAL(itfSignalIsSelected4RadarArea(ZCHX::Data::ITF_RadarArea)));
    connect(mMapWidget, SIGNAL(signalIsSelected4Ais(ZCHX::Data::ITF_AIS,bool,bool)), this,SIGNAL(itfSignalIsSelected4Ais(ZCHX::Data::ITF_AIS,bool,bool)));
    connect(mMapWidget, SIGNAL(signalIsSelected4RadarPoint(ZCHX::Data::ITF_RadarPoint,bool)), this,SIGNAL(itfSignalIsSelected4RadarPoint(ZCHX::Data::ITF_RadarPoint,bool)));
    connect(mMapWidget, SIGNAL(signalIsSelected4WarringZone(ZCHX::Data::ITF_WarringZone)),this,SIGNAL(itfSignalIsSelected4WarringZone(ZCHX::Data::ITF_WarringZone)));
    connect(mMapWidget, SIGNAL(signalIsSelected4WarringLine(ZCHX::Data::ITF_WarringZone)),this,SIGNAL(itfSignalIsSelected4WarringLine(ZCHX::Data::ITF_WarringZone)));
    connect(mMapWidget, SIGNAL(signalSendPointNealyCamera(int,double,double)),this,SIGNAL(itfSendPointNealyCamera(int,double,double)));
    connect(mMapWidget, SIGNAL(signalIsSelected4CameraVideoWarn(ZCHX::Data::ITF_CameraVideoWarn)), this, SIGNAL(itfSignalIsSelected4CameraVideoWarn(ZCHX::Data::ITF_CameraVideoWarn)));
    connect(mMapWidget, SIGNAL(signalIsSelected4CameraDev(ZCHX::Data::ITF_CameraRod)), this, SIGNAL(itfSignalIsSelected4CameraDev(ZCHX::Data::ITF_CameraDev)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4CameraDev(ZCHX::Data::ITF_CameraDev)), this, SIGNAL(signalIsDoubleClicked4CameraDev(ZCHX::Data::ITF_CameraDev)));

    connect(mMapWidget, SIGNAL(signalCreateCoastDataLINE(ZCHX::Data::ITF_CoastData)),this,SIGNAL(itfCreateCoastDataLine(ZCHX::Data::ITF_CoastData)));
    connect(mMapWidget, SIGNAL(signalCreateSeabedPipeLineLINE(ZCHX::Data::ITF_SeabedPipeLine)),this,SIGNAL(itfCreateSeabedPipeLineLine(ZCHX::Data::ITF_SeabedPipeLine)));
    connect(mMapWidget, SIGNAL(signalCreateStructurePoint(ZCHX::Data::ITF_Structure)),this,SIGNAL(itfCreateStructurePoint(ZCHX::Data::ITF_Structure)));
    connect(mMapWidget, SIGNAL(signalCreateAreaNetZone(ZCHX::Data::ITF_AreaNet)),this,SIGNAL(itfCreateAreaNetZone(ZCHX::Data::ITF_AreaNet)));

    connect(mMapWidget, SIGNAL(signalCreateChannelZone(ZCHX::Data::ITF_Channel)),this,SIGNAL(itfCreateChannelZone(ZCHX::Data::ITF_Channel)));
    connect(mMapWidget, SIGNAL(signalIsSelected4ChannelZone(ZCHX::Data::ITF_Channel)),this,SIGNAL(itfSignalIsSelected4ChannelZone(ZCHX::Data::ITF_Channel)));

    connect(mMapWidget, SIGNAL(signalCreateMooringZone(ZCHX::Data::ITF_Mooring)),this,SIGNAL(itfCreateMooringZone(ZCHX::Data::ITF_Mooring)));
    connect(mMapWidget, SIGNAL(signalIsSelected4MooringZone(ZCHX::Data::ITF_Mooring)),this,SIGNAL(itfSignalIsSelected4MooringZone(ZCHX::Data::ITF_Mooring)));

    connect(mMapWidget, SIGNAL(signalCreateCardMouthZone(ZCHX::Data::ITF_CardMouth)),this,SIGNAL(itfCreateCardMouthZone(ZCHX::Data::ITF_CardMouth)));
    connect(mMapWidget, SIGNAL(signalIsSelected4CardMouthZone(ZCHX::Data::ITF_CardMouth)),this,SIGNAL(itfSignalIsSelected4CardMouthZone(ZCHX::Data::ITF_CardMouth)));

    connect(mMapWidget, SIGNAL(signalAddFleet(const ZCHX::Data::ITF_AIS&)), this, SIGNAL(itfAddFleet(const ZCHX::Data::ITF_AIS&)));
    connect(mMapWidget, SIGNAL(signalRemoveFleet(const ZCHX::Data::ITF_AIS&)), this, SIGNAL(itfRemoveFleet(const ZCHX::Data::ITF_AIS&)));

    //其他目标的双击信号的关联
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4Ais(ZCHX::Data::ITF_AIS)), this, SIGNAL(signalIsDoubleClicked4Ais(ZCHX::Data::ITF_AIS)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4RadarPoint(ZCHX::Data::ITF_RadarPoint)), this, SIGNAL(signalIsDoubleClicked4RadarPoint(ZCHX::Data::ITF_RadarPoint)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4RadarArea(ZCHX::Data::ITF_RadarArea)), this, SIGNAL(signalIsDoubleClicked4RadarArea(ZCHX::Data::ITF_RadarArea)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4CameraRod(ZCHX::Data::ITF_CameraRod)), this, SIGNAL(signalIsDoubleClicked4CameraRod(ZCHX::Data::ITF_CameraRod)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4IslandLine(ZCHX::Data::ITF_IslandLine)), this, SIGNAL(signalIsDoubleClicked4IslandLine(ZCHX::Data::ITF_IslandLine)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4LocalMark(ZCHX::Data::ITF_LocalMark)), this, SIGNAL(signalIsDoubleClicked4LocalMark(ZCHX::Data::ITF_LocalMark)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4PastrolStation(ZCHX::Data::ITF_PastrolStation)), this, SIGNAL(signalIsDoubleClicked4PastrolStation(ZCHX::Data::ITF_PastrolStation)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4WarringLine(ZCHX::Data::ITF_WarringZone)), this, SIGNAL(signalIsDoubleClicked4WarringLine(ZCHX::Data::ITF_WarringZone)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4WarringZone(ZCHX::Data::ITF_WarringZone)), this, SIGNAL(signalIsDoubleClicked4WarringZone(ZCHX::Data::ITF_WarringZone)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4CameraVideoWarn(ZCHX::Data::ITF_CameraVideoWarn)), this, SIGNAL(signalIsDoubleClicked4CameraVideoWarn(ZCHX::Data::ITF_CameraVideoWarn)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4ChannelZone(ZCHX::Data::ITF_Channel)), this, SIGNAL(signalIsDoubleClicked4ChannelZone(ZCHX::Data::ITF_Channel)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4MooringZone(ZCHX::Data::ITF_Mooring)), this, SIGNAL(signalIsDoubleClicked4MooringZone(ZCHX::Data::ITF_Mooring)));
    connect(mMapWidget, SIGNAL(signalIsDoubleClicked4CardMouthZone(ZCHX::Data::ITF_CardMouth)), this, SIGNAL(signalIsDoubleClicked4CardMouthZone(ZCHX::Data::ITF_CardMouth)));
    connect(mMapWidget,SIGNAL(signalIsSelected4RouteLine(ZCHX::Data::RouteLine)),this,SIGNAL(itfSignalIsSelected4RouteLine(ZCHX::Data::RouteLine)));
    connect(mMapWidget,SIGNAL(signalDeleteRoutePoint(ZCHX::Data::RouteLine,int)),this,SIGNAL(itfSignalDeleteRoutePoint(ZCHX::Data::RouteLine,int)));
    connect(mMapWidget,SIGNAL(signalModfiyRoutePoint(ZCHX::Data::RouteLine,int)),this,SIGNAL(itfSignalModfiyRoutePoint(ZCHX::Data::RouteLine,int)));
    connect(mMapWidget,SIGNAL(signalChangeRouteLine(ZCHX::Data::RouteLine)),this,SIGNAL(itfSignalChangeRouteLine(ZCHX::Data::RouteLine)));
    connect(mMapWidget,SIGNAL(signalIsEcdisCameraTrackTarget(ZCHX::Data::ITF_CameraTrackTarget)), this, SIGNAL(itfSignalIsEcdisCameraTrackTarget(ZCHX::Data::ITF_CameraTrackTarget)));

    connect(mMapWidget,SIGNAL(signalIsSelected4ShipPlanLine(ZCHX::Data::ShipPlanLine)),this,SIGNAL(itfSignalIsSelected4ShipPlanLine(ZCHX::Data::ShipPlanLine)));
    connect(mMapWidget,SIGNAL(signalDeleteShipPlanPoint(ZCHX::Data::ShipPlanLine,int)),this,SIGNAL(itfSignalDeleteShipPlanPoint(ZCHX::Data::ShipPlanLine,int)));
    connect(mMapWidget,SIGNAL(signalModfiyShipPlanPoint(ZCHX::Data::ShipPlanLine,int)),this,SIGNAL(itfSignalModfiyShipPlanPoint(ZCHX::Data::ShipPlanLine,int)));
    connect(mMapWidget,SIGNAL(signalChangeShipPlanLine(ZCHX::Data::ShipPlanLine)),this,SIGNAL(itfSignalChangeShipPlanLine(ZCHX::Data::ShipPlanLine)));

    connect(mMapWidget,SIGNAL(signalOpenCameraDisplayWin()),this,SIGNAL(itfSignalOpenCameraDisplayWin()));
    connect(mMapWidget,SIGNAL(signalScreenShot()),this,SIGNAL(itfSignalScreenShot()));
    connect(mMapWidget,SIGNAL(signalSendSeletedAISTarget(QList<ZCHX::Data::ITF_AIS>)),this,SIGNAL(itfSignalSendSeletedAISTarget(QList<ZCHX::Data::ITF_AIS>)));
    connect(mMapWidget,SIGNAL(signalSendPictureInPictureTarget(int, QString)),this,SIGNAL(itfSignalSendPictureInPictureTarget(int, QString)));
    connect(mMapWidget,SIGNAL(signalCreateShipSimutionLine(std::vector<std::pair<double,double> >)),this,SIGNAL(itfSignalCreateShipSimutionLine(std::vector<std::pair<double,double> >)));
    connect(mMapWidget,SIGNAL(signalSendLocalMarkPos(double,double)),this,SIGNAL(itfSignalSendLocalMarkPos(double,double)));
    connect(mMapWidget,SIGNAL(signalSendReferencePos(double,double)),this,SIGNAL(itfSignalSendReferencePos(double,double)));
    connect(mMapWidget,SIGNAL(signalShowLocationMarkOrReferencePos(bool)),this,SIGNAL(itfSignalShowLocationMarkOrReferencePos(bool)));
    connect(mMapWidget,SIGNAL(signalInvokeHotSpot(ZCHX::Data::ITF_CloudHotSpot)),this,SIGNAL(itfSignalInvokeHotSpot(ZCHX::Data::ITF_CloudHotSpot)));
    connect(mMapWidget,SIGNAL(signalAddShipExtrapolation(QString, bool)), this,SIGNAL(itfSignalAddShipExtrapolation(QString, bool)));
    connect(mMapWidget,SIGNAL(signalSendHistoryTrail(QString, bool)), this,SIGNAL(itfSignalSendHistoryTrail(QString, bool)));
    connect(mMapWidget,SIGNAL(signalSendRealTimeTrail(QString, bool)), this,SIGNAL(itfSignalSendRealTimeTrail(QString, bool)));
    connect(mMapWidget,SIGNAL(signalIsSelectedSpecialRoutePoint(ZCHX::Data::SpecialRoutePoint)),this, SIGNAL(itfSignalIsSelectedSpecialRoutePoint(ZCHX::Data::SpecialRoutePoint)));
    connect(mMapWidget,SIGNAL(signalCreateFlowLine(ZCHX::Data::CustomFlowLine)),this,SIGNAL(itfSignalCreateFlowLine(ZCHX::Data::CustomFlowLine)));
    connect(mMapWidget,SIGNAL(signalCreateBlackOrWhiteList(QString,int)),this,SIGNAL(itfSignalCreateBlackOrWhiteList(QString, int)));
    connect(mMapWidget,SIGNAL(signalCreateCPATrack(QString)),this,SIGNAL(itfSignalCreateCPATrack(QString)));
    connect(mMapWidget,SIGNAL(signalLoading()),this, SIGNAL(itfSignalLoading()));
    connect(mMapWidget,SIGNAL(signalLoadOver()),this, SIGNAL(itfSignalLoadOver()));
    connect(mMapWidget,SIGNAL(signalUpdateWarringZoneOrLineState(int,int)),this, SIGNAL(itfSignalUpdateWarringZoneOrLineState(int, int)));
    connect(mMapWidget,SIGNAL(signalUpdateChannelZoneState(int,int)),this, SIGNAL(itfSignalUpdateChannelZoneState(int, int)));
    connect(mMapWidget,SIGNAL(signalUpdateMooringZoneState(int,int)),this, SIGNAL(itfSignalUpdateMooringZoneState(int, int)));
    connect(mMapWidget,SIGNAL(signalUpdateCardMouthZoneState(int,int)),this, SIGNAL(itfSignalUpdateCardMouthZoneState(int, int)));
    connect(mMapWidget,SIGNAL(signalMapIsRoaming()),this, SIGNAL(itfSignalMapIsRoaming()));
    connect(mMapWidget,SIGNAL(signalSendCameraNetGrid(ZCHX::Data::ITF_NetGrid)), this, SIGNAL(itfSignalSendCameraNetGrid(ZCHX::Data::ITF_NetGrid)));
    connect(mMapWidget,SIGNAL(signalSendPTZLocation(double, double)),this,SIGNAL(itfSignalSendPTZLocation(double, double)));
    connect(mMapWidget, SIGNAL(sigElementHoverChanged(qt::Element*)), this, SIGNAL(sigLayerElementHoverChanged(qt::Element*)));
    connect(mMapWidget,  SIGNAL(sigElementSelectionChanged(qt::Element*)), this, SIGNAL(sigLayerElementSelectionChanged(qt::Element*)));
}

void MainWindow::itfSetAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    MapLayerMgr::instance()->getAisLayer()->setAisData(data, true);
}

bool MainWindow::itfSetSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    MapLayerMgr::instance()->getAisLayer()->setSingleAisData(id, data);
}

void MainWindow::itfRemoveAisHistoryData(QString id)
{
    MapLayerMgr::instance()->getAisLayer()->removeAisHistoryData(id);
}

void MainWindow::itfSetHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    MapLayerMgr::instance()->getAisLayer()->setHistoryAisData(data);
}

void MainWindow::itfSetClearHistoryData(bool states)
{
    //MapLayerMgr::instance()->getAisLayer()->setClearHistoryData(states);
    //if(mMapWidget) mMapWidget->getRadarDataMgr()->setClearHistoryData(states);
}

void MainWindow::itfSetConsAisData(const ZCHX::Data::ITF_AIS &data)
{
    MapLayerMgr::instance()->getAisLayer()->setConsAisData(data);
}

void MainWindow::itfSetRadarEchoData(const QMap<QDateTime, ZCHX::Data::ITF_RadarEchoMap> &data)
{
    //if(mMapWidget) mMapWidget->setRadarEchoData(data);
}
void MainWindow::itfSetRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_RadarPoint item, data) {
        res.append(std::shared_ptr<RadarPointElement>(new RadarPointElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetHistoryRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint> &data)
{
    itfSetRadarPointData(data);
}


void MainWindow::itfSetRadarAreaData(const QList<ZCHX::Data::ITF_RadarArea> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR_AREA);
    if(!layer) return;
    QList<std::shared_ptr<Element>> list;
    for(int i=0; i < data.count(); ++i)
    {
        list.append(std::shared_ptr<RadarAreaElement>(new RadarAreaElement(data[i], mMapWidget)));
    }
    layer->addElements(list, true);
}

void MainWindow::itfSetCameraRodData(const QList<ZCHX::Data::ITF_CameraRod> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_WIREROD);
    if(!layer) return;
    QList<std::shared_ptr<Element>> list;
    for(int i=0; i < data.count(); ++i)
    {
        list.append(std::shared_ptr<RodElement>(new RodElement(data[i], mMapWidget)));
    }
    layer->addElements(list, true);
}

void MainWindow::itfSetCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data, ZCHX::Data::ITF_CameraDev::ITF_CAMERA_PARENT parent)
{
    if(!mMapWidget) return;
    if(parent == ZCHX::Data::ITF_CameraDev::PARENT_ROD){
        std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_WIREROD);
        if(!layer) return;
        foreach (ZCHX::Data::ITF_CameraDev item, data) {
            //获取相机对应的杆
            RodElement *rod = dynamic_cast<RodElement*>(layer->getElement(item.mParentEleID).get());
            if(rod){
                rod->updateCamera(item);
            }
        }

    } else if(parent == ZCHX::Data::ITF_CameraDev::PARENT_AIS) {
        MapLayerMgr::instance()->getAisLayer()->updateCamera(data);
    } else if(parent == ZCHX::Data::ITF_CameraDev::PARENT_NONE) {
        std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_CAMERA);
        if(!layer) return;
        QList<std::shared_ptr<Element>> res;
        foreach (ZCHX::Data::ITF_CameraDev item, data) {
            res.append(std::shared_ptr<CameraElement>(new CameraElement(item, mMapWidget)));
        }
        layer->addElements(res, true);
    }
}

void MainWindow::itfSetAisCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    itfSetCameraDevData(data, ZCHX::Data::ITF_CameraDev::PARENT_AIS);
}

void MainWindow::itfSetPastrolStation(const QList<ZCHX::Data::ITF_PastrolRadarStation> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_PATROL_SITE);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_PastrolRadarStation item, data) {
        res.append(std::shared_ptr<PastrolStationElement>(new PastrolStationElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetRadarStation(const QList<ZCHX::Data::ITF_PastrolRadarStation> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR_SITE);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_PastrolRadarStation item, data) {
        res.append(std::shared_ptr<PastrolStationElement>(new PastrolStationElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetWarringZoneData(const QList<ZCHX::Data::ITF_WarringZone> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_DEFENCE);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_WarringZone item, data) {
        res.append(std::shared_ptr<WarningZoneElement>(new WarningZoneElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetShipSiumtionData(const std::vector<std::pair<double, double> > &data)
{
    //if(mMapWidget) mMapWidget->setShipSiumtionData(data);
}

void MainWindow::itfSetAISDataToSiumtion(const QList<ZCHX::Data::ITF_AIS> &data)
{
    //if(mMapWidget) mMapWidget->setAisDataToSiumtion(data);
}

void MainWindow::itfSetCoastData(const QList<ZCHX::Data::ITF_CoastData> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_COASTDATA);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_CoastData item, data) {
        res.append(std::shared_ptr<CoastElement>(new CoastElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfImportCoastData(const std::vector<std::pair<double, double> > & data)
{
    ZCHX::Data::ITF_CoastData zone;
    zone.path = data;
    zone.id = 0;
    zone.beachLength = zchxMapDataUtils::getTotalDistance(data);

    CoastDataInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.manageOrganization = dlg.getOrg();
        zone.responsibilityAccount = dlg.getAccount();
        zone.height = dlg.getHeight();

        if(!zone.name.isEmpty())
        {
            emit itfCreateCoastDataLine(zone);
        }
    }
}

void MainWindow::itfSetSeabedPipeLine(const QList<ZCHX::Data::ITF_SeabedPipeLine> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_SEABEDPIPELINE);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_SeabedPipeLine item, data) {
        res.append(std::shared_ptr<SeabedPipeLineElement>(new SeabedPipeLineElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfImportSeabedPipeLine(const std::vector<std::pair<double, double> > & data)
{
    ZCHX::Data::ITF_SeabedPipeLine zone;
    zone.path = data;
    zone.id = 0;
    zone.lineLength = zchxMapDataUtils::getTotalDistance(data);

    CoastDataInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.manageOrganization = dlg.getOrg();
        zone.responsibilityAccount = dlg.getAccount();
        zone.depth = dlg.getHeight();

        if(!zone.name.isEmpty())
        {
            emit itfCreateSeabedPipeLineLine(zone);
        }
    }
}

void MainWindow::itfSetStructure(const QList<ZCHX::Data::ITF_Structure> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_STRUCTURE);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_Structure item, data) {
        res.append(std::shared_ptr<StructureElement>(new StructureElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfImportStructure(const std::vector<std::pair<double, double> > &data)
{
//    ZCHX_DATA_FACTORY->getStructureMgr()->importData(data);
}

void MainWindow::itfSetAreaNet(const QList<ZCHX::Data::ITF_AreaNet> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AREANET);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_AreaNet item, data) {
        res.append(std::shared_ptr<AreaNetElement>(new AreaNetElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfImportAreaNet(const std::vector<std::pair<double, double> > &data)
{
    ZCHX::Data::ITF_AreaNet zone;
    zone.path = data;
    zone.id = 0;
    zone.area = zchxMapDataUtils::getTotalArea(data);

    QInputDialog input_dialog;
    input_dialog.setCancelButtonText(tr("Cancel"));
    input_dialog.setOkButtonText(tr("OK"));
    input_dialog.setWindowTitle(tr("WARRING ZONE"));
    input_dialog.setLabelText(tr("(Default 5 minute warning)Name:"));
    if (input_dialog.exec())
    {
        zone.name = input_dialog.textValue();
    }

    if(!zone.name.isEmpty())
    {
        emit itfCreateAreaNetZone(zone);
    }
}

void MainWindow::itfSetChannel(const QList<ZCHX::Data::ITF_Channel> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_CHANNEL);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_Channel item, data) {
        res.append(std::shared_ptr<ChannelElement>(new ChannelElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfImportChannel(const std::vector<std::pair<double, double> > &data)
{
    ZCHX::Data::ITF_Channel zone;
    zone.path = data;
    zone.id = 0;

    ChannelInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.courseType = dlg.getChannelType();
        zone.isWarn = dlg.getChannelStatus();
        zone.speedLimit = dlg.getMaxSpeed();
        zone.collisionThreshold = dlg.getCollisionMaxDis();
        zone.yawThreshold = dlg.getYawMinDis();
        zone.overtakThreshold = dlg.getOvertak();
        zone.dropAnchorThreshold = dlg.getDropAnchor();
        zone.overtakShortDis = dlg.getOvertakShortDis();
        zone.overtakVerticalDis = dlg.getOvertakVerticalDis();
        zone.acceleraThreshold = dlg.getAcceleraThreshold();
        zone.acceleraDfference = dlg.getAcceleraDfference();
        zone.isConverseWarn = dlg.isConverseWarn();
        zone.isOverloadWarn = dlg.isOverloadWarn();

        zone.fillColor = dlg.getColor();
    }

    if(!zone.name.isEmpty())
    {
        emit itfCreateChannelZone(zone);
    }
}

void MainWindow::itfSelectChannelLine(const QString& name, const ZCHX::Data::ITF_ChannelLine & line)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_CHANNEL);
    if(!layer) return;

    ChannelElement* ele = dynamic_cast<ChannelElement*>(layer->getElement(name).get());
    if(!ele) return;
    ZCHX::Data::ITF_Channel item = ele->data();
    for (int j = 0; j < item.lineList.size(); j++)
    {
        if (item.lineList[j] == line)
        {
            ele->setLineSelected(j, true);
        }
        else
        {
            ele->setLineSelected(j, false);
        }
    }
}

void MainWindow::itfCancelChannelLine(const QString& name)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_CHANNEL);
    if(!layer) return;

    ChannelElement* ele = dynamic_cast<ChannelElement*>(layer->getElement(name).get());
    if(!ele) return;
    ZCHX::Data::ITF_Channel item = ele->data();
    for (int j = 0; j < item.lineList.size(); j++)
    {
        ele->setLineSelected(j, false);
    }
}

void MainWindow::itfSetMooring(const QList<ZCHX::Data::ITF_Mooring> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_MOORING);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_Mooring item, data) {
        res.append(std::shared_ptr<MooringElement>(new MooringElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfImportMooring(const std::vector<std::pair<double, double> > &data)
{
    ZCHX::Data::ITF_Mooring zone;
    zone.path = data;
    zone.id = 0;

    MooringInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.anchorType = dlg.getMooringType();
        zone.isWarn = dlg.getMooringStatus();
        zone.fillColor = dlg.getColor();
        zone.displaceDis = dlg.getDisplaceDis();
        zone.displaceCycle = dlg.getDisplaceCycle();
    }

    if(!zone.name.isEmpty())
    {
        emit itfCreateMooringZone(zone);
    }
}

void MainWindow::itfSetCardMouth(const QList<ZCHX::Data::ITF_CardMouth> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_CARDMOUTH);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_CardMouth item, data) {
        res.append(std::shared_ptr<CardMouthElement>(new CardMouthElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfImportCardMouth(const std::vector<std::pair<double, double> > &path)
{
    ZCHX::Data::ITF_CardMouth zone;
    zone.path = path;
    zone.id = 0;

    CardMouthInfoDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        zone.name = dlg.getName();
        zone.caMouCapConType = dlg.getType();
        zone.isAisUnopened = dlg.isAisUnopened();
        zone.isWarn = dlg.getWarning();
        zone.fillColor = dlg.getColor();
        zone.overlength = dlg.getOverlength();
    }

    if(!zone.name.isEmpty())
    {
        emit itfCreateCardMouthZone(zone);
    }
}

void MainWindow::itfSetFleet(const QList<ZCHX::Data::ITF_Fleet> &data)
{
    QMap<QString, ZCHX::Data::ITF_Fleet> fleetMap;
    for(int i = 0; i < data.size(); ++i)
    {
        fleetMap.insert(data[i].aisId, data[i]);
    }
    if(mMapWidget) mMapWidget->setFleet(fleetMap);
}

void MainWindow::itfSetShipAlarmAscendMap(const QMap<QString, ZCHX::Data::ITF_ShipAlarmAscend> &map)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_ALARMASCEND);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_ShipAlarmAscend item, map) {
        res.append(std::shared_ptr<ShipAlarmAscendElement>(new ShipAlarmAscendElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfUpdateWarringZone(const QList<ZCHX::Data::ITF_WarringZone> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_DEFENCE);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_WarringZone item, data) {
        res.append(std::shared_ptr<WarningZoneElement>(new WarningZoneElement(item, mMapWidget)));
    }
    layer->addElements(res, false);
}

void MainWindow::itfUpdateWarringZone(const ZCHX::Data::ITF_WarringZone &zone)
{
    itfUpdateWarringZone(QList<ZCHX::Data::ITF_WarringZone>() << zone);
}

void MainWindow::removeWarrningZone(const ZCHX::Data::ITF_WarringZone &zone)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_DEFENCE);
    if(!layer) return;
    layer->removeElement(zone.getID());
}

QList<ZCHX::Data::ITF_WarringZone> MainWindow::getAllWarrningZone() const
{
    QList<ZCHX::Data::ITF_WarringZone> res;
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_DEFENCE);
    if(!layer) return res;
    std::list<std::shared_ptr<Element>> list = layer->getElements();
    for(std::shared_ptr<Element> ele : list)
    {
        WarningZoneElement* item = dynamic_cast<WarningZoneElement*>(ele.get());
        if(item){
            res.append(item->data());
        }
    }
    return res;
}

void MainWindow::itfSetIslandLineData(const QList<ZCHX::Data::ITF_IslandLine> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_ISLAND);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_IslandLine item, data) {
        res.append(std::shared_ptr<IslandLineElement>(new IslandLineElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetLocalMarkData(const QList<ZCHX::Data::ITF_LocalMark> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_LOCALMARK);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_LocalMark item, data) {
        res.append(std::shared_ptr<LocalMarkElement>(new LocalMarkElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetCameraGdyData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_GDY);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_CameraDev item, data) {
        res.append(std::shared_ptr<CameraElement>(new CameraElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetCameraPlanData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_UAV);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_CameraDev item, data) {
        res.append(std::shared_ptr<CameraElement>(new CameraElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetDangerousCircleData(const QList<ZCHX::Data::ITF_DangerousCircle> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_DANGEROUS_CIRLE);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_DangerousCircle item, data) {
        res.append(std::shared_ptr<DangerousCircleElement>(new DangerousCircleElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetDangerousCircleRange(const double range)
{
    //每一个危险圈设定半径  这里取消
    //if(mMapWidget) mMapWidget->setDangerousCircleRange(range);
}

void MainWindow::itfSetRadarFeatureZoneDagta(const QList<ZCHX::Data::ITF_RadarFeaturesZone> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR_FRETURE_AREA);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_RadarFeaturesZone item, data) {
        res.append(std::shared_ptr<RadarFeatureZoneElement>(new RadarFeatureZoneElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetRouteLineData(const QList<ZCHX::Data::RouteLine> &data)
{
    std::vector<RouteLineElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::RouteLine tmp = data.at(i);
        list.push_back(new RouteLineElement(tmp));

    }
    if(mMapWidget) mMapWidget->getRouteDataMgr()->setRouteLineData(list);
}

void MainWindow::itfSetALLSpecialRouteLineData(const QList<ZCHX::Data::SpecialRouteLine> &data)
{
    std::vector<SpecialRouteLineElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::SpecialRouteLine tmp = data.at(i);
        list.push_back(new SpecialRouteLineElement(tmp));
    }
    if(mMapWidget) mMapWidget->getRouteDataMgr()->setALLSpecialRouteLineData(list);

}

void MainWindow::itfSetSpacilRouteLineData(const QList<ZCHX::Data::SpecialRouteLine> &data)
{
    std::vector<SpecialRouteLineElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::SpecialRouteLine tmp = data.at(i);
        list.push_back(new SpecialRouteLineElement(tmp));

    }
    if(mMapWidget) mMapWidget->getRouteDataMgr()->setSpacilRouteLineData(list);
}

void MainWindow::itfSetPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data)
{
    if(mMapWidget) mMapWidget->getRouteDataMgr()->setPickUpSpacilRoutePoint(data);
}

void MainWindow::itfSetHistoryRouteLineData(const QList<ZCHX::Data::RouteLine> &data)
{
    std::vector<RouteLineElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::RouteLine tmp = data.at(i);
        list.push_back(new RouteLineElement(tmp));

    }
    if(mMapWidget) mMapWidget->getRouteDataMgr()->setHistoryRouteLineData(list);
}

void MainWindow::itfSetRouteDefaultCableData(const ZCHX::Data::CableAssembly &data, int uSlack)
{
    if(mMapWidget) mMapWidget->getRouteDataMgr()->setRouteDefaultCableData(data,uSlack);
}

void MainWindow::itfSetRouteConfigDisplay(int uRouteID, bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable, bool bShowIndex)
{
    //if(mMapWidget) mMapWidget->setRouteConfigDisplay(uRouteID, bShowPoint, bShowType, bShowLine, bShowCable, bShowIndex);
}

void MainWindow::itfSetRouteDisplayFlag(int uRouteID, int uDisplayFlag)
{
    //if(mMapWidget) mMapWidget->setRouteDisplayFlag(uRouteID,uDisplayFlag);
}

void MainWindow::itfCloseCurRouteLineData()
{
    //if(mMapWidget) mMapWidget->closeCurRouteLineData();
}

void MainWindow::itfDeleteRouteLineDataFromProjectID(const int uProjectID)
{
    //if(mMapWidget) mMapWidget->deleteRouteLineDataFromProjectID(uProjectID);
}

void MainWindow::itfSetShipPlanLineData(const QList<ZCHX::Data::ShipPlanLine> &data)
{
    std::vector<ShipPlanLineElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::ShipPlanLine tmp = data.at(i);
        list.push_back(new ShipPlanLineElement(tmp));

    }
    if(mMapWidget) mMapWidget->getShipPlanDataMgr()->setShipPlanLineData(list);
}

void MainWindow::itfSetAllProjectShipPlanLineData(const QList<ZCHX::Data::ShipPlanLine> &data)
{
    std::vector<ShipPlanLineElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::ShipPlanLine tmp = data.at(i);
        list.push_back(new ShipPlanLineElement(tmp));
    }

    if(mMapWidget) mMapWidget->getShipPlanDataMgr()->setAllProjecrShipPlanLineData(list);
}

void MainWindow::itfSetShipPlanProjectID(const int ProjectID)
{
    //路由的时候已经传入工程ID了
}

void MainWindow::itfSetRouteLinePickupLocation(const int uRouteID, const double dLat, const double dLon)
{
    //if(mMapWidget) mMapWidget->setRouteLinePickupLocation(uRouteID,dLat,dLon);
}

void MainWindow::itfSetRouteLinePickupInfor(const double dKp, bool bShowLength, const double dLength, bool bShowSlope, const double dSlope)
{
    //if(mMapWidget) mMapWidget->setRouteLinePickupInfor(dKp,bShowLength,dLength,bShowSlope,dSlope);
}

void MainWindow::itfChangeCurrentRouteLine(const ZCHX::Data::RouteLine &data, int uRouteID)
{
    //if(mMapWidget) mMapWidget->changeCurrentRouteLine(data,uRouteID);
}

void MainWindow::itfChangeRoutePathAssessStatus(int uAssessStatus, int uRouteID)
{
    //if(mMapWidget) mMapWidget->changeRoutePathAssessStatus(uAssessStatus,uRouteID);
}

void MainWindow::itfSetSimulateData(int uSimulateDataID, int uSimulateFlag)
{
    //if(mMapWidget) mMapWidget->setSimulateData(uSimulateDataID,uSimulateFlag);
}

void MainWindow::itfSetSimulateLocation(float fCurSimulateKP)
{
    //if(mMapWidget) mMapWidget->setSimulateLocation(fCurSimulateKP);
}

void MainWindow::itfSetCameraObservationZoneData(const QList<ZCHX::Data::ITF_CameraView> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_CAMERA_VIEW);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_CameraView item, data) {
        res.append(std::shared_ptr<CameraViewElement>(new CameraViewElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfSetRadarVideoData(double dCentreLon, double dCentreLat, double dDistance, int uType, int uLoopNum)
{

    ((zchxRadarVideoLayer*)(LayerMgr->getLayer(ZCHX::LAYER_RADARVIDEO).get()))->setRadarVideoData(dCentreLon,dCentreLat,dDistance,uType,uLoopNum);
}

void MainWindow::itfSetRadarVideoPixmap(int uIndex, const QPixmap &objPixmap, const QPixmap &prePixmap)
{
    ((zchxRadarVideoLayer*)(LayerMgr->getLayer(ZCHX::LAYER_RADARVIDEO).get()))->setRadarVideoPixmap(uIndex,objPixmap,prePixmap);
}

void MainWindow::itfSetCurrentRadarVideoPixmap(const QPixmap &objPixmap)
{
    ((zchxRadarVideoLayer*)(LayerMgr->getLayer(ZCHX::LAYER_RADARVIDEO).get())) ->setCurrentRadarVideoPixmap(objPixmap);
}

//void MainWindow::itfSetMultibeamData(const QList<ZCHX::Data::ITF_Multibeam> &data, const double dMinLon, const double dMinLat, const double dMaxLon, const double dMaxLat, const double dMinX, const double dMinY, const double dMinZ, const double dMaxX, const double dMaxY, const double dMaxZ)
//{
//    std::vector<Multibeam> list;
//    for(int i=0; i< data.count(); ++i)
//    {
//        ZCHX::Data::ITF_Multibeam tmp = data.at(i);
//        Multibeam item(tmp);
//        list.push_back(item);
//    }

//    if(mMapWidget) mMapWidget->setMultibeamData(list,dMinLon,dMinLat,dMaxLon,dMaxLat,dMinX,dMinY,dMinZ,dMaxX,dMaxY,dMaxZ);
//}

void MainWindow::itfSetMultibeamDataByImg(int iRouteID, const QPixmap &samplePixmap, const QPixmap &objPixmap, double dMinLon, double dMinLat, double dMaxLon, double dMaxLat)
{
    //if(mMapWidget) mMapWidget->setMultibeamDataByImg(iRouteID, samplePixmap, objPixmap, dMinLon, dMinLat, dMaxLon, dMaxLat);
}

void MainWindow::itfSetAllProjectRouteLineData(const QList<ZCHX::Data::RouteLine> &data)
{

    std::vector<RouteLineElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::RouteLine tmp = data.at(i);
        list.push_back(new RouteLineElement(tmp));

    }

    if(mMapWidget) mMapWidget->getRouteDataMgr()->setAllProjectRouteLineData(list);

}

void MainWindow::itfSetRouteCrossData(const QList<ZCHX::Data::ITF_RouteCross> &data)
{
    std::vector<RouteCrossElement*> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::ITF_RouteCross tmp = data.at(i);
        list.push_back(new RouteCrossElement(tmp));

    }

    if(mMapWidget) mMapWidget->getRouteDataMgr()->setRouteCrossData(list);
}
void MainWindow::itfSetCameraVideoWarnData(const QList<ZCHX::Data::ITF_VideoTarget> &data)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_WARNING_TARGET);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_VideoTarget item, data) {
        res.append(std::shared_ptr<VideoTargetElement>(new VideoTargetElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleEllipse &item)
{
    //if(mMapWidget) mMapWidget->appendElementItem(item);
}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleTriangle &item)
{
    //if(mMapWidget) mMapWidget->appendElementItem(item);
}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleLine &item)
{
    //if(mMapWidget) mMapWidget->appendElementItem(item);
}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleRect &item)
{
    //if(mMapWidget) mMapWidget->appendElementItem(item);
}

void MainWindow::itfRemoveElementItem(Element *pitem)
{
    //if(mMapWidget) mMapWidget->removeElementItem(pitem);
}

void MainWindow::itfClearElementData()
{
    //if(mMapWidget) mMapWidget->clearElementData();
}


void MainWindow::itfSetPickUpRadarInfo(qint32 tracknumber)
{
    //if(mMapWidget) mMapWidget->SetPickUpRadarInfo(tracknumber);
}

void MainWindow::itfSetPickUpAisInfo(QString id)
{
    MapLayerMgr::instance()->getAisLayer()->setPickUpAisInfo(id);
}

void MainWindow::itfSetPickUpPosition(QString id)
{
    //if(mMapWidget) mMapWidget->SetPickUpPosition(id);
}

void MainWindow::itfSetPickUpRouteCross(const int id)
{
    //if(mMapWidget) mMapWidget->SetPickUpRouteCross(id);
}

//void MainWindow::itfSetShipSimulationExtrapolationDatas(const QList<ZCHX::Data::ITF_AIS> &DataList)
//{
//    //if(mMapWidget) mMapWidget->setShipSimulationExtrapolationDatas(DataList);
//}

void MainWindow::itfUpdateShipSimulationExtrapolationData(const QString &id, int time)
{
    MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->updateExtrapolationTime(id, time);
}

void MainWindow::itfAppendShipSimulationExtrapolationData(const QString &id, int time)
{
    MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->appendExtrapolation(id, time);
    MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->updateExtrapolationTime(id, time);
}

void MainWindow::itfDeleteShipSimulationExtrapolationData(const QString &id)
{
    MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->removeExtrapolation(id);
}

ZCHX::Data::ExtrapolateList MainWindow::itfGetShipSimulationExtrapolationData()
{
    return MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->getExtrapolationList();
}

void MainWindow::itfSetRealTimeShipTrailDatas(const QMap<QString, QList<ZCHX::Data::ITF_AIS> > &DataList)
{
    QMap<QString, QList<ZCHX::Data::ITF_AIS> >::const_iterator it = DataList.begin();
    for(; it != DataList.end(); it++)
    {
        MapLayerMgr::instance()->getAisLayer()->setRealtimeTailTrack(it.key(), it.value());
    }

}



void MainWindow::itfSetRouteHistogramData(const int RouteId, const QMap<int, QList<int> > &DataList)
{
    //if(mMapWidget) mMapWidget->setRouteHistogramData(RouteId,DataList);
}

void MainWindow::itfSetRealTimeFlowLineHistogramData(const QMap<int, QList<int> > &DataList)
{
    //if(mMapWidget) mMapWidget->setRealTimeFlowLineHistogramData(DataList);
}

void MainWindow::itfSetInTheAreaOfTrackData(const QList<QList<ZCHX::Data::LatLon> > &data)
{
    //if(mMapWidget) mMapWidget->setInTheAreaOfTrackData(data);
}

void MainWindow::itfSetRolePrivilegeList(const QList<int> &list)
{

    //if(mMapWidget) mMapWidget->setRolePrivilegeList(list);
}
void MainWindow::itfSetIsHideMap(bool b)
{
    if(mMapWidget) mMapWidget->setIsHideMap(b);
}

void MainWindow::itfSetNavigateionging(bool b)
{
    if(mMapWidget) mMapWidget->setIsNavigation(b);
}

void MainWindow::itfSetIsOpenMeet(bool b)
{
    if(mMapWidget) mMapWidget->setIsOpenMeet(b);
}

void MainWindow::itfSetPuickupType(const ZCHX::Data::ECDIS_PICKUP_TYPEs &type)
{
    if(mMapWidget) mMapWidget->setCurPickupType(type);
}

void MainWindow::itfUpdateCameraDevStatus(uint cameid, uint st)
{
    //if(mMapWidget) mMapWidget->zchxUpdateCameraDevStatus(cameid, st);
}
void MainWindow::itfSetRadarTrackPoint(bool b)
{
    if(mMapWidget) mMapWidget->setEnableRadarTrackPoint(b);
}

void MainWindow::itfSetEnableCameraTargerTrack(bool b)
{
    if(mMapWidget) mMapWidget->setEnableCameracombinedTrack(b);
}

void MainWindow::itfSetEnableRouteHistogram(bool b)
{
    if(mMapWidget) mMapWidget->setEnableRouteHistogram(b);
}

void MainWindow::itfSetEnableShipTag(int tag)
{
    MapLayerMgr::instance()->getAisLayer()->SetEnableShipTag(tag);
}

void MainWindow::itfSetEnableTooBarVisable(bool visible)
{
    //d->m_toolbar->setVisible(visible);
}

void MainWindow::itfsetPluginUseModel(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL &mod)
{
    if(mMapWidget) mMapWidget->setCurPluginUserModel(mod);
}

void MainWindow::itfsetSelectedShip()
{
    mMapWidget->setETool2DrawTrackTarget();
}

void MainWindow::itfsetSelectedAllTrack()
{
    mMapWidget->setETool2DrawCameraTrackTarget();
}

void MainWindow::itfsetSelectedRouteOrCross()
{
    mMapWidget->setETool2DrawRouteOrCross();
}

double MainWindow::itfzchxUtilToolAngle4north()
{
    return mMapWidget->zchxUtilToolAngle4north();
}

void MainWindow::itfzchxUtilToolSetAngle4north(double ang)
{
//    qDebug()<<"MainWindow::itfzchxUtilToolSetAngle4north 当前角度:" << ang;
    if(mMapWidget) mMapWidget->zchxUtilToolSetAngle4north(ang);
}

bool MainWindow::itfzchxUtilToolPoint4CurWindow(double lat, double lon, QPointF &p)
{
    return  mMapWidget->zchxUtilToolPoint4CurWindow(lat,lon,p);
}

bool MainWindow::itfzchxUtilToolLL4CurPoint(const QPointF &p, double &lat, double &lon)
{
    //    QPoint tmp(p.x(),p.y());
    //    QPoint zxl = mapFromParent(tmp);
    //    qDebug()<<"zxl"<<zxl;
    return mMapWidget->zchxUtilToolLL4CurPoint(p,lat,lon);
}

uint MainWindow::itfzchxUtilToolCurZoom()
{
    return mMapWidget->zchxUtilToolCurZoom();
}


void MainWindow::setMapUnit(const MapUnit &unit)
{
    if(mMapWidget) mMapWidget->setMapUnit(unit);
}

void MainWindow::setIsShowStatusBar(bool bShow)
{
    statusBar()->setVisible(bShow);
}

Element* MainWindow::getCurrentSelectedElement()
{
    return mMapWidget->getCurrentSelectedElement();
}

void MainWindow::itfChannelZoneData4id(int id, ZCHX::Data::tagITF_Channel &info, bool &ok)
{
    std::shared_ptr<MapLayer> layer = LayerMgr->getLayer(ZCHX::LAYER_CHANNEL);
    if(layer){
        ChannelElement* ele = dynamic_cast<ChannelElement*>(layer->getElement(QString::number(id)).get());
        if(ele){
            info = ele->data();
            ok = true;
            return;
        }
    }
    ok = false;
}

void MainWindow::itfMooringZoneData4id(int id, ZCHX::Data::tagITF_Mooring &info, bool &ok)
{
    std::shared_ptr<MapLayer> layer = LayerMgr->getLayer(ZCHX::LAYER_MOORING);
    if(layer){
        MooringElement* ele = dynamic_cast<MooringElement*>(layer->getElement(QString::number(id)).get());
        if(ele){
            info = ele->data();
            ok = true;
            return;
        }
    }
    ok = false;
}

void MainWindow::itfCardMouthZoneData4id(int id, ZCHX::Data::tagITF_CardMouth &info, bool &ok)
{
    std::shared_ptr<MapLayer> layer = LayerMgr->getLayer(ZCHX::LAYER_CARDMOUTH);
    if(layer){
        CardMouthElement* ele = dynamic_cast<CardMouthElement*>(layer->getElement(QString::number(id)).get());
        if(ele){
            info = ele->data();
            ok = true;
            return;
        }
    }
    ok = false;
}

bool MainWindow::getIsAtiAdaptor() const
{
    return true;
   // return mMapWidget->getIsAtiAdaptor();
}

void MainWindow::setIsAtiAdaptor(bool isAtiAdaptor)
{
    //if(mMapWidget) mMapWidget->setIsAtiAdaptor(isAtiAdaptor);
}

void MainWindow::setUseRightKey(bool bUseRightKey)
{
    if(mMapWidget) mMapWidget->setUseRightKey(bUseRightKey);
}



void MainWindow::closeEvent(QCloseEvent * e)
{
    e->accept();
}
//zxl
/**
 * @brief MainWindow::OnMeasureAreaAction
 * @anthor:zxl
 * area measurement
 */
void MainWindow::OnMeasureAreaAction()
{
     mMapWidget->setETool2DrawArea();
}

/**
 * @brief MainWindow::OnDirAngleAction
 * @anthor:zxl
 * draw direct angle
 */
void MainWindow::OnDirAngleAction()
{
    mMapWidget->setETool2DrawDirAngle();
}

void MainWindow::OnDistantcAction()
{
    mMapWidget->setETool2DrawDistance();
}

void MainWindow::OnExitEToolAction()
{
    mMapWidget->releaseDrawStatus();
}

void MainWindow::OnWarringZONEAction()
{
    mMapWidget->setETool2DrawZONE();
}

void MainWindow::OnWarringZONESelected()
{
    mMapWidget->setETool2SelectZONE();
}

void MainWindow::OnWarringZONEmove()
{
    mMapWidget->setETool2moveZONE();
}

void MainWindow::OnWarringZONEmoveCtrlPoint()
{
    mMapWidget->setETool2ctrlZONE();
}

void MainWindow::OnWarringZONEDelCtrlPoint()
{
    mMapWidget->setETool2delCtrlZONE();
}

void MainWindow::OnWarringZONEAddCtrlPoint()
{
    mMapWidget->setETool2addCtrlZONE();
}

void MainWindow::OnCoastDataLineAdd()
{
    mMapWidget->setETool2Draw4CoastDataLine();
}

void MainWindow::OnSeabedPipeLineLineAdd()
{
    mMapWidget->setETool2Draw4SeabedPipeLineLine();
}

void MainWindow::OnStructurePointAdd()
{
    mMapWidget->setETool2Draw4StructurePoint();
}

void MainWindow::OnAreaNetZoneAdd()
{
    mMapWidget->setETool2Draw4AreaNetZone();
}

void MainWindow::OnChannelAreaAdd()
{
    mMapWidget->setETool2Draw4ChannelArea();
}

void MainWindow::OnChannelZONESelected()
{
    mMapWidget->setETool2SelectChannel();
}

void MainWindow::OnChannelZONEMove()
{
    mMapWidget->setETool2moveChannel();
}

void MainWindow::OnChannelZONEMoveCtrlPoint()
{
    mMapWidget->setETool2ctrlChannel();
}

void MainWindow::OnChannelZONEDelCtrlPoint()
{
    mMapWidget->setETool2delCtrlChannel();
}

void MainWindow::OnChannelZONEAddCtrlPoint()
{
    mMapWidget->setETool2addCtrlChannel();
}

void MainWindow::OnMooringAreaAdd()
{
    mMapWidget->setETool2Draw4MooringArea();
}

void MainWindow::OnMooringZONESelected()
{
    mMapWidget->setETool2SelectMooring();
}

void MainWindow::OnMooringZONEMove()
{
    mMapWidget->setETool2moveMooring();
}

void MainWindow::OnMooringZONEMoveCtrlPoint()
{
    mMapWidget->setETool2ctrlMooring();
}

void MainWindow::OnMooringZONEDelCtrlPoint()
{
    mMapWidget->setETool2delCtrlMooring();
}

void MainWindow::OnMooringZONEAddCtrlPoint()
{
    mMapWidget->setETool2addCtrlMooring();
}

void MainWindow::OnCardMouthAreaAdd()
{
    mMapWidget->setETool2Draw4CardMouthArea();
}

void MainWindow::OnCardMouthZONESelected()
{
    mMapWidget->setETool2SelectCardMouth();
}

void MainWindow::OnCardMouthZONEMove()
{
    mMapWidget->setETool2moveCardMouth();
}

void MainWindow::OnCardMouthZONEMoveCtrlPoint()
{
    mMapWidget->setETool2ctrlCardMouth();
}

void MainWindow::OnCardMouthZONEDelCtrlPoint()
{
    mMapWidget->setETool2delCtrlCardMouth();
}

void MainWindow::OnCardMouthZONEAddCtrlPoint()
{
    mMapWidget->setETool2addCtrlCardMouth();
}

void MainWindow::OnIslandLineAdd()
{
    mMapWidget->setETool2Draw4IslandLine();
}

void MainWindow::OnIslandLineSelected()
{
    mMapWidget->setETool2Select4IslandLine();
}

void MainWindow::OnIslandLineMove()
{
    mMapWidget->setETool2move4IslandLine();
}

void MainWindow::OnIslandLineMoveCtrlPoint()
{
    mMapWidget->setETool2moveCtrlPoint4IslandLine();
}

void MainWindow::OnIslandLineDelCtrlPoint()
{
    mMapWidget->setETool2delCtrlPoint4IslandLine();
}

void MainWindow::OnIslandLineAddCtrlPoint()
{
    mMapWidget->setETool2addCtrlPoint4IslandLine();
}

void MainWindow::itfToolBarShipPlanLineAdd()
{
    mMapWidget->setETool2DrawShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineSelected()
{
     mMapWidget->setETool2SelectShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineInsertCtrlPoint()
{
    mMapWidget->setETool2insertCtrlPointShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineMoveCtrlPoint()
{
    mMapWidget->setETool2moveCtrlPointShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineDelCtrlPoint()
{
    mMapWidget->setETool2delCtrlPointShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineAddCtrlPoint()
{
    mMapWidget->setETool2addCtrlPointShipPlanLine();
}

void MainWindow::itfSetShipPlanWaterDepth(QList<ZCHX::Data::ITF_WaterDepth> &WaterData)
{
    //mMapWidget->setShipPlanWaterDepth(WaterData);
}

void MainWindow::itfSetHistoryTrackStyle(const QString &color, const int lineWidth)
{
    MapLayerMgr::instance()->getAisLayer()->setHistoryTrackStyle(color, lineWidth);
}

void MainWindow::itfSetPrepushTrackStyle(const QString &color, const int lineWidth)
{
    MapLayerMgr::instance()->getAisLayer()->setPrepushTrackStyle(color, lineWidth);
}

void MainWindow::itfSetMapUnit(const MapUnit& uint)
{
    if(mMapWidget) mMapWidget->setMapUnit(uint);
}

void MainWindow::OnPickupAction()
{
    if(mMapWidget) mMapWidget->setETool2DrawPickup();
}

void MainWindow::OnShowAllAction()
{
    mMapWidget->ShowAll();
}

void MainWindow::OnLocalMarkAction()
{
    mMapWidget->setETool2DrawLocalMark();
}

void MainWindow::OnGpsAction()
{
    mMapWidget->setETool2DrawGps();
}

void MainWindow::OnSelBaseDisplayStyle()
{
    mMapWidget->setMapStyle(MapStyleBase);
}

void MainWindow::OnSelStandardDisplayStyle()
{
    mMapWidget->setMapStyle(MapStyleStandard);
}

void MainWindow::OnSelAllDisplayStyle()
{
    mMapWidget->setMapStyle(MapStyleAll);
}

void MainWindow::OnSelDayBlackBackDisplyStyle()
{
    mMapWidget->setMapStyle(MapStyleEcdisDayBlackBack);
}

void MainWindow::OnSelNightDisplyStyle()
{
     mMapWidget->setMapStyle(MapStyleEcdisNight);
}

void MainWindow::OnSelDayDUSKDisplyStyle()
{
     mMapWidget->setMapStyle(MapStyleEcdisDayDUSK);
}

void MainWindow::OnSelDayWhiteBackDisplyStyle()
{
     mMapWidget->setMapStyle(MapStyleEcdisDayWhiteBack);
}

void MainWindow::OnSelDayBrightDisplyStyle()
{
     mMapWidget->setMapStyle(MapStyleEcdisDayBright);
}

void MainWindow::OnZONESelected()
{
    mMapWidget->setETool2SelectCommonZONE();
}

void MainWindow::OnSetMapAngle(double angle)
{
    itfzchxUtilToolSetAngle4north(angle);
}

void MainWindow::setMapCenterAndZoom(double lat, double lon, int zoom)
{
    if(mMapWidget) mMapWidget->setCenterAndZoom(ZCHX::Data::LatLon(lat, lon), zoom);
}

void MainWindow::setMapCenterAndRotate(double lat, double lon, double ang)
{
    if(mMapWidget) mMapWidget->setMapRotateAtLL(lat, lon, ang * 180 / GLOB_PI);
}

void MainWindow::setMapCenterAndRotate_new(double lat, double lon, double ang)
{
    if(mMapWidget) mMapWidget->setMapRotateAtLL(lat, lon, ang);
}

void MainWindow::resetMapRotate(double lat, double lon)
{
    if(mMapWidget) mMapWidget->resetMapRotate(lat,lon);
}

void MainWindow::setMapCenter(double lat, double lon)
{
    if(mMapWidget) mMapWidget->setCenterAndZoom(ZCHX::Data::LatLon(lat, lon), -1);
}

void MainWindow::resetMap()
{
    if(mMapWidget) mMapWidget->reset();
}

void MainWindow::zchxSetStyleAutoChange(bool val)
{
    if(mMapWidget) mMapWidget->setStyleAutoChange(val);
}

void MainWindow::zchxSetGISDayBrightTime(const QTime &t)
{
    if(mMapWidget) mMapWidget->setGISDayBrightTime(t);
}

void MainWindow::zchxSetGISDuskTime(const QTime &t)
{
    if(mMapWidget) mMapWidget->setGISDuskTime(t);
}

void MainWindow::zchxSetGISNightTime(const QTime &t)
{
    if(mMapWidget) mMapWidget->setGISNightTime(t);
}

void MainWindow::zchxClearAisData(qint64 cTime, uint sec)
{
    //if(mMapWidget) mMapWidget->clearAisData(cTime, sec);
}

void MainWindow::itfzchxUtilToolCameraRodFocus(uint cameid, int tag)
{
    //if(mMapWidget) mMapWidget->zchxCameraRodFocus(cameid,tag);
}

void MainWindow::itfToolBarZoomIn()
{
    mMapWidget->ScalePlus();
}

void MainWindow::itfToolBarZoomOut()
{
     mMapWidget->ScaleMinus();
}

void MainWindow::itfToolBarRoam()
{
    OnExitEToolAction();
}

void MainWindow::itfToolBarMeasureDistance()
{
    OnDistantcAction();
}

void MainWindow::itfToolBarMeasureArea()
{
    OnMeasureAreaAction();
}

void MainWindow::itfToolBarElecBearLine()
{
    OnDirAngleAction();
}

void MainWindow::itfToolBarDrawArea()
{
    OnWarringZONEAction();
}

void MainWindow::itfToolBarWarringZONESelected()
{
    OnWarringZONESelected();
}

void MainWindow::itfToolBarWarringZONEmove()
{
    OnWarringZONEmove();
}

void MainWindow::itfToolBarWarringZONEmoveCtrlPoint()
{
    OnWarringZONEmoveCtrlPoint();
}

void MainWindow::itfToolBarWarringZONEDelCtrlPoint()
{
    OnWarringZONEDelCtrlPoint();
}

void MainWindow::itfToolBarWarringZONEAddCtrlPoint()
{
    OnWarringZONEAddCtrlPoint();
}

void MainWindow::itfToolBarCoastDataLineAdd()
{
    OnCoastDataLineAdd();
}

void MainWindow::itfToolBarSeabedPipeLineAdd()
{
    OnSeabedPipeLineLineAdd();
}

void MainWindow::itfToolBarStructurePointAdd()
{
    OnStructurePointAdd();
}

void MainWindow::itfToolBarAreaNetAreaAdd()
{
    OnAreaNetZoneAdd();
}

void MainWindow::itfToolBarChannelAreaAdd()
{
    OnChannelAreaAdd();
}

void MainWindow::itfToolBarChannelZONESelected()
{
    OnChannelZONESelected();
}

void MainWindow::itfToolBarChannelZONEmove()
{
    OnChannelZONEMove();
}

void MainWindow::itfToolBarChannelZONEmoveCtrlPoint()
{
    OnChannelZONEMoveCtrlPoint();
}

void MainWindow::itfToolBarChannelZONEDelCtrlPoint()
{
    OnChannelZONEDelCtrlPoint();
}

void MainWindow::itfToolBarChannelZONEAddCtrlPoint()
{
    OnChannelZONEAddCtrlPoint();
}

void MainWindow::itfToolBarMooringAreaAdd()
{
    OnMooringAreaAdd();
}

void MainWindow::itfToolBarMooringZONESelected()
{
    OnMooringZONESelected();
}

void MainWindow::itfToolBarMooringZONEmove()
{
    OnMooringZONEMove();
}

void MainWindow::itfToolBarMooringZONEmoveCtrlPoint()
{
    OnMooringZONEMoveCtrlPoint();
}

void MainWindow::itfToolBarMooringZONEDelCtrlPoint()
{
    OnMooringZONEDelCtrlPoint();
}

void MainWindow::itfToolBarMooringZONEAddCtrlPoint()
{
    OnMooringZONEAddCtrlPoint();
}

void MainWindow::itfToolBarCardMouthAreaAdd()
{
    OnCardMouthAreaAdd();
}

void MainWindow::itfToolBarCardMouthZONESelected()
{
    OnCardMouthZONESelected();
}

void MainWindow::itfToolBarCardMouthZONEmove()
{
    OnCardMouthZONEMove();
}

void MainWindow::itfToolBarCardMouthZONEmoveCtrlPoint()
{
    OnCardMouthZONEMoveCtrlPoint();
}

void MainWindow::itfToolBarCardMouthZONEDelCtrlPoint()
{
    OnCardMouthZONEDelCtrlPoint();
}

void MainWindow::itfToolBarCardMouthZONEAddCtrlPoint()
{
    OnCardMouthZONEAddCtrlPoint();
}

void MainWindow::itfToolBarIslandLineAdd()
{
    OnIslandLineAdd();
}

void MainWindow::itfToolBarIslandLineSelected()
{
    OnIslandLineSelected();
}

void MainWindow::itfToolBarIslandLineMove()
{
    OnIslandLineMove();
}

void MainWindow::itfToolBarIslandLineMoveCtrlPoint()
{
    OnIslandLineMoveCtrlPoint();
}

void MainWindow::itfToolBarIslandLineDelCtrlPoint()
{
    OnIslandLineDelCtrlPoint();
}

void MainWindow::itfToolBarIslandLineAddCtrlPoint()
{
    OnIslandLineAddCtrlPoint();
}

void MainWindow::itfToolBarPickup()
{
    OnPickupAction();
}

void MainWindow::itfToolBarPickPoints()
{
    if(mMapWidget) mMapWidget->setETool2DrawPickPoints();
}

void MainWindow::itfToolBarShowAll()
{
    OnShowAllAction();
}

void MainWindow::itfToolBarLocalMark()
{
    OnLocalMarkAction();
}

void MainWindow::itfToolBarSelectAnRegion()
{
    mMapWidget->selectAnRegion();
}

void MainWindow::itfToolBarLocationMark()
{
    mMapWidget->setLocationMark();
}

void MainWindow::itfToolBarFixedReferencePoint()
{
    mMapWidget->setFixedReferencePoint();
}

void MainWindow::itfToolBarZONESelected()
{
    OnZONESelected();
}

void MainWindow::itfToolBarDrawRadarArea()
{
    mMapWidget->setETool2DrawRadarZONE();
}
void MainWindow::itfToolBarGps()
{
    OnGpsAction();
}

void MainWindow::itfToolBarMapModelChanged(ZCHX::Data::ECDIS_DISPLAY_MODEL mod)
{
    switch (mod) {
    case ZCHX::Data::ECDIS_MODEL_BASE:
        OnSelBaseDisplayStyle();
        break;
    case ZCHX::Data::ECDIS_MODEL_STANDARD:
        OnSelStandardDisplayStyle();
        break;
    case ZCHX::Data::ECDIS_MODEL_ALL:
        OnSelAllDisplayStyle();
        break;
    default:
        OnSelBaseDisplayStyle();
        break;
    }
}

void MainWindow::itfToolBarColorModelChanged(ZCHX::Data::ECDISCOLOR mod)
{
    switch (mod) {
    case ZCHX::Data::ECDIS_COLOR_DAYBLACKBACK:
        OnSelDayBlackBackDisplyStyle();
        break;
    case ZCHX::Data::ECDIS_COLOR_NIGHT:
        OnSelNightDisplyStyle();
        break;
    case ZCHX::Data::ECDIS_COLOR_DAYDUSK:
        OnSelDayDUSKDisplyStyle();
        break;
    case ZCHX::Data::ECDIS_COLOR_DAYWHITEBACK:
        OnSelDayWhiteBackDisplyStyle();
        break;
    case ZCHX::Data::ECDIS_COLOR_DAYBRIGHT:
        OnSelDayBrightDisplyStyle();
        break;
    default:
        OnSelDayBlackBackDisplyStyle();
        break;
    }
}

void MainWindow::itfWarringZoneData4id(int uuid, ZCHX::Data::ITF_WarringZone &info, bool &ok)
{
    std::shared_ptr<MapLayer> layer = LayerMgr->getLayer(ZCHX::LAYER_DEFENCE);
    if(layer){
        WarningZoneElement* ele = dynamic_cast<WarningZoneElement*>(layer->getElement(QString::number(uuid)).get());
        if(ele){
            info = ele->data();
            ok = true;
        }
    }
    ok = false;
}

void MainWindow::itfIslandData4id(int id,  ZCHX::Data::ITF_IslandLine &info, bool &ok)
{
    std::shared_ptr<MapLayer> layer = LayerMgr->getLayer(ZCHX::LAYER_ISLAND);
    if(layer){
        IslandLineElement* ele = dynamic_cast<IslandLineElement*>(layer->getElement(QString::number(id)).get());
        if(ele){
            info = ele->data();
            ok = true;
            return;
        }
    }
    ok = false;
}

void MainWindow::iftSetElementFlashStep(int step)
{
    if(mMapWidget) mMapWidget->setWarnColorAlphaStep(step);
}

void MainWindow::iftSetIsWarningType(bool bWarningType)
{
    mMapWidget->setIsWarningType(bWarningType);
}

void MainWindow::itfAddLayer(std::shared_ptr<MapLayer> layer, std::shared_ptr<MapLayer> parent)
{
    MapLayerMgr::instance()->addLayer(layer, parent);
}

void MainWindow::itfAddLayer(const QString &curLayer, const QString &curDisplayName, bool curVisible, const QString &parentLayer)
{
    MapLayerMgr::instance()->addLayer(curLayer, curDisplayName, curVisible, parentLayer);
}

bool MainWindow::itfContainsLayer(const QString &type) const
{
    return MapLayerMgr::instance()->containsLayer(type);
}

QStringList MainWindow::itfGetLayerList() const
{
    return MapLayerMgr::instance()->getLayerList();
}

std::shared_ptr<MapLayer> MainWindow::itfGetLayer(const QString &type)
{
    return MapLayerMgr::instance()->getLayer(type);
}

const std::list<std::shared_ptr<MapLayer> > &MainWindow::itfGetLayerTree()
{
    return MapLayerMgr::instance()->getLayerTree();
}

void MainWindow::itfUpdateIPCastDeviceList(const QList<ZCHX::Data::IPCastDevice>& list)
{    
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_WIREROD);
    if(!layer) return;
    foreach (ZCHX::Data::IPCastDevice item, list) {
        //获取相机对应的杆
        RodElement *rod = dynamic_cast<RodElement*>(layer->getElement(item.rodID).get());
        if(rod){
            rod->updateIPC(item);
        }
    }
}

void MainWindow::itfSetGPSDataList(std::list<std::shared_ptr<ZCHX::Data::GPSPoint> > list)
{
    return mMapWidget->setGPSDataList(list);
}

void MainWindow::itfSetGPSData(std::shared_ptr<ZCHX::Data::GPSPoint> data)
{
    return mMapWidget->setGPSData(data);
}

void MainWindow::itfClearGPSData()
{
    return mMapWidget->clearGPSData();
}


void MainWindow::setCurrentProjectID(int id)
{
    mMapWidget->setCurrentProjectID(id);
}

void MainWindow::itfSetCableBaseDataList(const QList<ZCHX::Data::CableBaseData> &list)
{
    if(mMapWidget) mMapWidget->setCableBaseDataList(list);
}

void MainWindow::itfSetCableInterfaceDataList(const QList<ZCHX::Data::CableInterfaceData> &list)
{
    if(mMapWidget) mMapWidget->setCableInterfaceDataList(list);
}

void MainWindow::itfSetDisplayRouteIndex(bool display)
{
    if(mMapWidget) mMapWidget->setDisplayRouteIndex(display);
}

void MainWindow::itfSetDisplayRouteAc(bool display)
{
    if(mMapWidget) mMapWidget->setDisplayRouteAc(display);
}

void MainWindow::itfAppendFocusAis(const QStringList &list)
{
    foreach (QString val, list) {
        if(!MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->appendConcern(val)) break;
    }
}

void MainWindow::itfRemoveFocusAis(const QStringList &list)
{
    foreach (QString val, list) {
        MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->removeConcern(val);
    }
}

void MainWindow::itfAppendFocusRadar(const QStringList& list)
{
    foreach (QString val, list) {
        if(!MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR)->appendConcern(val)) break;
    }
}

void MainWindow::itfRemoveFocusRadar(const QStringList& list)
{
    foreach (QString val, list) {
        MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR)->removeConcern(val);
    }
}

void MainWindow::itfAppendRadarTailTrackList(const QStringList &list)
{
    foreach (QString id, list) {
        if(!MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR)->appendRealtimeTrack(id)) break;
    }
}

void MainWindow::itfRemoveRadarTailTrackList(const QStringList &list)
{
    foreach (QString id, list) {
        MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR)->removeRealtimeTrack(id);
    }
}
void MainWindow::itfAppendAisTailTrackList(const QStringList &list)
{
    foreach (QString id, list) {
        if(!MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->appendRealtimeTrack(id)) break;
    }
}

void MainWindow::itfRemoveAisTailTrackList(const QStringList &list)
{
    foreach (QString id, list) {
        MapLayerMgr::instance()->getLayer(ZCHX::LAYER_AIS)->removeRealtimeTrack(id);
    }
}

void MainWindow::itfToolBarCameraNetGridAdd(const QSizeF& size, const QString& camera)
{
    mMapWidget->setETool2DrawCameraNetGrid(size, camera);
}

void MainWindow::itfSetCameraNetGridList(const QList<ZCHX::Data::ITF_NetGrid> & list)
{
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_CAMERANETGRID);
    if(!layer) return;
    QList<std::shared_ptr<Element>> res;
    foreach (ZCHX::Data::ITF_NetGrid item, list) {
        res.append(std::shared_ptr<GridElement>(new GridElement(item, mMapWidget)));
    }
    layer->addElements(res, true);
}

void MainWindow::itfPickUpPTZ()
{
    if(mMapWidget) mMapWidget->setETool2PickUpPTZ();
}

void MainWindow::itfSetZoomLableDisplay(bool display)
{
    if(mMapWidget) mMapWidget->setZoomLableDisplay(display);
}
}

