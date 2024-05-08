#include "settingswidget.h"
#include "linkedcombobox.h"
#include "linkedlineedit.h"
#include "linkedslider.h"
#include "ui_settingswidget.h"
#include "utils.h"
#include <QFileDialog>

SettingsWidget::SettingsWidget(FlirCamera *camera, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
    , cam(camera)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    // Linking every input widget with their respective labels

    // For the Exposure
    LinkedLineEdit *linkedMaxExposure = new LinkedLineEdit(nullptr, ui->maxExposureValue, ui->maxExposureInput); linkedMaxExposure->setParent(this);
    LinkedLineEdit *linkedMinExposure = new LinkedLineEdit(nullptr, ui->minExposureValue, ui->minExposureInput); linkedMinExposure->setParent(this);
    LinkedSlider *linkedExposure = new LinkedSlider(nullptr, ui->exposureValue, ui->exposureInput); linkedExposure->setParent(this);

    // For the Gain
    LinkedLineEdit *linkedMaxGain = new LinkedLineEdit(nullptr, ui->maxGainValue, ui->maxGainInput); linkedMaxGain->setParent(this);
    LinkedLineEdit *linkedMinGain = new LinkedLineEdit(nullptr, ui->minGainValue, ui->minGainInput); linkedMinGain->setParent(this);
    LinkedSlider *linkedGain = new LinkedSlider(nullptr, ui->gainValue, ui->exposureInput_2); linkedGain->setParent(this);

    // For the Triggers
    LinkedComboBox *linkedTrigger = new LinkedComboBox(nullptr, ui->triggerValue, ui->triggerInput); linkedTrigger->setParent(this);
    LinkedComboBox *linkedTriggerMode = new LinkedComboBox(nullptr, ui->triggerModeValue, ui->triggerModeInput); linkedTriggerMode->setParent(this);

    // Connecting the values
    QObject::connect(ui->maxExposureInput, &QLineEdit::returnPressed, this, [this]() {
        // Sets the maximum value for the exposure slider to the value of maxExposureInput
        QString value = ui->maxExposureInput->text();
        int intValue = value.toInt();
        ui->exposureInput->setMaximum(intValue);
    });
    QObject::connect(ui->minExposureInput, &QLineEdit::returnPressed, this, [this]() {
        // Sets the minimum value of the exposure slider to the value of minExposureInput
        QString value = ui->minExposureInput->text();
        int intValue = value.toInt();
        ui->exposureInput->setMinimum(intValue);
    });
    QObject::connect(ui->exposureInput, &QSlider::sliderMoved, cam, &FlirCamera::setExposureTime);
    QObject::connect(cam, &FlirCamera::exposureTimeChanged, ui->exposureInput, &QSlider::setValue);
    QObject::connect(ui->autoExposureInput, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state){
        // Sets the actual gainMode of the camera to the chosen state in the checkbox
        bool isAuto = (state == Qt::Checked);
        ui->exposureInput->setEnabled(!isAuto);
        cam->setExposureAuto(isAuto);
    });

    QObject::connect(ui->maxGainInput, &QLineEdit::returnPressed, this, [this]() {
        // Sets the maximum value of the gain slider according to maxGainInput
        QString value = ui->maxGainInput->text();
        int intValue = value.toInt();
        ui->exposureInput_2->setMaximum(intValue);
    });
    QObject::connect(ui->minGainInput, &QLineEdit::returnPressed, this, [this]() {
        // Sets the minimum value of the gain slider according to minGainInput
        QString value = ui->minGainInput->text();
        int intValue = value.toInt();
        ui->exposureInput_2->setMinimum(intValue);
    });
    QObject::connect(cam, &FlirCamera::gainChanged, ui->exposureInput_2, &QSlider::setValue);
    QObject::connect(ui->exposureInput_2, &QSlider::sliderMoved, cam, &FlirCamera::setGain);
    QObject::connect(ui->autoGainInput, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state) {
        // Sets the actual gainMode of the camera to the chosen state in the checkbox
        bool isAuto = (state == Qt::Checked);
        ui->exposureInput_2->setEnabled(!isAuto);
        cam->setGainAuto(isAuto);
    });
    ui->autoExposureInput->setChecked(cam->isExposureAuto());
    ui->maxExposureInput->setText(QString::number(defaultExpoMax));
    ui->minExposureInput->setText(QString::number(defaultExpoMin));
    initExposureSlider();

    ui->autoGainInput->setChecked(cam->isGainAuto());
    ui->maxGainInput->setText(QString::number(defaultGainMax));
    ui->minGainInput->setText(QString::number(defaultGainMin));
    initGainSlider();

    QObject::connect(ui->triggerInput, &QComboBox::currentTextChanged, cam, &FlirCamera::setTrigger);
    QObject::connect(cam, &FlirCamera::triggerChange, ui->triggerInput, &QComboBox::setCurrentText);

    QObject::connect(ui->triggerModeInput, &QComboBox::currentTextChanged, cam, &FlirCamera::setTriggerMode);
    QObject::connect(cam, &FlirCamera::triggerModeChange, ui->triggerModeInput, &QComboBox::setCurrentText);
    QObject::connect(ui->imageButton, &QPushButton::clicked, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        qInfo() << dir;
        emit fileNameChanged(dir);
    });
    initTrigger();
    initTriggerMode();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

// Initializes the exposure slider so that it has the right limits
void SettingsWidget::initExposureSlider() {
    int defaultExposureValue = cam->getExposureTime();
    QSlider *exposureInput = ui->exposureInput;
    exposureInput->setMaximum(defaultExpoMax);
    exposureInput->setMinimum(defaultExpoMin);
    cam->setExposureTime(defaultExposureValue);
}

// Initializes the trigger combobox with values
void SettingsWidget::initTrigger(){
    ui->triggerInput->addItem("software");
    ui->triggerInput->addItem("line0");
}

// Initializes the triggermode combobox with values
void SettingsWidget::initTriggerMode(){
    ui->triggerModeInput->addItem("off");
    ui->triggerModeInput->addItem("on");
}
// Initializes the default values of the gain slider
void SettingsWidget::initGainSlider()
{
    int defaultGainValue = cam->getGain();
    QSlider *gainInput = ui->exposureInput_2;
    gainInput->setMaximum(defaultGainMax);
    gainInput->setMinimum(defaultGainMin);
    cam->setGain(defaultGainValue);
}

// Method to hide the settings page if its possible, used to ensure that it disseapears when you click on something else in the main window
void SettingsWidget::updateVisibility() {
    if (this->isVisible()) {
        this->hide();
    }
}
