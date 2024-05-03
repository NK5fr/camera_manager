#include "camerawidget.h"
#include "ui_camerawidget.h"
#include "utils.h"
#include "qstyle.h"
#include "qmessagebox.h"

CameraWidget::CameraWidget(FlirCamera *flircam, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::CameraWidget),
    cam(flircam)

{
    ui->setupUi(this);
    QScreen *screen =  QGuiApplication::primaryScreen();
    setWindowTitle("Camera Manager");
    changeCameraInfo();
    this->settings = new SettingsWidget(cam, nullptr);

    ui->framerateButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogInfoView));
    connect(cam, SIGNAL(streaming(bool)), this, SLOT(changeAcquisition(bool)));
    connect(cam, SIGNAL(imageRetrieved(ImagePtr, int)), this, SLOT(getCameraImage(ImagePtr, int)));
    connect(ui->settingsButton, SIGNAL(released()), this, SLOT(showSettings()));
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startAcquisition()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopAcquisition()));
    ui->stopButton->setEnabled(false);
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
    ui->name->setText(QString::fromStdString(cam->getModelName()));
    ui->serial->setText(QString::fromStdString(cam->getVendorName()));
}


void CameraWidget::showSettings() {
    if (settings->isVisible()) {
        settings->hide();
    } else {
        settings->show();
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
}

void CameraWidget::startAcquisition()
{
    cam->startAquisition();
}

void CameraWidget::stopAcquisition()
{
    cam->stopAquisition();
}

void CameraWidget::changeAcquisition(bool mode)
{
    ui->startButton->setEnabled(!mode);
    ui->stopButton->setEnabled(mode);
}

void CameraWidget::stopExisting() {
    if (this->settings->isVisible()) {
        this->settings->hide();
    }
    delete this->settings;
    delete this;
}

void CameraWidget::closeEvent(QCloseEvent *event) {
    this->stopExisting();
}
