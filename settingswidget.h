#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "flir_camera.h"

#include <QWidget>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(FlirCamera *camera, QWidget *parent);
    ~SettingsWidget();
    void initExposureSlider();
    int defaultExpoMax = 20000;
    int defaultExpoMin = 6;
private:
    Ui::SettingsWidget *ui;
    FlirCamera* cam;
private slots:
    void setMaxSliderWidth();
    void setExposureMode(Qt::CheckState state);
};

#endif // SETTINGSWIDGET_H
