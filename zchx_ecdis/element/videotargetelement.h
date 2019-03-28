#ifndef VIDEOTARGETELE_H
#define VIDEOTARGETELE_H

#include "fixelement.h"

namespace qt {
//人车船
class  VideoTargetElement : public FixElement<ZCHX::Data::ITF_VideoTarget>
{
public:
    explicit VideoTargetElement(const ZCHX::Data::ITF_VideoTarget & data, zchxMapWidget* work);

    uint getTargetStatus() const;
    uint getTargetType() const;
    uint getAlarmType() const;
    QString getObjId() const;
    QString getAlarmColor() const;

    void drawElement(QPainter* painter);
private:
    QStringList  mTargetIImgList;
};

}

#endif // VIDEOTARGETELE_H
