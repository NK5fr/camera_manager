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
    cam = camList.GetByIndex(0);
    cam->Init();

    cam->AcquisitionFrameRateEnable.SetValue(true);
    cam->AcquisitionFrameRate.SetValue(15.0);

    cam->BeginAcquisition();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getCameraImage()));
    timer->start(30);
}

MainWindow::~MainWindow()
{
    image->Release();
    camList.Clear();
    cam->EndAcquisition();
    cam->DeInit();
    system->ReleaseInstance();
    delete ui;
}

void MainWindow::getCameraImage()
{
    image = cam->GetNextImage(1000);
    ImagePtr convertedImage = processor.Convert(image, PixelFormat_BGR8);
    QImage image((uchar*) convertedImage->GetData(), convertedImage->GetWidth(), convertedImage->GetHeight(), convertedImage->GetStride(), QImage::Format_BGR888);
    ui->cameraRendering->setPixmap(QPixmap::fromImage(image.scaled(ui->cameraRendering->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}
