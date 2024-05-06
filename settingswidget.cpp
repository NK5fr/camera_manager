#include "settingswidget.h"
#include "linkedcombobox.h"
#include "linkedlineedit.h"
#include "linkedslider.h"
#include "ui_settingswidget.h"
#include "utils.h"

SettingsWidget::SettingsWidget(FlirCamera *camera, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
    , cam(camera)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    LinkedLineEdit *linkedMaxExposure = new LinkedLineEdit(nullptr, ui->maxExposureValue, ui->maxExposureInput); linkedMaxExposure->setParent(this);
    LinkedLineEdit *linkedMinExposure = new LinkedLineEdit(nullptr, ui->minExposureValue, ui->minExposureInput); linkedMinExposure->setParent(this);
    LinkedSlider *linkedExposure = new LinkedSlider(nullptr, ui->exposureValue, ui->exposureInput); linkedExposure->setParent(this);

    LinkedLineEdit *linkedMaxGain = new LinkedLineEdit(nullptr, ui->maxGainValue, ui->maxGainInput); linkedMaxGain->setParent(this);
    LinkedLineEdit *linkedMinGain = new LinkedLineEdit(nullptr, ui->minGainValue, ui->minGainInput); linkedMinGain->setParent(this);
    LinkedSlider *linkedGain = new LinkedSlider(nullptr, ui->gainValue, ui->exposureInput_2); linkedGain->setParent(this);


    LinkedComboBox *linkedTrigger = new LinkedComboBox(nullptr, ui->triggerValue, ui->triggerInput); linkedTrigger->setParent(this);
    LinkedComboBox *linkedTriggerMode = new LinkedComboBox(nullptr, ui->triggerModeValue, ui->triggerModeInput); linkedTriggerMode->setParent(this);


    QObject::connect(ui->maxExposureInput, SIGNAL(returnPressed()), this, SLOT(setMaxExpoSliderWidth()));
    QObject::connect(ui->minExposureInput, SIGNAL(returnPressed()), this, SLOT(setMinExpoSliderWidth()));
    QObject::connect(ui->exposureInput, SIGNAL(sliderMoved(int)), cam, SLOT(setExposureTime(int)));
    QObject::connect(cam, SIGNAL(exposureTimeChanged(int)), ui->exposureInput, SLOT(setValue(int)));
    QObject::connect(ui->autoExposureInput, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(setExposureMode(Qt::CheckState)));

    QObject::connect(ui->maxGainInput, SIGNAL(returnPressed()), this, SLOT(setMaxGainSliderWidth()));
    QObject::connect(ui->minGainInput, SIGNAL(returnPressed()), this, SLOT(setMinGainSliderWidth()));
    QObject::connect(cam, SIGNAL(gainChanged(int)), ui->exposureInput_2, SLOT(setValue(int)));
    QObject::connect(ui->exposureInput_2, SIGNAL(sliderMoved(int)), cam, SLOT(setGain(int)));
    QObject::connect(ui->autoGainInput, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(setGainMode(Qt::CheckState)));

    ui->autoExposureInput->setChecked(cam->isExposureAuto());
    ui->maxExposureInput->setText(QString::number(defaultExpoMax));
    ui->minExposureInput->setText(QString::number(defaultExpoMin));
    ui->minExposureInput->setText(QString::number(defaultExpoMin));
    initExposureSlider();

    ui->autoGainInput->setChecked(cam->isGainAuto());
    ui->maxGainInput->setText(QString::number(defaultGainMax));
    ui->minGainInput->setText(QString::number(defaultGainMin));

    ui->autoGainInput->setChecked(cam->isGainAuto());
    ui->maxGainInput->setText(QString::number(defaultGainMax));
    ui->minGainInput->setText(QString::number(defaultGainMin));
    initGainSlider();

    QObject::connect(ui->triggerInput, SIGNAL(currentTextChanged(QString)), cam, SLOT(setTrigger(QString)));
    QObject::connect(cam, SIGNAL(triggerChange(QString)), ui->triggerInput, SLOT(setCurrentText(QString)));

    QObject::connect(ui->triggerModeInput, SIGNAL(currentTextChanged(QString)), cam, SLOT(setTriggerMode(QString)));
    QObject::connect(cam, SIGNAL(triggerModeChange(QString)), ui->triggerModeInput, SLOT(setCurrentText(QString)));

    initTrigger();
    initTriggerMode();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::setMaxExpoSliderWidth() {
    QString value = ui->maxExposureInput->text();
    int intValue = value.toInt();
    ui->exposureInput->setMaximum(intValue);
}

void SettingsWidget::setExposureMode(Qt::CheckState state) {
    bool isAuto = (state == Qt::Checked);
    ui->exposureInput->setEnabled(!isAuto);
    cam->setExposureAuto(isAuto);
}

void SettingsWidget::setMinExpoSliderWidth() {
    QString value = ui->minExposureInput->text();
    int intValue = value.toInt();
    ui->exposureInput->setMinimum(intValue);
}


void SettingsWidget::initExposureSlider() {
    int defaultExposureValue = cam->getExposureTime();
    QSlider *exposureInput = ui->exposureInput;
    exposureInput->setMaximum(defaultExpoMax);
    exposureInput->setMinimum(defaultExpoMin);
    cam->setExposureTime(defaultExposureValue);
}

void SettingsWidget::setGainMode(Qt::CheckState state) {
    bool isAuto = (state == Qt::Checked);
    ui->exposureInput_2->setEnabled(!isAuto);
    cam->setGainAuto(isAuto);
}

void SettingsWidget::initTrigger(){
    ui->triggerInput->addItem("software");
    ui->triggerInput->addItem("line0");
}

void SettingsWidget::initTriggerMode(){
    ui->triggerModeInput->addItem("off");
    ui->triggerModeInput->addItem("on");
}

void SettingsWidget::setMaxGainSliderWidth() {
    QString value = ui->maxGainInput->text();
    int intValue = value.toInt();
    ui->exposureInput_2->setMaximum(intValue);
}

void SettingsWidget::setMinGainSliderWidth() {
    QString value = ui->minGainInput->text();
    int intValue = value.toInt();
    ui->exposureInput_2->setMinimum(intValue);
}

void SettingsWidget::initGainSlider()
{
    int defaultGainValue = cam->getGain();
    QSlider *gainInput = ui->exposureInput_2;
    gainInput->setMaximum(defaultGainMax);
    gainInput->setMinimum(defaultGainMin);
    cam->setGain(defaultGainValue);
}
