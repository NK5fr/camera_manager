#ifndef UTILS_H
#define UTILS_H

#include <QRect>
#include <QScreen>
#include <iostream>
#include <ctime>


namespace utils {
    QRect reCenterWidget(QWidget *toCenter, QWidget *widget);
    QRect centerOnPoint(QRect toCenter, QPoint point);
    QRect offSet(QWidget *toOffset, char direction, int offset);
    int showError(std::string text);
    std::string getTime();
}

#endif // UTILS_H
