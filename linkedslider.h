#ifndef LINKEDSLIDER_H
#define LINKEDSLIDER_H

#include "ilinked.h"
#include "qslider.h"

class LinkedSlider : public ILinked
{
public:
    explicit LinkedSlider(QObject *parent = nullptr,
                          QLabel *label = nullptr,
                          QSlider *input = nullptr,
                          std::string stringTitle = "");
    QSlider *input;
public slots:
    void changeLabel();
};

#endif // LINKEDSLIDER_H
