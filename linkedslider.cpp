#include "linkedslider.h"
#include "qtimer.h"
LinkedSlider::LinkedSlider(QObject *parent, QLabel *label, QSlider *input, std::string stringTitle)
{
    this->label = label;
    this->input = input;
    this->title = QString::fromStdString(stringTitle);
    QObject::connect(input, &QSlider::valueChanged, this, &LinkedSlider::changeLabel);
    // Ensures that changeLabel is called right after the constructor is done executing, so that you have the default value
    QTimer::singleShot(0, this, &LinkedSlider::changeLabel);
}

// This method changed the linked label's value to the new ComboBox value
void LinkedSlider::changeLabel()
{
    QString newText = QString("");
    newText.append(title);
    newText.append(QString::number(input->value()));
    label->setText(newText);
}
