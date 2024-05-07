#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils.h"
#include "settingswidget.h"
#include "qstyle.h"
#include <QMessageBox>
#include "camerawidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Check number of devices found
    string devicesNumber = "Devices found: " + std::to_string(camList.GetSize());
    ui->devicesNumber->setText(QString::fromStdString(devicesNumber));

    // Init all cameras found in a camera list
    for(int i = 0; i < camList.GetSize(); ++i){
        flirCamList.push_back(new FlirCamera(camList.GetByIndex(i)));
    }

    // Init the list widget with found cameras
    setListWidget();

    // Listen to item clicked in the list widget
    connect(ui->cameraList, &QListWidget::itemClicked, this, &MainWindow::openCameraWidget);

    // Listen to refresh button clicked
    connect(ui->refresh, &QPushButton::clicked, this, &MainWindow::refresh);

    // Listen to refresh time event
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refresh);

    // Start timer
    refreshTimer->start(3000);
}

MainWindow::~MainWindow()
{
    camList.Clear(); // Clear the cam list
    for(FlirCamera* cam : flirCamList){ // Delete all cameras
        delete cam;
    }
    system->ReleaseInstance(); // Release system
    delete ui;
}

// Open the camera window
void MainWindow::openCameraWidget(QListWidgetItem * item)
{
    int idx = ui->cameraList->row(item); // Get item index
    if(flirCamList[idx]->isConnected() && !open){ // Check if the cam is still connected and if no cam is open
        refreshTimer->stop(); // Stop timer during camera diffusion
        open = true;
        CameraWidget *currentWidget = new CameraWidget(flirCamList[idx], nullptr);
        currentWidget->show();
        connect(currentWidget, &CameraWidget::widgetClosed, this, &MainWindow::cameraWidgetClosed); // Listen to camera closing
    }else if(!flirCamList[idx]->isConnected()){ // If the camera is disconnected then refresh
        utils::showError("Camera not found");
        refresh();
    }else{ // If a cam is open
        utils::showError("A camera is already opened");
    }
}

// Catch main window closing
void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    QApplication::closeAllWindows(); // Close all opened window
}

// Create one item in the list for each cam
void MainWindow::setListWidget()
{
    ui->cameraList->clear();
    for(FlirCamera *cam : flirCamList){
        string serial = cam->getSerial();
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(serial));
        item->setTextAlignment(Qt::AlignCenter); // Change text alignement to center
        item->setFont(QFont ("Courier", 14)); // Change font
        ui->cameraList->addItem(item);
    }
}

// Clear all cam list and recreate them
void MainWindow::refresh()
{
    flirCamList.clear();
    camList.Clear();
    camList = system->GetCameras();
    string devicesNumber = "Devices found: " + std::to_string(camList.GetSize());
    ui->devicesNumber->setText(QString::fromStdString(devicesNumber));
    for(int i = 0; i < camList.GetSize(); ++i){
        flirCamList.push_back(new FlirCamera(camList.GetByIndex(i)));
    }
    setListWidget();
}

// Catch camera window closing
void MainWindow::cameraWidgetClosed()
{
    open = false;
    refreshTimer->start(3000); // restart timer
    refresh(); // refresh automatically
}

