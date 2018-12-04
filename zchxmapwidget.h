#ifndef ZCHXMAPWIDGET_H
#define ZCHXMAPWIDGET_H

#include <QWidget>
#include <QPaintEvent>
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

protected:
    void paintEvent(QPaintEvent* e);

signals:

public slots:
    void append(const QPixmap& img, int x, int y);
private:
    QList<MapData> mDataList;

};

#endif // ZCHXMAPWIDGET_H
