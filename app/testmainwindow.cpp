#include "testmainwindow.h"
#include "ui_testmainwindow.h"
#include <QDateTime>
#include "qt/zchxutils.hpp"

TestMainWindow::TestMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestMainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(new QVBoxLayout);
    m_pEcdisWin = new qt::MainWindow;
    ui->centralwidget->layout()->addWidget(m_pEcdisWin);

    //添加防区图层
    std::shared_ptr<qt::MapLayer> warningZoneLayer(new qt::MapLayer(qt::ZCHX::LAYER_DEFENCE, qt::ZCHX::TR_LAYER_DEFENCE, true));
    m_pEcdisWin->itfAddLayer(warningZoneLayer);

    //添加船舶计划航线图层
    std::shared_ptr<qt::MapLayer> pShipPlanLineLayer(new qt::MapLayer(qt::ZCHX::LAYER_SHIPPLAN_LINE,qt::ZCHX::TR_LAYER_SHIPPLAN_LINE,true));
    m_pEcdisWin->itfAddLayer(pShipPlanLineLayer);

    //经纬网格
    std::shared_ptr<qt::MapLayer> pLonlatGirdLayer(new qt::MapLayer(qt::ZCHX::LAYER_LONLAT_GIRD,qt::ZCHX::TR_LAYER_LONLAT_GIRD,false));
    m_pEcdisWin->itfAddLayer(pLonlatGirdLayer);

    //ais
    std::shared_ptr<qt::MapLayer> pAisLayer(new qt::MapLayer(qt::ZCHX::LAYER_AIS,qt::ZCHX::TR_LAYER_AIS,true));
    m_pEcdisWin->itfAddLayer(pAisLayer);

    std::shared_ptr<qt::MapLayer> pAisCurrentLayer(new qt::MapLayer(qt::ZCHX::LAYER_AIS_CURRENT,qt::ZCHX::TR_LAYER_AIS_CURRENT,true));
    m_pEcdisWin->itfAddLayer(pAisCurrentLayer);

    std::shared_ptr<qt::MapLayer> pAisTrackLayer(new qt::MapLayer(qt::ZCHX::LAYER_AIS_TRACK,qt::ZCHX::TR_LAYER_AIS_TRACK,true));
    m_pEcdisWin->itfAddLayer(pAisTrackLayer);

    //雷达
    std::shared_ptr<qt::MapLayer> pRadarLayer(new qt::MapLayer(qt::ZCHX::LAYER_RADAR,qt::ZCHX::TR_LAYER_RADAR,true));
    m_pEcdisWin->itfAddLayer(pRadarLayer);

    std::shared_ptr<qt::MapLayer> pRadarCurrentLayer(new qt::MapLayer(qt::ZCHX::LAYER_RADAR_CURRENT,qt::ZCHX::TR_LAYER_RADAR_CURRENT,true));
    m_pEcdisWin->itfAddLayer(pRadarCurrentLayer);

    std::shared_ptr<qt::MapLayer> pRadarTrackLayer(new qt::MapLayer(qt::ZCHX::LAYER_RADAR_TRACK,qt::ZCHX::TR_LAYER_RADAR_TRACK,true));
    m_pEcdisWin->itfAddLayer(pRadarTrackLayer);

    std::shared_ptr<qt::MapLayer> pNavigationMark(new qt::MapLayer(qt::ZCHX::LAYER_NAVIGATION_MARK,qt::ZCHX::TR_LAYER_NAVIGATION_MARK,true));
    m_pEcdisWin->itfAddLayer(pNavigationMark);

    std::shared_ptr<qt::MapLayer> pTowerRod(new qt::MapLayer(qt::ZCHX::LAYER_TOWER_ROD,qt::ZCHX::TR_LAYER_TOWER_ROD,true));
    m_pEcdisWin->itfAddLayer(pTowerRod);

    //雷达回波
    std::shared_ptr<qt::MapLayer> pRadarVideo(new qt::MapLayer(qt::ZCHX::LAYER_RADARVIDEO,qt::ZCHX::TR_LAYER_RADARVIDEO,true));
    m_pEcdisWin->itfAddLayer(pRadarVideo);

    //历史雷达和AIS
    std::shared_ptr<qt::MapLayer> pHistoryAis(new qt::MapLayer(qt::ZCHX::LAYER_HISTORY_AIS,qt::ZCHX::TR_LAYER_HISTORY_AIS,true));
    m_pEcdisWin->itfAddLayer(pHistoryAis);

    std::shared_ptr<qt::MapLayer> pHistoryRadar(new qt::MapLayer(qt::ZCHX::LAYER_HISTORY_RADAR,qt::ZCHX::TR_LAYER_HISTORY_RADAR,true));
    m_pEcdisWin->itfAddLayer(pHistoryRadar);

    //添加航道图层
    std::shared_ptr<qt::MapLayer> channelLayer(new qt::MapLayer(qt::ZCHX::LAYER_CHANNEL, qt::ZCHX::TR_LAYER_CHANNEL, true));
    m_pEcdisWin->itfAddLayer(channelLayer);

    //添加锚泊图层
    std::shared_ptr<qt::MapLayer> mooringLayer(new qt::MapLayer(qt::ZCHX::LAYER_MOORING, qt::ZCHX::TR_LAYER_MOORING, true));
    m_pEcdisWin->itfAddLayer(mooringLayer);

    //添加自定义区域图层
    std::shared_ptr<qt::MapLayer> defineZoneLayer(new qt::MapLayer(qt::ZCHX::LAYER_DEFINEZONE, qt::ZCHX::TR_LAYER_DEFINEZONE, true));
    m_pEcdisWin->itfAddLayer(defineZoneLayer);

    //添加卡口图层
    std::shared_ptr<qt::MapLayer> cardMouthLayer(new qt::MapLayer(qt::ZCHX::LAYER_CARDMOUTH, qt::ZCHX::TR_LAYER_CARDMOUTH, true));
    m_pEcdisWin->itfAddLayer(cardMouthLayer);

    //添加船舶预警图层
    std::shared_ptr<qt::MapLayer> shipAlarmLayer(new qt::MapLayer(qt::ZCHX::LAYER_ALARMASCEND, qt::ZCHX::TR_LAYER_ALARMASCEND, true));
    m_pEcdisWin->itfAddLayer(shipAlarmLayer);

    //添加危险圈图层
    std::shared_ptr<qt::MapLayer> dangerLayer(new qt::MapLayer(qt::ZCHX::LAYER_DANGEROUS_CIRLE, qt::ZCHX::TR_LAYER_DANGEROUS_CIRLE, true));
    m_pEcdisWin->itfAddLayer(dangerLayer);

    //本地标注
    std::shared_ptr<qt::MapLayer> localMarkLayer(new qt::MapLayer(qt::ZCHX::LAYER_LOCALMARK, qt::ZCHX::TR_LAYER_LOCALMARK, true));
    m_pEcdisWin->itfAddLayer(localMarkLayer);
    //相机图层
    std::shared_ptr<qt::MapLayer> cameraMarkLayer(new qt::MapLayer(qt::ZCHX::LAYER_CAMERA, qt::ZCHX::TR_LAYER_CAMERA, true));
    m_pEcdisWin->itfAddLayer(cameraMarkLayer);
    //相机网格图层
    std::shared_ptr<qt::MapLayer> cameraGridLayer(new qt::MapLayer(qt::ZCHX::LAYER_CAMERANETGRID, qt::ZCHX::TR_LAYER_CAMERANETGRID, true));
    m_pEcdisWin->itfAddLayer(cameraGridLayer);

    mTestTimer = new QTimer;
    mTestTimer->setInterval(3000);
    connect(mTestTimer, SIGNAL(timeout()), this, SLOT(slotTimerout()));
    mTestTimer->setSingleShot(true);
    mTestTimer->start();
}

TestMainWindow::~TestMainWindow()
{
    delete ui;
}

void TestMainWindow::slotTimerout()
{
    //Lat=20.123456780000001
    //Lon=110.12345678
    qsrand(QDateTime::currentDateTime().toTime_t());
    QList<qt::ZCHX::Data::tagITF_RadarPoint> radarList;
    for(int i=0; i<10; i++)
    {
//        qt::ZCHX::Data::ITF_AIS data;
//        data.mmsi = i*100+1000;
//        data.id = QString("AIS_%1").arg(data.mmsi);
//        data.lat =  20.123456 + qrand() % 1000 * 0.0001;
//        data.lon =  110.12345678 + qrand() % 1000 * 0.0001;
//        data.type = qt::RADARSHIP;
//        std::vector<std::pair<double, double> > path;
//        path.push_back(std::pair<double, double>(data.lat, data.lon));
//        data.setPath(path);
//        aislist.append(data);
        qt::ZCHX::Data::tagITF_RadarPoint item;
        //构建雷达的尾迹
//        qDebug()<<"track point size:"<<data.tracks().tracks_size();
        double lat =  22.223456 + qrand() % 1000 * 0.0001;
        double lon =  113.08345678 + qrand() % 1000 * 0.0001;
        item.path.push_back(std::pair<double,double>(lat, lon));
        item.lat = lat;
        item.lon = lon;
        item.trackNumber = i*100+1000;
        item.cog = 110;
        item.sog = 1.2;
        item.status = 1;
        item.warnStatusColor = Qt::red;
//        if(item.status>=1)
        radarList.append(item);
    }

    m_pEcdisWin->itfSetRadarPointData(radarList);
}
