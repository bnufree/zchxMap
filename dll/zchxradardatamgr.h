#ifndef ZCHXRADARDATAMGR_H
#define ZCHXRADARDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>

class zchxMapWidget;

class zchxRadarDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxRadarDataMgr(zchxMapWidget* w, QObject *parent = 0);
    void    setRadarPointData(const std::vector<std::shared_ptr<DrawElement::RadarPointElement> > &data);
    void    showRadarPoint(QPainter* painter);

signals:

public slots:


private:
    std::vector<std::shared_ptr<DrawElement::RadarPointElement> > m_RadarPoint;
    zchxMapWidget       *mDisplayWidget;
};

#endif // ZCHXRADARDATAMGR_H
