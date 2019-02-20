#pragma once

#include "../zchxutils.hpp"
#include "../zchx_ecdis_global.h"
#include <QGeoCoordinate>


namespace qt
{
class zchxMapFrameWork;
class MapLayer;
class zchxMapWidget;
class Element;
//TODO: 由于使用了很多拷贝构造函数, 所以不能继承qobject
class ZCHX_ECDIS_EXPORT ElementPrivate// : public QObject
{
    //Q_OBJECT
protected:
    Element *q;
public:
    ElementPrivate();

//TODO: 存在经常崩溃的问题
//    std::shared_ptr<qt::MapLayer> layer;
    MapLayer *layer;
    zchxMapFrameWork *framework;
    zchxMapWidget *drawWidget;

    friend class Element;
};

}
