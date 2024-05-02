#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "SpinGenApi/SpinnakerGenApi.h"
#include "Spinnaker.h"
#include <iostream>
#include <sstream>

#include "flir_camera.h"
#include "settings_page.h"

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    SystemPtr system;
    CameraList camList;
    FlirCamera *cam;
    SettingsPage *page;
    void getCamera();
    void changeCameraInfo();
private slots:
    void showSettings();
    void getCameraImage();
};
#endif // MAINWINDOW_H
