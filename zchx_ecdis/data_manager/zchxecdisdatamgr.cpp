#include "zchxecdisdatamgr.h"

namespace qt {
zchxEcdisDataMgr::zchxEcdisDataMgr(zchxMapWidget* w, int type, QObject *parent) : QObject(parent)
  ,mDisplayWidget(w)
  ,mType(type)
  ,mMaxConcernNum(0)
  ,mMaxRealtimeTailTrackNum(0)
  ,mMaxHistoryTrackNum(0)
  ,mReplaceRealtimeTailTrackWhenOver(true)
  ,mReplaceConcernWhenOver(true)
  ,mReplaceHistoryTrackWhenOver(true)
  ,mMaxExtrapolationNum(10)
  ,mReplaceExtrapolationWhenOver(true)
  ,mDataDisplayMode(Data_RealTime)
{

}


void zchxEcdisDataMgr::show(QPainter* painter, double offset_x, double offset_y)
{

}

bool zchxEcdisDataMgr::updateActiveItem(const QPoint &pt)
{
    return false;
}

QList<QAction*> zchxEcdisDataMgr::getRightMenuActions(const QPoint &pt)
{
    return QList<QAction*> ();
}

void zchxEcdisDataMgr::appendConcernList(const QStringList &list , bool fromMenu)
{
    QStringList skip_list;
    for(int i=0; i<list.size(); i++)
    {
        QString id = list[i];
        if(mConcernList.contains(id)){
            if(fromMenu){
                //取消关注,从菜单执行的取消关注
                removeConcern(id);
            } else {
                //从外部接口调用什么也不操作
                continue;
            }
        } else {
            //当前列表不存在对应的id,检查是否超限
            if(mConcernList.size() < mMaxConcernNum) {
                mConcernList.append(id);
            } else {
                //超出最大值的情况
                if(!mReplaceConcernWhenOver)
                {
                    skip_list.append(list.mid(i, list.size() - i));
                    break;
                } else
                {
                    removeConcern(mConcernList.first());
                    mConcernList.append(id);
                }
            }
        }

    }

    if(skip_list.size() != 0)
    {
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1个关注目标.\n%2未添加").arg(mMaxConcernNum).arg(skip_list.join(",")));
    }
    return;
}

void zchxEcdisDataMgr::removeConcernList(const QStringList &list)
{
    foreach (QString id, list) {
        removeConcern(id);
    }
}

void zchxEcdisDataMgr::appendRealtimeTailTrackList(const QStringList &list , bool fromMenu)
{
    QStringList skip_list;
    for(int i=0; i<list.size(); i++)
    {
        QString id = list[i];
        if(mRealtimeTailTrackList.contains(id)){
            if(fromMenu){
                //取消关注,从菜单执行的取消关注
                removeRealtimeTailTrack(id);
            } else {
                //从外部接口调用什么也不操作
                continue;
            }
        } else {
            //当前列表不存在对应的id,检查是否超限
            if(mRealtimeTailTrackList.size() < mMaxRealtimeTailTrackNum) {
                mRealtimeTailTrackList.append(id);
            } else {
                //超出最大值的情况
                if(!mReplaceRealtimeTailTrackWhenOver)
                {
                    skip_list.append(list.mid(i, list.size() - i));
                    break;
                } else
                {
                    removeRealtimeTailTrack(mRealtimeTailTrackList.first());
                    mRealtimeTailTrackList.append(id);
                }
            }
        }

    }

    if(skip_list.size() != 0)
    {
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1条实时轨迹.\n%2未添加").arg(mMaxRealtimeTailTrackNum).arg(skip_list.join(",")));
    }
    return;
}

void zchxEcdisDataMgr::removeRealtimeTailTrackList(const QStringList &list)
{
    foreach (QString id, list) {
        removeRealtimeTailTrack(id);
    }
}

void zchxEcdisDataMgr::appendHistoryTrackList(const QStringList &list , bool fromMenu)
{
    QStringList skip_list;
    for(int i=0; i<list.size(); i++)
    {
        QString id = list[i];
        if(mHistoryTrackList.contains(id)){
            if(fromMenu){
                //取消关注,从菜单执行的取消关注
                removeHistoryTrack(id);
            } else {
                //从外部接口调用什么也不操作
                continue;
            }
        } else {
            //当前列表不存在对应的id,检查是否超限
            if(mHistoryTrackList.size() < mMaxHistoryTrackNum) {
                mHistoryTrackList.append(id);
            } else {
                //超出最大值的情况
                if(!mReplaceHistoryTrackWhenOver)
                {
                    skip_list.append(list.mid(i, list.size() - i));
                    break;
                } else
                {
                    removeHistoryTrack(mHistoryTrackList.first());
                    mHistoryTrackList.append(id);
                }
            }
        }

    }

    if(skip_list.size() != 0)
    {
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1条历史轨迹.\n%2未添加").arg(mMaxRealtimeTailTrackNum).arg(skip_list.join(",")));
    }
    return;
}
void zchxEcdisDataMgr::removeHistoryTrackList(const QStringList &list)
{
    foreach (QString id, list) {
        removeHistoryTrack(id);
    }
}

ZCHX::Data::ExtrapolateList zchxEcdisDataMgr::getExtrapolationList() const
{
    return mExtrapolationList;
}

double zchxEcdisDataMgr::getExtrapolationTime(const QString &id) const
{
    int index = mExtrapolationList.indexOf(ZCHX::Data::ExtrapolateParam(id));
    if(index < 0) return 0.0;
    return mExtrapolationList[index].mVal;
}

void zchxEcdisDataMgr::appendExtrapolationList(const QStringList &list , bool fromMenu)
{
    QStringList skip_list;
    for(int i=0; i<list.size(); i++)
    {
        QString id = list[i];
        if(mExtrapolationList.contains(id)){
            if(fromMenu){
                //取消关注,从菜单执行的取消关注
                removeExtrapolation(id);
            } else {
                //从外部接口调用什么也不操作
                continue;
            }
        } else {
            //当前列表不存在对应的id,检查是否超限
            if(mExtrapolationList.size() < mMaxExtrapolationNum) {
                mExtrapolationList.append(ZCHX::Data::ExtrapolateParam(id));
            } else {
                //超出最大值的情况
                if(!mReplaceExtrapolationWhenOver)
                {
                    skip_list.append(list.mid(i, list.size() - i));
                    break;
                } else
                {
                    removeExtrapolation(mExtrapolationList.first().mID);
                    mExtrapolationList.append(ZCHX::Data::ExtrapolateParam(id));
                }
            }
        }

    }

    if(skip_list.size() != 0)
    {
        QMessageBox::warning(0, QObject::tr("告警"), QObject::tr("最多显示%1条模拟外推数据.\n%2未添加").arg(mMaxExtrapolationNum).arg(skip_list.join(",")));
    }
    return;
}
void zchxEcdisDataMgr::removeExtrapolationList(const QStringList &list)
{
    foreach (QString id, list) {
        removeExtrapolation(id);
    }
}

void zchxEcdisDataMgr::updateExtrapolationTime(const QString &id, double time)
{
    int index = mExtrapolationList.indexOf(ZCHX::Data::ExtrapolateParam(id));
    if(index < 0) return;
    mExtrapolationList[index].mVal = time;
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

QAction* zchxEcdisDataMgr::addAction(const QString &text, const QObject *obj, const char* slot, void* userData)
{
    if(!obj || !slot || strlen(slot) == 0) return 0;

    QAction *result(new QAction(text, this));
    if(QString(slot).contains("(bool)"))
        connect(result, SIGNAL(triggered(bool)), obj, slot);
    else
        connect(result,SIGNAL(triggered()),obj,slot);

    if(userData) result->setData(QVariant::fromValue(userData));
    return result;
}

Element* zchxEcdisDataMgr::getElementOfSender()
{
    QAction *act = qobject_cast<QAction*>(sender());
    if(!act) return NULL;
    Element *ele = (Element*)(act->data().value<void*>());
    return ele ;
}

void zchxEcdisDataMgr::setPictureInPicture()
{

}

void zchxEcdisDataMgr::setSimulationExtrapolation()
{

}

void zchxEcdisDataMgr::setHistoryTraces()
{

}

void zchxEcdisDataMgr::setRealTimeTraces()
{

}

void zchxEcdisDataMgr::invokeLinkageSpot()
{

}

void zchxEcdisDataMgr::setConcern(){}
}


