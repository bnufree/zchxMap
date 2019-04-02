#include <QSettings>
#include "warningzoneparadialog.h"
#include "ui_warningzoneparadialog.h"

namespace qt {
WarningZoneParaDialog::WarningZoneParaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningZoneParaDialog)
{
    ui->setupUi(this);
    ui->WarningradioButton->setChecked(true);
    ui->frame->setEnabled(true);
    ui->frame_3->setVisible(false);
    ui->OtherradioButton->hide();
    setLangague();
}

WarningZoneParaDialog::~WarningZoneParaDialog()
{
    delete ui;
}

QString WarningZoneParaDialog::getName()
{
    return ui->nameLineEdit->text();
}

int WarningZoneParaDialog::getType()
{
    if ((ui->type_1_RButton->isChecked())&(ui->type_2_RButton->isChecked()))
    {
        return 4;
    }
    else if(ui->type_1_RButton->isChecked())
    {
        return 1;
    }
    else if(ui->type_2_RButton->isChecked())
    {
        return 2;
    }
    return 0;
}

float WarningZoneParaDialog::getValue()
{
    return float(ui->valueDSpinBox->value());
}

void WarningZoneParaDialog::setLangague()
{
    setWindowTitle(tr("warning Area"));
    ui->nameLabel->setText(tr("name"));
    ui->typeLabel->setText(tr("type"));
    ui->type_1_RButton->setText(tr("Key Regions"));
    ui->type_2_RButton->setText(tr("Speed Monitoring Area"));
    ui->valueLabel->setText(tr("speed"));
    ui->unitLable->setText(tr("Section"));
    ui->WarningradioButton->setText(tr("Warning"));
    ui->notwarningradioButton->setText(tr("Not Warning"));
    ui->okBtn->setText(tr("Ok"));
    ui->cancelBtn->setText(tr("Cancel"));
    ui->OtherradioButton->setText(tr("Other"));
}

int WarningZoneParaDialog::getWarning()
{
    if(ui->WarningradioButton->isChecked())
    {
        return 1;
    }
    else if(ui->notwarningradioButton->isChecked())
    {
        return 2;
    }
    else if(ui->OtherradioButton->isCheckable())
    {
        return 3;
    }
}

void WarningZoneParaDialog::on_okBtn_clicked()
{
    QString sName = ui->nameLineEdit->text();
    if(sName.isEmpty()||sName.isNull())
        return;
    accept();
}

void WarningZoneParaDialog::on_cancelBtn_clicked()
{
    reject();
}

void WarningZoneParaDialog::on_type_1_RButton_clicked(bool checked)
{
    bool isCheck = ui->type_2_RButton->isChecked();
    if(checked && !isCheck)
    {
        ui->valueLabel->setVisible(false);
        ui->unitLable->setVisible(false);
        ui->valueDSpinBox->setVisible(false);
    }
}

void WarningZoneParaDialog::on_type_2_RButton_clicked(bool checked)
{
    if(checked)
    {
        ui->valueLabel->setVisible(true);
        ui->unitLable->setVisible(true);
        ui->valueDSpinBox->setVisible(true);
        ui->frame_3->setVisible(true);
    }
    else
    {
        ui->valueLabel->setVisible(false);
        ui->unitLable->setVisible(false);
        ui->valueDSpinBox->setVisible(false);
        ui->frame_3->setVisible(false);
    }
}


void WarningZoneParaDialog::on_WarningradioButton_clicked()
{
    ui->type_1_RButton->setEnabled(true);
    ui->type_2_RButton->setEnabled(true);
}

void WarningZoneParaDialog::on_notwarningradioButton_clicked()
{
    //ui->frame->setEnabled(false);
    ui->type_1_RButton->setEnabled(false);
    ui->type_2_RButton->setEnabled(false);
    ui->valueLabel->setVisible(false);
    ui->unitLable->setVisible(false);
    ui->valueDSpinBox->setVisible(false);
    ui->frame_3->setVisible(false);
}

void WarningZoneParaDialog::on_OtherradioButton_clicked()
{
    if(ui->OtherradioButton->isChecked())
    {
        ui->valueLabel->setVisible(true);
        ui->unitLable->setVisible(true);
        ui->valueDSpinBox->setVisible(true);
        ui->frame_3->setVisible(false);
        ui->type_1_RButton->setEnabled(false);
        ui->type_2_RButton->setEnabled(false);
    }
}
}
