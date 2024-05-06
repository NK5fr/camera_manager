#include "frameratecontroller.h"
#include "ui_frameratecontroller.h"
#include "utils.h"
#include "linkedslider.h"

FrameRateController::FrameRateController(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FrameRateController)
{
    ui->setupUi(this);
    //this->setGeometry(utils::reCenterOffSet(this->geometry(), QApplication::primaryScreen(), 'l', 400));
    ui->frameRateInput->setMinimum(1);
    ui->frameRateInput->setMaximum(60);
    ui->frameRateInput->setValue(30);
    LinkedSlider *linkedFrameRate = new LinkedSlider(nullptr, ui->frameRateValue, ui->frameRateInput, "Fixed FrameRate: "); linkedFrameRate->setParent(this);
    QObject::connect(ui->frameRateInput, SIGNAL(valueChanged(int)), this, SLOT(changeFixedFrameRate(int)));
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

FrameRateController::~FrameRateController()
{
    delete ui;
}

void FrameRateController::changeFixedFrameRate(int value)
{
    emit fixedFrameRateChanged(value);
}
