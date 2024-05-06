#include "linkedslider.h"
#include "qtimer.h"
LinkedSlider::LinkedSlider(QObject *parent, QLabel *label, QSlider *input, std::string stringTitle)
{
    this->label = label;
    this->input = input;
    this->title = QString::fromStdString(stringTitle);
    QObject::connect(input, SIGNAL(valueChanged(int)), this, SLOT(changeLabel()));
    QTimer::singleShot(0, this, &LinkedSlider::changeLabel);
}

void LinkedSlider::changeLabel()
{
    QString newText = QString("");
    newText.append(title);
    newText.append(QString::number(input->value()));
    label->setText(newText);
}
