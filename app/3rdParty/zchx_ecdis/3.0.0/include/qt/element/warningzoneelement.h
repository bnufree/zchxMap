#ifndef WARNINGZONEELEMENT_H
#define WARNINGZONEELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {

//防区及防线
class WarningZoneElement : public Element
{
public:
    explicit WarningZoneElement(const ZCHX::Data::ITF_WarringZone &ele, zchxMapFrameWork* f);

    void setData(const ZCHX::Data::ITF_WarringZone &ele);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    ZCHX::Data::ITF_WarringZone data() const;

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

    QString getDefenceColor(){return m_data.fillColor;}
    QString getWarnColor(){return m_data.warnColor;}
    //指定点是否在告警区域线上
    bool contains(int range, double x, double y) const;
    void drawElement(QPainter* painter);

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_WarringZone m_data;
    int              m_activePathPoint; //当前处于活动的路径点的索引
};
}


#endif // WARNINGZONEELEMENT_H
