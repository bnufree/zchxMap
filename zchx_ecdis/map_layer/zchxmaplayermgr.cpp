#include "zchxmaplayermgr.h"
#include "zchxmapwidget.h"
#include <QDomDocument>

namespace qt {
MapLayerMgr::MapLayerMgr(zchxMapWidget* w, QObject *parent) : QObject(parent),
    mDisplayWidget(w)
{

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

    std::shared_ptr<MapLayer> layer(new MapLayer(mDisplayWidget, type, trSource, visible) );
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

bool MapLayerMgr::isAnyLayerVisible(const QString &type1, const QString &type2, const QString &type3, const QString &type4, const QString &type5)
{
    return (isLayerVisible(type1) || isLayerVisible(type2) || isLayerVisible(type3) || isLayerVisible(type4) || isLayerVisible(type5));
}
}