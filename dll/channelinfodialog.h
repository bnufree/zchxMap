#ifndef CHANNELINFODIALOG_H
#define CHANNELINFODIALOG_H

#include <QDialog>

namespace Ui {
class ChannelInfoDialog;
}

class ChannelInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChannelInfoDialog(QWidget *parent = 0);
    ~ChannelInfoDialog();

    QString getName();
    int getChannelType();
    bool getChannelStatus();
    double getMaxSpeed();
    double getCollisionMaxDis();
    double getYawMinDis();
    double getOvertak();
    double getDropAnchor();
    double getOvertakShortDis();
    double getOvertakVerticalDis();
    double getAcceleraThreshold();
    double getAcceleraDfference();
    bool isConverseWarn();
    bool isOverloadWarn();
    QString getColor();

private slots:
    void on_comfirmButton_clicked();

    void on_cancelButton_clicked();

    void on_colorPushButton_clicked();

    void on_typeComboBox_currentIndexChanged(int index);

private:
    Ui::ChannelInfoDialog *ui;
    QString m_color;
};

#endif // CHANNELINFODIALOG_H
