#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include "SpinGenApi/SpinnakerGenApi.h"
#include "Spinnaker.h"
#include <iostream>
#include <sstream>

#include "flir_camera.h"
#include "frameratecontroller.h"
#include "settingswidget.h"

#include <QElapsedTimer>
#include <QPushButton>
#include <QTimer>
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
    void updateWindowVisibility();
    void setButtonEnabled(QPushButton *button, bool mode);
    void initZoomSlider();
private:
    Ui::CameraWidget *ui;
    FlirCamera *cam;
    SettingsWidget *settings;
    FrameRateController *controller;
    void changeCameraInfo();
    QTimer *refreshTimer = new QTimer();
    int fpsSum = 0;
    int fpsCount = 0;
    int zoom = 6;
    string filePath = "../../images/";
    QElapsedTimer elapsedTimer;
    bool shouldTakePicture = false;
    void showSettings();
    void getCameraImage(ImagePtr, int);
    void startAcquisition();
    void stopAcquisition();
    void changeAcquisition(bool streaming);
    void stopExisting();
    void showFrameRateController();
    void mousePressEvent(QMouseEvent *event);
    void takePicture(ImagePtr, int);
    void displayImage(ImagePtr convertedImage);
    int calculateFrameRate();
signals:
    void widgetClosed();
};

#endif // CAMERAWIDGET_H
