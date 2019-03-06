#ifndef ZCHXDRAWCAMERANETGRIDTOOL_H
#define ZCHXDRAWCAMERANETGRIDTOOL_H

#include "zchxdrawtool.h"
namespace qt{
class zchxDrawCameraNetGridTool : public zchxDrawTool
{
    Q_OBJECT
public:
    explicit zchxDrawCameraNetGridTool(zchxMapWidget* w, QObject *parent = 0);
    void show(QPainter *painter);
    void appendPoint(const QPointF& pnt);
    void endDraw();
    void setCameraGridParam(const QString &id, const QSizeF &size);
private:
    ZCHX::Data::ITF_CameraNetGrid makeCameraGrid(const ZCHX::Data::LatLon &sll, const ZCHX::Data::LatLon &ell);

signals:

public slots:
private:
    QSizeF                                           mSize;
    QString                                          mCameraID;
};
}

#endif // ZCHXDRAWAREATOOL_H
