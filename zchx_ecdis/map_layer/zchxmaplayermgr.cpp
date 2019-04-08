#include "zchxmaplayermgr.h"
#include "zchxmapwidget.h"
#include "zchxaismaplayer.h"
#include "zchxradarvideolayer.h"
#include "zchxshipalarmascendlayer.h"
#include "../profiles.h"

namespace qt {
MapLayerMgr* MapLayerMgr::minstance = 0;
MapLayerMgr::MGarbage MapLayerMgr::Garbage;

MapLayerMgr::MapLayerMgr(QObject *parent) : QObject(parent), m_drawWidget(0)
{

}

MapLayerMgr::~MapLayerMgr()
{
        qDebug()<<__FUNCTION__<<__LINE__;
}

MapLayerMgr* MapLayerMgr::instance()
{
    if(minstance == 0)
    {
        minstance = new MapLayerMgr;
    }
    return minstance;
}

zchxAisMapLayer* MapLayerMgr::getAisLayer()
{
    zchxAisMapLayer* layer = dynamic_cast<zchxAisMapLayer*>(getLayer(ZCHX::LAYER_AIS).get());
    return layer;
}

void MapLayerMgr::clear()
{
    //removeAllElement();
    removeAllLayer();
    setDrawWidget(0);
}

void MapLayerMgr::removeAllElement()
{
    for(std::shared_ptr<MapLayer> layer : m_layerList)
    {
        if(layer) layer->removeAllElement();
    }
}

void MapLayerMgr::removeAllLayer()
{
    m_layerList.clear();
    m_layerTree.clear();
}

void MapLayerMgr::loadEcdisLayers()
{
    //雷达回波图层
     addLayer(std::shared_ptr<MapLayer>(new zchxRadarVideoLayer(m_drawWidget, true)));
    //ais
    addLayer(std::shared_ptr<MapLayer>(new zchxAisMapLayer(m_drawWidget, true)));
    addLayer(ZCHX::LAYER_HISTORY_AIS, ZCHX::TR_LAYER_HISTORY_AIS, true, ZCHX::LAYER_AIS);  //AIS历史轨迹
    //雷达
    addLayer(ZCHX::LAYER_RADAR, ZCHX::TR_LAYER_RADAR, true);

    //添加相机
    addLayer(ZCHX::LAYER_CAMERA, ZCHX::TR_LAYER_CAMERA, true);
    addLayer(ZCHX::LAYER_CAMERANETGRID, ZCHX::TR_LAYER_CAMERANETGRID, true);
    addLayer(ZCHX::LAYER_CAMERA_VIEW, ZCHX::TR_LAYER_CAMERA_VIEW, true);
    addLayer(ZCHX::LAYER_GDY, ZCHX::TR_LAYER_GDY, true);                    //光电仪
    addLayer(ZCHX::LAYER_UAV, ZCHX::TR_LAYER_UAV, true);                    //无人机

    //添加防区图层
    addLayer(ZCHX::LAYER_DEFENCE, ZCHX::TR_LAYER_DEFENCE, true);

    //经纬网格
    addLayer(ZCHX::LAYER_LONLAT_GIRD,ZCHX::TR_LAYER_LONLAT_GIRD,false);

    //航标图层
    addLayer(ZCHX::LAYER_NAVIGATION_MARK,ZCHX::TR_LAYER_NAVIGATION_MARK,true);

    //添加塔杆
    addLayer(ZCHX::LAYER_TOWER_ROD,ZCHX::TR_LAYER_TOWER_ROD,true);

    //添加航道图层
    addLayer(ZCHX::LAYER_CHANNEL, ZCHX::TR_LAYER_CHANNEL, true);

    //添加锚泊图层
    addLayer(ZCHX::LAYER_MOORING, ZCHX::TR_LAYER_MOORING, true);

    //添加自定义区域图层
    addLayer(ZCHX::LAYER_DEFINEZONE, ZCHX::TR_LAYER_DEFINEZONE, true);

    //添加卡口图层
    addLayer(ZCHX::LAYER_CARDMOUTH, ZCHX::TR_LAYER_CARDMOUTH, true);

    //添加船舶预警图层
     addLayer(std::shared_ptr<MapLayer>(new zchxShipAlarmAscendLayer(m_drawWidget, true)));

    //添加危险圈图层
    addLayer(ZCHX::LAYER_DANGEROUS_CIRLE, ZCHX::TR_LAYER_DANGEROUS_CIRLE, true);

    //本地标注
    addLayer(ZCHX::LAYER_LOCALMARK, ZCHX::TR_LAYER_LOCALMARK, true);
    //天气图层
    addLayer(ZCHX::LAYER_WEATHER, ZCHX::TR_LAYER_WEATHER, true);

    addLayer(ZCHX::LAYER_GPS, ZCHX::TR_LAYER_GPS, true);
    addLayer(ZCHX::LAYER_GPS_CURRENT, ZCHX::TR_LAYER_GPS_CURRENT, true, ZCHX::LAYER_GPS);
    addLayer(ZCHX::LAYER_GPS_TRACK, ZCHX::TR_LAYER_GPS_TRACK, true,ZCHX::LAYER_GPS);

    addLayer(ZCHX::LAYER_CDMA, ZCHX::TR_LAYER_CDMA, true);
    addLayer(ZCHX::LAYER_CDMA_TARGET, ZCHX::TR_LAYER_CDMA_TARGET, true, ZCHX::LAYER_CDMA);
    addLayer(ZCHX::LAYER_CDMA_TRACK, ZCHX::TR_LAYER_CDMA_TRACK, true,ZCHX::LAYER_CDMA);
    addLayer(ZCHX::LAYER_BIGDIPPER, ZCHX::TR_LAYER_BIGDIPPER, true);
    addLayer(ZCHX::LAYER_BIGDIPPER_TARGET, ZCHX::TR_LAYER_BIGDIPPER_TARGET, true, ZCHX::LAYER_BIGDIPPER);
    addLayer(ZCHX::LAYER_BIGDIPPER_TRACK, ZCHX::TR_LAYER_BIGDIPPER_TRACK, true, ZCHX::LAYER_BIGDIPPER);

    addLayer(ZCHX::LAYER_SPECIAL_ROUTELINE, ZCHX::TR_LAYER_SPECIAL_ROUTELINE, true);
    addLayer(ZCHX::LAYER_HIGHLEVEL, ZCHX::TR_LAYER_HIGHLEVEL, true);
    addLayer(ZCHX::LAYER_IMAGEMAP, ZCHX::TR_LAYER_IMAGEMAP, true);

    addLayer(ZCHX::LAYER_COASTDATA, ZCHX::TR_LAYER_COASTDATA, true);
    addLayer(ZCHX::LAYER_SEABEDPIPELINE, ZCHX::TR_LAYER_SEABEDPIPELINE, true);
    addLayer(ZCHX::LAYER_STRUCTURE, ZCHX::TR_LAYER_STRUCTURE, true);
    addLayer(ZCHX::LAYER_AREANET, ZCHX::TR_LAYER_AREANET, true);
    addLayer(ZCHX::LAYER_AIS_STATION, ZCHX::TR_LAYER_AIS_STATION, true);
    addLayer(ZCHX::LAYER_AIS_FUSION, ZCHX::TR_LAYER_AIS_FUSION, true);
    addLayer(ZCHX::LAYER_TEN_GRID, ZCHX::TR_LAYER_TEN_GRID, true);
    addLayer(ZCHX::LAYER_THIRTY_GRID, ZCHX::TR_LAYER_THIRTY_GRID, true);
    addLayer(ZCHX::LAYER_CAMERA_REGION, ZCHX::TR_LAYER_CAMERA_REGION, true);
    addLayer(ZCHX::LAYER_VESSEL_TARGET, ZCHX::TR_LAYER_VESSEL_TARGET, true);
    addLayer(ZCHX::LAYER_VESSEL_TRACK, ZCHX::TR_LAYER_VESSEL_TRACK, true);
    addLayer(ZCHX::LAYER_RECT, ZCHX::LAYER_RECT, true);
    addLayer(ZCHX::LAYER_RADAR_SITE, ZCHX::TR_LAYER_RADAR_STATION, true);
    addLayer(ZCHX::LAYER_PATROL_SITE, ZCHX::TR_LAYER_PATROL, true);

    //添加雷达配置
    std::shared_ptr<MapLayer> layer = MapLayerMgr::instance()->getLayer(ZCHX::LAYER_RADAR);
    if(layer){
        layer->setMaxConcernNum(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_CONCERN_NUM, 10).toInt());
        layer->setConcernReplace(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_REPLACE_CONCERN, true).toBool());
        layer->setMaxRealtimeTrackNum(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_TAIL_TRACK_NUM, 10).toInt());
        layer->setRealtimeTrackReplace(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_REPLACE_TAIL_TRACK, true).toBool());
        layer->setMaxHistoryTrackNum(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_HISTORY_TRACK_NUM, 10).toInt());
        layer->setHistoryTrackReplace(Profiles::instance()->value(RADAR_DISPLAY_SETTING, RADAR_REPLACE_HISTORY_TRACK, true).toBool());
    }

}

void MapLayerMgr::loadLayers()
{
    QString xmlPath = QString("%1/mapdata/maplayers.xml").arg(QApplication::applicationDirPath());
    QFile xmlFile(xmlPath);
    if(!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly))
    {
        qWarning() << "Map Layer's configuration file has error: " << xmlPath;
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&xmlFile))
    {
        qWarning() << "xml file have error: " << xmlPath;
        return;
    }
    QDomElement rootNode = doc.documentElement();
    QDomElement layerNode = rootNode.firstChildElement("MapLayer");
    while(!layerNode.isNull())
    {
        _readMapLayerNode(layerNode);
        layerNode = layerNode.nextSiblingElement("MapLayer");
    }

    return;
}

void MapLayerMgr::_readMapLayerNode(QDomElement node, std::shared_ptr<MapLayer> parent)
{
    if(node.isNull()) return;
    QString type = node.attribute("type");
    QString trSource = node.attribute("tr");
    bool visible = (node.attribute("visible") == "true");
    QString strMode = node.attribute("mode");

    std::shared_ptr<MapLayer> layer(new MapLayer(type, trSource, visible) );
    if(strMode.contains("display"))
    {
        layer->setMode(ZCHX::Data::ECDIS_PLUGIN_USE_DISPLAY_MODEL);
    }
    if(strMode.contains("editor"))
    {
        layer->setMode(ZCHX::Data::ECDIS_PLUGIN_USE_EDIT_MODEL);
    }

    addLayer(layer, parent);

    //detect children layers
    QDomElement layerNode = node.firstChildElement("MapLayer");
    while(!layerNode.isNull())
    {
        _readMapLayerNode(layerNode, layer);
        layerNode = layerNode.nextSiblingElement("MapLayer");
    }
}

void MapLayerMgr::addLayer(std::shared_ptr<MapLayer> layer, std::shared_ptr<MapLayer> parent)
{
    if(!layer) return;
    qDebug()<<"layer:"<<layer->type()<<layer->displayName()<<" parent:"<<(parent.get() == NULL ? "" : parent->type());
    if(containsLayer(layer->type()))
    {
        qDebug() << "Layer: " << layer->type() << " had been added!";
        return;
    }

    //connect(layer.get(), &MapLayer::visibleChanged, this, &DrawWidget::_maplayerVisibleChanged, Qt::UniqueConnection);

    if(parent)
    {
        parent->addChildLayer(layer);
    } else
    {
        m_layerTree.push_back(layer);
    }
    m_layerList.push_back(layer);
}

void MapLayerMgr::addLayer(const QString& curLayer, const QString& curDisplayName, bool curVisible, const QString &parentLayer)
{
    if(curLayer.isEmpty()) return;
    //check curlayer exist or not
    if(containsLayer(curLayer))
    {
        qDebug() << "Layer: " << curLayer << " had been added!";
        return;
    }
    addLayer(std::shared_ptr<MapLayer>(new MapLayer(m_drawWidget, curLayer, curDisplayName, curVisible)), getLayer(parentLayer));
}

bool MapLayerMgr::containsLayer(const QString &type) const
{
    for(std::shared_ptr<MapLayer> layer : m_layerList)
    {
        if(layer->type() == type)
            return true;
    }
    return false;
}

QStringList MapLayerMgr::getLayerList() const
{
    QStringList list;
    for(std::shared_ptr<MapLayer> layer : m_layerList)
    {
        list.append(layer->type() );
    }
    return list;
}

std::shared_ptr<MapLayer> MapLayerMgr::getLayer(const QString &type)
{
    std::shared_ptr<MapLayer> nullLayer;
    for(std::shared_ptr<MapLayer> layer : m_layerList)
    {
        if(layer->type() == type)
            return layer;
    }
    return nullLayer;
}

const std::list<std::shared_ptr<MapLayer> > &MapLayerMgr::getLayerTree()
{
    return m_layerTree;
}

bool MapLayerMgr::isLayerVisible(const QString &type)
{
    if(type.isEmpty()) return true;

    std::shared_ptr<MapLayer> layer = getLayer(type);

    //    qDebug()<<type<<layer.get()<<layer->visible();

    return (layer && layer->visible());
}

bool MapLayerMgr::isLayerVisible(std::shared_ptr<MapLayer> layer)
{
    if(!layer) return false;
    return isLayerVisible(layer->type());
}

bool MapLayerMgr::isAnyLayerVisible(const QString &type1, const QString &type2, const QString &type3, const QString &type4, const QString &type5)
{
    return (isLayerVisible(type1) || isLayerVisible(type2) || isLayerVisible(type3) || isLayerVisible(type4) || isLayerVisible(type5));
}

void MapLayerMgr::show(QPainter *painter)
{
    for(std::shared_ptr<MapLayer> layer : m_layerTree)
    {
        if(!layer) continue;
        if(layer->visible())
        {
            layer->drawLayer(painter);
        }
    }
}
}
