#ifndef UTILS_H
#define UTILS_H

#include <QRect>
#include <QScreen>

namespace utils {
    QRect reCenter(QWidget *toCenter, QScreen *screen);
    QRect reCenter(int width, int height, QScreen *screen);
    QRect reCenterOffSet(QWidget *toCenter, QScreen *screen, char direction, int offset);
    QRect reCenterWidget(QWidget *toCenter, QWidget *widget);
    QRect center(QRect toCenter, QRect rect);
    QRect centerOnPoint(QRect toCenter, QPoint point);
    QRect offSet(QWidget *toOffset, char direction, int offset);
    int showError(std::string text);
}

#endif // UTILS_H
