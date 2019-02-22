#ifndef MOORINGINFODIALOG_H
#define MOORINGINFODIALOG_H

#include <QDialog>

namespace Ui {
class MooringInfoDialog;
}

namespace qt {
class MooringInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MooringInfoDialog(QWidget *parent = 0);
    ~MooringInfoDialog();

    QString getName();
    int getMooringType();
    bool getMooringStatus();
    double getDisplaceDis();
    int getDisplaceCycle();
    QString getColor();

private slots:
    void on_comfirmButton_clicked();

    void on_cancelButton_clicked();

    void on_colorPushButton_clicked();

private:
    Ui::MooringInfoDialog *ui;
    QString m_color;
};
}

#endif // MOORINGINFODIALOG_H
