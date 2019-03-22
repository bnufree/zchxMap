#ifndef ZCHXRADARVIDEODMGR_H
#define ZCHXRADARVIDEODMGR_H

#include "zchxtemplatedatamgr.h"

namespace qt {
class zchxRadarVideoMgr : public zchxTemplateDataMgr<RadarVideoGlowElement, ZCHX::Data::ITF_RadarVideoGLow>
{
    Q_OBJECT
public:
    explicit zchxRadarVideoMgr(zchxMapWidget* w, QObject *parent = 0);
    void    setIsDisplay(bool sts);
    void    setRadarVideoData(double dCentreLon, double dCentreLat, double dDistance, int uDisplayType,int uLoopNum);
    void    setRadarVideoPixmap(int uIndex, const QPixmap &objPixmap, const QPixmap &prePixmap);
    void    setCurrentRadarVideoPixmap(const QPixmap &objPixmap);
signals:




public slots:


private:
    ZCHX::Data::ITF_RadarVideoGLow      mRadarVideoData;
};
}

#endif // ZCHXRADARVIDEODMGR_H
