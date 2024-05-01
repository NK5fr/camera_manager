#ifndef UTILS_H
#define UTILS_H

#include <QRect>
#include <QScreen>


class utils
{
public:
    utils();
    static QRect reCenter(QRect rect, QScreen *screen);
    static QRect reCenter(QScreen *screen, int width, int height);
    static QRect reCenterOffSet(QRect rect, QScreen *screen, char direction, int offset);
};

#endif // UTILS_H
