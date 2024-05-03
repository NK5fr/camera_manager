#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "SpinGenApi/SpinnakerGenApi.h"
#include "Spinnaker.h"
#include <iostream>
#include <sstream>

#include "camerawidget.h"
#include "flir_camera.h"
#include "settingswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    SystemPtr system = System::GetInstance();
    CameraList camList = system->GetCameras();
    std::vector<FlirCamera*> flirCamList;
    void initCameras();
    void addCameraWidget(int index);
    void reloadCameraList();
private:
    Ui::MainWindow *ui;
private slots:
    void showDefaultCam();
};
#endif // MAINWINDOW_H
