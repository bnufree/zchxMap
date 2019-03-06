#ifndef RADARFEATUREZONE_H
#define RADARFEATUREZONE_H

#include <QDialog>

namespace Ui {
class RadarFeatureZone;
}

class RadarFeatureZone : public QDialog
{
    Q_OBJECT

public:
    explicit RadarFeatureZone(QWidget *parent = 0);
    ~RadarFeatureZone();
    QString getName();
    int getType();
    void setLangague();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::RadarFeatureZone *ui;
};

#endif // RADARFEATUREZONE_H
