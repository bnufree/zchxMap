#ifndef STRUCTUREINFODIALOG_H
#define STRUCTUREINFODIALOG_H

#include <QDialog>

namespace Ui {
class StructureInfoDialog;
}

namespace qt {
class StructureInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StructureInfoDialog(QWidget *parent = 0);
    ~StructureInfoDialog();
    QString getName();
    double getArea();
    double getHeight();

private slots:
    void on_comfirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::StructureInfoDialog *ui;
};
}

#endif // STRUCTUREINFODIALOG_H
