#include "localmarkdlg.h"
#include "ui_localmarkdlg.h"

#include<QMessageBox>

namespace qt {
LocalMarkDlg::LocalMarkDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocalMarkDlg)
{
    ui->setupUi(this);
    init();
}

void LocalMarkDlg::setLocalMarkPos(double lon, double lat)
{
    ui->lonLedt->setText(QString::number(lon));
    ui->latLedt->setText(QString::number(lat));
}

void LocalMarkDlg::getLocalMardData(ZCHX::Data::ITF_LocalMark &data) const
{
    data = localMarkData;
}

void LocalMarkDlg::init()
{
    connect(ui->setApplyBtn, SIGNAL(clicked(bool)), this, SLOT(onSetApplyBtnClicked()));
}

LocalMarkDlg::~LocalMarkDlg()
{
    delete ui;
}



void LocalMarkDlg::onSetApplyBtnClicked()
{
    QString localMarkName = ui->localNameLedt->text().trimmed();
    if(localMarkName.isEmpty())
    {
        QMessageBox::information(this,tr("Error Info"),tr("Please input local mark name!"));
        return;
    }

    localMarkData.uuid = 0;
    localMarkData.name = localMarkName;
    localMarkData.ll = {ui->latLedt->text().trimmed().toDouble(),ui->lonLedt->text().trimmed().toDouble()};
    accept();
}
}
