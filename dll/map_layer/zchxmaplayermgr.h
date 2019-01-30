#ifndef ZCHXMAPLAYERMGR_H
#define ZCHXMAPLAYERMGR_H

#include <QObject>
#include "zchxMapLayer.h"


class zchxMapWidget;

class zchxMapLayerMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxMapLayerMgr(zchxMapWidget* w, QObject *parent = 0);
    //地图图层管理
    //从配置文件加载地图图层配置文件,图层的顺序按照从上到下的顺序, 最上面为最顶层
    void loadLayers();
    //添加图层 layer 父图层, 默认为空
    void addLayer(std::shared_ptr<zchxMapLayer> layer, std::shared_ptr<zchxMapLayer> parent = 0);
    //判断地图中是否包含 type 指定的图层
    bool containsLayer(const QString &type) const;
    //获取所有的图层的类型的列表
    QStringList getLayerList() const;
    // 获取 type 指定的图层
    std::shared_ptr<zchxMapLayer> getLayer(const QString &type);
    // 获取所有的图层并保持树形结构
    const std::list<std::shared_ptr<zchxMapLayer> > &getLayerTree();
    //获取图层是否可见
    bool isLayerVisible(const QString &type);
    bool isAnyLayerVisible(const QString &type1,
                           const QString &type2,
                           const QString &type3 = QString(),
                           const QString &type4 = QString(),
                           const QString &type5 = QString());
private:
    void _readMapLayerNode(QDomElement node, std::shared_ptr<zchxMapLayer> parent = 0);

signals:

public slots:
private:
    std::list<std::shared_ptr<zchxMapLayer> > m_layerList;
    std::list<std::shared_ptr<zchxMapLayer> > m_layerTree;
    zchxMapWidget                         *mDisplayWidget;
};

#endif // ZCHXMAPLAYERMGR_H
