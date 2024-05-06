#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "flir_camera.h"

#include <QWidget>
#include <iostream>
#include <sstream>

using namespace std;

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
    void initGainSlider();
    int defaultGainMax = 18;
    int defaultGainMin = 1;
    void initTrigger();
    void initTriggerMode();
private:
    Ui::SettingsWidget *ui;
    FlirCamera* cam;
private slots:
    void setMaxExpoSliderWidth();
    void setExposureMode(Qt::CheckState state);
    void setMinExpoSliderWidth();
    void setMinGainSliderWidth();
    void setMaxGainSliderWidth();
    void setGainMode(Qt::CheckState state);
};

#endif // SETTINGSWIDGET_H
