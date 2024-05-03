#include "frameratecontroller.h"
#include "ui_frameratecontroller.h"
#include "utils.h"

FrameRateController::FrameRateController(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FrameRateController)
{
    ui->setupUi(this);
    this->setGeometry(utils::reCenterOffSet(this->geometry(), QApplication::primaryScreen(), 'l', 500));
}

FrameRateController::~FrameRateController()
{
    delete ui;
}
