#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zchxmapthread.h"
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>

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
    if(rect.topLeft().x() >0 && rect.topLeft().y() >= 0)
    {
        //开始加载地图
        if(!mMapthread)
        {
            mMapthread = new zchxMapThread;
            mMapthread->appendTask(zchxMapTask(rect.width(), rect.height(), ui->ecdis->zoom()));
            connect(mMapthread, SIGNAL(signalSendCurPixmap(QPixmap,int,int)), this, SLOT(slotRecvMapData(QPixmap,int,int)));
            connect(mMapthread, SIGNAL(signalSendCurSize(int,int)), this, SLOT(updateGridLayout(int,int)));
            mMapthread->start();
        }


    }
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
