#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zchxmapthread.h"
#include "zchxmapdownloadthread.h"
#include "map_layer/zchxmaplayermgr.h"
#include "camera_mgr/zchxcameradatasmgr.h"
#include "data_manager/zchxaisdatamgr.h"
#include "zchxuserdefinesdatamgr.h"
#include "zchxradardatamgr.h"
#include "zchxroutedatamgr.h"
#include "zchxshipplandatamgr.h"
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mMapWidget(0)
{
    ui->setupUi(this);
    ui->statusBar->setVisible(false);
    ui->source->addItem(tr("TMS"), TILE_TMS);
    ui->source->addItem(tr("谷歌"), TILE_GOOGLE);
    ui->progressBar->setVisible(false);
    ui->download->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateGridLayout(int x, int y)
{
    if(!mMapWidget) return;
    mMapWidget->clear();
}


void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    if(!mMapWidget)
    {
        mMapWidget = new zchxMapWidget(ui->ecdis_frame);
        connect(mMapWidget, SIGNAL(signalDisplayCurPos(double,double)), this, SLOT(slotUpdateCurrentPos(double,double)));
        connect(mMapWidget, SIGNAL(signalSendNewMap(double,double,int)), this, SLOT(slotDisplayMapCenterAndZoom(double,double,int)));
        initSignalConnect();
    }
    mMapWidget->setGeometry(0, 0, ui->ecdis_frame->size().width(), ui->ecdis_frame->size().height());
    emit itfSignalIsEcdisResize();
}

void MainWindow::slotRecvMapData(const QPixmap &data, int x, int y)
{
    if(!mMapWidget) return;
    mMapWidget->append(data, x, y);
}

void MainWindow::on_load_clicked()
{
    //QRect rect =  ui->ecdis->geometry();
    double lon = ui->lon->text().toDouble();
    double lat = ui->lat->text().toDouble();
    int zoom = ui->zoom->text().toInt();
    if(!mMapWidget) return;
    mMapWidget->setCenterAndZoom(LatLon(lat, lon), zoom);
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
    ui->progressBar->setMaximum(18);
    ui->progressBar->setValue(0);
    zchxMapDownloadThread* thread = new zchxMapDownloadThread;
    connect(thread, SIGNAL(signalDownloadFinished(int)), this, SLOT(slotUpdateDownloadProgress(int)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
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
    if(!mMapWidget) return;
    mMapWidget->setSource(ui->source->currentData().toInt());
}

void MainWindow::on_image_num_clicked(bool checked)
{
    if(!mMapWidget) return;
    mMapWidget->setDisplayImgeNum(checked);
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
    connect(mMapWidget,SIGNAL(signalSendCameraNetGrid(ZCHX::Data::ITF_CameraNetGrid)), this, SIGNAL(itfSignalSendCameraNetGrid(ZCHX::Data::ITF_CameraNetGrid)));
}

void MainWindow::itfSetAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    mMapWidget->getAisDataMgr()->setAisData(data);
}

bool MainWindow::itfSetSingleAisData(QString id, const QList<ZCHX::Data::ITF_AIS> &data)
{
    return mMapWidget->getAisDataMgr()->setSingleAisData(id, data);
}

void MainWindow::itfRemoveAisHistoryData(QString id)
{
    mMapWidget->getAisDataMgr()->removeAisHistoryData(id);
}

void MainWindow::itfSetHistoryAisData(const QList<ZCHX::Data::ITF_AIS> &data)
{
    mMapWidget->getAisDataMgr()->setHistoryAisData(data);
}

void MainWindow::itfSetClearHistoryData(bool states)
{
    mMapWidget->getAisDataMgr()->setClearHistoryData(states);
    //mMapWidget->getRadarDataMgr()->setClearHistoryData(states);
}

void MainWindow::itfSetConsAisData(const ZCHX::Data::ITF_AIS &data)
{
    mMapWidget->getAisDataMgr()->setConsAisData(data);
}

void MainWindow::itfSetRadarEchoData(const QMap<QDateTime, ZCHX::Data::ITF_RadarEchoMap> &data)
{
    //mMapWidget->setRadarEchoData(data);
}
void MainWindow::itfSetRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint> &data)
{
    std::vector<DrawElement::RadarPointElement> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::RadarPointElement item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getRadarDataMgr()->setRadarPointData(list);
}

void MainWindow::itfSetHistoryRadarPointData(const QList<ZCHX::Data::ITF_RadarPoint> &data)
{
    std::vector<DrawElement::RadarPointElement> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::RadarPointElement item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getRadarDataMgr()->setHistoryRadarPointData(list);
}


void MainWindow::itfSetRadarAreaData(const QList<ZCHX::Data::ITF_RadarArea> &data)
{
    std::vector<DrawElement::RadarAreaElement> list;
    for(int i=0; i < data.count(); ++i)
    {
        DrawElement::RadarAreaElement item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getRadarDataMgr()->setRadarAreaData(list);
}

void MainWindow::itfSetCameraRodData(const QList<ZCHX::Data::ITF_CameraRod> &data)
{
    std::vector<DrawElement::CameraRodElement> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::CameraRodElement item(data.at(i));
        item.setIsFocus(false);
        list.push_back(item);
        //        qDebug()<<"mainWindow:"<<data.at(i).ll.lat << data.at(i).ll.lon;
    }
    mMapWidget->getCameraMgr()->setCameraRodData(list);
}

void MainWindow::itfSetCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    std::vector<DrawElement::CameraElement> list;
    for(int i = 0; i < data.count(); ++i)
    {
        DrawElement::CameraElement item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getCameraMgr()->setCameraDevData(list);
}

void MainWindow::itfSetAisCameraDevData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    //qDebug()<<__FUNCTION__<<__LINE__;
    std::vector<DrawElement::CameraElement> list;
    for(int i = 0; i < data.count(); ++i)
    {
        DrawElement::CameraElement item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getCameraMgr()->setCameraDevData(list);
}

void MainWindow::itfSetPastrolStation(const QList<ZCHX::Data::ITF_PastrolStation> &data)
{
    std::vector<DrawElement::PastrolStation> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::PastrolStation item(data.at(i));
        list.push_back(item);
        //        qDebug()<<"mainWindow:"<<data.at(i).ll.lat << data.at(i).ll.lon;
    }
    mMapWidget->getUserDefinesDataMgr()->setPastrolStation(list);
}

void MainWindow::itfSetWarringZoneData(const QList<ZCHX::Data::ITF_WarringZone> &data)
{
    std::vector<DrawElement::WarringZONE> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::WarringZONE item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setWarringZONEData(list);
}

void MainWindow::itfSetShipSiumtionData(const std::vector<std::pair<double, double> > &data)
{
    //mMapWidget->setShipSiumtionData(data);
}

void MainWindow::itfSetAISDataToSiumtion(const QList<ZCHX::Data::ITF_AIS> &data)
{
    //mMapWidget->setAisDataToSiumtion(data);
}

void MainWindow::itfSetCoastData(const QList<ZCHX::Data::ITF_CoastData> &data)
{
    std::vector<DrawElement::CoastData> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::CoastData item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setCoastData(list);
}

void MainWindow::itfImportCoastData(const std::vector<std::pair<double, double> > & data)
{
    mMapWidget->getUserDefinesDataMgr()->importCoastData(data);
}

void MainWindow::itfSetSeabedPipeLine(const QList<ZCHX::Data::ITF_SeabedPipeLine> &data)
{
    std::vector<DrawElement::SeabedPipeLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::SeabedPipeLine item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setSeabedPipeLine(list);
}

void MainWindow::itfImportSeabedPipeLine(const std::vector<std::pair<double, double> > & data)
{
    mMapWidget->getUserDefinesDataMgr()->importSeabedPipeLine(data);
}

void MainWindow::itfSetStructure(const QList<ZCHX::Data::ITF_Structure> &data)
{
    std::vector<DrawElement::Structure> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::Structure item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setStructure(list);
}

void MainWindow::itfImportStructure(const std::pair<double, double> &data)
{
    //mMapWidget->importStructure(data);
}

void MainWindow::itfSetAreaNet(const QList<ZCHX::Data::ITF_AreaNet> &data)
{
    std::vector<DrawElement::AreaNet> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::AreaNet item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setAreaNet(list);
}

void MainWindow::itfImportAreaNet(const std::vector<std::pair<double, double> > &data)
{
    mMapWidget->getUserDefinesDataMgr()->importAreaNet(data);
}

void MainWindow::itfSetChannel(const QList<ZCHX::Data::ITF_Channel> &data)
{
    std::vector<DrawElement::Channel> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::Channel item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setChannel(list);
}

void MainWindow::itfImportChannel(const std::vector<std::pair<double, double> > &data)
{
    mMapWidget->getUserDefinesDataMgr()->importChannel(data);
}

void MainWindow::itfSelectChannelLine(int channelId, const ZCHX::Data::ITF_ChannelLine & line)
{
    mMapWidget->getUserDefinesDataMgr()->selectChannelLine(channelId, line);
}

void MainWindow::itfCancelChannelLine(int channelId)
{
    mMapWidget->getUserDefinesDataMgr()->cancelChannelLine(channelId);
}

void MainWindow::itfSetMooring(const QList<ZCHX::Data::ITF_Mooring> &data)
{
    std::vector<DrawElement::Mooring> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::Mooring item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setMooring(list);
}

void MainWindow::itfImportMooring(const std::vector<std::pair<double, double> > &data)
{
    mMapWidget->getUserDefinesDataMgr()->importMooring(data);
}

void MainWindow::itfSetCardMouth(const QList<ZCHX::Data::ITF_CardMouth> &data)
{
    std::vector<DrawElement::CardMouth> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::CardMouth item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setCardMouth(list);
}

void MainWindow::itfImportCardMouth(const std::vector<std::pair<double, double> > &data)
{
    mMapWidget->getUserDefinesDataMgr()->importCardMouth(data);
}

void MainWindow::itfSetFleet(const QList<ZCHX::Data::ITF_Fleet> &data)
{
    QMap<QString, ZCHX::Data::ITF_Fleet> fleetMap;
    for(int i = 0; i < data.size(); ++i)
    {
        fleetMap.insert(data[i].aisId, data[i]);
    }
    mMapWidget->setFleet(fleetMap);
}

void MainWindow::itfSetShipAlarmAscendMap(const QMap<QString, ZCHX::Data::ITF_ShipAlarmAscend> &shipAlarmAscendMap)
{
    //mMapWidget->setShipAlarmAscendMap(shipAlarmAscendMap);
}

void MainWindow::itfUpdateWarringZone(const QList<ZCHX::Data::ITF_WarringZone> &data)
{
    mMapWidget->getUserDefinesDataMgr()->updateWarrningZone(data);
}

void MainWindow::itfUpdateWarringZone(const ZCHX::Data::ITF_WarringZone &zone)
{
    mMapWidget->getUserDefinesDataMgr()->updateWarrningZone(zone);
}

void MainWindow::removeWarrningZone(const ZCHX::Data::ITF_WarringZone &zone)
{
    mMapWidget->getUserDefinesDataMgr()->removeWarrningZone(zone);
}

QList<ZCHX::Data::ITF_WarringZone> MainWindow::getAllWarrningZone() const
{
    return mMapWidget->getUserDefinesDataMgr()->getAllWarrningZone();
}

void MainWindow::itfSetIslandLineData(const QList<ZCHX::Data::ITF_IslandLine> &data)
{
//    qDebug()<<"接收到环岛线:"<<data.size();
    std::vector<DrawElement::IslandLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::IslandLine item(data[i]);
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setIslandLineData(list);
}

void MainWindow::itfSetLocalMarkData(const QList<ZCHX::Data::ITF_LocalMark> &data)
{
    std::vector<DrawElement::LocalMark> list;
    for(int i=0; i< data.count(); ++i)
    {
        DrawElement::LocalMark item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getUserDefinesDataMgr()->setLocalMarkData(list);
}

void MainWindow::itfSetCameraGdyData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    std::vector<DrawElement::CameraElement> list;
    for(int i = 0; i < data.size(); ++i) {
        DrawElement::CameraElement item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getCameraMgr()->setCameraDevData(list);
}

void MainWindow::itfSetCameraPlanData(const QList<ZCHX::Data::ITF_CameraDev> &data)
{
    std::vector<DrawElement::CameraElement> list;
    for(int i = 0; i < data.size(); ++i) {
        DrawElement::CameraElement item(data.at(i));
        list.push_back(item);
    }
    mMapWidget->getCameraMgr()->setCameraDevData(list);
}

void MainWindow::itfSetDangerousCircleData(const QList<ZCHX::Data::ITF_DangerousCircle> &data)
{
    std::vector<DrawElement::DangerousCircle> list;
    uint uZoom = itfzchxUtilToolCurZoom();
    for(int i = 0; i < data.size(); ++i)
    {
        DrawElement::DangerousCircle item(data.at(i));
        list.push_back(item);
    }

    mMapWidget->getUserDefinesDataMgr()->setDangerousCircleData(list);
}

void MainWindow::itfSetDangerousCircleRange(const double range)
{
    //每一个危险圈设定半径  这里取消
    //mMapWidget->setDangerousCircleRange(range);
}

void MainWindow::itfSetRadarFeatureZoneDagta(const QList<ZCHX::Data::ITF_RadarFeaturesZone> &data)
{
    std::vector<DrawElement::RadarFeatureZone> list;
    for(int i=0; i< data.size(); ++i)
    {
        DrawElement::RadarFeatureZone item(data.at(i));
        list.push_back(item);

    }

    mMapWidget->getRadarDataMgr()->setRadarFeatureZoneData(list);
}

void MainWindow::itfSetRouteLineData(const QList<ZCHX::Data::RouteLine> &data)
{
    std::vector<DrawElement::RouteLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::RouteLine tmp = data.at(i);
        DrawElement::RouteLine item(tmp);
        list.push_back(item);

    }
    mMapWidget->getRouteDataMgr()->setRouteLineData(list);
}

void MainWindow::itfSetALLSpecialRouteLineData(const QList<ZCHX::Data::SpecialRouteLine> &data)
{
    std::vector<DrawElement::SpecialRouteLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::SpecialRouteLine tmp = data.at(i);
        DrawElement::SpecialRouteLine item(tmp);
        list.push_back(item);
    }
    mMapWidget->getRouteDataMgr()->setALLSpecialRouteLineData(list);

}

void MainWindow::itfSetSpacilRouteLineData(const QList<ZCHX::Data::SpecialRouteLine> &data)
{
    std::vector<DrawElement::SpecialRouteLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::SpecialRouteLine tmp = data.at(i);
        DrawElement::SpecialRouteLine item(tmp);
        list.push_back(item);

    }
    mMapWidget->getRouteDataMgr()->setSpacilRouteLineData(list);
}

void MainWindow::itfSetPickUpSpacilRoutePoint(const ZCHX::Data::SpecialRoutePoint &data)
{
    mMapWidget->getRouteDataMgr()->setPickUpSpacilRoutePoint(data);
}

void MainWindow::itfSetHistoryRouteLineData(const QList<ZCHX::Data::RouteLine> &data)
{
    std::vector<DrawElement::RouteLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::RouteLine tmp = data.at(i);
        DrawElement::RouteLine item(tmp);
        list.push_back(item);

    }
    mMapWidget->getRouteDataMgr()->setHistoryRouteLineData(list);
}

void MainWindow::itfSetRouteDefaultCableData(const ZCHX::Data::CableAssembly &data, int uSlack)
{
    mMapWidget->getRouteDataMgr()->setRouteDefaultCableData(data,uSlack);
}

void MainWindow::itfSetRouteConfigDisplay(int uRouteID, bool bShowPoint,bool bShowType,bool bShowLine,bool bShowCable, bool bShowIndex)
{
    //mMapWidget->setRouteConfigDisplay(uRouteID, bShowPoint, bShowType, bShowLine, bShowCable, bShowIndex);
}

void MainWindow::itfSetRouteDisplayFlag(int uRouteID, int uDisplayFlag)
{
    //mMapWidget->setRouteDisplayFlag(uRouteID,uDisplayFlag);
}

void MainWindow::itfCloseCurRouteLineData()
{
    //mMapWidget->closeCurRouteLineData();
}

void MainWindow::itfDeleteRouteLineDataFromProjectID(const int uProjectID)
{
    //mMapWidget->deleteRouteLineDataFromProjectID(uProjectID);
}

void MainWindow::itfSetShipPlanLineData(const QList<ZCHX::Data::ShipPlanLine> &data)
{
    std::vector<DrawElement::ShipPlanLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::ShipPlanLine tmp = data.at(i);
        DrawElement::ShipPlanLine item(tmp);
        list.push_back(item);

    }
    mMapWidget->getShipPlanDataMgr()->setShipPlanLineData(list);
}

void MainWindow::itfSetAllProjectShipPlanLineData(const QList<ZCHX::Data::ShipPlanLine> &data)
{
    std::vector<DrawElement::ShipPlanLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::ShipPlanLine tmp = data.at(i);
        DrawElement::ShipPlanLine item(tmp);
        list.push_back(item);
    }

    mMapWidget->getShipPlanDataMgr()->setAllProjecrShipPlanLineData(list);
}

void MainWindow::itfSetShipPlanProjectID(const int ProjectID)
{
    //路由的时候已经传入工程ID了
}

void MainWindow::itfSetRouteLinePickupLocation(const int uRouteID, const double dLat, const double dLon)
{
    //mMapWidget->setRouteLinePickupLocation(uRouteID,dLat,dLon);
}

void MainWindow::itfSetRouteLinePickupInfor(const double dKp, bool bShowLength, const double dLength, bool bShowSlope, const double dSlope)
{
    //mMapWidget->setRouteLinePickupInfor(dKp,bShowLength,dLength,bShowSlope,dSlope);
}

void MainWindow::itfChangeCurrentRouteLine(const ZCHX::Data::RouteLine &data, int uRouteID)
{
    //mMapWidget->changeCurrentRouteLine(data,uRouteID);
}

void MainWindow::itfChangeRoutePathAssessStatus(int uAssessStatus, int uRouteID)
{
    //mMapWidget->changeRoutePathAssessStatus(uAssessStatus,uRouteID);
}

void MainWindow::itfSetSimulateData(int uSimulateDataID, int uSimulateFlag)
{
    //mMapWidget->setSimulateData(uSimulateDataID,uSimulateFlag);
}

void MainWindow::itfSetSimulateLocation(float fCurSimulateKP)
{
    //mMapWidget->setSimulateLocation(fCurSimulateKP);
}

void MainWindow::itfSetCameraObservationZoneData(const QList<ZCHX::Data::ITF_CameraObservationZone> &data)
{
    std::vector<DrawElement::CameraObservationZone> list;
    for(int i=0; i< data.size(); ++i)
    {
        DrawElement::CameraObservationZone item(data.at(i));
        list.push_back(item);
    }

    mMapWidget->getCameraMgr()->setCameraObservationZoneData(list);
}

void MainWindow::itfSetRadarVideoData(double dCentreLon, double dCentreLat, double dDistance, int uType, int uLoopNum)
{

    //mMapWidget->setRadarVideoData(dCentreLon,dCentreLat,dDistance,uType,uLoopNum);
}

void MainWindow::itfSetRadarVideoPixmap(int uIndex, const QPixmap &objPixmap, const QPixmap &prePixmap)
{
    //mMapWidget->setRadarVideoPixmap(uIndex,objPixmap,prePixmap);
}

void MainWindow::itfSetCurrentRadarVideoPixmap(const QPixmap &objPixmap)
{
    //mMapWidget->setCurrentRadarVideoPixmap(objPixmap);
}

//void MainWindow::itfSetMultibeamData(const QList<ZCHX::Data::ITF_Multibeam> &data, const double dMinLon, const double dMinLat, const double dMaxLon, const double dMaxLat, const double dMinX, const double dMinY, const double dMinZ, const double dMaxX, const double dMaxY, const double dMaxZ)
//{
//    std::vector<DrawElement::Multibeam> list;
//    for(int i=0; i< data.count(); ++i)
//    {
//        ZCHX::Data::ITF_Multibeam tmp = data.at(i);
//        DrawElement::Multibeam item(tmp);
//        list.push_back(item);
//    }

//    mMapWidget->setMultibeamData(list,dMinLon,dMinLat,dMaxLon,dMaxLat,dMinX,dMinY,dMinZ,dMaxX,dMaxY,dMaxZ);
//}

void MainWindow::itfSetMultibeamDataByImg(int iRouteID, const QPixmap &samplePixmap, const QPixmap &objPixmap, double dMinLon, double dMinLat, double dMaxLon, double dMaxLat)
{
    //mMapWidget->setMultibeamDataByImg(iRouteID, samplePixmap, objPixmap, dMinLon, dMinLat, dMaxLon, dMaxLat);
}

void MainWindow::itfSetAllProjectRouteLineData(const QList<ZCHX::Data::RouteLine> &data)
{

    std::vector<DrawElement::RouteLine> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::RouteLine tmp = data.at(i);
        DrawElement::RouteLine item(tmp);
        list.push_back(item);

    }

    mMapWidget->getRouteDataMgr()->setAllProjectRouteLineData(list);

}

void MainWindow::itfSetRouteCrossData(const QList<ZCHX::Data::ITF_RouteCross> &data)
{
    std::vector<DrawElement::RouteCross> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::ITF_RouteCross tmp = data.at(i);
        DrawElement::RouteCross item(tmp);
        list.push_back(item);

    }

    mMapWidget->getRouteDataMgr()->setRouteCrossData(list);
}
void MainWindow::itfSetCameraVideoWarnData(const QList<ZCHX::Data::ITF_CameraVideoWarn> &data)
{

    std::vector<DrawElement::CameraVideoWarn> list;

    for(int i=0; i< data.size(); ++i)
    {

        DrawElement::CameraVideoWarn item(data.at(i));

        list.push_back(item);

    }

    mMapWidget->getCameraMgr()->setCameraVideoWarnData(list);

}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleEllipse &item)
{
    //mMapWidget->appendElementItem(item);
}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleTriangle &item)
{
    //mMapWidget->appendElementItem(item);
}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleLine &item)
{
    //mMapWidget->appendElementItem(item);
}

void MainWindow::itfAppendElementItem(const ZCHX::Data::ITF_EleRect &item)
{
    //mMapWidget->appendElementItem(item);
}

void MainWindow::itfRemoveElementItem(DrawElement::Element *pitem)
{
    //mMapWidget->removeElementItem(pitem);
}

void MainWindow::itfClearElementData()
{
    //mMapWidget->clearElementData();
}


void MainWindow::itfSetPickUpRadarInfo(qint32 tracknumber)
{
    //mMapWidget->SetPickUpRadarInfo(tracknumber);
}

void MainWindow::itfSetPickUpAisInfo(QString id)
{
    mMapWidget->getAisDataMgr()->SetPickUpAisInfo(id);
}

void MainWindow::itfSetPickUpPosition(QString id)
{
    //mMapWidget->SetPickUpPosition(id);
}

void MainWindow::itfSetPickUpRouteCross(const int id)
{
    //mMapWidget->SetPickUpRouteCross(id);
}

void MainWindow::itfSetShipSimulationExtrapolationDatas(const QList<ZCHX::Data::ITF_AIS> &DataList)
{
    //mMapWidget->setShipSimulationExtrapolationDatas(DataList);
}

void MainWindow::itfSetRealTimeShipTrailDatas(const QMap<QString, QList<ZCHX::Data::ITF_AIS> > &DataList)
{
    //mMapWidget->setRealTimeShipTrailDatas(DataList);
}

void MainWindow::itfSetRadarTailTrackList(const QList<int> &numbers)
{
    //mMapWidget->setRadarTailTracklist(numbers);
}

void MainWindow::itfSetRouteHistogramData(const int RouteId, const QMap<int, QList<int> > &DataList)
{
    //mMapWidget->setRouteHistogramData(RouteId,DataList);
}

void MainWindow::itfSetRealTimeFlowLineHistogramData(const QMap<int, QList<int> > &DataList)
{
    //mMapWidget->setRealTimeFlowLineHistogramData(DataList);
}

void MainWindow::itfSetInTheAreaOfTrackData(const QList<QList<ZCHX::Data::LatLon> > &data)
{
    //mMapWidget->setInTheAreaOfTrackData(data);
}

void MainWindow::itfSetRolePrivilegeList(const QList<int> &list)
{

    //mMapWidget->setRolePrivilegeList(list);
}
void MainWindow::itfSetIsHideMap(bool b)
{
    mMapWidget->setIsHideMap(b);
}

void MainWindow::itfSetNavigateionging(bool b)
{
    mMapWidget->setIsNavigation(b);
}

void MainWindow::itfSetIsOpenMeet(bool b)
{
    mMapWidget->setIsOpenMeet(b);
}

void MainWindow::itfSetPuickupType(const ZCHX::Data::ECDIS_PICKUP_TYPE &type)
{
    mMapWidget->setCurPickupType(type);
}

void MainWindow::itfUpdateCameraDevStatus(uint cameid, uint st)
{
    //mMapWidget->zchxUpdateCameraDevStatus(cameid, st);
}
void MainWindow::itfSetRadarTrackPoint(bool b)
{
    mMapWidget->setEnableRadarTrackPoint(b);
}

void MainWindow::itfSetEnableCameraTargerTrack(bool b)
{
    mMapWidget->setEnableCameracombinedTrack(b);
}

void MainWindow::itfSetEnableRouteHistogram(bool b)
{
    mMapWidget->setEnableRouteHistogram(b);
}

void MainWindow::itfSetEnableShipTag(const QList<bool> &bList)
{
    //mMapWidget->SetEnableShipTag(bList);
}

void MainWindow::itfSetEnableTooBarVisable(bool visible)
{
    //d->m_toolbar->setVisible(visible);
}

void MainWindow::itfsetPluginUseModel(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL &mod)
{
    mMapWidget->setCurPluginUserModel(mod);
}

void MainWindow::itfsetSelectedShip()
{
    //mMapWidget->setETool2DrawTrackTarget();
}

void MainWindow::itfsetSelectedAllTrack()
{
    //mMapWidget->setETool2DrawCameraTrackTarget();
}

void MainWindow::itfsetSelectedRouteOrCross()
{
    //mMapWidget->setETool2DrawRouteOrCross();
}

double MainWindow::itfzchxUtilToolAngle4north()
{
    return mMapWidget->zchxUtilToolAngle4north();
}

void MainWindow::itfzchxUtilToolSetAngle4north(double ang)
{
//    qDebug()<<"MainWindow::itfzchxUtilToolSetAngle4north 当前角度:" << ang;
    mMapWidget->zchxUtilToolSetAngle4north(ang);
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
    mMapWidget->setMapUnit(unit);
}

void MainWindow::setIsShowStatusBar(bool bShow)
{
    statusBar()->setVisible(bShow);
}

std::shared_ptr<DrawElement::Element> MainWindow::getCurrentSelectedElement()
{
    return mMapWidget->getCurrentSelectedElement();
}

void MainWindow::itfChannelZoneData4id(int uuid, ZCHX::Data::tagITF_Channel &info, bool &ok)
{
    ok = mMapWidget->getUserDefinesDataMgr()->zchxChannelZoneData4id(uuid, info);
}

void MainWindow::itfMooringZoneData4id(int uuid, ZCHX::Data::tagITF_Mooring &info, bool &ok)
{
    ok = mMapWidget->getUserDefinesDataMgr()->zchxMooringZoneData4id(uuid, info);
}

void MainWindow::itfCardMouthZoneData4id(int uuid, ZCHX::Data::tagITF_CardMouth &info, bool &ok)
{
    ok = mMapWidget->getUserDefinesDataMgr()->zchxCardMouthZoneData4id(uuid, info);
}

bool MainWindow::getIsAtiAdaptor() const
{
    return true;
    //return mMapWidget->getIsAtiAdaptor();
}

void MainWindow::setIsAtiAdaptor(bool isAtiAdaptor)
{
    //mMapWidget->setIsAtiAdaptor(isAtiAdaptor);
}

void MainWindow::setUseRightKey(bool bUseRightKey)
{
    mMapWidget->setUseRightKey(bUseRightKey);
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
    //mMapWidget->setETool2DrawArea();
}

/**
 * @brief MainWindow::OnDirAngleAction
 * @anthor:zxl
 * draw direct angle
 */
void MainWindow::OnDirAngleAction()
{
    //mMapWidget->setETool2DrawDirAngle();
}

void MainWindow::OnDistantcAction()
{
    //mMapWidget->setETool2DrawDistance();
}

void MainWindow::OnExitEToolAction()
{
    //mMapWidget->releaseDrawStatus();
}

void MainWindow::OnWarringZONEAction()
{
    //mMapWidget->setETool2DrawZONE();
}

void MainWindow::OnWarringZONESelected()
{
    //mMapWidget->setETool2SelectZONE();
}

void MainWindow::OnWarringZONEmove()
{
    //mMapWidget->setETool2moveZONE();
}

void MainWindow::OnWarringZONEmoveCtrlPoint()
{
    //mMapWidget->setETool2ctrlZONE();
}

void MainWindow::OnWarringZONEDelCtrlPoint()
{
    //mMapWidget->setETool2delCtrlZONE();
}

void MainWindow::OnWarringZONEAddCtrlPoint()
{
    //mMapWidget->setETool2addCtrlZONE();
}

void MainWindow::OnCoastDataLineAdd()
{
    //mMapWidget->setETool2Draw4CoastDataLine();
}

void MainWindow::OnSeabedPipeLineLineAdd()
{
    //mMapWidget->setETool2Draw4SeabedPipeLineLine();
}

void MainWindow::OnStructurePointAdd()
{
    //mMapWidget->setETool2Draw4StructurePoint();
}

void MainWindow::OnAreaNetZoneAdd()
{
    //mMapWidget->setETool2Draw4AreaNetZone();
}

void MainWindow::OnChannelAreaAdd()
{
    //mMapWidget->setETool2Draw4ChannelArea();
}

void MainWindow::OnChannelZONESelected()
{
    //mMapWidget->setETool2SelectChannel();
}

void MainWindow::OnChannelZONEMove()
{
    //mMapWidget->setETool2moveChannel();
}

void MainWindow::OnChannelZONEMoveCtrlPoint()
{
    //mMapWidget->setETool2ctrlChannel();
}

void MainWindow::OnChannelZONEDelCtrlPoint()
{
    //mMapWidget->setETool2delCtrlChannel();
}

void MainWindow::OnChannelZONEAddCtrlPoint()
{
    //mMapWidget->setETool2addCtrlChannel();
}

void MainWindow::OnMooringAreaAdd()
{
    //mMapWidget->setETool2Draw4MooringArea();
}

void MainWindow::OnMooringZONESelected()
{
    //mMapWidget->setETool2SelectMooring();
}

void MainWindow::OnMooringZONEMove()
{
    //mMapWidget->setETool2moveMooring();
}

void MainWindow::OnMooringZONEMoveCtrlPoint()
{
    //mMapWidget->setETool2ctrlMooring();
}

void MainWindow::OnMooringZONEDelCtrlPoint()
{
    //mMapWidget->setETool2delCtrlMooring();
}

void MainWindow::OnMooringZONEAddCtrlPoint()
{
    //mMapWidget->setETool2addCtrlMooring();
}

void MainWindow::OnCardMouthAreaAdd()
{
    //mMapWidget->setETool2Draw4CardMouthArea();
}

void MainWindow::OnCardMouthZONESelected()
{
    //mMapWidget->setETool2SelectCardMouth();
}

void MainWindow::OnCardMouthZONEMove()
{
    //mMapWidget->setETool2moveCardMouth();
}

void MainWindow::OnCardMouthZONEMoveCtrlPoint()
{
    //mMapWidget->setETool2ctrlCardMouth();
}

void MainWindow::OnCardMouthZONEDelCtrlPoint()
{
    //mMapWidget->setETool2delCtrlCardMouth();
}

void MainWindow::OnCardMouthZONEAddCtrlPoint()
{
    //mMapWidget->setETool2addCtrlCardMouth();
}

void MainWindow::OnIslandLineAdd()
{
    //mMapWidget->setETool2Draw4IslandLine();
}

void MainWindow::OnIslandLineSelected()
{
    //mMapWidget->setETool2Select4IslandLine();
}

void MainWindow::OnIslandLineMove()
{
    //mMapWidget->setETool2move4IslandLine();
}

void MainWindow::OnIslandLineMoveCtrlPoint()
{
    //mMapWidget->setETool2moveCtrlPoint4IslandLine();
}

void MainWindow::OnIslandLineDelCtrlPoint()
{
    //mMapWidget->setETool2delCtrlPoint4IslandLine();
}

void MainWindow::OnIslandLineAddCtrlPoint()
{
    //mMapWidget->setETool2addCtrlPoint4IslandLine();
}

void MainWindow::itfToolBarShipPlanLineAdd()
{
    //mMapWidget->setETool2DrawShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineSelected()
{
     //mMapWidget->setETool2SelectShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineInsertCtrlPoint()
{
    //mMapWidget->setETool2insertCtrlPointShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineMoveCtrlPoint()
{
    //mMapWidget->setETool2moveCtrlPointShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineDelCtrlPoint()
{
    //mMapWidget->setETool2delCtrlPointShipPlanLine();
}

void MainWindow::itfToolBarShipPlanLineAddCtrlPoint()
{
    //mMapWidget->setETool2addCtrlPointShipPlanLine();
}

void MainWindow::itfSetShipPlanWaterDepth(QList<ZCHX::Data::ITF_WaterDepth> &WaterData)
{
    //mMapWidget->setShipPlanWaterDepth(WaterData);
}

void MainWindow::itfSetHistoryTrackStyle(const QString &color, const int lineWidth)
{
   //mMapWidget->setHistoryTrackStyle(color, lineWidth);
}

void MainWindow::itfSetPrepushTrackStyle(const QString &color, const int lineWidth)
{
    //mMapWidget->setPrepushTrackStyle(color, lineWidth);
}

void MainWindow::itfSetMapUnit(const MapUnit& uint)
{
    mMapWidget->setMapUnit(uint);
}

void MainWindow::OnPickupAction()
{
    mMapWidget->setETool2DrawPickup();
    mMapWidget->setCurPickupType(ZCHX::Data::ECDIS_PICKUP_ALL);
}

void MainWindow::OnShowAllAction()
{
    //mMapWidget->ShowAll();
}

void MainWindow::OnLocalMarkAction()
{
    //mMapWidget->setETool2DrawLocalMark();
}

void MainWindow::OnGpsAction()
{
    //mMapWidget->setETool2DrawGps();
}

void MainWindow::OnSelBaseDisplayStyle()
{
    //mMapWidget->setMapStyle(MapStyleBase);
}

void MainWindow::OnSelStandardDisplayStyle()
{
    //mMapWidget->SetMapStyle(MapStyleStandard);
}

void MainWindow::OnSelAllDisplayStyle()
{
    //mMapWidget->SetMapStyle(MapStyleAll);
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
    //mMapWidget->setETool2SelectCommonZONE();
}

void MainWindow::OnSetMapAngle(double angle)
{
    itfzchxUtilToolSetAngle4north(angle);
}

void MainWindow::setMapCenterAndZoom(double lat, double lon, int zoom)
{
    mMapWidget->setCenterAndZoom(LatLon(lat, lon), zoom);
}

void MainWindow::setMapCenterAndRotate(double lat, double lon, double ang)
{
    mMapWidget->setMapRotateAtLL(lat, lon, ang * 180 / GLOB_PI);
}

void MainWindow::setMapCenterAndRotate_new(double lat, double lon, double ang)
{
    mMapWidget->setMapRotateAtLL(lat, lon, ang);
}

void MainWindow::resetMapRotate(double lat, double lon)
{
    mMapWidget->resetMapRotate(lat,lon);
}

void MainWindow::setMapCenter(double lat, double lon)
{
    mMapWidget->setCenterAndZoom(LatLon(lat, lon), -1);
}

void MainWindow::resetMap()
{
    mMapWidget->reset();
}

void MainWindow::zchxSetStyleAutoChange(bool val)
{
    mMapWidget->setStyleAutoChange(val);
}

void MainWindow::zchxSetGISDayBrightTime(const QTime &t)
{
    mMapWidget->setGISDayBrightTime(t);
}

void MainWindow::zchxSetGISDuskTime(const QTime &t)
{
    mMapWidget->setGISDuskTime(t);
}

void MainWindow::zchxSetGISNightTime(const QTime &t)
{
    mMapWidget->setGISNightTime(t);
}

void MainWindow::zchxClearAisData(qint64 cTime, uint sec)
{
    //mMapWidget->clearAisData(cTime, sec);
}

void MainWindow::itfzchxUtilToolCameraRodFocus(uint cameid, int tag)
{
    //mMapWidget->zchxCameraRodFocus(cameid,tag);
}

void MainWindow::itfToolBarZoomIn()
{
    //mMapWidget->ScalePlus();
}

void MainWindow::itfToolBarZoomOut()
{
    //mMapWidget->ScaleMinus();
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
    //mMapWidget->selectAnRegion();
}

void MainWindow::itfToolBarLocationMark()
{
    //mMapWidget->setLocationMark();
}

void MainWindow::itfToolBarFixedReferencePoint()
{
    //mMapWidget->setFixedReferencePoint();
}

void MainWindow::itfToolBarZONESelected()
{
    OnZONESelected();
}

void MainWindow::itfToolBarDrawRadarArea()
{
    //mMapWidget->setETool2DrawRadarZONE();
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
    ok = mMapWidget->getUserDefinesDataMgr()->zchxWarringZoneData4id(uuid, info);
}

bool MainWindow::itfWarringZoneDataByName(const QString &name, ZCHX::Data::ITF_WarringZone &info)
{
    return mMapWidget->getUserDefinesDataMgr()->zchxWarringZoneDataByName(name, info);
}

bool MainWindow::itfWarringZoneDataByName(const std::string &name, ZCHX::Data::ITF_WarringZone &info)
{
    return mMapWidget->getUserDefinesDataMgr()->zchxWarringZoneDataByName(name, info);
}

void MainWindow::itfIslandData4id(int uuid, ZCHX::Data::ITF_IslandLine &info, bool &ok)
{
    ok = mMapWidget->getUserDefinesDataMgr()->zchxIslandData4id(uuid, info);
}

void MainWindow::iftSetElementFlashStep(int step)
{
    if(mMapWidget) mMapWidget->setWarnColorAlphaStep(step);
}

void MainWindow::iftSetIsWarningType(bool bWarningType)
{
    //mMapWidget->setIsWarningType(bWarningType);
}

void MainWindow::itfAddLayer(std::shared_ptr<zchxMapLayer> layer, std::shared_ptr<zchxMapLayer> parent)
{
    zchxMapLayerMgr *mgr = mMapWidget->getMapLayerMgr();
    if(!mgr) return;
    mgr->addLayer(layer, parent);
}

bool MainWindow::itfContainsLayer(const QString &type) const
{
    zchxMapLayerMgr *mgr = mMapWidget->getMapLayerMgr();
    if(!mgr) return false;
    return mgr->containsLayer(type);
}

QStringList MainWindow::itfGetLayerList() const
{
    zchxMapLayerMgr *mgr = mMapWidget->getMapLayerMgr();
    if(!mgr) return QStringList();
    return mgr->getLayerList();
}

std::shared_ptr<zchxMapLayer> MainWindow::itfGetLayer(const QString &type)
{
    zchxMapLayerMgr *mgr = mMapWidget->getMapLayerMgr();
    if(!mgr) return 0;
    return mgr->getLayer(type);
}

const std::list<std::shared_ptr<zchxMapLayer> > &MainWindow::itfGetLayerTree()
{
    zchxMapLayerMgr *mgr = mMapWidget->getMapLayerMgr();
    if(!mgr) return std::list<std::shared_ptr<zchxMapLayer> >();
    return mgr->getLayerTree();
}

void MainWindow::itfUpdateIPCastDeviceList(std::list<std::shared_ptr<ZCHX::Data::IPCastDevice> > list)
{
    zchxCameraDatasMgr *mgr = mMapWidget->getCameraMgr();
    if(mgr)
    {
        mgr->updateIPCastDeviceList(list);
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
    //mMapWidget->setCurrentProjectID(id);
}

void MainWindow::itfSetCableBaseDataList(const QList<ZCHX::Data::CableBaseData> &list)
{
    mMapWidget->setCableBaseDataList(list);
}

void MainWindow::itfSetCableInterfaceDataList(const QList<ZCHX::Data::CableInterfaceData> &list)
{
    mMapWidget->setCableInterfaceDataList(list);
}

void MainWindow::itfSetDisplayRouteIndex(bool display)
{
    mMapWidget->setDisplayRouteIndex(display);
}

void MainWindow::itfSetDisplayRouteAc(bool display)
{
    mMapWidget->setDisplayRouteAc(display);
}

void MainWindow::itfAppendFocusAis(const QStringList &list)
{
    //mMapWidget->appendFocusAis(list);
}

void MainWindow::itfRemoveFocusAis(const QStringList &list)
{
    //mMapWidget->removeFocusAis(list);
}

void MainWindow::itfAppendFocusRadar(const QList<int>& list)
{
    //mMapWidget->appendFocusRadarPoint(list);
}

void MainWindow::itfRemoveFocusRadar(const QList<int>& list)
{
    //mMapWidget->removeFocusRadarPoint(list);
}

void MainWindow::itfToolBarCameraNetGridAdd(const QSizeF& size, const QString& camera)
{
    //mMapWidget->setETool2DrawCameraNetGrid(size, camera);
}

void MainWindow::itfSetCameraNetGridList(const QList<ZCHX::Data::ITF_CameraNetGrid> & list)
{
    //mMapWidget->setCameraNetGridList(list);
}

