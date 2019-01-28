#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zchxmapthread.h"
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include "zchxmapdownloadthread.h"
#include <QSpacerItem>
#include "zchxmapwidget.h"

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
    }
    mMapWidget->setGeometry(0, 0, ui->ecdis_frame->size().width(), ui->ecdis_frame->size().height());
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
    mMapWidget->setCenterAndZoom(Wgs84LonLat(lon, lat), zoom);
}

void MainWindow::slotUpdateCurrentPos(double lon, double lat)
{
    ui->pos_label->setText(QString("%1, %2").arg(lon, 0, 'f', 6).arg(lat, 0, 'f', 6));
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
