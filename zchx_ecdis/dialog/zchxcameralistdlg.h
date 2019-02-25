#ifndef ZCHXCAMERALISTDWG_H
#define ZCHXCAMERALISTDWG_H
#include "../zchxutils.hpp"
#include <QVBoxLayout>
#include <QListView>
#include <QLabel>
#include <QDialog>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPushButton>

namespace qt {
class ZCHXCameraListDlg : public QDialog
{
    Q_OBJECT
public:
    explicit  ZCHXCameraListDlg(const QList<ZCHX::Data::ITF_CameraDev> &cameraList, QWidget * parent = 0);
    QList<ZCHX::Data::ITF_CameraDev> getItems() const;
    void setItems(const QList<ZCHX::Data::ITF_CameraDev> &cameraList);
    void initStyleSheet();

protected:
    bool eventFilter(QObject *, QEvent *e);

signals:
    /**
     * @brief 相机图标双击事件
     */
    void cameraDevSelected(const ZCHX::Data::ITF_CameraDev &);
    void cameraDevDoubleClicked(const ZCHX::Data::ITF_CameraDev &);

private slots:
    void cameraClicked(QModelIndex idx);
    void cameraDoubleClicked(QModelIndex idx);

private:
    QList<ZCHX::Data::ITF_CameraDev> m_cameraList;
    QListView *m_pCamerListView;
    QStandardItemModel *m_pModel;
    QPushButton *m_pCloseBtn;
};
}

#endif // ZCHXCAMERALISTDWG_H
