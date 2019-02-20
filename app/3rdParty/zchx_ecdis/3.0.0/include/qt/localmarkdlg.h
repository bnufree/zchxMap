#ifndef LOCALMARKDLG_H
#define LOCALMARKDLG_H

#include "zchxutils.hpp"
#include <QDialog>

namespace Ui {
class LocalMarkDlg;
}
namespace qt {
class LocalMarkDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LocalMarkDlg(QWidget *parent = 0);
    ~LocalMarkDlg();

    void setLocalMarkPos(double lon, double lat);
    void getLocalMardData(ZCHX::Data::ITF_LocalMark &) const;

private slots:
    void onSetApplyBtnClicked();

private:
    void init();
    Ui::LocalMarkDlg *ui;
    ZCHX::Data::ITF_LocalMark localMarkData;
};
}
#endif // LOCALMARKDLG_H
