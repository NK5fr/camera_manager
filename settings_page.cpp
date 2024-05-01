#include "settings_page.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSlider>
#include <qlabel.h>
#include "utils.h"
#include "linkedlineedit.h"
#include "linkedslider.h"
#include "linkedcombobox.h"

SettingsPage::SettingsPage(FlirCamera *camera, QWidget *parent)
    : QWidget{parent}
    , cam(camera)
{
    QScreen *screen =  QGuiApplication::primaryScreen();
    this->setGeometry(utils::reCenterOffSet(utils::reCenter(screen, 300,300), screen, 'r', 500));
    QFormLayout *layout = new QFormLayout(this);
    QLabel *deviceLabel = new QLabel("Device ID:");
    QLabel *maxExposureLabel = new QLabel("Max Exposure Time:");
    QLabel *autoExposureLabel = new QLabel("Enable Auto Exposure:");
    QLabel *exposureLabel = new QLabel("Exposure Time:");
    QLabel *triggerLabel = new QLabel("Trigger Source:");

    QLabel *deviceValue = new QLabel("");
    deviceValue->setObjectName("deviceValue");
    deviceValue->setAlignment(Qt::AlignCenter);

    QLabel *maxExposureValue = new QLabel("");
    maxExposureValue->setObjectName("serialValue");
    maxExposureValue->setAlignment(Qt::AlignCenter);

    QLabel *autoExposureValue = new QLabel("");
    autoExposureValue->setObjectName("autoExposureValue");
    autoExposureValue->setAlignment(Qt::AlignCenter);

    QLabel *exposureValue = new QLabel("");
    exposureValue->setObjectName("exposureValue");
    exposureValue->setAlignment(Qt::AlignCenter);

    QLabel *triggerValue= new QLabel("");
    triggerValue->setObjectName("triggerValue");
    triggerValue->setAlignment(Qt::AlignCenter);


    QLineEdit *deviceInput = new QLineEdit(this);
    deviceInput->setObjectName("deviceInput");

    QLineEdit *maxExposureInput = new QLineEdit(this);
    maxExposureInput->setObjectName("maxExposureInput");

    QCheckBox *autoExposureInput = new QCheckBox(this);
    autoExposureInput->setObjectName("autoExposureInput");

    QSlider *exposureInput = new QSlider(this);
    exposureInput->setObjectName("exposureInput");
    exposureInput->setOrientation(Qt::Horizontal);
    exposureInput->setTickPosition(QSlider::TicksBelow);

    QComboBox *triggerInput = new QComboBox(this);
    triggerInput->setObjectName("triggerInput");


    std::vector<QLabel*> labels = {deviceLabel, maxExposureLabel, autoExposureLabel, exposureLabel, triggerLabel};
    std::vector<QWidget*> inputs = {deviceInput, maxExposureInput, autoExposureInput, exposureInput, triggerInput};
    std::vector<QLabel*> values = {deviceValue, maxExposureValue, autoExposureValue, exposureValue, triggerValue};
    for (int i = 0 ; i < labels.size() ; i++) {
        layout->addRow(labels[i], inputs[i]);
        layout->addRow(values[i]);
    }
    LinkedLineEdit *linkedMaxExposure = new LinkedLineEdit(nullptr, maxExposureValue, maxExposureInput); linkedMaxExposure->setParent(this);
    LinkedLineEdit *linkedDevice = new LinkedLineEdit(nullptr, deviceValue, deviceInput); linkedDevice->setParent(this);
    LinkedSlider *linkedExposure = new LinkedSlider(nullptr, exposureValue, exposureInput); linkedExposure->setParent(this);
    LinkedComboBox *linkedTrigger = new LinkedComboBox(nullptr, triggerValue, triggerInput); linkedTrigger->setParent(this);

    QObject::connect(maxExposureInput, SIGNAL(returnPressed()), this, SLOT(setMaxSliderWidth()));
    QObject::connect(exposureInput, SIGNAL(sliderMoved(int)), cam, SLOT(setExposureTime(int)));
    QObject::connect(cam, SIGNAL(exposureTimeChanged(int)), exposureInput, SLOT(setValue(int)));
    QObject::connect(autoExposureInput, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(setExposureMode(Qt::CheckState)));

    autoExposureInput->setChecked(cam->getExposureTime());

    maxExposureInput->setText(QString::number(defaultExpoMax));

    exposureInput->setMaximum(defaultExpoMax);
    if (!cam->getExposureAuto()) {
        exposureInput->setValue(cam->getExposureTime());
    }
    exposureInput->setMinimum(defaultExpoMin);
}

void SettingsPage::setMaxSliderWidth() {
    QString value = this->findChild<QLineEdit *>("maxExposureInput")->text();
    int intValue = value.toInt();
    QSlider *slider = this->findChild<QSlider *>("exposureInput");
    slider->setMaximum(intValue);
}

void SettingsPage::setExposureMode(Qt::CheckState state) {
    bool isAuto = (state == Qt::Checked);
    this->findChild<QSlider*>("exposureInput")->setEnabled(!isAuto);
    cam->setExposureMode(isAuto);
}
