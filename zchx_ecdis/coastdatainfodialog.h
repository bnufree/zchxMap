#ifndef COASTDATAINFODIALOG_H
#define COASTDATAINFODIALOG_H

#include <QDialog>

namespace Ui {
class CoastDataInfoDialog;
}

namespace qt {
class CoastDataInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CoastDataInfoDialog(QWidget *parent = 0);
    ~CoastDataInfoDialog();

    QString getName();
    QString getOrg();
    QString getAccount();
    double getHeight();

private slots:
    void on_comfirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CoastDataInfoDialog *ui;
};
}

#endif // COASTDATAINFODIALOG_H
