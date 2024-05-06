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
    QObject::connect(input, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLabel()));
    QTimer::singleShot(0, this, &LinkedComboBox::changeLabel);
}

void LinkedComboBox::changeLabel()
{
    QString newText = QString("");
    newText.append(title);
    newText.append(input->currentText());
    label->setText(newText);
}
