#pragma once

#include <QObject>
#include <memory>
#include "../zchxutils.hpp"
#include "../zchxmapwidget.h"
#include "../zchx_ecdis_global.h"

class QGeoCoordinate;
class QPainter;

namespace qt {
class MapLayerPrivate;
class Element;

class ZCHX_ECDIS_EXPORT MapLayer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MapLayer)

    /*!
     * \property MapLayer::type
     * \brief 地图图层的类型
     * 用于区分不同的图层, 由 maplayers.xml 配置文件中图层的 type 属性进行定义
     */
    Q_PROPERTY(QString type READ type WRITE setType)
    /*!
     * \property MapLayer::displayName
     * \brief 地图图层的名字
     * 用于标识不同的图层, 由 maplayers.xml 配置文件中图层的 tr 属性进行定义
     * \note 需要进行翻译
     */
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    /*!
     * \property MapLayer::visible
     * \brief 地图图层 是否可见
     * 用于设置图层是否是可见的, 由 maplayers.xml 配置文件中图层的 visible 属性进行定义
     */
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
public:
    MapLayer(const QString &type, const QString &displayName, bool visible, QObject *parent = 0);
    MapLayer(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible, QObject *parent = 0);
    explicit MapLayer(QObject *parent = 0);

    /*!
     * \brief 图层的类型名
     */
    QString type() const;
    void setType(const QString &type);

    /*!
     * \brief 图层支持的显示模式
     */
    ZCHX::Data::ECDIS_PLUGIN_USE_MODELs mode() const;
    void setMode(const ZCHX::Data::ECDIS_PLUGIN_USE_MODEL &mode, bool addOrRemove = true);

    /*!
     * \brief 图层的翻译文本, 需要使用PSFW_TR函数进行翻译
     */
    QString displayName() const;
    void setDisplayName(const QString &displayName);

    /*!
     * \brief 图层是否可见
     */
    bool visible() const;
    void setVisible(bool visible);
    //父图层获取更新
    void setParentLayer(std::shared_ptr<MapLayer> parent);
    std::shared_ptr<MapLayer> getParentLayer() const;
    bool hasParentLayer() const;

    /*!
     * \brief 图层是否包含子图层
     */
    bool hasChildLayer() const;
    /*!
     * \brief 获取图层的子图层列表
     */
    const std::list<std::shared_ptr<MapLayer> > &getChildLayers();
    /*!
     * \brief 添加一个子图层, 最后添加的子图层在最顶端
     */
    void addChildLayer(std::shared_ptr<MapLayer> layer);
    /*!
     * \brief 移除一个子图层
     */
    void removeChildLayer(std::shared_ptr<MapLayer> layer);
    /*!
     * \brief 获取指定的类型
     */
    std::shared_ptr<MapLayer> getChildLayer(const QString &type);

    /*!
     * \brief 添加一个图元到当前图层
     */
    void addElement(std::shared_ptr<Element> element);
    /*!
     * \brief 从当前图层移除一个图元
     */
    void removeElement(std::shared_ptr<Element> element);
    /*!
     * \brief 获取当前图层中的所有图元的列表, 最先添加的图层再列表最前面
     */
    std::list<std::shared_ptr<Element> > getElements();
    Element*                             getElement(const QString& name);

    /*!
     * \brief 更新图层
     */
    void update();
    /*!
     * \brief 是否允许图层更新
     */
    bool getEnableUpdate() const;
    void setEnableUpdate(bool enableUpdate);

    /*!
     * \brief 绘图图层及图层中的图元
     */
    void drawLayer(QPainter *painter);

    std::shared_ptr<Element> pickUpElement(QPointF pos, const QGeoCoordinate &geoPos);
    std::shared_ptr<Element> pickUpElement(QPointF pos);
    std::shared_ptr<Element> pickUpElement(const QString &id);

    /*!
     * \brief 将经纬度转化为当前窗体的屏幕坐标
     */
    QPointF convertToView(double lon, double lat);

    bool clearElementState();
    bool clearActiveState();
    bool clearFocusState();
    bool clearHoverState();
    //图层是否可以选择
    void setPickupAvailable(bool sts);
    bool getPickupAvailable() const;
    //关注
    //尾迹
    //历史轨迹
    //预推

signals:
    void visibleChanged(bool visible);
    void displayNameChanged(QString displayName);
};
}
