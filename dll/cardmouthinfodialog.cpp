#include "cardmouthinfodialog.h"
#include "ui_cardmouthinfodialog.h"
#include <QColorDialog>

CardMouthInfoDialog::CardMouthInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardMouthInfoDialog)
{
    ui->setupUi(this);
}

CardMouthInfoDialog::~CardMouthInfoDialog()
{
    delete ui;
}

QString CardMouthInfoDialog::getName()
{
    return ui->nameLineEdit->text();
}

bool CardMouthInfoDialog::getWarning()
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

int CardMouthInfoDialog::getType()
{
    return ui->typeComboBox->currentIndex() + 1;
}

bool CardMouthInfoDialog::isAisUnopened()
{
    if (ui->aisComboBox->currentIndex() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double CardMouthInfoDialog::getOverlength()
{
    return ui->overlengthLineEdit->text().toDouble();
}

QString CardMouthInfoDialog::getColor()
{
    return m_color;
}

void CardMouthInfoDialog::on_colorPushButton_clicked()
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

void CardMouthInfoDialog::on_comfirmButton_clicked()
{
    accept();
}

void CardMouthInfoDialog::on_cancelButton_clicked()
{
    reject();
}
