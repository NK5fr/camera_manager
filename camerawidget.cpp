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
    this->controller = new FrameRateController(nullptr, cam->getMaxFps());
    ui->framerateButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogInfoView));

    connect(cam, SIGNAL(streaming(bool)), this, SLOT(changeAcquisition(bool)));
    connect(cam, SIGNAL(imageRetrieved(ImagePtr, int)), this, SLOT(getCameraImage(ImagePtr, int)));
    connect(ui->settingsButton, SIGNAL(released()), this, SLOT(showSettings()));
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startAcquisition()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopAcquisition()));
    this->setButtonEnabled(ui->stopButton, false);

    connect(ui->framerateButton, SIGNAL(clicked(bool)), this, SLOT(showFrameRateController()));
    connect(controller, SIGNAL(fixedFrameRateChanged(int)), cam, SLOT(updateFixedFrameRate(int)));

    cam->startAquisition();

    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(testCameraStatus()));
    refreshTimer->start(3000);

    elapsedTimer.start();
    ui->framerate->setText(QString::number(30));
    this->initZoomSlider();
    connect(ui->zoomInput, &QSlider::valueChanged, this, &CameraWidget::changeZoom);
    connect(ui->zoomInput, &QSlider::sliderPressed, this, &CameraWidget::updateWindowVisibility);

    connect(ui->takePicture, &QPushButton::clicked, this, [this](){shouldTakePicture = true;});
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
    updateWindowVisibility();
    if (!cam->isConnected()) {
        close();
    } else if (settings->isVisible()) {
        settings->hide();
    } else {
        settings->show();
        settings->setGeometry(utils::reCenterWidget((QWidget*) settings, (QWidget*)ui->cameraRendering));
        settings->setGeometry(utils::offSet((QWidget*) settings, 'r', 1.25*settings->width()));
    }
}

void CameraWidget::getCameraImage(ImagePtr convertedImage, int count)
{
    if(shouldTakePicture){
        ostringstream filename;
        filename << "../../images/image" << count << ".png";
        convertedImage->Save(filename.str().c_str());
        shouldTakePicture = false;
    }

    QImage image((uchar *) convertedImage->GetData(),
                 convertedImage->GetWidth(),
                 convertedImage->GetHeight(),
                 convertedImage->GetStride(),
                 QImage::Format_BGR888);
    ui->cameraRendering->setPixmap(QPixmap::fromImage(
        image.scaled(QSize(100*this->zoom,66*this->zoom), Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    //ui->cameraRendering->setText(QString::number(count));

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
    updateWindowVisibility();
    if (!cam->isConnected()) {
        close();
    } else{
        ui->framerate->setText(QString::number(cam->getFixedFrameRate()));
        cam->startAquisition();
    }
}

void CameraWidget::stopAcquisition()
{
    updateWindowVisibility();
    if (!cam->isConnected()) {
        close();
    } else {
        ui->framerate->setText(QString::number(0));
        cam->stopAquisition();
    }
}

void CameraWidget::changeAcquisition(bool mode)
{
    this->setButtonEnabled(ui->startButton, mode);
    this->setButtonEnabled(ui->stopButton, !mode);
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
    updateWindowVisibility();
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

void CameraWidget::mousePressEvent(QMouseEvent *event)
{
    updateWindowVisibility();
}

void CameraWidget::changeZoom(int zoom)
{
    this->zoom = zoom;
}

void CameraWidget::updateWindowVisibility() {
    controller->updateVisibility();
    settings->updateVisibility();
}

void CameraWidget::setButtonEnabled(QPushButton *button, bool mode) {
    button->setEnabled(mode);
    button->setAttribute(Qt::WA_TransparentForMouseEvents, mode);
}

void CameraWidget::initZoomSlider() {
    ui->zoomInput->setMaximum(15);
    ui->zoomInput->setMinimum(1);
    ui->zoomInput->setValue(6);
}
