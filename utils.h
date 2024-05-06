#ifndef UTILS_H
#define UTILS_H

#include <QRect>
#include <QScreen>


namespace utils {
    void reCenter(QWidget toCenter, QScreen *screen);
    QRect reCenter(int width, int height, QScreen *screen);
    void reCenterOffSet(QWidget toCenter, QScreen *screen, char direction, int offset);
    void reCenterWidget(QWidget toCenter, QWidget widget);
    void center(QRect toCenter, QRect rect);
    void centerOnPoint(QRect toCenter, QPoint point);
    int showError(std::string text);
}

#endif // UTILS_H
