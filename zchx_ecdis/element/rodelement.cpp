#include "rodelement.h"
#include "zchxmapframe.h"
#include "cameraelement.h"
#include "ipcelement.h"
#include "zchxmapwidget.h"

using namespace qt;

RodElement::RodElement(const ZCHX::Data::ITF_CameraRod &data,zchxMapWidget* f)
    :FixElement<ZCHX::Data::ITF_CameraRod>(data, ZCHX::Data::ELE_ROD, ZCHX::LAYER_WIREROD, f)
{
}


QList<ZCHX::Data::ITF_CameraDev> RodElement::getCameraList() const
{
    return mCameraList;
}


QList<ZCHX::Data::IPCastDevice> RodElement::getIPCList() const
{
    return mIpcList;
}


void RodElement::setStatus(ZCHX::Data::CAMERAROD_STATUS st)
{
    m_data.nStatus = st;
}

ZCHX::Data::CAMERAROD_STATUS RodElement::status() const
{
    return m_data.nStatus;
}

void RodElement::updateCamera(const ZCHX::Data::ITF_CameraDev &data)
{
    bool found = false;
    for(int i=0; i<mCameraList.size(); i++)
    {
        if(mCameraList[i].nDBID == data.nDBID)
        {
            mCameraList[i] = data;
            found = true;
            break;
        }
    }
    if(!found) mCameraList.append(data);
}

void RodElement::updateIPC(const ZCHX::Data::IPCastDevice &data)
{
    bool found = false;
    for(int i=0; i<mIpcList.size(); i++)
    {
        if(mIpcList[i].id == data.id)
        {
            mIpcList[i] = data;
            found = true;
            break;
        }
    }
    if(!found) mIpcList.append(data);
}

void RodElement::drawElement(QPainter *painter)
{
    if(!isDrawAvailable(painter)) return;
    //这里不显示字图元 通过杆的操作才显示子图元
    /*绘制图片元素*/
    int curScale = getDrawScaleSize();
    QPixmap cameraRadImg        = ZCHX::Utils::getImage(":/element/gan_normal.png", Qt::green, curScale);
    QPixmap cameraRodWarringImg = ZCHX::Utils::getImage(":/element/gan_bug.png", Qt::yellow, curScale);
    QPixmap cameraRodErrImg     = ZCHX::Utils::getImage(":/element/gan_error.png", Qt::red, curScale);
    QPixmap cameraRodFocus      = ZCHX::Utils::getImage(":/element/gan_bg.png", Qt::yellow, curScale);

    ZCHX::Data::ITF_CameraRod data =  this->data();
    QPointF pos = getCurrentPos();
    QRect rect(0, 0, cameraRadImg.width(),cameraRadImg.height());
    rect.moveCenter(pos.toPoint());
    updateBouningRect(pos, cameraRadImg.width(), cameraRadImg.height());
    updateGeometry(pos, getDrawScaleSize());
    if(getIsHover())
    {
        //通过推算经纬度计算半径
        ZCHX::Data::LatLon drll(0, 0);
        ZCHX::Utils::distbear_to_latlon(data.nLatLon.lat,data.nLatLon.lon,800,0,drll.lat,drll.lon);
        ZCHX::Data::Point2D drPos = mView->framework()->LatLon2Pixel(drll);
        double dr = sqrt(abs(drPos.y - pos.y()) * abs(drPos.y-pos.y()) + abs(drPos.x - pos.x()) * abs(drPos.x - pos.x()));
        PainterPair chk(painter);
        painter->setBrush(QBrush(QColor(0,255,255,125)));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(pos,dr,dr);
    }
    if(getIsFocus()) //是否聚焦状态
    {
        painter->drawPixmap(rect,cameraRodFocus);
    }

    ZCHX::Data::CAMERAROD_STATUS status = this->status();
    if(ZCHX::Data::CAMERAROD_WARRING == status)
    {
        painter->drawPixmap(rect, cameraRodWarringImg);
    }
    else if(ZCHX::Data::CAMERAROD_ERROR == status)
    {
        painter->drawPixmap(rect, cameraRodErrImg);
    }
    else
    {
        painter->drawPixmap(rect, cameraRadImg);
    }

    drawActive(painter);
}

void RodElement::copyDataFromOther(std::shared_ptr<Element> other)
{
    if(!other) return;
    RodElement *src = dynamic_cast<RodElement*>(other.get());
    if(src)
    {
        this->setData(src->data());
    }
}

void RodElement::updateElementStatus(qint64 ele, bool sts)
{
    if(sts) {
        if(mErrEleList.contains(ele)) mErrEleList.removeOne(ele);
        if(!mOKEleList.contains(ele)) mOKEleList.append(ele);
    } else {
        if(!mErrEleList.contains(ele)) mErrEleList.append(ele);
        if(mOKEleList.contains(ele)) mOKEleList.removeOne(ele);
    }
    if(mErrEleList.size() == 0) {
        setStatus(ZCHX::Data::CAMERAROD_OK);
    } else {
        setStatus(ZCHX::Data::CAMERAROD_ERROR);
    }
}
