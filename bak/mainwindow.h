#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

namespace Ui {
class MainWindow;
}
class zchxMapWidget;
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
    void slotDisplayMapCenterAndZoom(double lon, double lat, int zoom);
    void slotUpdateDownloadProgress(int val);
private slots:
    void on_load_clicked();

    void on_download_clicked();

    void on_source_activated(const QString &arg1);

    void on_source_currentIndexChanged(int index);

    void on_image_num_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    zchxMapWidget *mMapWidget;
};

#endif // MAINWINDOW_H