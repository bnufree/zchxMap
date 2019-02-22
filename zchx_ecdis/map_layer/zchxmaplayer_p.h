#pragma once

#include <QtCore/private/qobject_p.h>
#include <memory>
#include "element/IDrawElement.hpp"
#include "../zchxmapwidget.h"
#include "../zchxutils.hpp"

namespace qt {
class MapLayer;
class MapLayerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(MapLayer)

public:
    MapLayerPrivate();
    MapLayerPrivate(const QString &type, const QString &displayName, bool visible);
    MapLayerPrivate(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible);

private:
    std::list<std::shared_ptr<MapLayer> > m_children;
    std::list<std::shared_ptr<Element> > m_elements;
    QString m_type;
    QString m_displayName;
    bool m_visible;
    bool m_enableUpdate;
    ZCHX::Data::ECDIS_PLUGIN_USE_MODELs m_mode;

    zchxMapWidget *m_drawWidget;
};
}
