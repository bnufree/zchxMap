#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zchxmapthread.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mMapthread(0)
{
    ui->setupUi(this);
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
            mMapthread->appendTask(zchxMapTask(rect.width(), rect.height()));
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
