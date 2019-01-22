#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zchxmapthread.h"
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include "zchxmapdownloadthread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mMapthread(0)
{
    ui->setupUi(this);
    mPosLabel = new QLabel(this);
    ui->statusBar->setLayout(new QHBoxLayout);
    ui->statusBar->layout()->addWidget(new QLabel);
    ui->statusBar->layout()->addWidget(mPosLabel);
    ui->statusBar->layout()->addWidget(new QLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateGridLayout(int x, int y)
{
    ui->ecdis->clear();
}


void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    QRect rect =  ui->ecdis->geometry();
//    if(rect.topLeft().x() >0 && rect.topLeft().y() >= 0)
//    {
//        qDebug()<<"init zoom:"<<ui->ecdis->zoom();
//        //开始加载地图
//        if(!mMapthread)
//        {
//            mMapthread = new zchxMapThread;
//            zchxMapTask task(rect.width(), rect.height(), ui->ecdis->zoom());
//            ui->ecdis->setCenterPoint(QPointF(task.lon, task.lat));
//            ui->ecdis->setCurZoom(task.zoom);
//            mMapthread->appendTask(task);
//            connect(mMapthread, SIGNAL(signalSendCurPixmap(QPixmap,int,int)), this, SLOT(slotRecvMapData(QPixmap,int,int)));
//            connect(mMapthread, SIGNAL(signalSendCurSize(int,int)), this, SLOT(updateGridLayout(int,int)));
//            connect(ui->ecdis, SIGNAL(signalDisplayCurMap(double,double,int)),
//                    this, SLOT(slotDisplayNewMap(double,double,int)));
//            mMapthread->start();
//        }


//    }
}

void MainWindow::slotRecvMapData(const QPixmap &data, int x, int y)
{
    ui->ecdis->append(data, x, y);
}

void MainWindow::on_load_clicked()
{
    QRect rect =  ui->ecdis->geometry();
    double lon = ui->lon->text().toDouble();
    double lat = ui->lat->text().toDouble();
    int zoom = ui->zoom->text().toInt();
    ui->ecdis->clear();
    mMapthread->appendTask(zchxMapTask(lon, lat, rect.width(), rect.height(), zoom));
    ui->ecdis->setCurZoom(zoom);
}

void MainWindow::slotUpdateCurrentPos(double lon, double lat)
{
    mPosLabel->setText(QString("%1, %2").arg(lon, 0, 'f', 6).arg(lat, 0, 'f', 6));
}

void MainWindow::slotDisplayNewMap(double lon, double lat, int zoom)
{
    QRect rect =  ui->ecdis->geometry();
    zchxMapTask task(lon, lat, rect.width(), rect.height(), zoom);
    mMapthread->appendTask(task);
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
