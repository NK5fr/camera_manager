#ifndef LINKEDCOMBOBOX_H
#define LINKEDCOMBOBOX_H

#include "ilinked.h"

#include <qcombobox.h>

class LinkedComboBox : public ILinked
{
public:
    LinkedComboBox(QObject *parent = nullptr,
                   QLabel *label = nullptr,
                   QComboBox *input = nullptr,
                   std::string stringTitle = "");
    QComboBox *input;
public slots:
    void changeLabel();
};

#endif // LINKEDCOMBOBOX_H
