#ifndef VIDEOTARGETELEMENT_H
#define VIDEOTARGETELEMENT_H

#include "IDrawElement.hpp"
#include <QtCore>

namespace qt {
//人车船
class  VideoTargetElement : public Element
{
public:
    explicit VideoTargetElement(const ZCHX::Data::ITF_VideoTarget & data, zchxMapFrameWork* work);

    const ZCHX::Data::ITF_VideoTarget & getData() const;
    void setData(const ZCHX::Data::ITF_VideoTarget& data);

    uint getTargetStatus() const;
    uint getTargetType() const;
    uint getAlarmType() const;
    QString getObjId() const;
    QString getAlarmColor() const;

    void drawElement(QPainter* painter);
    void updateGeometry(QPointF, int){}
private:
    ZCHX::Data::ITF_VideoTarget m_data;
    QStringList  mTargetIImgList;
};

}

#endif // VIDEOTARGETELEMENT_H
