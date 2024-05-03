#ifndef UTILS_H
#define UTILS_H

#include <QRect>
#include <QScreen>


namespace utils {
    QRect reCenter(QRect rect, QScreen *screen);
    QRect reCenter(QScreen *screen, int width, int height);
    QRect reCenterOffSet(QRect rect, QScreen *screen, char direction, int offset);
    int showError(std::string text);
}

#endif // UTILS_H
