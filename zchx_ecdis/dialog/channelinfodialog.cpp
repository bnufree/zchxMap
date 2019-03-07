#include "channelinfodialog.h"
#include "ui_channelinfodialog.h"
#include <QColorDialog>

namespace qt {
ChannelInfoDialog::ChannelInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChannelInfoDialog),
    m_color("")
{
    ui->setupUi(this);
}

ChannelInfoDialog::~ChannelInfoDialog()
{
    delete ui;
}

QString ChannelInfoDialog::getName()
{
    return ui->nameLineEdit->text();
}

int ChannelInfoDialog::getChannelType()
{
    return ui->typeComboBox->currentIndex() + 1;
}

bool ChannelInfoDialog::getChannelStatus()
{
    if (ui->statusComboBox->currentIndex() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double ChannelInfoDialog::getMaxSpeed()
{
    return ui->speedLineEdit->text().toDouble();
}

double ChannelInfoDialog::getYawMinDis()
{
    return ui->yawMinDisLineEdit->text().toDouble();
}

double ChannelInfoDialog::getOvertak()
{
    return ui->overtakLineEdit->text().toDouble();
}

double ChannelInfoDialog::getDropAnchor()
{
    return ui->dropAnchorLineEdit->text().toDouble();
}

double ChannelInfoDialog::getOvertakShortDis()
{
    return ui->overtakShortDisLineEdit->text().toDouble();
}

double ChannelInfoDialog::getOvertakVerticalDis()
{
    return ui->overtakVerticalDisLineEdit->text().toDouble();
}

double ChannelInfoDialog::getAcceleraThreshold()
{
    return ui->acceleraThresholdLineEdit->text().toDouble();
}

double ChannelInfoDialog::getAcceleraDfference()
{
    return ui->acceleraDfferenceLineEdit->text().toDouble();
}

bool ChannelInfoDialog::isConverseWarn()
{
    if (ui->converseWarnComboBox->currentIndex() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ChannelInfoDialog::isOverloadWarn()
{
    if (ui->overloadWarnComboBox->currentIndex() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double ChannelInfoDialog::getCollisionMaxDis()
{
    return ui->collisionMaxDisLineEdit->text().toDouble();
}

QString ChannelInfoDialog::getColor()
{
    return m_color;
}

void ChannelInfoDialog::on_comfirmButton_clicked()
{
    accept();
}

void ChannelInfoDialog::on_cancelButton_clicked()
{
    reject();
}

void ChannelInfoDialog::on_colorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::red, this, tr("Color"));
    if(!color.isValid())
    {
        return;
    }
    m_color = color.name();

    QPalette palette;
    palette.setColor(QPalette::Background, color);
    ui->colorLabel->setAutoFillBackground(true);  //一定要这句，否则不行
    ui->colorLabel->setPalette(palette);
}

void ChannelInfoDialog::on_typeComboBox_currentIndexChanged(int index)
{
    // 码头
    if (index == 4)
    {
        ui->statusComboBox->setCurrentIndex(1);
        ui->statusComboBox->setEnabled(false);
    }
    else
    {
        ui->statusComboBox->setEnabled(true);
    }
}
}
