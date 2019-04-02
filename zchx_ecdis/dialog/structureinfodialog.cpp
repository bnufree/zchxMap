#include "structureinfodialog.h"
#include "ui_structureinfodialog.h"

namespace qt {
StructureInfoDialog::StructureInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructureInfoDialog)
{
    ui->setupUi(this);
}

StructureInfoDialog::~StructureInfoDialog()
{
    delete ui;
}

QString StructureInfoDialog::getName()
{
    return ui->nameLineEdit->text();
}

double StructureInfoDialog::getArea()
{
    return ui->areaLineEdit->text().toDouble();
}

double StructureInfoDialog::getHeight()
{
    return ui->heightLineEdit->text().toDouble();
}

void StructureInfoDialog::on_comfirmButton_clicked()
{
    accept();
}

void StructureInfoDialog::on_cancelButton_clicked()
{
    reject();
}
}
