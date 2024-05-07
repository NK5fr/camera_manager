#include "linkedlineedit.h"
#include "ctype.h"
#include "qtimer.h"
#include "qtooltip.h"
#include "string.h"
LinkedLineEdit::LinkedLineEdit(QObject *parent,
                               QLabel *label,
                               QLineEdit *input,
                               std::string stringTitle)
{
    this->label = label;
    this->input = input;
    this->title = QString::fromStdString(stringTitle);
    QObject::connect(input, &QLineEdit::returnPressed, this, &LinkedLineEdit::changeLabel);
    // Ensures that changeLabel is called right after the constructor is done executing, so that you have the default value
    QTimer::singleShot(0, this, &LinkedLineEdit::changeLabel);
}

// This method changed the linked label's value to the new ComboBox value
void LinkedLineEdit::changeLabel()
{
    QString value = input->text();
    qlonglong numberValue = value.toLongLong();
    QString newValue = QString::number(numberValue);
    QString newText = QString("");
    newText.append(title);
    newText.append(newValue);
    label->setText(newText);
}
