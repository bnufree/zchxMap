#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H

#include <QMainWindow>
#include "qt/mainwindow.h"
#include "qt/map_layer/zchxMapLayer.h"

namespace Ui {
class TestMainWindow;
}

class TestMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestMainWindow(QWidget *parent = 0);
    ~TestMainWindow();
public slots:
    void slotTimerout();
    void slotRecvPickPnts(const QList<ZCHX::Data::LatLon>& ll);

private:
    Ui::TestMainWindow *ui;
    qt::MainWindow  *m_pEcdisWin;
    QTimer *mTestTimer;
};

#endif // TESTMAINWINDOW_H
