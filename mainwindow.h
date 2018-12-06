#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

namespace Ui {
class MainWindow;
}

class zchxMapThread;
class QLabel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *e);
public slots:
    void updateGridLayout(int x, int y);
    void slotRecvMapData(const QPixmap& data, int x, int y);
    void slotUpdateCurrentPos(double lon, double lat);
private slots:
    void on_load_clicked();

private:
    Ui::MainWindow *ui;
    zchxMapThread* mMapthread;
    QLabel*        mPosLabel;
};

#endif // MAINWINDOW_H
