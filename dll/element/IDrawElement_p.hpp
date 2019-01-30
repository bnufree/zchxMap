#pragma once

#include <qt/zchxutils.hpp>
#include <qt/zchx_ecdis_global.h>
#include <QGeoCoordinate>

class Framework;
namespace qt
{
class DrawWidget;
class MapLayer;
}

namespace DrawElement{

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
    qt::MapLayer *layer;
    Framework *framework;
    qt::DrawWidget *drawWidget;

    friend class Element;
};

}
