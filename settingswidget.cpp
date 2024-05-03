#include "linkedcombobox.h"
#include "linkedlineedit.h"
#include "linkedslider.h"
#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "utils.h"

SettingsWidget::SettingsWidget(FlirCamera *camera, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget),
    cam(camera)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    LinkedLineEdit *linkedMaxExposure = new LinkedLineEdit(nullptr, ui->maxExposureValue, ui->maxExposureInput); linkedMaxExposure->setParent(this);
    LinkedLineEdit *linkedDevice = new LinkedLineEdit(nullptr, ui->deviceValue, ui->deviceInput); linkedDevice->setParent(this);
    LinkedSlider *linkedExposure = new LinkedSlider(nullptr, ui->exposureValue, ui->exposureInput); linkedExposure->setParent(this);
    LinkedComboBox *linkedTrigger = new LinkedComboBox(nullptr, ui->triggerValue, ui->triggerInput); linkedTrigger->setParent(this);


    QObject::connect(ui->maxExposureInput, SIGNAL(returnPressed()), this, SLOT(setMaxSliderWidth()));
    QObject::connect(ui->exposureInput, SIGNAL(sliderMoved(int)), cam, SLOT(setExposureTime(int)));
    QObject::connect(cam, SIGNAL(exposureTimeChanged(int)), ui->exposureInput, SLOT(setValue(int)));
    QObject::connect(ui->autoExposureInput, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(setExposureMode(Qt::CheckState)));

    ui->autoExposureInput->setChecked(cam->isExposureAuto());
    ui->maxExposureInput->setText(QString::number(defaultExpoMax));
    initExposureSlider();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::setMaxSliderWidth() {
    QString value = ui->maxExposureInput->text();
    int intValue = value.toInt();
    ui->exposureInput->setMaximum(intValue);
}

void SettingsWidget::initExposureSlider() {
    qInfo() << cam->getExposureTime();
    int defaultExposureValue = cam->getExposureTime();
    QSlider *exposureInput = ui->exposureInput;
    exposureInput->setMaximum(defaultExpoMax);
    exposureInput->setMinimum(defaultExpoMin);
    exposureInput->setOrientation(Qt::Horizontal);
    cam->setExposureTime(defaultExposureValue);
    qInfo() << cam->getExposureTime();
}

void SettingsWidget::setExposureMode(Qt::CheckState state) {
    bool isAuto = (state == Qt::Checked);
    ui->exposureInput->setEnabled(!isAuto);
    cam->setExposureAuto(isAuto);
}
