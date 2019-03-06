#include "zchxdrawzonetool.h"
#include "dialog/radarfeaturezone.h"
#include "dialog/defenceinfodialog.h"
#include "zchxmapframe.h"

using namespace qt;

zchxDrawZoneTool::zchxDrawZoneTool(zchxMapWidget* w, int type, int num,  QObject *parent) : zchxDrawTool(w, type, parent)
{
    mMinPntsNum = num;
}

void zchxDrawZoneTool::show(QPainter *painter)
{
    if(!isReady() || !painter) return;
    PainterPair chk(painter);
    painter->setPen(QPen(Qt::red,1,Qt::DashLine));
    painter->setBrush(QBrush(Qt::blue, Qt::Dense7Pattern));
    painter->drawPolygon(&mPoints[0], getPointSize());
}

bool zchxDrawZoneTool::checkPnts()
{
    if(getPointSize() < mMinPntsNum) {
        QMessageBox box;
        box.setWindowTitle(tr("Warring"));
        box.setText(tr("At least %1 points.").arg(mMinPntsNum));
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,tr("OK"));
        box.exec ();
        return false;
    }
    return true;
}

void zchxDrawRadarFeatureZoneTool::endDraw()
{
    if(checkPnts() && isReady()){
        ZCHX::Data::ITF_RadarFeaturesZone radar_zone_data;
        for(QPointF pnt : mPoints) {
            radar_zone_data.pointList.push_back(mWidget->framework()->Pixel2LatLon(pnt));
        }
        RadarFeatureZone objDialog;
        if(objDialog.exec() == QDialog::Accepted)
        {
            radar_zone_data.name = objDialog.getName();
            radar_zone_data.zoneType = objDialog.getType();
            emit signalCreateRadarFeatureZONE(radar_zone_data);
        }
    }
    zchxDrawZoneTool::endDraw();
}

void zchxDrawWarningZoneTool::endDraw()
{
    if(checkPnts() && isReady()){
        std::vector<std::pair<double, double>> path;
        for(QPointF pnt : mPoints) {
            ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(pnt);
            path.push_back(std::pair<double, double>(ll.lat, ll.lon));
        }
        ZCHX::Data::ITF_WarringZone zone;
        zone.path = path;
        zone.id = 0;
        zone.shape = ZCHX::Data::ITF_WarringZone::ZONE_POLYGON; //防区

        DefenceInfoDialog dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            zone.name = dlg.getName();
            zone.isWarn = dlg.getWarning();
            zone.defType = dlg.getType();
            zone.dropAnchorThreshold = dlg.getDropAnchorThreshold();
            zone.speedLimit = dlg.getSpeedLimit();
            zone.fillColor = dlg.getColor();
        }
        if(!zone.name.isEmpty())
        {
            emit signalCreateWarringZONE(zone);
        }
    }
    zchxDrawZoneTool::endDraw();
}
