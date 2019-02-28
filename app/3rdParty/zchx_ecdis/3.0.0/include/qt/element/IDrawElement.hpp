#pragma once

#include "../zchxutils.hpp"
#include "../zchx_ecdis_global.h"
#include <QtGui>

class QGeoCoordinate;

namespace qt{
class zchxMapFrameWork;
class zchxMapWidget;
class MapLayer;

enum RADARTYPE{RADARSHIP,RADARPLAN,WARRINGZONE};

class ElementPrivate;
//TODO: 由于使用了很多拷贝构造函数, 所以不能继承qobject
class ZCHX_ECDIS_EXPORT Element// : public QObject
{
protected:
    ElementPrivate *d;
public:
    Element(const double &lat, const double &lon, zchxMapFrameWork* frame, ZCHX::Data::ELETYPE type = ZCHX::Data::ELE_NONE, const QColor& flashColor = QColor());
    Element(const Element &element);
    virtual ~Element();

    /*!
     * \brief 获取图元当前所在的图层
     * \note 有可能为空
     */
    std::shared_ptr<MapLayer> getLayer();

    /*!
     * \brief 获取图元的经纬度
     */
    std::pair<double, double> getLatLon() const;
    void setLatLon(const std::pair<double, double> &ll);
    void setLatLon(double lat, double lon);
    double lat() const;
    double lon() const;

    /*!
     * \brief 图元的激活状态, 例如: 选中, 编辑, 绘制等
     */
    bool getIsActive() const;
    void setIsActive(bool value);

    /*!
     * \brief 图元的类型
     */
    ZCHX::Data::ELETYPE getElementType() const;
    void setElementType(const ZCHX::Data::ELETYPE &type);

    /*!
     * \brief 图元的id
     */
    int getUuid() const;

    /*!
     * \brief 图元是否被鼠标悬垂
     */
    bool getIsHover() const;
    void setIsHover(bool value);

    /*!
     * \brief 图元是否包含焦点
     */
    bool getIsFocus() const;
    void setIsFocus(bool value);

    /*!
     * \brief 图元是否被用户关注
     */
    bool getIsConcern() const;
    void setIsConcern(bool value);
    bool getIsRealtimeTailTrack() const;
    void setIsRealtimeTailTrack(bool value);
    bool getIsHistoryTrack() const;
    void setIsHistoryTrack(bool value);

    //图元的颜色更新
    QColor getBorderColor() const;
    void   setBorderColor(const QColor& color);
    QColor getFillingColor() const;
    void   setFillingColor(const QColor& color);
    QColor getTextColor() const;
    void   setTextColor(const QColor& color);
    QColor getConcernColor() const;
    void   setConcernColor(const QColor& color);
    //会遇显示是否开始
    void   setIsOpenMeet(bool sts) {isOpenMeet = sts;}
    bool   getIsOpenMeet() const {return isOpenMeet;}

    //设置更新标记
    void   setIsUpdate(bool sts) {isUpdate = sts;}
    bool   getIsUpdate() const {return isUpdate;}

    /*!
     * \brief 用于显示的经纬度
     */
    double getDisplayLat() const;
    void setDisplayLat(double value);

    double getDisplayLon() const;
    void setDisplayLon(double value);

    /*!
     * \brief 是否使用显示用的经纬度
     * 主要用于对 显示图元的经纬度 和 实际的经纬度 进行区分, 以便实现重叠图元的分离
     * 但是此功能会导致图元位置和实际不符, 不建议
     */
    bool isUseDisplayLatLon() const;
    void setUseDisplayLatLon(bool value);
    void updateFlashRegionColor(const QColor& color);

    /*!
     * \brief 判断 x y 构成的坐标点在不在宽度为 2 * range 的矩形内
     * 以自身经纬度为中心, 构造 2 * range大小正方形, 并判断是否包含x,y
     * \param framework 用来把经纬度坐标转换为像素
     * \param range 矩形的宽度的一半
     * \param x 坐标点
     * \param y 坐标点
     * \return 如果在矩形内则返回true, 否则返回false
     */
    virtual bool contains(int range, double x, double y) const;
    virtual bool contains(const QPoint& pos) const;
    virtual bool contains(const QGeoCoordinate &geoPos) const;
    virtual bool isEmpty() const;

    QPointF getViewPos(zchxMapFrameWork *f = 0);

    /*!
     * \brief 绘制图元的内容, 子类需要实现此接口, 以便实现自身绘制
     * 子类中必须在实现自身的绘制后, 调用此接口完成底层绘图和子图形的绘图
     */
    virtual void drawElement(QPainter *painter);
    virtual void drawActive(QPainter *painter);
    virtual void drawFocus(QPainter *painter);
    virtual void updateGeometry(QPointF pos, qreal size);
    virtual void drawHover(QPainter *painter) {}

    void addChild(std::shared_ptr<Element> child);
    void removeChild(std::shared_ptr<Element> child);
    std::list<std::shared_ptr<Element> > getChildren(ZCHX::Data::ELETYPE type = ZCHX::Data::ELE_NONE) const;

    std::shared_ptr<Element> parent();
    void setParent(std::shared_ptr<Element> ele);

    void setUuid(const QUuid &uuid);
    QUuid getUuid2() const;

    /*!
     * \brief 绘制 速度等比线 和 方向角度线
     * \param painter 绘图设备
     * \param pos 位置
     * \param sog 对地航速
     * \param cog 对地航向
     * \param rot 转向率
     */
    void drawSpeedDirectionLine(QPainter *painter, QPointF pos, qreal sog, qreal cog, qreal rot);
    /*!
     * \brief 绘制闪烁区域
     * \param painter 绘图设备
     * \param pos 位置
     * \param status 当前状态, 0 则不绘制; 如果status预定义的color不存在则可以指定
     * \param color 指定的color
     * \param radius 半径
     */
    void drawFlashRegion(QPainter *painter, QPointF pos, int status, QColor color/* = QColor()*/, qreal radius = 0);

    /*!
     * \brief 把经纬度轨迹转化为屏幕坐标轨迹
     */
    std::vector<QPointF> convert2QtPonitList(const std::vector<std::pair<double, double>>& path);
    /*!
     * \brief 把经纬度点转化为屏幕坐标点
     */
    QPointF convertToView(double lon, double lat);


    qint64 getUpdateUTC() const;
    void setUpdateUTC(const qint64 &updateUTC);

    bool getForceImage() const;
    void setForceImage(bool forceImage);

    bool getGeometryChanged() const;
    void setGeometryChanged(bool geometryChanged);

    /*!
     * \brief 根据缩放比例, 获取不同的长度尺寸
     */
    int getDrawScaleSize() const;

    QString getStrID() const;
    void setStrID(const QString &strID);
    //颜色初始化
    virtual void initFromSettings();
    //取得当前图元在屏幕坐标的位置
    QPointF getCurrentPos();
    //
    std::shared_ptr<MapLayer> layer() {return m_layer;}
    zchxMapFrameWork* framework() const {return m_framework;}
    void setFrameWork(zchxMapFrameWork* f) {m_framework = f;}

    //检查层设定是否显示
    bool isLayervisible();

protected://TODO: 添加私有类, 实现成员变量对外隐藏, 且防止依赖扩展情况
    double elelat;
    double elelon;
    double displayLat;
    double displayLon;
    bool useDisplayLatLon;
    bool   isActive;
    bool   isHover;
    bool   isFocus;
    bool   isConcern;           //用户是否关注
    bool   isRealtimeTailTrack;         //是否显示尾迹点
    bool   isHistroyTrack;              //是否显示历史轨迹
    bool   isOpenMeet;          //是否开启会遇显示
    bool   isUpdate;            //是否更新过
    int    uuid;
    QString m_strID;
    ZCHX::Data::ELETYPE  m_element_type;
    QPointF m_pos;
    //QMap<int, QColor> m_mapFlashColor;
    QColor      mFlashColor;
    bool m_forceImage;

    std::list<std::shared_ptr<Element> > m_children;
    std::shared_ptr<Element>             m_parent;

    std::shared_ptr<MapLayer> m_layer;
    zchxMapFrameWork *m_framework;
    QUuid m_uuid;
    qint64 m_updateUTC;

    QRectF m_boundingRectSmall;
    QRectF m_boundingRect;
    QRectF m_activeRect;
    QRectF m_focusRect;
    bool m_geometryChanged;
    QColor      mBorderColor;           //图元边框颜色
    QColor      mFillingColor;             //图元填充颜色
    QColor      mTextColor;             //图元文本颜色
    QColor      mConcernColor;          //图元关注颜色

    static int g_maxLineLength;

public:
    friend class MapLayer;
    //friend class qt::DrawWidget;
};

}
