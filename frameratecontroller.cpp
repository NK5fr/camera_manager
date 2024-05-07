#include "frameratecontroller.h"
#include "linkedslider.h"
#include "ui_frameratecontroller.h"
#include "utils.h"

FrameRateController::FrameRateController(QWidget *parent, int maxFpsValue)
    : QWidget(parent)
    , ui(new Ui::FrameRateController)
{
    ui->setupUi(this);

    // Setting up the limits of the slider
    ui->frameRateInput->setMinimum(1);
    if(maxFpsValue > 60){
        ui->frameRateInput->setMaximum(60);
    }else{
        ui->frameRateInput->setMaximum(maxFpsValue);
    }
    ui->frameRateInput->setValue(30);

    // Linking the slider with the label
    LinkedSlider *linkedFrameRate = new LinkedSlider(nullptr, ui->frameRateValue, ui->frameRateInput, "Fixed FrameRate: ");
    linkedFrameRate->setParent(this);

    // Removing the menu of the window
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QObject::connect(ui->frameRateInput, &QSlider::valueChanged, this, [this](int newValue){
        emit fixedFrameRateChanged(newValue);
    });
}

FrameRateController::~FrameRateController()
{
    delete ui;
}

// Method to hide the controller if its possible, used to ensure that it disseapears when you click on something else in the main window
void FrameRateController::updateVisibility() {
    if (this->isVisible()) {
        this->hide();
    }
}
