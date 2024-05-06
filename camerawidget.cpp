#include "camerawidget.h"
#include "frameratecontroller.h"
#include "linkedslider.h"
#include "qmessagebox.h"
#include "qstyle.h"
#include "ui_camerawidget.h"
#include "utils.h"

CameraWidget::CameraWidget(FlirCamera *flircam, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CameraWidget)
    , cam(flircam)
{
    ui->setupUi(this);
    setWindowTitle("Camera Manager");
    changeCameraInfo();
    this->settings = new SettingsWidget(cam, nullptr);
    this->controller = new FrameRateController(nullptr);

    //settings->setGeometry(utils::centerOnPoint(settings->geometry(), QPoint(649,649)));
    //qInfo() << this->geometry();
    //settings->setGeometry(utils::centerOnPoint(settings->geometry(), QPoint(500,100)));

    ui->framerateButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogInfoView));

    connect(cam, SIGNAL(streaming(bool)), this, SLOT(changeAcquisition(bool)));
    connect(cam, SIGNAL(imageRetrieved(ImagePtr, int)), this, SLOT(getCameraImage(ImagePtr, int)));
    connect(ui->settingsButton, SIGNAL(released()), this, SLOT(showSettings()));
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startAcquisition()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopAcquisition()));

    ui->stopButton->setEnabled(false);

    connect(ui->framerateButton, SIGNAL(clicked(bool)), this, SLOT(showFrameRateController()));
    connect(controller, SIGNAL(fixedFrameRateChanged(int)), cam, SLOT(updateFixedFrameRate(int)));

    cam->startAquisition();

    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(testCameraStatus()));
    refreshTimer->start(3000);

    elapsedTimer.start();
    ui->framerate->setText(QString::number(30));
}

CameraWidget::~CameraWidget()
{
    qInfo() << "destroyed camera widget";
    if (cam->isSteaming()) {
        cam->stopAquisition();
    }
    delete ui;
}

void CameraWidget::changeCameraInfo()
{
    ui->name->setText(QString::fromStdString(cam->getModelName() + " " + cam->getSerial()));
    ui->serial->setText(QString::fromStdString(cam->getVendorName()));
}

void CameraWidget::showSettings()
{
    if (!cam->isConnected()) {
        close();
    } else if (settings->isVisible()) {
        settings->hide();
    } else {
        settings->show();
        qInfo() << settings->width();
        qInfo() << settings->height();
        //settings->setGeometry(utils::reCenterOffSet((QWidget*) settings, QApplication::primaryScreen(), 'r', (this->width())));
        settings->setGeometry(utils::reCenterWidget((QWidget*) settings, (QWidget*)ui->cameraRendering));
        settings->setGeometry(utils::offSet((QWidget*) settings, 'r', 3*this->width()/4));
    }
}

void CameraWidget::getCameraImage(ImagePtr convertedImage, int count)
{
    //cout << count << endl;
    QImage image((uchar *) convertedImage->GetData(),
                 convertedImage->GetWidth(),
                 convertedImage->GetHeight(),
                 convertedImage->GetStride(),
                 QImage::Format_BGR888);
    ui->cameraRendering->setPixmap(QPixmap::fromImage(
        image.scaled(ui->cameraRendering->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    int elapsed = elapsedTimer.elapsed();
    fpsSum += 1000 / elapsed;
    ++fpsCount;
    if (fpsCount >= cam->getFixedFrameRate()) {
        ui->framerate->setText(QString::number(fpsSum / fpsCount));
        fpsSum = 0;
        fpsCount = 0;
    }
    elapsedTimer.restart();
}

void CameraWidget::startAcquisition()
{
    if (!cam->isConnected()) {
        close();
    } else{
        ui->framerate->setText(QString::number(cam->getFixedFrameRate()));
        cam->startAquisition();
    }
}

void CameraWidget::stopAcquisition()
{
    if (!cam->isConnected()) {
        close();
    } else {
        ui->framerate->setText(QString::number(0));
        cam->stopAquisition();
    }
}

void CameraWidget::changeAcquisition(bool mode)
{
    ui->startButton->setEnabled(!mode);
    ui->stopButton->setEnabled(mode);
}

void CameraWidget::stopExisting()
{
    refreshTimer->stop();
    if (this->settings->isVisible()) {
        this->settings->hide();
    }
    delete this->settings;
    delete this->controller;
    qInfo() << "deleted settings & controller";
    delete this;
}

void CameraWidget::showFrameRateController()
{
    if (!cam->isConnected()) {
        close();
    } else if (this->controller->isVisible()) {
        this->controller->hide();
    } else {
        this->controller->show();
        controller->setGeometry(utils::reCenterWidget((QWidget*) controller, (QWidget*) ui->framerateButton));
    }
}

void CameraWidget::closeEvent(QCloseEvent *event)
{
    qInfo() << "close event camera widget";
    emit widgetClosed();
    this->stopExisting();
}

void CameraWidget::testCameraStatus()
{
    if (!cam->isConnected()) {
        close();
    }
}
