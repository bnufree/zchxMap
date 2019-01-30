#ifndef CARDMOUTHINFODIALOG_H
#define CARDMOUTHINFODIALOG_H

#include <QDialog>

namespace Ui {
class CardMouthInfoDialog;
}

class CardMouthInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CardMouthInfoDialog(QWidget *parent = 0);
    ~CardMouthInfoDialog();

    QString getName();
    int getType();
    bool isAisUnopened();
    bool getWarning();
    double getOverlength();
    QString getColor();

private slots:
    void on_colorPushButton_clicked();

    void on_comfirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CardMouthInfoDialog *ui;
    QString m_color;
};

#endif // CARDMOUTHINFODIALOG_H
