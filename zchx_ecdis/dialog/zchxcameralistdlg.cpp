#include "zchxcameralistdlg.h"
#include <QtGui>

namespace qt {
ZCHXCameraListDlg::ZCHXCameraListDlg(const QList<ZCHX::Data::ITF_CameraDev> &cameraList, QWidget *parent)
    : QDialog(parent),
      m_cameraList(cameraList),
      m_pCamerListView(new QListView(this)),
      m_pModel(new QStandardItemModel(this)),
      m_pCloseBtn(new QPushButton(this))
{
    m_pCamerListView->installEventFilter(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_pCloseBtn);
    layout->addWidget(m_pCamerListView);
    setLayout(layout);
    setLayoutDirection(Qt::LeftToRight);

    m_pCamerListView->setIconSize(QSize(64,64));
    m_pCamerListView->setGridSize(QSize(85,110));
    m_pCamerListView->setEditTriggers(0);
    m_pCamerListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pCamerListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pCamerListView->setModel(m_pModel);
    m_pCamerListView->setFlow(QListView::LeftToRight);
    m_pCamerListView->setViewMode(QListView::IconMode);
    m_pCamerListView->setMovement(QListView::Static);
    m_pCamerListView->setWordWrap(true);

    //init model
    setItems(cameraList);
    connect(m_pCamerListView,SIGNAL(clicked(QModelIndex)),this,SLOT(cameraClicked(QModelIndex)));
    connect(m_pCamerListView,&QListView::doubleClicked,this,&ZCHXCameraListDlg::cameraDoubleClicked);
    connect(m_pCloseBtn,SIGNAL(clicked()),this,SLOT(accept()));

    initStyleSheet();
    m_pCloseBtn->setStyleSheet("QPushButton{background:rgba(39,39,39,0);border-image:url(:/ui/x.png);border:none;max-height:20px;min-height:20px; max-width:20px;min-width:20px;font-size:10pt;border-radius:3px;}");
    m_pCamerListView->setStyleSheet("QListView{background:rgba(33,33,33,172);border:none;color:#FFFFFF;font-size:10pt;}");
}

QList<ZCHX::Data::ITF_CameraDev> ZCHXCameraListDlg::getItems() const
{
    return m_cameraList;
}

void ZCHXCameraListDlg::setItems(const QList<ZCHX::Data::ITF_CameraDev> &cameraList)
{
    m_pModel->clear();
    resize(m_pCamerListView->gridSize().width() * cameraList.size(), height());

    m_cameraList = cameraList;
    for(const ZCHX::Data::ITF_CameraDev camera : m_cameraList)
    {
        QString iconName = ZCHX::Utils::getImagePath(camera.nType, camera.nStatus);

        QStandardItem * item = new QStandardItem(camera.szCamName);
        item->setToolTip(camera.szCamName);
        item->setIcon(QIcon(iconName));

        if(camera.nStatus != 0)
            item->setFlags(Qt::NoItemFlags);

        m_pModel->appendRow(item);
    }
    m_pModel->setRowCount(m_cameraList.count());
}

void ZCHXCameraListDlg::initStyleSheet()
{
    QString style("*{\
                  font-family:Microsoft YaHei;\
            font-size:12pt;  \
}\
QWidget,QFrame,QDialog{\
background:rgba(0,0,0,0);\
border:none;\
}"
"QScrollBar:vertical{\
width:8px;\
background:rgba(0,0,0,0%);\
margin:0px,0px,0px,0px;\
padding-top:9px;\
padding-bottom:9px;}"
"QScrollBar::handle:vertical{\
width:8px;\
background:rgba(0,0,0,25%);\
border-radius:4px;\
min-height:20;}"
"QScrollBar::handle:vertical:hover {\
width:8px;\
background:rgba(0,0,0,50%);\
border-radius:4px;\
min-height:20;}"
"QScrollBar::add-line:vertical{\
height:9px;width:8px;\
border-image:url(:/scrollbar/scrollbar/3.png);\
subcontrol-position:bottom;\}"
"QScrollBar::sub-line:vertical\
{\
height:9px;width:8px;\
border-image:url(:/scrollbar/scrollbar/1.png);\
subcontrol-position:top;\
}\
QScrollBar::add-line:vertical:hover\
{\
                         height:9px;width:8px;\
border-image:url(:/scrollbar/scrollbar/4.png);\
subcontrol-position:bottom;\
}\
QScrollBar::sub-line:vertical:hover\
{\
                         height:9px;width:8px;\
border-image:url(:/scrollbar/scrollbar/2.png);\
subcontrol-position:top;\
}\
QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical\
{\
                         background:rgba(0,0,0,10%);\
border-radius:4px;\
}\
QScrollBar:horizontal\
{\
height:8px;\
background:rgba(0,0,0,0%);\
margin:0px,0px,0px,0px;\
    padding-left:9px;\
    padding-right:9px;\
}\
QScrollBar::handle:horizontal\
{\
                       width:8px;\
background:rgba(0,0,0,25%);\
border-radius:4px;\
min-height:20;\
}"
"QScrollBar::handle:horizontal:hover{ height:8px;background:rgba(0,0,0,50%); border-radius:4px;min-height:20;}"
"QScrollBar::add-line:horizontal{ height:9px;width:8px;border-image:url(:/scrollbar/scrollbar/33.png);subcontrol-position:right; }"
"QScrollBar::sub-line:horizontal{ height:9px;width:8px;border-image:url(:/scrollbar/scrollbar/11.png);subcontrol-position:left;}"
"QScrollBar::add-line:horizontal:hover{ height:9px;width:8px; border-image:url(:/scrollbar/scrollbar/44.png);subcontrol-position:right;}"
"QScrollBar::sub-line:horizontal:hover{height:9px;width:8px;border-image:url(:/scrollbar/scrollbar/22.png);subcontrol-position:left;}"
"QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:rgba(0,0,0,10%); border-radius:4px;}");
setStyleSheet(style);
}

bool ZCHXCameraListDlg::eventFilter(QObject *, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(e);
        Q_ASSERT(mouseEvent);
        QModelIndex index = m_pCamerListView->indexAt(mouseEvent->pos());
        if(index.isValid())
        {
            cameraDoubleClicked(index);
        }
    }
    return false;
}

/**
 * @brief ZCHXCameraListDlg::cameraClicked
 */
void ZCHXCameraListDlg::cameraClicked(QModelIndex idx)
{
    ZCHX::Data::ITF_CameraDev info = m_cameraList[idx.row()];
    //    QString text = QString("<span style=\"color:#FFFFFF; font-size:12pt\">  %1    %2<br>  %3</p></span>").arg(info.szCamName).arg(info.szControlIP).arg(info.szPreviewRtspUrl);
    //    m_pCamerInfoLabel->setText(text);
    emit cameraDevSelected(info);
    accept();
}

void ZCHXCameraListDlg::cameraDoubleClicked(QModelIndex idx)
{
    ZCHX::Data::ITF_CameraDev info = m_cameraList[idx.row()];
    emit cameraDevDoubleClicked(info);
}
}
