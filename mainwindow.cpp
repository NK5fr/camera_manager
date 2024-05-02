#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QScreen *screen =  QGuiApplication::primaryScreen();
    utils::reCenter(screen, 500,400);

    ui->setupUi(this);
    setWindowTitle("Camera Manager");
    getCamera();
    this->page = new SettingsPage(cam, nullptr);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getCameraImage()));
    timer->start(30);
    connect(ui->settingsButton, SIGNAL(released()), this, SLOT(showSettings()));
}

MainWindow::~MainWindow()
{
    qInfo() << "MainWindow cleared";
    camList.Clear();
    delete cam;
    system->ReleaseInstance();
    delete ui;
}

void MainWindow::getCamera()
{

    this->system = System::GetInstance();
    this->camList = system->GetCameras();
    const unsigned int numCam = camList.GetSize();
    if (numCam == 0) {
        QMessageBox noCamWarning;
        noCamWarning.setText("No cameras were detected!\nThe application will close now.");
        noCamWarning.setDefaultButton(QMessageBox::Ok);
        noCamWarning.exec();
        QApplication::quit();
        return;
    }
    try {
        cam = new FlirCamera(camList.GetByIndex(0));
        cam->startAquisition();
    } catch (Spinnaker::Exception e) {
        QMessageBox exception;
        exception.setText(e.what());
        exception.setDefaultButton(QMessageBox::Ok);
        exception.exec();
        QApplication::quit();
        return;
    }
}

void MainWindow::changeCameraInfo()
{
    //ui->name->setText();
}


void MainWindow::showSettings() {
    if (page->isVisible()) {
        page->hide();
    } else {
        page->show();
    }
}

void MainWindow::getCameraImage()
{
    ImagePtr convertedImage = cam->getNextImageConverted();
    QImage image((uchar *) convertedImage->GetData(),
                 convertedImage->GetWidth(),
                 convertedImage->GetHeight(),
                 convertedImage->GetStride(),
                 QImage::Format_BGR888);
    ui->cameraRendering->setPixmap(QPixmap::fromImage(
        image.scaled(ui->cameraRendering->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}
