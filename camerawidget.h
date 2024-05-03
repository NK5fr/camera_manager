#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include "SpinGenApi/SpinnakerGenApi.h"
#include "Spinnaker.h"
#include <iostream>
#include <sstream>

#include "flir_camera.h"
#include "settingswidget.h"

#include <QWidget>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

namespace Ui {
class CameraWidget;
}

class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    CameraWidget(FlirCamera *cam = nullptr, QWidget *parent = nullptr);
    ~CameraWidget();

    void closeEvent(QCloseEvent *event);
private:
    Ui::CameraWidget *ui;
    FlirCamera *cam;
    SettingsWidget *settings;
    void changeCameraInfo();
private slots:
    void showSettings();
    void getCameraImage(ImagePtr, int);
    void startAcquisition();
    void stopAcquisition();
    void changeAcquisition(bool streaming);
    void stopExisting();
};

#endif // CAMERAWIDGET_H
