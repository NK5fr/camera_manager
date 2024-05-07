#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "flir_camera.h"

#include <QWidget>
#include <iostream>
#include <sstream>
#include <string.h>
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
    void updateVisibility();
    void showFileDialog();
private:
    Ui::SettingsWidget *ui;
    FlirCamera *cam;
    void setMaxExpoSliderWidth();
    void setExposureMode(Qt::CheckState state);
    void setMinExpoSliderWidth();
    void setMinGainSliderWidth();
    void setMaxGainSliderWidth();
    void setGainMode(Qt::CheckState state);
signals:
    void fileNameChanged(QString filePath);
};

#endif // SETTINGSWIDGET_H
