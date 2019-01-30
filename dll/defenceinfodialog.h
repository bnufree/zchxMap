#ifndef DEFENCEINFODIALOG_H
#define DEFENCEINFODIALOG_H

#include <QDialog>

namespace Ui {
class DefenceInfoDialog;
}

class DefenceInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DefenceInfoDialog(QWidget *parent = 0);
    ~DefenceInfoDialog();

    QString getName();
    bool getWarning();
    int getType();
    double getDropAnchorThreshold();
    double getSpeedLimit();
    QString getColor();

private slots:
    void on_colorPushButton_clicked();

    void on_comfirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::DefenceInfoDialog *ui;
    QString m_color;
};

#endif // DEFENCEINFODIALOG_H
