#ifndef ZCHXUSERDEFINESDATAMGR_H
#define ZCHXUSERDEFINESDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include <QPainter>
#include <QMutex>

class zchxMapWidget;

class zchxUserDefinesDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxUserDefinesDataMgr(zchxMapWidget* w, QObject *parent = 0);
    //防区数据
    QList<ZCHX::Data::ITF_WarringZone> getAllWarrningZone() const;
    void setWarringZONEData(const std::vector<DrawElement::WarringZONE> &data);
    void updateWarrningZone(const QList<ZCHX::Data::ITF_WarringZone> &zonelist);
    void updateWarrningZone(const ZCHX::Data::ITF_WarringZone &zone);
    void removeWarrningZone(const ZCHX::Data::ITF_WarringZone &zone);
    DrawElement::WarringZONE* zchxWarringZoneItem(const std::string &name);
    bool zchxWarringZoneData4id(int uuid, ZCHX::Data::ITF_WarringZone &info); //id获取防区信息
    bool zchxWarringZoneDataByName(const QString &name, ZCHX::Data::ITF_WarringZone &info); //名称获取防区信息
    bool zchxWarringZoneDataByName(const std::string &name, ZCHX::Data::ITF_WarringZone &info);

    //海岸数据
    void updateCoastDataZone(const QList<ZCHX::Data::ITF_CoastData> &zonelist);
    void updateCoastDataZone(const ZCHX::Data::ITF_CoastData &zone);
    void setCoastData(const std::vector<DrawElement::CoastData> & data); //设置海岸数据
    void importCoastData(const std::vector<std::pair<double, double> > & path); //导入海岸数据
    DrawElement::CoastData* zchxCoastDataZoneItem(const std::string &name);

    //底管线
    void updateSeabedPipeLineZone(const QList<ZCHX::Data::ITF_SeabedPipeLine> &zonelist);
    void updateSeabedPipeLineZone(const ZCHX::Data::ITF_SeabedPipeLine &zone);
    void setSeabedPipeLine(const std::vector<DrawElement::SeabedPipeLine> & data); //设置海底管线
    void importSeabedPipeLine(const std::vector<std::pair<double, double> > & path); //导入海底管线
    DrawElement::SeabedPipeLine *zchxSeabedPipeLineZoneItem(const std::string &name);

    //结构物
    void updateStructureZone(const QList<ZCHX::Data::ITF_Structure> &zonelist);
    void updateStructureZone(const ZCHX::Data::ITF_Structure &zone);
    void setStructure(const std::vector<DrawElement::Structure> & data); //设置结构物
    DrawElement::Structure *zchxStructureZoneItem(const std::string &name);

    //地理区域网络
    void updateAreaNetZone(const QList<ZCHX::Data::ITF_AreaNet> &zonelist);
    void updateAreaNetZone(const ZCHX::Data::ITF_AreaNet &zone);
    void setAreaNet(const std::vector<DrawElement::AreaNet> & data); //设置地理区域网络
    void importAreaNet(const std::vector<std::pair<double, double> > & path); //导入地理区域网络
    DrawElement::AreaNet *zchxAreaNetZoneItem(const std::string &name);

    //航道
    void updateChannelZone(const QList<ZCHX::Data::ITF_Channel> &zonelist);
    void updateChannelZone(const ZCHX::Data::ITF_Channel &zone);
    void setChannel(const std::vector<DrawElement::Channel> & data); //设置航道
    void importChannel(const std::vector<std::pair<double, double> > & path); //导入航道
    void selectChannelLine(int channelId, const ZCHX::Data::ITF_ChannelLine & line); // 选择航道线
    void cancelChannelLine(int channelId); // 取消航道线
    DrawElement::Channel *zchxChannelZoneItem(const std::string &name);
    bool zchxChannelZoneData4id(int uuid, ZCHX::Data::tagITF_Channel &info);

    //锚泊
    void updateMooringZone(const QList<ZCHX::Data::ITF_Mooring> &zonelist);
    void updateMooringZone(const ZCHX::Data::ITF_Mooring &zone);
    void setMooring(const std::vector<DrawElement::Mooring> & data); //设置锚泊
    void importMooring(const std::vector<std::pair<double, double> > & path); //导入锚泊
    DrawElement::Mooring *zchxMooringZoneItem(const std::string &name);
    bool zchxMooringZoneData4id(int uuid, ZCHX::Data::tagITF_Mooring &info);

    //卡口
    void updateCardMouthZone(const QList<ZCHX::Data::ITF_CardMouth> &zonelist);
    void updateCardMouthZone(const ZCHX::Data::ITF_CardMouth &zone);
    void setCardMouth(const std::vector<DrawElement::CardMouth> & data); //设置卡口
    void importCardMouth(const std::vector<std::pair<double, double> > & path); //导入卡口
    DrawElement::CardMouth *zchxCardMouthZoneItem(const std::string &name);
    bool zchxCardMouthZoneData4id(int uuid, ZCHX::Data::tagITF_CardMouth &info);

    //环岛线
    void setIslandLineData(const std::vector<DrawElement::IslandLine> &data);
    bool zchxIslandData4id(int uuid, ZCHX::Data::ITF_IslandLine &info);

    //位置标注
    void setLocalMarkData(const std::vector<DrawElement::LocalMark> & data);
    //危险圈
    void setDangerousCircleData(const std::vector<DrawElement::DangerousCircle> &data);
    //巡逻站
    void setPastrolStation(const std::vector<DrawElement::PastrolStation> &data);

signals:

public slots:


private:
    zchxMapWidget       *mDisplayWidget;
    std::vector<DrawElement::WarringZONE> m_WarringZone;        //告警区域数据也叫防区
    std::vector<DrawElement::IslandLine> m_IslandLine;          //环岛线
    std::vector<DrawElement::CoastData> m_coastDataLine;           //海岸数据
    std::vector<DrawElement::SeabedPipeLine> m_seabedPipeLineLine; //海底管线
    std::vector<DrawElement::Structure> m_structurePoint;          //结构物
    std::vector<DrawElement::AreaNet> m_areaNetZone;               //地理区域网络
    std::vector<DrawElement::Channel> m_channelZone;               //航道
    std::vector<DrawElement::Mooring> m_mooringZone;               //锚泊
    std::vector<DrawElement::CardMouth> m_cardMouthZone;               //卡口
    std::vector<DrawElement::LocalMark> m_LocalMark;            //位置标注对像
    std::vector<DrawElement::DangerousCircle> m_DangerousCircle;        //危险圈数据
    std::vector<DrawElement::PastrolStation> m_PastrolStation;  //巡逻站
};

#endif // ZCHXUSERDEFINESDATAMGR_H
