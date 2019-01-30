#include "coastdatainfodialog.h"
#include "ui_coastdatainfodialog.h"

CoastDataInfoDialog::CoastDataInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CoastDataInfoDialog)
{
    ui->setupUi(this);
}

CoastDataInfoDialog::~CoastDataInfoDialog()
{
    delete ui;
}

QString CoastDataInfoDialog::getName()
{
    return ui->nameLineEdit->text();
}

QString CoastDataInfoDialog::getOrg()
{
    return ui->orgLineEdit->text();
}

QString CoastDataInfoDialog::getAccount()
{
    return ui->accountLineEdit->text();
}

double CoastDataInfoDialog::getHeight()
{
    return ui->heightLineEdit->text().toDouble();
}

void CoastDataInfoDialog::on_comfirmButton_clicked()
{
    accept();
}

void CoastDataInfoDialog::on_cancelButton_clicked()
{
    reject();
}
