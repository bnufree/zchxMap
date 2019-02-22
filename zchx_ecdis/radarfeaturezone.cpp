#include <QSettings>
#include "radarfeaturezone.h"
#include "ui_radarfeaturezone.h"

RadarFeatureZone::RadarFeatureZone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RadarFeatureZone)
{
    ui->setupUi(this);
    ui->nameLabel->hide();
    ui->nameLineEdit->hide();
    setLangague();
}

RadarFeatureZone::~RadarFeatureZone()
{
    delete ui;
}

QString RadarFeatureZone::getName()
{
    return ui->nameLineEdit->text();
}

int RadarFeatureZone::getType()
{
    if(ui->static_button->isChecked())
    {
        return 5;
    }
    else if(ui->no_initialize_button->isChecked())
    {
        return 2;
    }
    else if(ui->trace_area_button->isChecked())
    {
        return 4;
    }
    else if(ui->no_track_button->isChecked())
    {
        return 3;
    }
    return 0;
}

void RadarFeatureZone::setLangague()
{

    setWindowTitle(tr("Radar Feature Area Attribute"));
    ui->nameLabel->setText(tr("Name:"));
    ui->typeLabel->setText(tr("Type:"));
    ui->no_track_button->setText(tr("No tracking area"));
    ui->trace_area_button->setText(tr("Tracking prediction area"));
    ui->no_initialize_button->setText(tr("No initialize area"));
    ui->static_button->setText(tr("Static initialization area"));
    ui->okBtn->setText(tr("Ok"));
    ui->cancelBtn->setText(tr("Cancel"));

}

void RadarFeatureZone::on_okBtn_clicked()
{
//    QString sName = ui->nameLineEdit->text();
//    if(sName.isEmpty()||sName.isNull())
//        return;
    accept();
}

void RadarFeatureZone::on_cancelBtn_clicked()
{
    reject();
}
