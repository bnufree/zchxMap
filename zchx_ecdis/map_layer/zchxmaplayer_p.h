#pragma once

#include <QtCore/private/qobject_p.h>
#include <memory>
#include "element/IDrawElement.hpp"
#include "../zchxmapwidget.h"
#include "../zchxutils.hpp"

namespace qt {
struct MapLayerEleOptSetting{
    int         mMaxOptNum;
    bool        mReplaceWhenOver;
    QStringList mOptIdList;

    MapLayerEleOptSetting()
    {
        mMaxOptNum = INT_MAX;
        mReplaceWhenOver = true;
        mOptIdList.clear();
    }
};

class MapLayer;
class MapLayerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(MapLayer)

public:
    MapLayerPrivate();
    MapLayerPrivate(const QString &type, const QString &displayName, bool visible);
    MapLayerPrivate(zchxMapWidget *drawWidget, const QString &type, const QString &displayName, bool visible);

private:
    std::list<std::shared_ptr<MapLayer> >           m_children;
    std::list<std::shared_ptr<Element> >            m_elements;
    QString                                         m_type;
    QString                                         m_displayName;
    bool                                            m_visible;
    bool                                            m_enableUpdate;
    bool                                            m_pickupAvailable;
    std::shared_ptr<MapLayer>                       m_parent;
    ZCHX::Data::ECDIS_PLUGIN_USE_MODELs             m_mode;
    zchxMapWidget                                   *m_drawWidget;
    //关注,尾迹,历史轨迹,模拟外推等的设定
    MapLayerEleOptSetting                           mConcernSetting;       //目标关注
    MapLayerEleOptSetting                           mRealtimeTailTrackSetting;
    MapLayerEleOptSetting                           mHistoryTrackSetting;
    MapLayerEleOptSetting                           mExtrapolationSetting;

};
}
