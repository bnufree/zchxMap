#include "zchxdrawmultibeam.h"
#include <QPainter>
#include <QDebug>
#include <QPointF>

// 排序函数
bool sortByX(const DrawElement::Multibeam &v1, const DrawElement::Multibeam &v2)
{
    return v1.data().m_dX < v2.data().m_dX;//升序排列
}

bool sortByY(const DrawElement::Multibeam &v1, const DrawElement::Multibeam &v2)
{
    return v1.data().m_dY < v2.data().m_dY;//升序排列
}

ZCHXDrawMultibeam::ZCHXDrawMultibeam(QObject *parent)
    :QObject(parent)
{
    moveToThread(&m_threadWork);
    m_threadWork.start();
    QObject::connect(this,SIGNAL(signalDrawMultibeam()),this,SLOT(zchxDrawMultibeam()));
}

void ZCHXDrawMultibeam::itfSetMultibeamData(const QList<ZCHX::Data::ITF_Multibeam> &data, const double dMinX, const double dMinY, const double dMinZ, const double dMaxX, const double dMaxY, const double dMaxZ)
{
    std::vector<DrawElement::Multibeam> list;
    for(int i=0; i< data.count(); ++i)
    {
        ZCHX::Data::ITF_Multibeam tmp = data.at(i);
        DrawElement::Multibeam item(tmp);
        list.push_back(item);
    }

    setMultibeamData(list,dMinX,dMinY,dMinZ,dMaxX,dMaxY,dMaxZ);
}

void ZCHXDrawMultibeam::setMultibeamData(const std::vector<DrawElement::Multibeam> &data, const double dMinX, const double dMinY, const double dMinZ, const double dMaxX, const double dMaxY, const double dMaxZ)
{
    m_Multibeam = data;
    if(m_Multibeam.size()<=0)
        return;

    std::sort(m_Multibeam.begin(), m_Multibeam.end(), sortByX);

    m_dMinX = dMinX;
    m_dMaxX = dMaxX;
    m_dMinY = dMinY;
    m_dMaxY = dMaxY;
//    m_dMinZ = dMinZ;
//    m_dMaxZ = dMaxZ;
    m_dMinZ = MIN_DEPTH;
    m_dMaxZ = MAX_DEPTH;
    m_dXInc = 0;
    m_dYInc = 0;
    QList<double> objXIncList;
    QList<double> objYIncList;
    objXIncList.clear();
    objYIncList.clear();

    int itemSize = m_Multibeam.size();
    for(int i = 0; i < itemSize - 1; i++)
    {
        const ZCHX::Data::ITF_Multibeam & objMultibeamData = m_Multibeam[i].data();
        const ZCHX::Data::ITF_Multibeam & nextMultibeamData = m_Multibeam[i + 1].data();
        double dXInc = objMultibeamData.m_dX - nextMultibeamData.m_dX;
        double dYInc = objMultibeamData.m_dY - nextMultibeamData.m_dY;

        if(qAbs(dXInc) > 0)
        {
           objXIncList.append(qAbs(dXInc));
        }
        if(qAbs(dYInc) > 0)
        {
           objYIncList.append(qAbs(dYInc));
        }
    }
    //查找最小的
    if(objXIncList.count() > 0)
    {
        m_dXInc = objXIncList[0];
        for(int i = 1;i<objXIncList.count();i++)
        {
            if(m_dXInc > objXIncList[i])
            {
                m_dXInc = objXIncList[i];
            }
        }
//        double dXSumInc = 0;
//        for(int i = 0; i < objXIncList.count(); i++)
//        {
//            dXSumInc += objXIncList[i];
//        }

//        m_dXInc = dXSumInc / objXIncList.count();
    }
    if(objYIncList.count()>0)
    {
        m_dYInc = objYIncList[0];
        for(int i = 1;i<objYIncList.count();i++)
        {
            if(m_dYInc>objYIncList[i])
            {
                m_dYInc = objYIncList[i];
            }
        }
//        double dYSumInc = 0;
//        for(int i = 0; i < objYIncList.count(); i++)
//        {
//            dYSumInc += objYIncList[i];
//        }

//        m_dYInc = dYSumInc / objYIncList.count();
    }
    initMultibeamColor();
}

void ZCHXDrawMultibeam::multibeamTransform(const double dRealX, const double dRealY, double &dPixmapX, double &dPixmapY)
{
    if(dRealX<m_dMinX)
    {
        dPixmapX = 0;
    }
    if(dRealX>m_dMaxX)
    {
        dPixmapX = m_uMultibeamPixmapWidth;
    }
    dPixmapX = (dRealX-m_dMinX)/(m_dMaxX-m_dMinX)*m_uMultibeamPixmapWidth;

    if(dRealY<m_dMinY)
    {
        dPixmapY = 0;
    }
    if(dRealY>m_dMaxY)
    {
        dPixmapY = m_uMultibeamPixmapHeight;
    }
    dPixmapY = (dRealY-m_dMinY)/(m_dMaxY-m_dMinY)*m_uMultibeamPixmapHeight;
}

void ZCHXDrawMultibeam::drawPolygon(int uIndex, double dCurX, double dCurY, double dXInc, double dYInc, double dCurHeight, QPainter &objPainter)
{
    double dPixmapX;
    double dPixmapY;
    multibeamTransform(dCurX,dCurY,dPixmapX,dPixmapY);

    //找相邻第一个点
    bool bOK1 = false;
    double dFirstX = dCurX + dXInc;
    double dFirstY = dCurY;
    double dFirstHeight = 0;
    if(isExistInMultibeamBySort(uIndex, dFirstX, dFirstY, dFirstHeight))
    {
        bOK1 = true;
    }

     //找相邻第二个点
    bool bOK2 = false;
    double dSecondX = dCurX;
    double dSecondY = dCurY + dYInc;
    double dSecondHeight = 0;
    if(isExistInMultibeamBySort(uIndex, dSecondX, dSecondY, dSecondHeight))
    {
        bOK2 = true;
    }

    if(bOK1 && bOK2)
    {
        double dPixmapFirstX;
        double dPixmapFirstY;
        multibeamTransform(dFirstX, dFirstY, dPixmapFirstX, dPixmapFirstY);

        double dPixmapSecondX;
        double dPixmapSecondY;
        multibeamTransform(dSecondX, dSecondY, dPixmapSecondX, dPixmapSecondY);

        double dColor = (dCurHeight + dFirstHeight + dSecondHeight) / 3.0;
        const QPointF points[3] = {
            QPointF(dPixmapX, dPixmapY),
            QPointF(dPixmapFirstX, dPixmapFirstY),
            QPointF(dPixmapSecondX, dPixmapSecondY)
        };

        objPainter.save();
        QColor objColor = getMultibeamColor(dColor);
        objPainter.setPen(objColor);
        objPainter.setBrush(objColor);
        objPainter.drawPolygon(points, 3);
        objPainter.restore();
    }
}

QPixmap ZCHXDrawMultibeam::zchxDrawSample()
{
    int width = 400;
    int height = 20;
    int textHeight = 20;
    int percentNum = 100;
    int itemWidth = width / percentNum;
    QPixmap objPixmap(width, height + textHeight);
    objPixmap.fill(Qt::transparent);//用透明色填充
    QPainter objPainter(&objPixmap);

    for(int i = 0; i < percentNum; i++)
    {
        if (m_colorMap.contains(i))
        {
            objPainter.save();
            QColor color = m_colorMap[i];
            objPainter.setPen(QPen(color, 1));
            objPainter.setBrush(color);
            objPainter.drawRect(itemWidth * i, textHeight, itemWidth, height);
            objPainter.restore();
        }
    }

    objPainter.save();

    int dMinZ = -2000;
    int dMaxZ = 4000;
    int startOffset = 400.0 / (m_dMaxZ - m_dMinZ) * width;
    int textOffset = 40;
    int depth = m_dMinZ;
    int textNum = 6;
    for (int i = 0; i <= textNum; i++)
    {
        depth = dMinZ + (dMaxZ - dMinZ) / textNum * i;
        if (depth == 0)
        {
            objPainter.drawText((width - startOffset) / textNum * i + startOffset, height - 5, QString::number(depth));
        }
        else
        {
            objPainter.drawText((width - startOffset - textOffset) / textNum * i + startOffset, height - 5, QString::number(depth));
        }
    }

    objPainter.restore();

    return objPixmap;
}

void ZCHXDrawMultibeam::zchxDrawMultibeam()
{
    m_uMultibeamPixmapWidth = 2000;
    m_uMultibeamPixmapHeight = 2000;
    QPixmap objPixmap(m_uMultibeamPixmapWidth, m_uMultibeamPixmapHeight);
    objPixmap.fill(Qt::transparent);//用透明色填充
    QPainter objPainter(&objPixmap);
    std::vector<DrawElement::Multibeam>::iterator it;

    int uIndex = 0;
    int size = m_Multibeam.size();
    for(; uIndex < size; uIndex++)
    {
        DrawElement::Multibeam ele = m_Multibeam.at(uIndex);
        ZCHX::Data::ITF_Multibeam objMultibeamData = ele.data();

        double dCurX = objMultibeamData.m_dX;
        double dCurY = objMultibeamData.m_dY;
        double dCurHeight = objMultibeamData.m_dHeight;

        //第一个三角形
        drawPolygon(uIndex, dCurX, dCurY, m_dXInc, m_dYInc, dCurHeight, objPainter);

        //第二个三角形
        drawPolygon(uIndex, dCurX, dCurY, -m_dXInc, -m_dYInc, dCurHeight, objPainter);

//        //第三个三角形
//        drawPolygon(uIndex, dCurX, dCurY, m_dXInc, -m_dYInc, dCurHeight, objPainter);

//        //第四个三角形
//        drawPolygon(uIndex, dCurX, dCurY, -m_dXInc, m_dYInc, dCurHeight, objPainter);

        emit signalDrawPercent(uIndex);
    }
    //qDebug()<<"emit--------------------------";

    QPixmap samplePixmap = zchxDrawSample();//绘制色阶条

    emit signalMultibeamPixmap(samplePixmap, objPixmap);
}

void ZCHXDrawMultibeam::initMultibeamColor()
{
    m_colorMap.clear();
    const int colorBarLength = 100;
    QColor color;
    float tempLength = colorBarLength / 4;
    for(int i = 0; i < tempLength / 2; i++)// jet
    {
        color.setRgbF((tempLength / 2 + i) / tempLength, 0, 0);
        m_colorMap[i] = color;
    }
    for(int i = tempLength / 2 + 1; i < tempLength / 2 + tempLength; i++)// jet
    {
        color.setRgbF(1, (i - tempLength / 2) / tempLength, 0);
        m_colorMap[i] = color;
    }
    for(int i = tempLength / 2 + tempLength + 1; i < tempLength / 2 + 2 * tempLength; i++)// jet
    {
        color.setRgbF((tempLength * 2 + tempLength / 2 - i) / tempLength, 1 , (i - tempLength - tempLength / 2) / tempLength);
        m_colorMap[i] = color;
    }
    for(int i = tempLength / 2 + 2 * tempLength + 1; i < tempLength / 2 + 3 * tempLength; i++)// jet
    {
        color.setRgbF(0, (tempLength * 3 + tempLength / 2 - i) / tempLength, 1);
        m_colorMap[i] = color;
    }
    for(int i = tempLength / 2 + 3 * tempLength + 1; i < colorBarLength; i++)// jet
    {
        color.setRgbF(0, 0, (colorBarLength - i + tempLength / 2) / (tempLength));
        m_colorMap[i] = color;
    }
}

// 颜色浅(红)->深(蓝)
QColor ZCHXDrawMultibeam::getMultibeamColor(double dValue)
{
    if(dValue > m_dMaxZ)
    {
        return QColor(0, 0, 255);
    }
    if(dValue < m_dMinZ)
    {
        return QColor(255, 0, 0);
    }
    if(m_dMaxZ - m_dMinZ < 0.0001)
    {
        return QColor(255, 0, 0);
    }
    double dTemp = (dValue - m_dMinZ) / (m_dMaxZ - m_dMinZ);
    int uIndex = dTemp * 100;
    if(uIndex < 0)
    {
        return QColor(255, 0, 0);
    }
    if(uIndex >= 100)
    {
        return QColor(0, 0, 255);
    }
    QColor objColor = m_colorMap[uIndex];
    return objColor;
}

bool ZCHXDrawMultibeam::isExistInMultibeam(const double dX, const double dY, double &dHeight)
{
    dHeight = 0;
    std::vector<DrawElement::Multibeam>::iterator it;
    for (it = m_Multibeam.begin(); it != m_Multibeam.end(); ++it)
    {
        DrawElement::Multibeam ele = (*it);
        ZCHX::Data::ITF_Multibeam objMultibeamData = ele.data();
        if((qAbs(objMultibeamData.m_dX - dX) < 1) && (qAbs(objMultibeamData.m_dY - dY) < 1))
        {
            dHeight = objMultibeamData.m_dHeight;
            return true;
        }
    }
    return false;
}

bool ZCHXDrawMultibeam::isExistInMultibeamBySort(const int index, const double dX, const double dY, double &dHeight)
{
    dHeight = 0;
    int size = m_Multibeam.size();

    for (int i = index; i < size; i++)
    {
        DrawElement::Multibeam ele = m_Multibeam.at(i);
        ZCHX::Data::ITF_Multibeam objMultibeamData = ele.data();

        if (objMultibeamData.m_dX > dX && (qAbs(objMultibeamData.m_dX - dX) > 1))
        {
            break;
        }

        if ((qAbs(objMultibeamData.m_dX - dX) < 1) && (qAbs(objMultibeamData.m_dY - dY) < 1))
        {
            dHeight = objMultibeamData.m_dHeight;
            return true;
        }
    }

    for (int i = index - 1; i >= 0; i--)
    {
        DrawElement::Multibeam ele = m_Multibeam.at(i);
        ZCHX::Data::ITF_Multibeam objMultibeamData = ele.data();

        if (objMultibeamData.m_dX < dX && (qAbs(objMultibeamData.m_dX - dX) > 1))
        {
            break;
        }

        if ((qAbs(objMultibeamData.m_dX - dX) < 1) && (qAbs(objMultibeamData.m_dY - dY) < 1))
        {
            dHeight = objMultibeamData.m_dHeight;
            return true;
        }
    }

    return false;
}
