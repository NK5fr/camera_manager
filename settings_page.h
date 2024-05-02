#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "flir_camera.h"

#include <QWidget>

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(FlirCamera *camera, QWidget *parent);
    void initExposureSlider();
private:
    FlirCamera* cam;
    int defaultExpoMax = 20000;
    int defaultExpoMin = 6;
signals:
private slots:
    void setExposureMode(Qt::CheckState state);
    void setMaxSliderWidth();
};

#endif // SETTINGS_PAGE_H
