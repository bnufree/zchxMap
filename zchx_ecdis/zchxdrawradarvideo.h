#ifndef ZCHXDRAWRADARVIDEO_H
#define ZCHXDRAWRADARVIDEO_H

#include <QObject>
#include <QThread>
#include <QMap>
#include "element/drawelement.hpp"

namespace qt {
struct Afterglow
{
    std::vector<RadarVideoElement> m_RadarVideo;
    std::vector<std::pair<double, double>> m_path;//雷达目标点集合
};
const int RadarVideoPixmapWidth = 1364*2;
const int RadarVideoPixmapHeight = 1364*2;
class ZCHXDrawRadarVideo : public QObject
{
    Q_OBJECT
public:
    explicit ZCHXDrawRadarVideo(QObject *parent = 0);
    ~ZCHXDrawRadarVideo();
//    void setRadarVideoData(int uMsgIndex, const std::vector<RadarVideo> &dataVec);
//    void setRadarPoint(const std::vector<std::pair<double, double> > &path);//设置雷达目标点集合
    void setIndex(const int uIndex);
signals:
    void signalDrawRadarVideo(const Afterglow &dataAfterglow);
    void signalDrawAfterglow(const Afterglow &dataAfterglow);//绘制余辉
    void signalRadarVideoPixmap(const int uIndex,const QPixmap &objPixmap);
public slots:
    void slotDrawRadarVideo(const Afterglow &dataAfterglow);//绘制回波
    void slotDrawAfterglow(const Afterglow &dataAfterglow);//绘制余辉
private:
    void initColor();//初始化颜色组
    QColor getColor(double dValue);//通过振幅值获取对应颜色值
    bool isInRadarPointRange(const double dLat,const double dLon,
                             const std::vector<std::pair<double, double>> path);
private:
    QThread                              m_threadWork;
    //std::vector<RadarVideo> m_RadarVideo;                    //回波数据
    //int                                  m_uMsgIndex;                     //(4个)扇区索引
    QMap<int,QColor> m_colorMap;//颜色map

    //绘制余辉参数
    //std::vector<std::pair<double, double>> m_path;//雷达目标点集合
    int m_uAfterglowType;//1,3,6,12
    int m_uAfterglowIndex;

    //QMutex      mMutex;
};
}

#endif // ZCHXDRAWRADARVIDEO_H
