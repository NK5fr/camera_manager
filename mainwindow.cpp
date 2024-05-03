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
    for(int i = 0; i < camList.GetSize(); ++i){
        flirCamList.push_back(new FlirCamera(camList.GetByIndex(i)));
        string serial = flirCamList[i]->getSerial();
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(serial));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont ("Courier", 14));
        ui->cameraList->addItem(item);
    }
    connect(ui->cameraList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openCameraWidget(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    camList.Clear();
    for(FlirCamera* cam : flirCamList){
        delete cam;
    }
    system->ReleaseInstance();
    delete ui;
}

bool MainWindow::oneCameraOpen()
{
    for(FlirCamera* cam : flirCamList){
        if(cam->isOpen()) return true;
    }
    return false;
}

void MainWindow::openCameraWidget(QListWidgetItem * item)
{
    int idx = ui->cameraList->row(item);
    if(!oneCameraOpen()){
        flirCamList[idx]->setOpen(true);
        CameraWidget *currentWidget = new CameraWidget(flirCamList[idx], nullptr);
        currentWidget->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    QApplication::closeAllWindows();
}
