#include "zchxecdisdatamgr.h"

namespace qt {
zchxEcdisDataMgr::zchxEcdisDataMgr(zchxMapWidget* w, int type, QObject *parent) : QObject(parent)
  ,mDisplayWidget(w)
  ,mType(type)
  ,mMaxConcernNum(0)
  ,mMaxTailTrackNum(0)
  ,mReplaceTrackWherOver(true)
  ,mReplaceConcernWhenOver(true)
  ,mDataDisplayMode(Data_RealTime)
{

}


void zchxEcdisDataMgr::show(QPainter *painter)
{

}

bool zchxEcdisDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}

bool zchxEcdisDataMgr::updateConcernList(const QString &id, bool fromMenu)
{
    if(mConcernList.contains(id))
    {
        //当前关注列表包含了对应的id,如果是从菜单进行操作,则取消关注
        if(fromMenu)
        {
            removeConcern(id);
        }
    } else {
        if(mConcernList.size() < mMaxConcernNum) {
            mConcernList.append(id);
        } else {
            //超出最大值的情况
            if(!mReplaceConcernWhenOver)
            {
                QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1个关注目标.").arg(mMaxConcernNum));
                return false;
            } else
            {
                removeConcern(mConcernList.first());
                mConcernList.append(id);
            }
        }
    }
    return true;
}

bool zchxEcdisDataMgr::updateTailTrackList(const QString &id, bool fromMenu)
{
    if(mTrackList.contains(id))
    {
        //当前轨迹列表包含了对应的id,如果是从菜单进行操作,则取消关注
        if(fromMenu)
        {
            removeTrack(id);
        }
    } else {
        if(mTrackList.size() < mMaxTailTrackNum) {
            mTrackList.append(id);
        } else {
            //超出最大值的情况
            if(!mReplaceTrackWherOver)
            {
                QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1条轨迹.").arg(mMaxTailTrackNum));
                return false;
            } else
            {
                removeTrack(mTrackList.first());
                mTrackList.append(id);
            }
        }
    }
    return true;
}

void zchxEcdisDataMgr::setDataDisplayMode(int mode)
{
    if(mDataDisplayMode != mode)
    {
        clearDisplayData();
        mDataDisplayMode = mode;
    }
}

int zchxEcdisDataMgr::getDataDisplayMode() const
{
    return mDataDisplayMode;
}
}


