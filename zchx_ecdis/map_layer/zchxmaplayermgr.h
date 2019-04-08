#ifndef ZCHXMAPLAYERMGR_H
#define ZCHXMAPLAYERMGR_H

#include <QObject>
#include <QDomDocument>
#include "zchxMapLayer.h"

namespace qt {
class zchxMapWidget;
class zchxAisMapLayer;

#define         LayerMgr          MapLayerMgr::instance()

class ZCHX_ECDIS_EXPORT MapLayerMgr : public QObject
{
    Q_OBJECT
public:
    ~MapLayerMgr();
    static MapLayerMgr *instance();
    void setDrawWidget(zchxMapWidget* w) {m_drawWidget = w;}
    zchxMapWidget * drawWidget() const {return m_drawWidget;}
    zchxAisMapLayer* getAisLayer();
    void removeAllElement();
    void removeAllLayer();
    void clear();


    //地图图层管理
    //从配置文件加载地图图层配置文件,图层的顺序按照从上到下的顺序, 最上面为最顶层
    void loadLayers();
    void loadEcdisLayers();
    //添加图层 layer 父图层, 默认为空
    void addLayer(std::shared_ptr<MapLayer> layer, std::shared_ptr<MapLayer> parent = 0);
    void addLayer(const QString& curLayer, const QString& curDisplayName, bool curVisible, const QString& parentLayer = QString());
    //判断地图中是否包含 type 指定的图层
    bool containsLayer(const QString &type) const;
    //获取所有的图层的类型的列表
    QStringList getLayerList() const;
    // 获取 type 指定的图层
    std::shared_ptr<MapLayer> getLayer(const QString &type);
    // 获取所有的图层并保持树形结构
    const std::list<std::shared_ptr<MapLayer> > &getLayerTree();
    //获取图层是否可见
    bool isLayerVisible(const QString &type);
    bool isLayerVisible(std::shared_ptr<MapLayer> layer);
    bool isAnyLayerVisible(const QString &type1,
                           const QString &type2,
                           const QString &type3 = QString(),
                           const QString &type4 = QString(),
                           const QString &type5 = QString());
    //
    void show(QPainter* painter);
private:
    void _readMapLayerNode(QDomElement node, std::shared_ptr<MapLayer> parent = 0);
    explicit MapLayerMgr(QObject *parent = 0);
    static MapLayerMgr     *minstance;

    class MGarbage // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~MGarbage()
        {
            if (MapLayerMgr::minstance) delete MapLayerMgr::minstance;
        }
    };
    static MGarbage Garbage; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

signals:
    void signalDisplayNameChanged(const QString& type, const QString& displayName);
public slots:
private:
    std::list<std::shared_ptr<MapLayer> > m_layerList;
    std::list<std::shared_ptr<MapLayer> > m_layerTree;
    zchxMapWidget*                        m_drawWidget;
};
}

#endif // ZCHXMAPLAYERMGR_H
