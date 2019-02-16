#ifndef ZCHXECDISDATAMGR_H
#define ZCHXECDISDATAMGR_H

#include <QObject>
#include "element/drawelement.hpp"
#include "zchxutils.hpp"
#include "zchxMapDatautils.h"
#include "profiles.h"
#include "zchxmapwidget.h"
#include "map_layer/zchxmaplayermgr.h"
#include <QPainter>

enum DataDisplayMode{
    Data_RealTime = 0,
    Data_History,
};

class zchxEcdisDataMgr : public QObject
{
    Q_OBJECT
public:
    explicit zchxEcdisDataMgr(zchxMapWidget* w, QObject *parent = 0);
    virtual void    show(QPainter* painter);
    int     getMaxConcernNum() const {return mMaxConcernNum;}
    int     getMaxTailTrackNum() const {return mMaxTailTrackNum;}
    virtual bool    updateActiveItem(const QPoint& pt);  //鼠标点击的时候更新选择的item
    bool    updateConcernList(const QString& id, bool fromMenu); //通过菜单的场合可以取消关注和添加关注
    bool    updateTailTrackList(const QString& id, bool fromMenu); //通过菜单的场合可以取消关注和添加关注
    virtual void    removeConcern(const QString& id) {mConcernList.removeOne(id);}
    virtual void    removeTrack(const QString& id) {mTrackList.removeOne(id);}
    bool    isConcern(const QString& id) {return mConcernList.contains(id);}
    bool    isTrack(const QString& id) {return mTrackList.contains(id);}

    void    setDataDisplayMode(int mode);
    int     getDataDisplayMode() const;
    virtual void clearDisplayData() {}

signals:

public slots:

protected:
    int                 mMaxConcernNum;             //最大关注列表数
    int                 mMaxTailTrackNum;           //最大尾迹数据数
    bool                mReplaceTrackWherOver;      //是否自动覆盖轨迹点显示当超出最大轨迹线数目
    bool                mReplaceConcernWhenOver;    //是否自动取消一个关注点
    int                 mDataDisplayMode;
    zchxMapWidget       *mDisplayWidget;
    QStringList         mConcernList;
    QStringList         mTrackList;
};

#endif // ZCHXECDISDATAMGR_H

