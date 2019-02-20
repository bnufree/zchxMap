#ifndef ZCHXDRAWMULTIBEAM_H
#define ZCHXDRAWMULTIBEAM_H
#include <QPixmap>
#include <QObject>
#include <QColor>
#include <QMap>
#include <QThread>
#include "element/drawelement.hpp"

#define MAX_DEPTH 4000
#define MIN_DEPTH -2400

namespace qt {
class ZCHX_ECDIS_EXPORT ZCHXDrawMultibeam :public QObject
{
    Q_OBJECT
public:
    ZCHXDrawMultibeam(QObject *parent=Q_NULLPTR);

    void itfSetMultibeamData(const QList<ZCHX::Data::ITF_Multibeam> &data,
                             const double dMinX,const double dMinY,
                             const double dMinZ,const double dMaxX,
                             const double dMaxY,const double dMaxZ);

    void setMultibeamData(const std::vector<Multibeam> &data,
                          const double dMinX, const double dMinY,
                          const double dMinZ, const double dMaxX,
                          const double dMaxY, const double dMaxZ);         //设置多波束数据
signals:
    void signalDrawMultibeam();
    void signalMultibeamPixmap(const QPixmap &samplePixmap, const QPixmap &objPixmap);
    void signalDrawPercent(int);
public slots:
    void zchxDrawMultibeam();//绘制多波束
protected:
    void multibeamTransform(const double dRealX,const double dRealY,double &dPixmapX,double &dPixmapY);    
    void drawPolygon(int uIndex, double dCurX, double dCurY, double dXInc, double dYInc, double dCurHeight, QPainter & objPainter);

    QPixmap zchxDrawSample();//绘制色阶条

    void initMultibeamColor();//初始化多波束数据的颜色组
    QColor getMultibeamColor(double dValue);//通过多波束的高度值获取对应颜色值
    bool isExistInMultibeam(const double dX, const double dY, double &dHeight);//多波束数据中是否存在该点，并返回高度值
    bool isExistInMultibeamBySort(const int index, const double dX, const double dY, double &dHeight);//多波束数据中是否存在该点，并返回高度值

private:
    std::vector<Multibeam> m_Multibeam;                    //多波束数据

    double m_dMinZ;//多波束数据高度最小值
    double m_dMaxZ;//多波束数据高度最大值
    double m_dMinX;
    double m_dMaxX;
    double m_dMinY;
    double m_dMaxY;
    double m_dXInc;//多波束数据的X间隔
    double m_dYInc;//多波束数据的Y间隔
    int m_uMultibeamPixmapWidth;//多波束数据图片宽度
    int m_uMultibeamPixmapHeight;//多波束数据图片高度
    QMap<int,QColor> m_colorMap;//多波束数据颜色map

    QThread             m_threadWork;
};
}

#endif // ZCHXDRAWMULTIBEAM_H
