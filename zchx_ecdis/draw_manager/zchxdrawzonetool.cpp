#include "zchxdrawzonetool.h"
#include "dialog/radarfeaturezone.h"
#include "dialog/defenceinfodialog.h"
#include "dialog/channelinfodialog.h"
#include "dialog/mooringinfodialog.h"
#include "dialog/cardmouthinfodialog.h"
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
    painter->drawPolygon(QPolygonF(mPoints.toVector()));
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

QList<QAction*> zchxDrawZoneTool::getRightMenuActions(const QPoint& pt)
{
    QList<QAction*> list;
    list.append(addAction(tr("重新开始"),this, SLOT(startDraw()), 0));
    list.append(addAction(tr("确认"),this, SLOT(endDraw()), 0));
    if(mWidget){
        list.append(addAction(tr("取消"), mWidget, SLOT(releaseDrawStatus()), 0));
    }
    return list;
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
            emit mWidget->signalCreateRadarFeatureZONE(radar_zone_data);
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
            emit mWidget->signalCreateWarringZONE(zone);
        }
    }
    zchxDrawZoneTool::endDraw();
}

void zchxDrawChannelZoneTool::endDraw()
{
    if(checkPnts() && isReady()){
        std::vector<std::pair<double, double>> path;
        for(QPointF pnt : mPoints) {
            ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(pnt);
            path.push_back(std::pair<double, double>(ll.lat, ll.lon));
        }
        ZCHX::Data::ITF_Channel zone;
        zone.path = path;
        zone.id = 0;

        ChannelInfoDialog dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            zone.name = dlg.getName();
            zone.courseType = dlg.getChannelType();
            zone.isWarn = dlg.getChannelStatus();
            zone.speedLimit = dlg.getMaxSpeed();
            zone.collisionThreshold = dlg.getCollisionMaxDis();
            zone.yawThreshold = dlg.getYawMinDis();
            zone.overtakThreshold = dlg.getOvertak();
            zone.dropAnchorThreshold = dlg.getDropAnchor();
            zone.overtakShortDis = dlg.getOvertakShortDis();
            zone.overtakVerticalDis = dlg.getOvertakVerticalDis();
            zone.acceleraThreshold = dlg.getAcceleraThreshold();
            zone.acceleraDfference = dlg.getAcceleraDfference();
            zone.isConverseWarn = dlg.isConverseWarn();
            zone.isOverloadWarn = dlg.isOverloadWarn();

            zone.fillColor = dlg.getColor();
        }

        if(!zone.name.isEmpty())
        {
            emit mWidget->signalCreateChannelZone(zone);
        }
    }

    zchxDrawZoneTool::endDraw();
}

void zchxDrawMooringZoneTool::endDraw()
{
    if(checkPnts() && isReady()){
        std::vector<std::pair<double, double>> path;
        for(QPointF pnt : mPoints) {
            ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(pnt);
            path.push_back(std::pair<double, double>(ll.lat, ll.lon));
        }
        ZCHX::Data::ITF_Mooring zone;
        zone.path = path;
        zone.id = 0;
        MooringInfoDialog dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            zone.name = dlg.getName();
            zone.anchorType = dlg.getMooringType();
            zone.isWarn = dlg.getMooringStatus();
            zone.fillColor = dlg.getColor();
            zone.displaceDis = dlg.getDisplaceDis();
            zone.displaceCycle = dlg.getDisplaceCycle();
        }
        if(!zone.name.isEmpty())
        {
            emit mWidget->signalCreateMooringZone(zone);
        }
    }

    zchxDrawZoneTool::endDraw();
}


void zchxDrawCardMouthTool::endDraw()
{
    if(checkPnts() && isReady()){
        std::vector<std::pair<double, double>> path;
        for(QPointF pnt : mPoints) {
            ZCHX::Data::LatLon ll = mWidget->framework()->Pixel2LatLon(pnt);
            path.push_back(std::pair<double, double>(ll.lat, ll.lon));
        }
        ZCHX::Data::ITF_CardMouth zone;
        zone.path = path;
        zone.id = 0;

        CardMouthInfoDialog dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            zone.name = dlg.getName();
            zone.caMouCapConType = dlg.getType();
            zone.isAisUnopened = dlg.isAisUnopened();
            zone.isWarn = dlg.getWarning();
            zone.fillColor = dlg.getColor();
            zone.overlength = dlg.getOverlength();
        }

        if(!zone.name.isEmpty())
        {
            emit mWidget->signalCreateCardMouthZone(zone);
        }
    }

    zchxDrawZoneTool::endDraw();
}
