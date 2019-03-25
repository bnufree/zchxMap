#pragma once

#include <QtCore/private/qobject_p.h>
#include <memory>
#include "element/IDrawElement.hpp"
#include "../zchxmapwidget.h"
#include "../zchxutils.hpp"

namespace qt {
template <typename T>
struct MapLayerEleOptSetting{
public:
    int             mMaxNum;
    bool            mAutoReplace;
    QList<T>        mList;

    MapLayerEleOptSetting()
    {
        mMaxNum = INT_MAX;
        mAutoReplace = true;
        mList.clear();
    }    
    int             getMaxOptNum() const {return mMaxNum;}
    void            setMaxOptNum(int number) {mMaxNum = number;}
    bool            isOpt(const T& id) const {return mList.contains(id);}
    QList<T>        getList() const {return mList;}
    bool            append(const T& id)
    {
        if(isOpt(id)) return true;

        //当前列表不存在对应的id,检查是否超限
        if(mList.size() < mMaxNum) {
            mList.append(id);
            return true;
        }

        //超出最大值的情况
        if(!mAutoReplace) return false;

        remove(mList.first());
        mList.append(id);
        return true;
    }

    void            remove(const T& id) {mList.removeOne(id);}
    void            setAutoReplace(bool replace) {mAutoReplace = replace;}
    bool            autoReplace() const {return mAutoReplace;}
    int             indexOf(const T& val) const {return mList.indexOf(val);}
    T&              valueAt(int index) {return mList[index];}
    T               valueAt(int index) const {return mList[index];}


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
    MapLayerEleOptSetting<QString>                              mConcernSetting;       //目标关注
    MapLayerEleOptSetting<QString>                              mRealtimeTailTrackSetting;
    MapLayerEleOptSetting<QString>                              mHistoryTrackSetting;
    MapLayerEleOptSetting<ZCHX::Data::ExtrapolateParam>         mExtrapolationSetting;

};
}
