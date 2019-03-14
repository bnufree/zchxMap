#ifndef MOVEELEMENT_H
#define MOVEELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {

class ZCHX_ECDIS_EXPORT MoveElement : public Element
{
public:
    explicit MoveElement(ZCHX::Data::ELETYPE type, const QString& layer, zchxMapWidget* w)
        : Element(0, 0, w, type), m_activePathPoint(-1)
    {
        setLayer(layer);
    }

    virtual std::vector<std::pair<double, double> > path() const = 0;
    virtual void setPath(const std::vector<std::pair<double, double> > &path) = 0;
    virtual std::vector<std::pair<double, double> > & getPath() = 0;
    virtual void updateOldPath() {m_path = path();}

    /**
     * @brief 改变第几个点的位置
     * @param idx
     * @param lat
     * @param lon
     */
    void changePathPoint(double lat, double lon);

    /**
     * @brief 整体平移
     * @param lat
     * @param lon
     */
    void moveTo(double lat, double lon);

    /**
     * @brief activePathPoint
     * @return
     */
    int activePathPoint() const {return m_activePathPoint;}
    void setActivePathPoint(int activePathPoint) {m_activePathPoint = activePathPoint;}
    void delPathPoint(int idx);
    void addCtrlPoint(std::pair<double, double> ps);

    //指定点是否在区域线上
    virtual bool contains(int range, double x, double y) const;
    virtual bool contains(const QPoint &pt) const;
    virtual void drawElement(QPainter* painter) {}
    void updateGeometry(QPointF, qreal){}
protected:
    int                                     m_activePathPoint; //当前处于活动的路径点的索引
    std::vector<std::pair<double, double>>  m_path; //初始路径
};


template <typename T>
class ZCHX_ECDIS_EXPORT zchxMoveElement : public MoveElement
{
public:
    explicit zchxMoveElement(const T& data, ZCHX::Data::ELETYPE type, const QString& layer, zchxMapWidget* w)
        : MoveElement(type, layer, w)
    {
        setData(data);
    }
    virtual void setData(const T& data)
    {
        m_data = data;
        updateOldPath();
        setIsUpdate(true);
    }
    T data() const {return m_data;}

    virtual std::vector<std::pair<double, double> > path() const {return m_data.path;}
    virtual void setPath(const std::vector<std::pair<double, double> > &path) {m_data.path = path;}
    virtual std::vector<std::pair<double, double> > & getPath() {return m_data.path;}

    std::string name() const {return m_data.name.toStdString();}
    void setName(const QString& name) {m_data.name = name;}
    virtual void drawElement(QPainter* painter) {}
    void updateGeometry(QPointF, qreal){}
protected:
    T                                       m_data;
};
}

#endif // MOVEELEMENT_H
