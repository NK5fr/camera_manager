#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils.h"
#include "settingswidget.h"
#include "qstyle.h"
#include <QMessageBox>
#include "camerawidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //initCameras();
    QObject::connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(showDefaultCam()));
}

MainWindow::~MainWindow()
{
    qInfo() << "MainWindow cleared";
    camList.Clear();
    system->ReleaseInstance();
    delete ui;
}

void MainWindow::initCameras() {
    if (this->camList.GetSize() == 0) {
        utils::showError("There are no cameras!");
        return;
    }
    for (int i = 0 ; i < this->camList.GetSize() ; i++) {
        this->addCameraWidget(i);
    }
}

void MainWindow::addCameraWidget(int index) {
    // try {
    //     FlirCamera *currentCamera = new FlirCamera(camList.GetByIndex(index));
    //     CameraWidget *currentWidget = new CameraWidget(currentCamera, nullptr);
    //     this->flirCamList.push_back(currentWidget);
    // } catch (Spinnaker::Exception e) {
    //     int ret = utils::showError(e.what());
    // }
}

void MainWindow::showDefaultCam() {
    FlirCamera *currentCamera;
    if (this->flirCamList.empty()) {
        currentCamera = new FlirCamera(camList.GetByIndex(0));
        this->flirCamList.push_back(currentCamera);
    } else {
        currentCamera = this->flirCamList[0];
    }
    CameraWidget *currentWidget = new CameraWidget(currentCamera, nullptr);
    currentWidget->setWindowModality(Qt::WindowModal);
    currentWidget->show();
}

void MainWindow::reloadCameraList() {
    this->system = System::GetInstance();
    this->camList = system->GetCameras();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    QApplication::closeAllWindows();
}
