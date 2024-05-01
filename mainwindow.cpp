#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QScreen *screen =  QGuiApplication::primaryScreen();
    utils::reCenter(screen, 500,400);

    ui->setupUi(this);
    setWindowTitle("Camera Manager");

    system = System::GetInstance();
    camList = system->GetCameras();
    cam = new FlirCamera(camList.GetByIndex(0));
    cam->startAquisition();
    this->page = new SettingsPage(cam, nullptr);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getCameraImage()));
    timer->start(30);
    connect(ui->settingsButton, SIGNAL(released()), this, SLOT(showSettings()));
}

MainWindow::~MainWindow()
{
    camList.Clear();
    delete cam;
    system->ReleaseInstance();
    delete ui;
}


void MainWindow::showSettings() {
    qInfo() << "clicked";
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
