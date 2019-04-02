#ifndef WARNINGZONEPARADIALOG_H
#define WARNINGZONEPARADIALOG_H

#include <QDialog>

namespace Ui {
class WarningZoneParaDialog;
}

namespace qt {
class WarningZoneParaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WarningZoneParaDialog(QWidget *parent = 0);
    ~WarningZoneParaDialog();
    QString getName();
    int getType();
    float getValue();
    void setLangague();
    int getWarning();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

    void on_type_1_RButton_clicked(bool checked);

    void on_type_2_RButton_clicked(bool checked);

    void on_WarningradioButton_clicked();
    
    void on_notwarningradioButton_clicked();
    
    void on_OtherradioButton_clicked();

private:
    Ui::WarningZoneParaDialog *ui;
};
}

#endif // WARNINGZONEPARADIALOG_H
