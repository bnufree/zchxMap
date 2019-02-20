#include "zchxdrawradarvideo.h"
#include <QDebug>
#include <math.h>
#include "zchxutils.hpp"

namespace qt {
const double PI = 3.1415926;
ZCHXDrawRadarVideo::ZCHXDrawRadarVideo(QObject *parent) : QObject(parent)
{
    initColor();
    m_uAfterglowIndex = 0;
    m_uAfterglowType = 3;
    moveToThread(&m_threadWork);
    m_threadWork.start();
    qRegisterMetaType<Afterglow>("Afterglow");
    QObject::connect(this,SIGNAL(signalDrawRadarVideo(Afterglow)),this,SLOT(slotDrawRadarVideo(Afterglow)));
    QObject::connect(this,SIGNAL(signalDrawAfterglow(Afterglow)),this,SLOT(slotDrawAfterglow(Afterglow)));
}

ZCHXDrawRadarVideo::~ZCHXDrawRadarVideo()
{
    if(m_threadWork.isRunning())
    {
        m_threadWork.terminate();
        m_threadWork.quit();
    }
}

//void ZCHXDrawRadarVideo::setRadarVideoData(int uMsgIndex, const std::vector<RadarVideo> &dataVec)
//{
//    m_uMsgIndex = uMsgIndex;
//    m_RadarVideo = dataVec;
//}

//void ZCHXDrawRadarVideo::setRadarPoint(const std::vector<std::pair<double, double> > &path)
//{
//    m_path.clear();
//    if(path.size()<=0)
//        return;
//    m_path = path;
//}

void ZCHXDrawRadarVideo::setIndex(const int uIndex)
{
    m_uAfterglowType = uIndex;
}

void ZCHXDrawRadarVideo::slotDrawRadarVideo(const Afterglow &dataAfterglow)
{
    QMutex mutex;
    mutex.lock();
    //qDebug()<<"slotDrawRadarVideo--------------------------";
    std::vector<RadarVideoElement> RadarVideo = dataAfterglow.m_RadarVideo;

    int uMultibeamPixmapWidth = RadarVideoPixmapWidth;
    int uMultibeamPixmapHeight = RadarVideoPixmapHeight;
    QPixmap objPixmap(uMultibeamPixmapWidth,uMultibeamPixmapHeight);
    objPixmap.fill(Qt::transparent);//用透明色填充
    //objPixmap.fill(QColor(200,200,200,100));
    QPainter objPainter(&objPixmap);
    objPainter.setRenderHint(QPainter::Antialiasing,true);
    //objPainter.resetTransform();   // 重设画笔
    objPainter.save();
    objPainter.translate(QPoint(uMultibeamPixmapWidth / 2, uMultibeamPixmapHeight / 2));    // 中心点
    std::vector<RadarVideoElement>::iterator it;

    for(it=RadarVideo.begin(); it != RadarVideo.end(); ++it)
    {

        RadarVideoElement ele = (*it);
        ZCHX::Data::ITF_RadarVideo data = ele.data();

        double dAzimuth = data.dAzimuth;
        double dArc = dAzimuth*2*PI/180.0;
        //qDebug()<<"dAzimuth"<<dAzimuth;
        QList<int> amplitudeList = data.amplitudeList;
        QList<int> indexList = data.indexList;
        objPainter.rotate(dAzimuth+180);
        QPointF objPoint;

        double dCentreLon = data.dCentreLon;//回波中心经度
        double dCentreLat = data.dCentreLat;//回波中心纬度
        double dStartDis = data.dStartDis;  //开始距离
        double dDisInterval = data.dDisInterval;//距离间隔
        double dLat;
        double dLon;

        for(int i = 0;i<amplitudeList.size();i++)
        {

            int position = indexList[i];
            int value = amplitudeList[i];

//            double dDistance = dStartDis +position*dDisInterval;
//            ZCHX::Utils::distbear_to_latlon(dCentreLat,dCentreLon,
//                                            dDistance,dAzimuth,dLat,dLon);

//            if(!isInRadarPointRange(dLat,dLon,dataAfterglow.m_path))
//            {
//                continue;
//            }
            int colorValue = value*2;

            if(colorValue>200)
                colorValue = 200;
            QColor objColor = this->getColor(value);
            //qDebug()<<"dAzimuth"<<dAzimuth<<"position"<<position<<"value"<<value;
            //objPainter.setPen(QPen(QColor(colorValue,255,colorValue),1 /*data.uBitResolution*/));
            int nPenWidth = 1;
            if(position>=200&&position<400)
            {
                nPenWidth = 2;
            }
            else if(position>=400&&position<600)
            {
                nPenWidth = 4;
            }
            else if(position>=600&&position<800)
            {
                nPenWidth = 6;
            }
            else if(position>=800&&position<1000)
            {
                nPenWidth = 8;
            }
            else if(position>=1000)
            {
                nPenWidth = 10;
            }
            objPainter.setPen(QPen(objColor,nPenWidth));
            objPoint.setX(0);
            objPoint.setY(position);
            objPainter.drawPoint(objPoint);
            //objPainter.drawText(objPoint,QString::number(value));

        }
        objPainter.rotate(-(dAzimuth+180));
    }
    objPainter.translate(QPoint(-uMultibeamPixmapWidth / 2, -uMultibeamPixmapHeight / 2));
    objPainter.restore();
    //qDebug()<<"emit--------------------------";
    signalRadarVideoPixmap(0,objPixmap);
    mutex.unlock();
}

void ZCHXDrawRadarVideo::slotDrawAfterglow(const Afterglow &dataAfterglow)
{
    QMutex mutex;
    mutex.lock();
    std::vector<RadarVideoElement> RadarVideo = dataAfterglow.m_RadarVideo;
    //qDebug()<<"slotProcessData--------------------------";
    int uMultibeamPixmapWidth = RadarVideoPixmapWidth;
    int uMultibeamPixmapHeight = RadarVideoPixmapHeight;
    QPixmap objPixmap(uMultibeamPixmapWidth,uMultibeamPixmapHeight);
    objPixmap.fill(Qt::transparent);//用透明色填充
    //objPixmap.fill(QColor(200,200,200,100));
    QPainter objPainter(&objPixmap);
    objPainter.setRenderHint(QPainter::Antialiasing,true);
    //objPainter.resetTransform();   // 重设画笔
    objPainter.save();
    objPainter.translate(QPoint(uMultibeamPixmapWidth / 2, uMultibeamPixmapHeight / 2));    // 中心点
    std::vector<RadarVideoElement>::iterator it;

    for(it=RadarVideo.begin(); it != RadarVideo.end(); ++it)
    {

        RadarVideoElement ele = (*it);
        ZCHX::Data::ITF_RadarVideo data = ele.data();

        double dAzimuth = data.dAzimuth;
        double dArc = dAzimuth*2*PI/180.0;
        //qDebug()<<"dAzimuth"<<dAzimuth;
        QList<int> amplitudeList = data.amplitudeList;
        QList<int> indexList = data.indexList;
        objPainter.rotate(dAzimuth+180);
        QPointF objPoint;
        double dCentreLon = data.dCentreLon;//回波中心经度
        double dCentreLat = data.dCentreLat;//回波中心纬度
        double dStartDis = data.dStartDis;  //开始距离
        double dDisInterval = data.dDisInterval;//距离间隔
        double dLat;
        double dLon;
        for(int i = 0;i<amplitudeList.size();i++)
        {

            int position = indexList[i];
            int value = amplitudeList[i];

//            double dDistance = dStartDis +position*dDisInterval;
//            ZCHX::Utils::distbear_to_latlon(dCentreLat,dCentreLon,
//                                            dDistance,dAzimuth,dLat,dLon);

//            if(!isInRadarPointRange(dLat,dLon,dataAfterglow.m_path))
//            {
//                continue;
//            }

            int colorValue = value*2;

            if(colorValue>200)
                colorValue = 200;
            QColor objColor = this->getColor(value);
            //qDebug()<<"dAzimuth"<<dAzimuth<<"position"<<position<<"value"<<value;
            //objPainter.setPen(QPen(QColor(colorValue,255,colorValue),1 /*data.uBitResolution*/));
            int nPenWidth = 1;
            if(position>=200&&position<400)
            {
                nPenWidth = 2;
            }
            else if(position>=400&&position<600)
            {
                nPenWidth = 4;
            }
            else if(position>=600&&position<800)
            {
                nPenWidth = 6;
            }
            else if(position>=800&&position<1000)
            {
                nPenWidth = 8;
            }
            else if(position>=1000)
            {
                nPenWidth = 10;
            }
            objPainter.setPen(QPen(objColor,nPenWidth));
            objPoint.setX(0);
            objPoint.setY(position);
            objPainter.drawPoint(objPoint);

        }
        objPainter.rotate(-(dAzimuth+180));
    }
    objPainter.translate(QPoint(-uMultibeamPixmapWidth / 2, -uMultibeamPixmapHeight / 2));
    objPainter.restore();
    signalRadarVideoPixmap(m_uAfterglowIndex,objPixmap);
    m_uAfterglowIndex++;
    m_uAfterglowIndex = m_uAfterglowIndex%m_uAfterglowType;

    //qDebug()<<"save--------------------------"<<m_uAfterglowIndex;
//    QString path = QCoreApplication::applicationDirPath();
//    QString str = QString("/data/Afterglow_%1.png").arg(m_uAfterglowIndex);
//    path = path+str;
//    qDebug()<<"path"<<path;
//    objPixmap.save(path);
    mutex.unlock();
}

void ZCHXDrawRadarVideo::initColor()
{
    m_colorMap.clear();
    const int colorBarLength = 100;
    QColor color;
    float tempLength=colorBarLength/4;
    for(int i=0;i<tempLength/2;i++)// jet
    {
        color.setRgbF(0,0,(tempLength/2+i)/tempLength);
        m_colorMap[i] = color;
    }
    for(int i=tempLength/2+1;i<tempLength/2+tempLength;i++)// jet
    {
        color.setRgbF(0,(i-tempLength/2)/tempLength,1);
        m_colorMap[i] = color;
    }
    for(int i=tempLength/2+tempLength+1;i<tempLength/2+2*tempLength;i++)// jet
    {
        color.setRgbF((i-tempLength-tempLength/2)/tempLength,1,(tempLength*2+tempLength/2-i)/tempLength);
        m_colorMap[i] = color;
    }
    for(int i=tempLength/2+2*tempLength+1;i<tempLength/2+3*tempLength;i++)// jet
    {
        color.setRgbF(1,(tempLength*3+tempLength/2-i)/tempLength,0);
        m_colorMap[i] = color;
    }
    for(int i=tempLength/2+3*tempLength+1;i<colorBarLength;i++)// jet
    {
        color.setRgbF((colorBarLength-i+tempLength/2)/(tempLength),0,0);
        m_colorMap[i] = color;
    }
}

QColor ZCHXDrawRadarVideo::getColor(double dValue)
{
    double m_dMinZ = 0;
    double m_dMaxZ = 255;
    if(dValue>m_dMaxZ)
    {
        return QColor(255,0,0);
    }
    if(dValue<m_dMinZ)
    {
        return QColor(0,0,255);
    }
    if(m_dMaxZ-m_dMinZ<0.0001)
    {
        return QColor(255,0,0);
    }
    double dTemp = (dValue-m_dMinZ)/(m_dMaxZ-m_dMinZ);
    int uIndex = dTemp*100;
    if(uIndex<0)
    {
        return QColor(0,0,255);
    }
    if(uIndex>=100)
    {
        return QColor(255,0,0);
    }
    QColor objColor = m_colorMap[uIndex];
    return objColor;
}

bool ZCHXDrawRadarVideo::isInRadarPointRange(const double dLat, const double dLon, const std::vector<std::pair<double, double> > path)
{
    int nNum = path.size();
    //qDebug()<<"m_path---num"<<nNum;
    if(nNum<=0)
    {
        return true;
    }
    std::vector<std::pair<double, double>> tempPath = path;
    const int uRange = 100;//m
    std::vector<std::pair<double, double>>::iterator it = tempPath.begin();
    for(;it!=tempPath.end();it++)
    {
        std::pair<double, double> data = *it;
        double dPointLat = data.first;
        double dPointLon = data.second;
        double dDistance = ZCHX::Utils::getDis(dPointLon,dPointLat,
                                                  dLon,dLat);

//        qDebug()<<"dPointLon"<<dPointLon;
//        qDebug()<<"dPointLat"<<dPointLat;
//        qDebug()<<"dLon"<<dLon;
//        qDebug()<<"dLat"<<dLat;
//        qDebug()<<"dDistance"<<dDistance;
        if(dDistance<=uRange)
        {
            return true;
        }
    }
    return false;
}
}
