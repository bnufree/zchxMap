#ifndef ZCHXUSERDEFINESDATAMGR_H
#define ZCHXUSERDEFINESDATAMGR_H

#include "zchxecdisdatamgr.h"

namespace qt {
template<typename K, typename T>
class zchxUserDefinesDataMgr : public zchxEcdisDataMgr
{
    Q_OBJECT
public:
    explicit zchxUserDefinesDataMgr(zchxMapWidget* w, int type, const QString& layer, QObject *parent = 0);
    virtual void show(QPainter *painter);
    virtual bool updateActiveItem(const QPoint &pt);
    //数据管理
    QList<T> getData() const;
    void setData(const QList<T> &list);
    void updateData(const QList<T> &list);
    void updateData(const T &zone);
    void removeData(const T &zone);
    virtual void importData(const std::vector<std::pair<double, double> > & path) {}
    K* item(const std::string &name);
    K* item(int id);
    bool data4id(int uuid, T &info); //id获取防区信息
    bool dataByName(const QString &name, T &info); //名称获取防区信息
    bool dataByName(const std::string &name, T &info);


protected:
    QList<std::shared_ptr<K>> mData;        //告警区域数据也叫防区
    QString                   mLayerName;
};

class zchxWarningZoneDataMgr : public zchxUserDefinesDataMgr<WarningZoneElement, ZCHX::Data::ITF_WarringZone>
{
    Q_OBJECT
public:
    explicit zchxWarningZoneDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<WarningZoneElement, ZCHX::Data::ITF_WarringZone>(w, ZCHX_DATA_MGR_WARNING_ZONE, ZCHX::LAYER_DEFENCE, parent) {}
};

class zchxCoastDataMgr : public zchxUserDefinesDataMgr<CoastElement, ZCHX::Data::ITF_CoastData>
{
    Q_OBJECT
public:
    explicit zchxCoastDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<CoastElement, ZCHX::Data::ITF_CoastData>(w, ZCHX_DATA_MGR_COAST, ZCHX::LAYER_COASTDATA, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxSeabedPipLineDataMgr : public zchxUserDefinesDataMgr<SeabedPipeLineElement, ZCHX::Data::ITF_SeabedPipeLine>
{
    Q_OBJECT
public:
    explicit zchxSeabedPipLineDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<SeabedPipeLineElement, ZCHX::Data::ITF_SeabedPipeLine>(w, ZCHX_DATA_MGR_SEABEDIPLINE, ZCHX::LAYER_SEABEDPIPELINE, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxChannelDataMgr : public zchxUserDefinesDataMgr<ChannelElement, ZCHX::Data::ITF_Channel>
{
    Q_OBJECT
public:
    explicit zchxChannelDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<ChannelElement, ZCHX::Data::ITF_Channel>(w, ZCHX_DATA_MGR_CHANNEL,  ZCHX::LAYER_CHANNEL, parent){}
    void selectChannelLine(int channelId, const ZCHX::Data::ITF_ChannelLine & line); // 选择航道线
    void cancelChannelLine(int channelId); // 取消航道线
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxStructureDataMgr : public zchxUserDefinesDataMgr<StructureElement, ZCHX::Data::ITF_Structure>
{
    Q_OBJECT
public:
    explicit zchxStructureDataMgr(zchxMapWidget* w, QObject *parent) :
        zchxUserDefinesDataMgr<StructureElement, ZCHX::Data::ITF_Structure>(w, ZCHX_DATA_MGR_STRUCTURE, ZCHX::LAYER_STRUCTURE, parent) {}
};

class zchxAreaNetDataMgr : public zchxUserDefinesDataMgr<AreaNetElement, ZCHX::Data::ITF_AreaNet>
{
    Q_OBJECT
public:
    explicit zchxAreaNetDataMgr(zchxMapWidget* w, QObject *parent):
       zchxUserDefinesDataMgr<AreaNetElement, ZCHX::Data::ITF_AreaNet>(w, ZCHX_DATA_MGR_AREANET, ZCHX::LAYER_AREANET, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxMooringDataMgr : public zchxUserDefinesDataMgr<MooringElement, ZCHX::Data::ITF_Mooring>
{
    Q_OBJECT
public:
    explicit zchxMooringDataMgr(zchxMapWidget* w, QObject *parent) :
        zchxUserDefinesDataMgr<MooringElement, ZCHX::Data::ITF_Mooring>(w, ZCHX_DATA_MGR_MOOR, ZCHX::LAYER_MOORING, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxCardMouthDataMgr : public zchxUserDefinesDataMgr<CardMouthElement, ZCHX::Data::ITF_CardMouth>
{
    Q_OBJECT
public:
    explicit zchxCardMouthDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<CardMouthElement, ZCHX::Data::ITF_CardMouth>(w, ZCHX_DATA_MGR_CARDMOUTH, ZCHX::LAYER_CARDMOUTH, parent) {}
    virtual void importData(const std::vector<std::pair<double, double> > & path);
};

class zchxPastrolStationDataMgr : public zchxUserDefinesDataMgr<PastrolStationElement, ZCHX::Data::ITF_PastrolStation>
{
    Q_OBJECT
public:
    explicit zchxPastrolStationDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<PastrolStationElement, ZCHX::Data::ITF_PastrolStation>(w, ZCHX_DATA_MGR_PASTROLSTATION, ZCHX::LAYER_PATROL_SITE, parent) {}
};

class zchxLocalMarkDataMgr : public zchxUserDefinesDataMgr<LocalMark, ZCHX::Data::ITF_LocalMark>
{
    Q_OBJECT
public:
    explicit zchxLocalMarkDataMgr(zchxMapWidget* w, QObject *parent) :
        zchxUserDefinesDataMgr<LocalMark, ZCHX::Data::ITF_LocalMark>(w, ZCHX_DATA_MGR_LOCAL_MARK, ZCHX::LAYER_LOCALMARK, parent) {}
};

class zchxDangerousDataMgr : public zchxUserDefinesDataMgr<DangerousCircle, ZCHX::Data::ITF_DangerousCircle>
{
    Q_OBJECT
public:
    explicit zchxDangerousDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<DangerousCircle, ZCHX::Data::ITF_DangerousCircle>(w, ZCHX_DATA_MGR_DANGEROUS, ZCHX::LAYER_DANGEROUS_CIRLE, parent) {}
};

class zchxIslandLineDataMgr : public zchxUserDefinesDataMgr<IslandLineElement, ZCHX::Data::ITF_IslandLine>
{
    Q_OBJECT
public:
    explicit zchxIslandLineDataMgr(zchxMapWidget* w, QObject *parent):
        zchxUserDefinesDataMgr<IslandLineElement, ZCHX::Data::ITF_IslandLine>(w, ZCHX_DATA_MGR_ISLANDLINE, ZCHX::LAYER_ISLAND, parent){}
};

}

#endif // ZCHXUSERDEFINESDATAMGR_H
