#include "testmainwindow.h"
#include "ui_testmainwindow.h"
#include "qt/mainwindow.h"

TestMainWindow::TestMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestMainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(new QVBoxLayout);
    ui->centralwidget->layout()->addWidget(new qt::MainWindow);
}

TestMainWindow::~TestMainWindow()
{
    delete ui;
}
