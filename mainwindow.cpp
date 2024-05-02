#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils.h"
#include "settingswidget.h"
#include "qstyle.h"
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
    changeCameraInfo();
    this->page = new SettingsWidget(cam, nullptr);

    ui->framerateButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogInfoView));
    connect(cam, SIGNAL(streaming(bool)), this, SLOT(changeAcquisition(bool)));
    connect(cam, SIGNAL(imageRetrieved(ImagePtr, int)), this, SLOT(getCameraImage(ImagePtr, int)));
    connect(ui->settingsButton, SIGNAL(released()), this, SLOT(showSettings()));
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startAcquisition()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopAcquisition()));

    ui->stopButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    qInfo() << "MainWindow cleared";
    camList.Clear();
    if (cam->isSteaming()) {
        cam->stopAquisition();
    }
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
    ui->name->setText(QString::fromStdString(cam->getModelName()));
    ui->serial->setText(QString::fromStdString(cam->getVendorName()));
}


void MainWindow::showSettings() {
    if (page->isVisible()) {
        page->hide();
    } else {
        page->show();
    }
}

void MainWindow::getCameraImage(ImagePtr convertedImage, int count)
{
    //cout << count << endl;
    QImage image((uchar *) convertedImage->GetData(),
                 convertedImage->GetWidth(),
                 convertedImage->GetHeight(),
                 convertedImage->GetStride(),
                 QImage::Format_BGR888);
    ui->cameraRendering->setPixmap(QPixmap::fromImage(
        image.scaled(ui->cameraRendering->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}

void MainWindow::startAcquisition()
{
    cam->startAquisition();
}

void MainWindow::stopAcquisition()
{
    cam->stopAquisition();
}

void MainWindow::changeAcquisition(bool mode)
{
    ui->startButton->setEnabled(!mode);
    ui->stopButton->setEnabled(mode);
}

