#ifndef LINKEDLINEEDIT_H
#define LINKEDLINEEDIT_H

#include "ilinked.h"
#include "qlineedit.h"

class LinkedLineEdit : public ILinked
{
public:
    explicit LinkedLineEdit(QObject *parent = nullptr,
                            QLabel *label = nullptr,
                            QLineEdit *input = nullptr,
                            std::string stringTitle = "");
    QLineEdit *input;
    void changeLabel();
};

#endif // LINKEDLINEEDIT_H
