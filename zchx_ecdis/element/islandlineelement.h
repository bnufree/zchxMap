#ifndef ISLANDLINEELE_H
#define ISLANDLINEELE_H

#include "moveelement.h"

namespace qt {
//海岸数据
class  IslandLineElement : public zchxMoveElement<ZCHX::Data::ITF_IslandLine>
{
public:
    explicit IslandLineElement(const ZCHX::Data::ITF_IslandLine &ele, zchxMapWidget* f)
        :zchxMoveElement<ZCHX::Data::ITF_IslandLine>(ele, ZCHX::Data::ELE_ISLAND_LINE, ZCHX::LAYER_ISLAND, f) {}

    std::string content() const;
    void setContent(const std::string &content);
    QString color() const;
    void setColor(QString color);
    void updateGeometry(QPointF, qreal){}
    void drawElement(QPainter *painter);
    void copyDataFromOther(std::shared_ptr<Element> other);

private:
    std::string      m_content;
};
}


#endif // ISLANDLINEELE_H
