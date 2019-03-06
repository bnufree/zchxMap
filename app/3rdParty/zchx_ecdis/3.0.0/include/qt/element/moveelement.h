#ifndef MOVEELEMENT_H
#define MOVEELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
class ZCHX_ECDIS_EXPORT MoveElement : public Element
{
public:
    explicit MoveElement(zchxMapFrameWork* f, ZCHX::Data::ELETYPE type);
    void setData();

    virtual std::vector<std::pair<double, double> > path() const = 0;
    virtual void setPath(const std::vector<std::pair<double, double> > &path) = 0;
    virtual std::vector<std::pair<double, double> > & getPath() = 0;

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
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);
    void delPathPoint(int idx);
    void addCtrlPoint(std::pair<double, double> ps);

    //指定点是否在区域线上
    virtual bool contains(int range, double x, double y) const;
    virtual bool contains(const QPoint &pt) const;
    virtual void drawElement(QPainter* painter) {}
    void updateGeometry(QPointF, int){}
protected:
    int              m_activePathPoint; //当前处于活动的路径点的索引
};

}


#endif // MOVEELEMENT_H
