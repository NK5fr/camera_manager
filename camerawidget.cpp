#include "camerawidget.h"
#include "frameratecontroller.h"
#include "linkedslider.h"
#include "qmessagebox.h"
#include "qstyle.h"
#include "ui_camerawidget.h"
#include "utils.h"
#include <iostream>
#include <ctime>

CameraWidget::CameraWidget(FlirCamera *flircam, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CameraWidget)
    , cam(flircam)
{
    ui->setupUi(this);

    // Change Window title and camera info labels
    setWindowTitle("Camera Manager");
    changeCameraInfo();

    // Create settings and frame rate contoller windows
    this->settings = new SettingsWidget(cam, nullptr);
    this->controller = new FrameRateController(nullptr, cam->getMaxFps());

    // Change frame rate button icon, disable stop button (camera is running by default),
    // Init the zoom slider values and set the frame rate text to the default value (30)
    ui->framerateButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogInfoView));
    this->setButtonEnabled(ui->stopButton, false);
    this->initZoomSlider();
    ui->framerate->setText(QString::number(30));

    // Listen to camera events (acquisition changed and image retrieved)
    connect(cam, &FlirCamera::streaming, this, &CameraWidget::changeAcquisition);
    connect(cam, &FlirCamera::imageRetrieved, this, &CameraWidget::getCameraImage);

    // Listen to buttons click event
    connect(ui->settingsButton, &QPushButton::released, this, &CameraWidget::showSettings);
    connect(ui->startButton, &QPushButton::clicked, this, &CameraWidget::startAcquisition);
    connect(ui->stopButton, &QPushButton::clicked, this, &CameraWidget::stopAcquisition);
    connect(ui->framerateButton, &QPushButton::clicked, this, &CameraWidget::showFrameRateController);
    connect(ui->takePicture, &QPushButton::clicked, this, [this](){shouldTakePicture = true;});

    // Listen to frame rate controller frame rate changed event
    connect(controller, &FrameRateController::fixedFrameRateChanged, cam, &FlirCamera::updateFixedFrameRate);

    // Listen to refresh timer event
    connect(refreshTimer, &QTimer::timeout, this, [this](){
        if (!cam->isConnected()) {
            close();
        }
    });

    // Listen to the slider events
    connect(ui->zoomInput, &QSlider::valueChanged, this, [this](int value){zoom = value;});
    connect(ui->zoomInput, &QSlider::sliderPressed, this, &CameraWidget::updateWindowVisibility);

    // Start the refresh timer,the camera and the elapsed timer
    refreshTimer->start(3000);
    cam->startAquisition();
    elapsedTimer.start();

    // Listen to the settings file name changed and changes it accordingly
    connect(settings, &SettingsWidget::fileNameChanged, this, [this](QString file){
        file.append("/");
        this->filePath = file.toStdString();
    });
}

CameraWidget::~CameraWidget()
{
    qInfo() << "destroyed camera widget";
    // Stop camera if is running
    if (cam->isSteaming()) {
        cam->stopAquisition();
    }
    delete ui;
}

// Set name and vendor name
void CameraWidget::changeCameraInfo()
{
    ui->name->setText(QString::fromStdString(cam->getModelName() + " " + cam->getSerial()));
    ui->serial->setText(QString::fromStdString(cam->getVendorName()));
}

// Show the settings window
void CameraWidget::showSettings()
{
    updateWindowVisibility(); // Close settings and frame rate controller windows if open
    if (!cam->isConnected()) { // Check if the camera is still connected
        close();
    } else if (settings->isVisible()) { // Close the settings window if it's opened
        settings->hide();
    } else { // Open the settings window
        settings->show();

        // Change settings window position
        settings->setGeometry(utils::reCenterWidget((QWidget*) settings, (QWidget*)ui->cameraRendering));
        settings->setGeometry(utils::offSet((QWidget*) settings, 'r', settings->width()));
    }
}

void CameraWidget::getCameraImage(ImagePtr convertedImage, int count)
{
    // Take a picture if asked
    if(shouldTakePicture){
        takePicture(convertedImage, count);
    }

    // Display image in a label
    displayImage(convertedImage);

    // Calculate framerate and diplay it if the value is 0 or above
    int result = calculateFrameRate();
    if(result >= 0){
        ui->framerate->setText(QString::number(result));
    }
}

// Take a picture
void CameraWidget::takePicture(ImagePtr convertedImage, int count){
    shouldTakePicture = false;
    ostringstream filename;
    filename << this->filePath << utils::getTime() << "-" << count << ".png";
    qInfo() << filename.str().c_str();
    try {
        convertedImage->Save(filename.str().c_str());
    } catch(Spinnaker::Exception exception) {
        utils::showError(exception.what());
    }
}

// Init and display image in a label
void CameraWidget::displayImage(ImagePtr convertedImage){
    QImage image((uchar *) convertedImage->GetData(),
                 convertedImage->GetWidth(),
                 convertedImage->GetHeight(),
                 convertedImage->GetStride(),
                 QImage::Format_BGR888);
    ui->cameraRendering->setPixmap(QPixmap::fromImage(
        image.scaled(QSize(100*this->zoom,66*this->zoom), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}

// Calculate and return the frame rate from the average recuperation time of each image
int CameraWidget::calculateFrameRate(){
    int result = -1;
    int elapsed = elapsedTimer.elapsed(); // check the recuperation time of one image
    fpsSum += 1000 / elapsed;
    ++fpsCount;
    if (fpsCount >= cam->getFixedFrameRate()) { // Change the result if the number of taken pictures is equal to the fixed frame rate
        result = fpsSum / fpsCount;
        fpsSum = 0;
        fpsCount = 0;
    }
    elapsedTimer.restart(); // restart to calculate the next recuperation time
    return result;
}

// Start the camera
void CameraWidget::startAcquisition()
{
    updateWindowVisibility(); // Close settings and frame rate controller windows if open
    if (!cam->isConnected()) { // Check if the camera is still connected
        close();
    } else{
        ui->framerate->setText(QString::number(cam->getFixedFrameRate())); // Reinit the frame rate text
        cam->startAquisition();
    }
}

// Stop camera
void CameraWidget::stopAcquisition()
{
    updateWindowVisibility(); // Close settings and frame rate controller windows if open
    if (!cam->isConnected()) { // Check if the camera is still connected
        close();
    } else {
        ui->framerate->setText(QString::number(0)); // Change the frame rate text to zero
        cam->stopAquisition();
    }
}

// Switch acquisition
void CameraWidget::changeAcquisition(bool mode)
{
    this->setButtonEnabled(ui->startButton, mode);
    this->setButtonEnabled(ui->stopButton, !mode);
    ui->startButton->setEnabled(!mode);
    ui->stopButton->setEnabled(mode);
}

// Delete all the elements contained in the current window
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
    updateWindowVisibility(); // Close settings and frame rate controller windows if open
    if (!cam->isConnected()) { // Check if the camera is still connected
        close();
    } else if (this->controller->isVisible()) { // Close the frame rate controller window if it's opened
        this->controller->hide();
    } else { // Open the frame rate controller window
        this->controller->show();

        // Change frame rate controller window position
        controller->setGeometry(utils::reCenterWidget((QWidget*) controller, (QWidget*) ui->framerateButton));
    }
}

// Catch window closing and delete all elements
void CameraWidget::closeEvent(QCloseEvent *event)
{
    qInfo() << "close event camera widget";
    emit widgetClosed();
    this->stopExisting();
}

// Catch click event to close settings and frame rate controller windows if open
void CameraWidget::mousePressEvent(QMouseEvent *event)
{
    updateWindowVisibility();
}

// Close settings and frame rate controller windows if open
void CameraWidget::updateWindowVisibility() {
    controller->updateVisibility();
    settings->updateVisibility();
}

// Enable or disable buttons
void CameraWidget::setButtonEnabled(QPushButton *button, bool mode) {
    button->setEnabled(mode);
    button->setAttribute(Qt::WA_TransparentForMouseEvents, mode);
}

// Init zoom slider values
void CameraWidget::initZoomSlider() {
    ui->zoomInput->setMaximum(15);
    ui->zoomInput->setMinimum(1);
    ui->zoomInput->setValue(6);
}
