#ifndef WARNINGZONEELEMENT_H
#define WARNINGZONEELEMENT_H

#include "moveelement.h"
namespace qt {

//防区及防线
class WarningZoneElement : public MoveElement
{
public:
    explicit WarningZoneElement(const ZCHX::Data::ITF_WarringZone &ele, zchxMapFrameWork* f);

    ZCHX::Data::ITF_WarringZone data() const;
    void setData(const ZCHX::Data::ITF_WarringZone &ele);

    std::vector<std::pair<double, double> > path() const;
    void setPath(const std::vector<std::pair<double, double> > &path);
    std::vector<std::pair<double, double> > & getPath();

    int id() const;
    void setId(int id);

    std::string name() const;
    void setName(const std::string &name);

    /**
     * @brief updateOldPath
     * 更新初始路径
     */
    void updateOldPath();
    void delPathPoint(int idx);

    QString getDefenceColor(){return m_data.fillColor;}
    QString getWarnColor(){return m_data.warnColor;}
    void drawElement(QPainter* painter);

    void updateGeometry(QPointF, int){}
private:
    std::vector<std::pair<double, double>> m_path; //初始路径
    std::string      m_name;
    ZCHX::Data::ITF_WarringZone m_data;
};
}


#endif // WARNINGZONEELEMENT_H
