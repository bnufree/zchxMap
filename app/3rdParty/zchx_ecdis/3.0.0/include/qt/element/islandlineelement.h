#ifndef ISLANDLINEELE_H
#define ISLANDLINEELE_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
//海岸数据
class  IslandLineElement : public Element
{
public:
    explicit IslandLineElement(const ZCHX::Data::ITF_IslandLine &ele, zchxMapWidget* f);

    ZCHX::Data::ITF_IslandLine data() const;
    void setData(const ZCHX::Data::ITF_IslandLine& data);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

//    int id() const;
//    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    std::string content() const;
    void setContent(const std::string &content);
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
     * @brief updateOldPath
     * 更新初始路径
     */
    void updateOldPath();

    /**
     * @brief activePathPoint
     * @return
     */
    int activePathPoint() const;
    void setActivePathPoint(int activePathPoint);
    void delPathPoint(int idx);
    void addCtrlPoint(std::pair<double, double> ps);

    QString color() const;
    void setColor(QString color);
    void updateGeometry(QPointF, qreal){}

//    bool contains(int range, double x, double y) const;
    void drawElement(QPainter *painter);

private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    RADARTYPE        m_type;
    std::string      m_name;
    int              m_id;
    std::string      m_content;
    ZCHX::Data::ITF_IslandLine m_data;
    int              m_activePathPoint; //当前处于活动的路径点的索引
    QString          m_color;
};
}


#endif // ISLANDLINEELE_H
