#ifndef ZCHXECDISDLGMGR_H
#define ZCHXECDISDLGMGR_H

#include <QObject>
#include "../zchxutils.hpp"
#include "../zchxMapDataUtils.h"

namespace qt {
class zchxMapWidget;
class RodElement;
class AisElement;
class zchxEcdisDlgMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxEcdisDlgMgr(zchxMapWidget* w, QObject *parent = 0);
    //调用相机列表对话框
    void zchxOpenCameraListDlg(RodElement *item);
    void zchxOpenCameraListDlg(AisElement *item);
    void zchxOpenCameraListDlg(QList<ZCHX::Data::ITF_CameraDev>& list, const ZCHX::Data::LatLon& ll = ZCHX::Data::LatLon());

signals:
    void signalIsSelected4CameraDev(const ZCHX::Data::ITF_CameraDev &info);//相机点击
    void signalCameraDoubleClicked(const ZCHX::Data::ITF_CameraDev &info);//相机双击

public slots:

private:
    zchxMapWidget       *mDisplayWidget;
};
}

#endif // ZCHXECDISDLGMGR_H
