#ifndef ZCHXMAPWIDGET_H
#define ZCHXMAPWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QPixmap>
#include "zchxecdisutils.h"

struct MapData{
    QPixmap img;
    int x;
    int y;
};

class zchxMapView;

class zchxMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit zchxMapWidget(QWidget *parent = 0);
    ~zchxMapWidget();
    void clear() {mDataList.clear(); update();}
    void setCurZoom(int zoom);
    int  zoom() const;
    void setCenterLL(const Wgs84LonLat& pnt );
    Wgs84LonLat centerLonlat() const;

protected:
    void paintEvent(QPaintEvent* e);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void resizeEvent(QResizeEvent *);
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent(QWheelEvent *);
#endif

signals:
    void signalDisplayCurMap(double lon, double lat, int zoom);

public slots:
    void append(const QPixmap& img, int x, int y);
private:
    QList<MapData> mDataList;
    zchxMapView*        mView;

};

#endif // ZCHXMAPWIDGET_H
