#include "linkedlineedit.h"
#include "string.h"
#include "string.h"
#include "ctype.h"
#include "qtooltip.h"
#include "qtimer.h"
LinkedLineEdit::LinkedLineEdit(QObject *parent, QLabel *label, QLineEdit *input, std::string stringTitle)
{
    this->label = label;
    this->input = input;
    this->title = QString::fromStdString(stringTitle);
    QObject::connect(input, SIGNAL(returnPressed()), this, SLOT(changeLabel()));
    QTimer::singleShot(0, this, &LinkedLineEdit::changeLabel);
}

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
