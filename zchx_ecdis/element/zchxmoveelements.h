#ifndef ZCHXUSERDEFINEMOVEELEMENTS_H
#define ZCHXUSERDEFINEMOVEELEMENTS_H

#include "moveelement.h"

namespace qt {
//防区及防线
class ZCHX_ECDIS_EXPORT WarningZoneElement : public zchxMoveElement<ZCHX::Data::ITF_WarringZone>
{
public:
    explicit WarningZoneElement(const ZCHX::Data::ITF_WarringZone &ele, zchxMapWidget* w)
        : zchxMoveElement<ZCHX::Data::ITF_WarringZone>(ele, ZCHX::Data::ELE_WARNING_ZONE, ZCHX::LAYER_DEFENCE, w) {}

    QString getDefenceColor(){return m_data.fillColor;}
    QString getWarnColor(){return m_data.warnColor;}
    void drawElement(QPainter* painter);
    void clicked(bool isDouble);
};

//海岸数据
class ZCHX_ECDIS_EXPORT CoastElement : public zchxMoveElement<ZCHX::Data::ITF_CoastData>
{
public:
    explicit CoastElement(const ZCHX::Data::ITF_CoastData &ele, zchxMapWidget* w)
        : zchxMoveElement<ZCHX::Data::ITF_CoastData>(ele, ZCHX::Data::ELE_COAST, ZCHX::LAYER_COASTDATA, w) {}
    void drawElement(QPainter* painter);
    void clicked(bool isDouble){}
};

//海底岸线数据
class ZCHX_ECDIS_EXPORT SeabedPipeLineElement : public zchxMoveElement<ZCHX::Data::ITF_SeabedPipeLine>
{
public:
    explicit SeabedPipeLineElement(const ZCHX::Data::ITF_SeabedPipeLine &ele,  zchxMapWidget* w)
        : zchxMoveElement<ZCHX::Data::ITF_SeabedPipeLine>(ele, ZCHX::Data::ELE_SEABEDPIPLINE, ZCHX::LAYER_SEABEDPIPELINE, w) {}
    void drawElement(QPainter* painter);
    void clicked(bool isDouble){}
};

//地理区域网络
class ZCHX_ECDIS_EXPORT AreaNetElement : public zchxMoveElement<ZCHX::Data::ITF_AreaNet>
{
public:
    explicit AreaNetElement(const ZCHX::Data::ITF_AreaNet &ele, zchxMapWidget* w)
        : zchxMoveElement<ZCHX::Data::ITF_AreaNet>(ele, ZCHX::Data::ELE_AREA_NET, ZCHX::LAYER_AREANET, w) {}
    void drawElement(QPainter *painter);
    void clicked(bool isDouble){}
};

//卡口
class ZCHX_ECDIS_EXPORT CardMouthElement : public zchxMoveElement<ZCHX::Data::ITF_CardMouth>
{
public:
    explicit CardMouthElement(const ZCHX::Data::ITF_CardMouth &ele, zchxMapWidget* w)
        : zchxMoveElement<ZCHX::Data::ITF_CardMouth>(ele, ZCHX::Data::ELE_CARD_MOUTH, ZCHX::LAYER_CARDMOUTH, w) {}
    void drawElement(QPainter *painter);
    void clicked(bool isDouble);
};

//航道
class ZCHX_ECDIS_EXPORT ChannelElement : public zchxMoveElement<ZCHX::Data::ITF_Channel>
{
public:
    explicit ChannelElement(const ZCHX::Data::ITF_Channel &ele, zchxMapWidget* w)
        : zchxMoveElement<ZCHX::Data::ITF_Channel>(ele, ZCHX::Data::ELE_CHANNEL, ZCHX::LAYER_CHANNEL, w) {}
    void drawElement(QPainter *painter);
    void setLineSelected(int i, bool selectStatus);
    void clicked(bool isDouble);
};

//锚泊
class ZCHX_ECDIS_EXPORT MooringElement : public zchxMoveElement<ZCHX::Data::ITF_Mooring>
{
public:
    explicit MooringElement(const ZCHX::Data::ITF_Mooring &ele, zchxMapWidget* w)
        : zchxMoveElement<ZCHX::Data::ITF_Mooring>(ele, ZCHX::Data::ELE_MOOR, ZCHX::LAYER_MOORING, w) {}
    void drawElement(QPainter *painter);
    void clicked(bool isDouble);
};
}


#endif // ZCHXUSERDEFINEMOVEELEMENTS_H
