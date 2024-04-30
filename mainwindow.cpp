#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Camera Manager");

    system = System::GetInstance();
    camList = system->GetCameras();
    cam = new FlirCamera(camList.GetByIndex(0));
    connect(cam, &FlirCamera::imageArrived, this, &MainWindow::getCameraImage);
    cam->startAquisition();
}

MainWindow::~MainWindow()
{
    camList.Clear();
    delete cam;
    system->ReleaseInstance();
    delete ui;
}

void MainWindow::getCameraImage(ImagePtr convertedImage, int count)
{
    cout << convertedImage->GetWidth() << " " << convertedImage->GetHeight() << endl;
    // QImage image((uchar*) convertedImage->GetData(), convertedImage->GetWidth(), convertedImage->GetHeight(), convertedImage->GetStride(), QImage::Format_BGR888);
    // delete convertedImage;
    // ui->cameraRendering->setPixmap(QPixmap::fromImage(image.scaled(ui->cameraRendering->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}
