#ifndef ZCHXMAPWIDGET_H
#define ZCHXMAPWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPixmap>

struct MapData{
    QPixmap img;
    int x;
    int y;
};

class zchxMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit zchxMapWidget(QWidget *parent = 0);
    void clear() {mDataList.clear(); update();}
    void setCurZoom(int zoom) {mCurrentZoom = zoom;}
    int  zoom() const {return mCurrentZoom;}

protected:
    void paintEvent(QPaintEvent* e);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent(QWheelEvent *);
#endif

signals:

public slots:
    void append(const QPixmap& img, int x, int y);
private:
    QList<MapData> mDataList;
    int             mCurrentZoom;

};

#endif // ZCHXMAPWIDGET_H
