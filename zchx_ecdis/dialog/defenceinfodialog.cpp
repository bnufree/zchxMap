#include "defenceinfodialog.h"
#include "ui_defenceinfodialog.h"
#include <QColorDialog>

namespace qt {
DefenceInfoDialog::DefenceInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DefenceInfoDialog)
{
    ui->setupUi(this);
}

DefenceInfoDialog::~DefenceInfoDialog()
{
    delete ui;
}

QString DefenceInfoDialog::getName()
{
    return ui->nameLineEdit->text();
}

bool DefenceInfoDialog::getWarning()
{
    if (ui->warningComboBox->currentIndex() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int DefenceInfoDialog::getType()
{
    return ui->typeComboBox->currentIndex() + 1;
}

double DefenceInfoDialog::getDropAnchorThreshold()
{
    return ui->dropAnchorThresholdLineEdit->text().toDouble();
}

double DefenceInfoDialog::getSpeedLimit()
{
    return ui->speedLimitLineEdit->text().toDouble();
}

QString DefenceInfoDialog::getColor()
{
    return m_color;
}

void DefenceInfoDialog::on_colorPushButton_clicked()
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

void DefenceInfoDialog::on_comfirmButton_clicked()
{
    accept();
}

void DefenceInfoDialog::on_cancelButton_clicked()
{
    reject();
}
}
