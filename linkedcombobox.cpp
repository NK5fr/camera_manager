#include "linkedcombobox.h"

#include <qtimer.h>

LinkedComboBox::LinkedComboBox(QObject *parent,
                               QLabel *label,
                               QComboBox *input,
                               std::string stringTitle)
{
    this->label = label;
    this->input = input;
    this->title = QString::fromStdString(stringTitle);
    QObject::connect(input, &QComboBox::currentIndexChanged, this, &LinkedComboBox::changeLabel);
    // Ensures that changeLabel is called right after the constructor is done executing, so that you have the default value
    QTimer::singleShot(0, this, &LinkedComboBox::changeLabel);
}

// This method changed the linked label's value to the new ComboBox value
void LinkedComboBox::changeLabel()
{
    QString newText = QString("");
    newText.append(title);
    newText.append(input->currentText());
    label->setText(newText);
}
