#include "mainwindow.h"
#include <QMessageBox>
#include "./ui_mainwindow.h"
#include "camerawidget.h"
#include "qstyle.h"
#include "settingswidget.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    string devicesNumber = "Devices found: " + std::to_string(camList.GetSize());
    ui->devicesNumber->setText(QString::fromStdString(devicesNumber));

    for (int i = 0; i < camList.GetSize(); ++i) {
        flirCamList.push_back(new FlirCamera(camList.GetByIndex(i)));
    }
    setListWidget();

    connect(ui->cameraList,
            SIGNAL(itemClicked(QListWidgetItem *)),
            this,
            SLOT(openCameraWidget(QListWidgetItem *)));
    connect(ui->refresh, SIGNAL(clicked()), this, SLOT(refresh()));

    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    refreshTimer->start(3000);
}

MainWindow::~MainWindow()
{

    try {
        camList.Clear();
        for (FlirCamera *cam : flirCamList) {
            delete cam;
        }
        system->ReleaseInstance();
        delete ui;
    } catch (Spinnaker::Exception exception) {
        qInfo() << exception.what();
    }
}

void MainWindow::openCameraWidget(QListWidgetItem *item)
{
    int idx = ui->cameraList->row(item);
    if (flirCamList[idx]->isConnected() && !open) {
        refreshTimer->stop();
        open = true;
        CameraWidget *currentWidget = new CameraWidget(flirCamList[idx], nullptr);
        currentWidget->show();
        connect(currentWidget, SIGNAL(widgetClosed()), this, SLOT(cameraWidgetClosed()));
    } else if (!flirCamList[idx]->isConnected()) {
        utils::showError("Camera not found");
        refresh();
    } else {
        utils::showError("A camera is already opened");
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    QApplication::closeAllWindows();
}

void MainWindow::setListWidget()
{
    ui->cameraList->clear();
    for (FlirCamera *cam : flirCamList) {
        string serial = cam->getSerial();
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(serial));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Courier", 14));
        ui->cameraList->addItem(item);
    }
}

void MainWindow::refresh()
{
    flirCamList.clear();
    camList.Clear();
    camList = system->GetCameras();
    string devicesNumber = "Devices found: " + std::to_string(camList.GetSize());
    ui->devicesNumber->setText(QString::fromStdString(devicesNumber));
    for (int i = 0; i < camList.GetSize(); ++i) {
        flirCamList.push_back(new FlirCamera(camList.GetByIndex(i)));
    }
    setListWidget();
}

void MainWindow::cameraWidgetClosed()
{
    open = false;
    refreshTimer->start(3000);
    refresh();
}
