#ifndef ZCHXUSERDEFINESDATAMGR_H
#define ZCHXUSERDEFINESDATAMGR_H

#include "zchxtemplatedatamgr.h"

namespace qt {

class zchxWarningZoneDataMgr : public zchxTemplateDataMgr<WarningZoneElement, ZCHX::Data::ITF_WarringZone>
{
public:
    explicit zchxWarningZoneDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<WarningZoneElement, ZCHX::Data::ITF_WarringZone>(w, ZCHX::DATA_MGR_WARNING_ZONE, ZCHX::LAYER_DEFENCE, parent) {}
    bool updateActiveItem(const QPoint &pt);
};

class zchxCoastDataMgr : public zchxTemplateDataMgr<CoastElement, ZCHX::Data::ITF_CoastData>
{
public:
    explicit zchxCoastDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<CoastElement, ZCHX::Data::ITF_CoastData>(w, ZCHX::DATA_MGR_COAST, ZCHX::LAYER_COASTDATA, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxSeabedPipLineDataMgr : public zchxTemplateDataMgr<SeabedPipeLineElement, ZCHX::Data::ITF_SeabedPipeLine>
{
public:
    explicit zchxSeabedPipLineDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<SeabedPipeLineElement, ZCHX::Data::ITF_SeabedPipeLine>(w, ZCHX::DATA_MGR_SEABEDIPLINE, ZCHX::LAYER_SEABEDPIPELINE, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxChannelDataMgr : public zchxTemplateDataMgr<ChannelElement, ZCHX::Data::ITF_Channel>
{
public:
    explicit zchxChannelDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<ChannelElement, ZCHX::Data::ITF_Channel>(w, ZCHX::DATA_MGR_CHANNEL,  ZCHX::LAYER_CHANNEL, parent){}
    void selectChannelLine(int channelId, const ZCHX::Data::ITF_ChannelLine & line); // 选择航道线
    void cancelChannelLine(int channelId); // 取消航道线
    virtual void importData(const std::vector<std::pair<double, double> > & path);
    bool updateActiveItem(const QPoint &pt);
};

class zchxStructureDataMgr : public zchxTemplateDataMgr<StructureElement, ZCHX::Data::ITF_Structure>
{
public:
    explicit zchxStructureDataMgr(zchxMapWidget* w, QObject *parent = 0) :
        zchxTemplateDataMgr<StructureElement, ZCHX::Data::ITF_Structure>(w, ZCHX::DATA_MGR_STRUCTURE, ZCHX::LAYER_STRUCTURE, parent) {}
};

class zchxAreaNetDataMgr : public zchxTemplateDataMgr<AreaNetElement, ZCHX::Data::ITF_AreaNet>
{
public:
    explicit zchxAreaNetDataMgr(zchxMapWidget* w, QObject *parent = 0):
       zchxTemplateDataMgr<AreaNetElement, ZCHX::Data::ITF_AreaNet>(w, ZCHX::DATA_MGR_AREANET, ZCHX::LAYER_AREANET, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxMooringDataMgr : public zchxTemplateDataMgr<MooringElement, ZCHX::Data::ITF_Mooring>
{
public:
    explicit zchxMooringDataMgr(zchxMapWidget* w, QObject *parent = 0) :
        zchxTemplateDataMgr<MooringElement, ZCHX::Data::ITF_Mooring>(w, ZCHX::DATA_MGR_MOOR, ZCHX::LAYER_MOORING, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
    bool updateActiveItem(const QPoint &pt);
};

class zchxCardMouthDataMgr : public zchxTemplateDataMgr<CardMouthElement, ZCHX::Data::ITF_CardMouth>
{
public:
    explicit zchxCardMouthDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<CardMouthElement, ZCHX::Data::ITF_CardMouth>(w, ZCHX::DATA_MGR_CARDMOUTH, ZCHX::LAYER_CARDMOUTH, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
    bool updateActiveItem(const QPoint &pt);
};

class zchxPastrolStationDataMgr : public zchxTemplateDataMgr<PastrolStationElement, ZCHX::Data::ITF_PastrolStation>
{
public:
    explicit zchxPastrolStationDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<PastrolStationElement, ZCHX::Data::ITF_PastrolStation>(w, ZCHX::DATA_MGR_PASTROLSTATION, ZCHX::LAYER_PATROL_SITE, parent) {}
};

class zchxLocalMarkDataMgr : public zchxTemplateDataMgr<LocalMarkElement, ZCHX::Data::ITF_LocalMark>
{
public:
    explicit zchxLocalMarkDataMgr(zchxMapWidget* w, QObject *parent = 0) :
        zchxTemplateDataMgr<LocalMarkElement, ZCHX::Data::ITF_LocalMark>(w, ZCHX::DATA_MGR_LOCAL_MARK, ZCHX::LAYER_LOCALMARK, parent) {}
};

class zchxDangerousDataMgr : public zchxTemplateDataMgr<DangerousCircleElement, ZCHX::Data::ITF_DangerousCircle>
{
public:
    explicit zchxDangerousDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<DangerousCircleElement, ZCHX::Data::ITF_DangerousCircle>(w, ZCHX::DATA_MGR_DANGEROUS, ZCHX::LAYER_DANGEROUS_CIRLE, parent) {}
};

class zchxIslandLineDataMgr : public zchxTemplateDataMgr<IslandLineElement, ZCHX::Data::ITF_IslandLine>
{
public:
    explicit zchxIslandLineDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<IslandLineElement, ZCHX::Data::ITF_IslandLine>(w, ZCHX::DATA_MGR_ISLANDLINE, ZCHX::LAYER_ISLAND, parent){}
};

class zchxShipAlarmAscendDataMgr : public zchxTemplateDataMgr<ShipAlarmAscendElement, ZCHX::Data::ITF_ShipAlarmAscend>
{
public:
    explicit zchxShipAlarmAscendDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<ShipAlarmAscendElement, ZCHX::Data::ITF_ShipAlarmAscend>(w, ZCHX::DATA_MGR_SHIPALARM_ASCEND, ZCHX::LAYER_ALARMASCEND, parent){}
    void show(QPainter* painter);
};

class zchxRadarFeatureZoneDataMgr : public zchxTemplateDataMgr<RadarFeatureZoneElement, ZCHX::Data::ITF_RadarFeaturesZone>
{
public:
    explicit zchxRadarFeatureZoneDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<RadarFeatureZoneElement, ZCHX::Data::ITF_RadarFeaturesZone>(w, ZCHX::DATA_MGR_RADAR_FEATURE_ZONE, ZCHX::LAYER_RADAR_FRETURE_AREA, parent){}
};

class zchxAisBaseStationDataMgr : public zchxTemplateDataMgr<AISBaseStationElement, ZCHX::Data::ITF_AISBASESTATION>
{
    explicit zchxAisBaseStationDataMgr(zchxMapWidget* w, QObject *parent = 0):
        zchxTemplateDataMgr<AISBaseStationElement, ZCHX::Data::ITF_AISBASESTATION>(w, ZCHX::DATA_MGR_AIS_STATION, ZCHX::LAYER_AIS_STATION, parent){}

};


}

#endif // ZCHXUSERDEFINESDATAMGR_H
