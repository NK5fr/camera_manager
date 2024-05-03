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
    LinkedSlider *linkedGain = new LinkedSlider(nullptr, ui->gainValue, ui->exposureInput_2); linkedGain->setParent(this);
    LinkedSlider *linkedExposure = new LinkedSlider(nullptr, ui->exposureValue, ui->exposureInput); linkedExposure->setParent(this);
    LinkedComboBox *linkedTrigger = new LinkedComboBox(nullptr, ui->triggerValue, ui->triggerInput); linkedTrigger->setParent(this);


    QObject::connect(ui->maxExposureInput, SIGNAL(returnPressed()), this, SLOT(setMaxSliderWidth()));
    QObject::connect(ui->exposureInput, SIGNAL(sliderMoved(int)), cam, SLOT(setExposureTime(int)));
    QObject::connect(cam, SIGNAL(exposureTimeChanged(int)), ui->exposureInput, SLOT(setValue(int)));
    QObject::connect(ui->autoExposureInput, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(setExposureMode(Qt::CheckState)));


    QObject::connect(cam, SIGNAL(gainChanged(int)), ui->exposureInput_2, SLOT(setValue(int)));
    QObject::connect(ui->exposureInput_2, SIGNAL(valueChanged(int)), cam, SLOT(setGain(int)));
    ui->autoExposureInput->setChecked(cam->isExposureAuto());
    ui->maxExposureInput->setText(QString::number(defaultExpoMax));
    initExposureSlider();
    initGainSlider();
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
    int defaultExposureValue = cam->getExposureTime();
    QSlider *exposureInput = ui->exposureInput;
    exposureInput->setMaximum(defaultExpoMax);
    exposureInput->setMinimum(defaultExpoMin);
    cam->setExposureTime(defaultExposureValue);
}

void SettingsWidget::initGainSlider() {
    int defaultGainValue = cam->getGain();
    QSlider *gainInput = ui->exposureInput_2;
    gainInput->setMaximum(18);
    gainInput->setMinimum(0);
    cam->setGain(defaultGainValue);

}

void SettingsWidget::setExposureMode(Qt::CheckState state) {
    bool isAuto = (state == Qt::Checked);
    ui->exposureInput->setEnabled(!isAuto);
    cam->setExposureAuto(isAuto);
}
