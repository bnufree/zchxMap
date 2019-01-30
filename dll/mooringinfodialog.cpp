#include "mooringinfodialog.h"
#include "ui_mooringinfodialog.h"
#include <QColorDialog>

MooringInfoDialog::MooringInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MooringInfoDialog),
    m_color("")
{
    ui->setupUi(this);
}

MooringInfoDialog::~MooringInfoDialog()
{
    delete ui;
}

QString MooringInfoDialog::getName()
{
    return ui->nameLineEdit->text();
}

int MooringInfoDialog::getMooringType()
{
    return ui->typeComboBox->currentIndex() + 1;
}

bool MooringInfoDialog::getMooringStatus()
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

double MooringInfoDialog::getDisplaceDis()
{
    return ui->displaceDisLineEdit->text().toDouble();
}

int MooringInfoDialog::getDisplaceCycle()
{
    return ui->displaceCycleLineEdit->text().toInt();
}

QString MooringInfoDialog::getColor()
{
    return m_color;
}

void MooringInfoDialog::on_comfirmButton_clicked()
{
    accept();
}

void MooringInfoDialog::on_cancelButton_clicked()
{
    reject();
}

void MooringInfoDialog::on_colorPushButton_clicked()
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
